/* Bitwise checks of the dense kernels against element-at-a-time reference
 * loops.  The GEMM contract (acc = +0, acc += a*b in increasing k, C -= acc)
 * makes the result independent of register blocking, CPU dispatch variant and
 * thread split, so every size and band offset must match the scalar loop
 * exactly.  Sizes straddle the 8x4 register tile, the 64-row parallel chunk,
 * the 128-row solve block and the 1024-entry solve gather buffer. */
#include "../src/vsdlss_m3_internal.h"
#include "../src/vsdlss_dense.h"
#include <string.h>

#define CHECK(e) do{if(!(e)){fprintf(stderr,"kernels line %d\n",__LINE__);return 1;}}while(0)

static double value(csi k, csi i) { return sin(0.7*(double)k+0.31*(double)i)+0.5; }

static void reference_gemm(csi m,csi n,csi k,const double *A,csi lda,
                           const double *B,csi ldb,double *C,csi ldc,csi tri)
{
    for(csi j=0;j<n;j++)for(csi i=0;i<m;i++){
        if(i+tri<j)continue;
        double acc=0;
        for(csi p=0;p<k;p++)acc+=A[i+p*lda]*B[j+p*ldb];
        C[i+j*ldc]-=acc;
    }
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

static int check_gemm(csi m,csi n,csi k,csi tri,int threads)
{
    csi lda=(m>n?m:n)+4,ldc=m+1;
    double *A=malloc((size_t)lda*(size_t)(k?k:1)*sizeof(double));
    double *got=malloc((size_t)ldc*n*sizeof(double)),*want=malloc((size_t)ldc*n*sizeof(double));
    CHECK(A&&got&&want);
    for(csi p=0;p<k;p++)for(csi i=0;i<lda;i++)A[p*lda+i]=value(p,i);
    for(csi u=0;u<ldc*n;u++)got[u]=want[u]=0.25*(double)u;
    CHECK(vsdlss_set_num_threads(threads)==VSDLSS_OK);
    /* B = rows 1..n of A, as in a supernodal update where the leading rows
     * of the external block are also the target columns. */
    vsdlss_gemm_nt_sub_par(m,n,k,A,lda,A+1,lda,got,ldc,tri);
    reference_gemm(m,n,k,A,lda,A+1,lda,want,ldc,tri);
    if(memcmp(got,want,(size_t)ldc*n*sizeof(double))!=0)
        fprintf(stderr,"gemm m=%lld n=%lld k=%lld tri=%lld nt=%d\n",(long long)m,(long long)n,(long long)k,(long long)tri,threads);
    CHECK(memcmp(got,want,(size_t)ldc*n*sizeof(double))==0);
    /* Serial kernel, split by rows, must agree with the whole-range call. */
    for(csi u=0;u<ldc*n;u++)got[u]=0.25*(double)u;
    csi half=m/2;
    vsdlss_gemm_nt_sub(half,n,k,A,lda,A+1,lda,got,ldc,tri);
    vsdlss_gemm_nt_sub(m-half,n,k,A+half,lda,A+1,lda,got+half,ldc,tri+half);
    CHECK(memcmp(got,want,(size_t)ldc*n*sizeof(double))==0);
    free(A);free(got);free(want);return 0;
}

static int check_potrf(csi width,csi ext,int threads)
{
    csi rows=width+ext;size_t count=(size_t)rows*width;
    double *l=calloc(count,sizeof(double)),*a=calloc(count,sizeof(double)),*b=calloc(count,sizeof(double));
    CHECK(l&&a&&b);
    for(csi j=0;j<width;j++)for(csi i=j;i<rows;i++)
        l[j*rows+i]=i==j?2.0+0.01*(double)j:0.05*value(j,i);
    for(csi j=0;j<width;j++)for(csi i=j;i<rows;i++){
        double v=0;for(csi k=0;k<=j;k++)v+=l[k*rows+i]*l[k*rows+j];
        a[j*rows+i]=v;
    }
    memcpy(b,a,count*sizeof(double));
    CHECK(vsdlss_set_num_threads(1)==VSDLSS_OK);
    CHECK(vsdlss_dense_potrf_panel(a,rows,width)==VSDLSS_OK);
    for(csi j=0;j<width;j++)for(csi i=0;i<rows;i++){
        if(i<j)CHECK(a[j*rows+i]==0);
        else CHECK(fabs(a[j*rows+i]-l[j*rows+i])<1e-12);
    }
    CHECK(vsdlss_set_num_threads(threads)==VSDLSS_OK);
    CHECK(vsdlss_dense_potrf_panel(b,rows,width)==VSDLSS_OK);
    CHECK(memcmp(a,b,count*sizeof(double))==0);
    free(l);free(a);free(b);return 0;
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
    static const csi dims[]={1,2,3,4,5,7,8,9,12,17,63,64,65,130};
    static const csi tris[]={-9,-1,0,1,3,8,VSDLSS_GEMM_FULL};
    int maxthreads=vsdlss_parallel_enabled()?4:1;
    for(int nt=1;nt<=maxthreads;nt*=2){
        for(size_t wi=0;wi<sizeof(widths)/sizeof(*widths);wi++)
            for(size_t ei=0;ei<sizeof(exts)/sizeof(*exts);ei++)
                if(check_solve(widths[wi],exts[ei],nt))return 1;
        for(size_t mi=0;mi<sizeof(dims)/sizeof(*dims);mi++)
            for(size_t ni=0;ni<sizeof(dims)/sizeof(*dims);ni+=2)
                for(size_t ti=0;ti<sizeof(tris)/sizeof(*tris);ti++)
                    if(check_gemm(dims[mi],dims[ni],dims[(mi+ni)%7],tris[ti],nt))return 1;
        /* Large enough to split over the team (work threshold). */
        if(check_gemm(700,48,300,0,nt)||check_gemm(513,67,129,VSDLSS_GEMM_FULL,nt))return 1;
        static const csi pw[][2]={{1,0},{5,3},{47,0},{48,1},{49,40},{97,200},{150,700}};
        for(size_t pi=0;pi<sizeof(pw)/sizeof(*pw);pi++)
            if(check_potrf(pw[pi][0],pw[pi][1],nt))return 1;
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
    puts("test_kernels: GEMM/solve kernels bitwise match the element-at-a-time "
         "reference, blocked POTRF reproduces L, at 1/2/4 threads");
    return 0;
}
