/* Bitwise equivalence of the blocked panel kernels against the element-at-a-
 * time reference loops they replaced.  The reference bodies below are copies
 * of the pre-optimisation kernels, so a failure here means the rewrite changed
 * floating-point results, not merely performance.  Sizes straddle every
 * blocking boundary (VSDLSS_PANEL_UPDATE_CB, the 256-row update tile, the
 * 128-row solve block and the 1024-entry solve gather buffer). */
#include "../src/vsdlss_m3_internal.h"
#include <string.h>

#define CHECK(e) do{if(!(e)){fprintf(stderr,"kernels line %d\n",__LINE__);return 1;}}while(0)

static double value(csi k, csi i) { return sin(0.7*(double)k+0.31*(double)i)+0.5; }

/* Former update kernel: one vsdlss_panel_dot per output element. */
static int reference_update(const double *a,csi rows,csi width,csi ext,
                            csi cfirst,csi clast,const csi *targets,
                            double *panel,csi lo,csi hi)
{
    int bad=0;
    for(csi col=cfirst;col<clast;col++){
        csi u=col*ext-col*(col-1)/2;
        for(csi row=col;row<ext;row++){
            csi target=targets[u++];
            if(target<lo||target>=hi){bad|=1;continue;}
            panel[target]-=vsdlss_panel_dot(a,rows,width,width+row,width+col);
            if(!isfinite(panel[target]))bad|=2;
        }
    }
    return bad;
}

/* Former external phase of vsdlss_panel_solve_generic. */
static vsdlss_status reference_solve(const double *a,csi begin,csi width,
                                     csi ext,const csi *index,double *x,int back)
{
    csi rows=width+ext;int bad=0;
    if(!back)for(csi j=0;j<width;j++){
        double d=a[j*rows+j];
        if(!isfinite(d)||d<=0)return VSDLSS_ERR_INVALID;
        x[begin+j]/=d;
        if(!isfinite(x[begin+j]))return VSDLSS_ERR_NONFINITE;
        for(csi r=j+1;r<width;r++)x[begin+r]-=a[j*rows+r]*x[begin+j];
    }
    for(csi t=0;t<(back?width:ext);t++){
        csi dest=back?begin+t:index[t];double v=x[dest];
        if(back)for(csi r=0;r<ext;r++)v-=a[t*rows+width+r]*x[index[r]];
        else for(csi j=0;j<width;j++)v-=a[j*rows+width+t]*x[begin+j];
        x[dest]=v;if(!isfinite(v))bad=1;
    }
    if(bad)return VSDLSS_ERR_NONFINITE;
    if(back)for(csi j=width;j-- >0;){
        double d=a[j*rows+j],v=x[begin+j];
        if(!isfinite(d)||d<=0)return VSDLSS_ERR_INVALID;
        for(csi r=j+1;r<width;r++)v-=a[j*rows+r]*x[begin+r];
        x[begin+j]=v/d;
        if(!isfinite(x[begin+j]))return VSDLSS_ERR_NONFINITE;
    }
    return VSDLSS_OK;
}

static int check_update(csi width,csi ext,int threads)
{
    csi rows=width+ext,slots=ext*(ext+1)/2;
    double *a=malloc((size_t)rows*width*sizeof(double));
    double *got=malloc((size_t)slots*sizeof(double));
    double *want=malloc((size_t)slots*sizeof(double));
    csi *targets=malloc((size_t)slots*sizeof(csi));
    CHECK(a&&got&&want&&targets);
    for(csi k=0;k<width;k++)for(csi i=0;i<rows;i++)a[k*rows+i]=value(k,i);
    for(csi u=0;u<slots;u++){targets[u]=u;got[u]=want[u]=0.25*(double)u;}
    CHECK(vsdlss_set_num_threads(threads)==VSDLSS_OK);
    /* Chunk exactly as vsdlss_sn_factorize does. */
    for(csi c0=0;c0<ext;c0+=VSDLSS_PANEL_UPDATE_CB){
        csi c1=c0+VSDLSS_PANEL_UPDATE_CB;if(c1>ext)c1=ext;
        CHECK(vsdlss_panel_update_range(a,rows,width,ext,c0,c1,targets,got,0,slots)==0);
    }
    CHECK(reference_update(a,rows,width,ext,0,ext,targets,want,0,slots)==0);
    CHECK(memcmp(got,want,(size_t)slots*sizeof(double))==0);
    /* One whole-range call must agree with the chunked one. */
    for(csi u=0;u<slots;u++)got[u]=0.25*(double)u;
    CHECK(vsdlss_panel_update_range(a,rows,width,ext,0,ext,targets,got,0,slots)==0);
    CHECK(memcmp(got,want,(size_t)slots*sizeof(double))==0);
    /* Out-of-range targets report bit 1 and leave the panel alone. */
    if(slots>0){
        csi saved=targets[slots-1];targets[slots-1]=slots+5;
        for(csi u=0;u<slots;u++)got[u]=0.25*(double)u;
        CHECK((vsdlss_panel_update_range(a,rows,width,ext,0,ext,targets,got,0,slots)&1)==1);
        CHECK(got[slots-1]==0.25*(double)(slots-1)); /* skipped, not written */
        targets[slots-1]=saved;
    }
    free(a);free(got);free(want);free(targets);return 0;
}

static int check_solve(csi width,csi ext,int threads)
{
    csi rows=width+ext,n=width+2*ext+7;
    double *a=malloc((size_t)rows*width*sizeof(double));
    double *got=malloc((size_t)n*sizeof(double)),*want=malloc((size_t)n*sizeof(double));
    csi *index=ext?malloc((size_t)ext*sizeof(csi)):NULL;
    CHECK(a&&got&&want&&(ext==0||index));
    for(csi k=0;k<width;k++)for(csi i=0;i<rows;i++)
        a[k*rows+i]=i==k?4.0+0.5*(double)k:0.03*value(k,i);
    /* Scattered, strictly increasing destinations outside the J block. */
    for(csi r=0;r<ext;r++)index[r]=width+(r*2)%(n-width);
    for(csi r=1;r<ext;r++)if(index[r]<=index[r-1])index[r]=index[r-1]+1;
    CHECK(ext==0||index[ext-1]<n);
    CHECK(vsdlss_set_num_threads(threads)==VSDLSS_OK);
    for(int back=0;back<2;back++){
        for(csi i=0;i<n;i++)got[i]=want[i]=value(back,i);
        CHECK(vsdlss_panel_solve_generic(a,0,width,ext,index,got,back)==VSDLSS_OK);
        CHECK(reference_solve(a,0,width,ext,index,want,back)==VSDLSS_OK);
        CHECK(memcmp(got,want,(size_t)n*sizeof(double))==0);
    }
    free(a);free(got);free(want);free(index);return 0;
}

int main(void)
{
    static const csi widths[]={1,2,3,4,5,7,16,33};
    static const csi exts[]={1,2,3,4,5,8,127,128,129,255,256,257,300,1023,1024,1025};
    int maxthreads=vsdlss_parallel_enabled()?4:1;
    for(int nt=1;nt<=maxthreads;nt*=2)
        for(size_t wi=0;wi<sizeof(widths)/sizeof(*widths);wi++)
            for(size_t ei=0;ei<sizeof(exts)/sizeof(*exts);ei++){
                /* Keep the update sweep cheap: it is O(ext^2 * width). */
                if(exts[ei]<=300&&check_update(widths[wi],exts[ei],nt))return 1;
                if(check_solve(widths[wi],exts[ei],nt))return 1;
            }
    /* ext == 0 has no external phase at all. */
    for(size_t wi=0;wi<sizeof(widths)/sizeof(*widths);wi++)
        if(check_solve(widths[wi],0,1))return 1;
    /* width*ext above the 100000 work threshold, so the solve really does
     * open a team and the parallel branch is the one compared bitwise. */
    {
        static const csi big[][2]={{128,1025},{256,600},{600,300}};
        for(int nt=1;nt<=maxthreads;nt*=2)
            for(size_t bi=0;bi<sizeof(big)/sizeof(*big);bi++)
                if(check_solve(big[bi][0],big[bi][1],nt))return 1;
        if(vsdlss_parallel_enabled())CHECK(vsdlss_parallel_last_team_size()>1);
    }
    CHECK(vsdlss_set_num_threads(1)==VSDLSS_OK);
    puts("test_kernels: blocked update and solve kernels bitwise match the "
         "element-at-a-time reference at 1/2/4 threads");
    return 0;
}
