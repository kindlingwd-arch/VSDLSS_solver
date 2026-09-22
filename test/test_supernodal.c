/* Randomised end-to-end checks of the left-looking supernodal path:
 * strict and relaxed layouts, every ordering, 1/2/4 threads.  Checks the
 * backward error of the M3 facade, bitwise identical factors and solutions
 * across thread counts, and that the relaxed panels contain the strict
 * pattern (explicit zeros are only added, never entries lost). */
#include "../src/vsdlss_m3_internal.h"
#include <string.h>

#define CHECK(e) do{if(!(e)){fprintf(stderr,"supernodal line %d: %s\n",__LINE__,#e);return 1;}}while(0)

static unsigned rng=12345u;
static unsigned next_rand(void){rng=rng*1664525u+1013904223u;return rng>>8;}

/* Random symmetric pattern: a few long-range edges per column, optional
 * dense-ish rows and block structure (several components). */
static vsdlss *random_spd(csi n,int per_col,int blocks,int dense_rows)
{
    csi cap=n*(per_col+2)+dense_rows*n+n,p=0;
    vsdlss *A=vsdlss_spalloc(n,n,cap,1,0);
    char *mark=calloc((size_t)n,1);
    if(!A||!mark){free(mark);vsdlss_spfree(A);return NULL;}
    for(csi j=0;j<n;j++){
        A->p[j]=p;
        csi blk=j*blocks/n, lo=blk*n/blocks;
        double diag=1.0;
        for(int t=0;t<per_col&&j>lo;t++){
            csi i=lo+(csi)(next_rand()%(unsigned)(j-lo));
            if(mark[i])continue;
            mark[i]=1;A->i[p]=i;A->x[p]=-0.5-0.001*(double)(next_rand()%1000);diag-=A->x[p];p++;
        }
        for(int d=0;d<dense_rows;d++){
            csi i=lo+d; if(i>=j||mark[i])continue;
            mark[i]=1;A->i[p]=i;A->x[p]=-0.01;diag+=0.01;p++;
        }
        for(csi k=A->p[j];k<p;k++)mark[A->i[k]]=0;
        A->i[p]=j;A->x[p]=diag+2.0*(double)per_col+(double)dense_rows;p++;
    }
    A->p[n]=p;
    /* Make it strictly diagonally dominant: also count the entries whose
     * row index is the diagonal's (they live in later columns). */
    for(csi j=0;j<n;j++)for(csi k=A->p[j];k<A->p[j+1]-1;k++)
        A->x[A->p[A->i[k]+1]-1]+=fabs(A->x[k]);
    free(mark);return A;
}

static int one_matrix(vsdlss *A)
{
    csi n=A->n;
    double *b=malloc((size_t)n*sizeof(double)),*x1=malloc((size_t)n*sizeof(double)),*xt=malloc((size_t)n*sizeof(double));
    CHECK(b&&x1&&xt);
    for(csi i=0;i<n;i++)b[i]=sin(0.37*(double)i)+0.1;
    for(int order=0;order<=5;order++){
        for(int nt=1;nt<=4;nt*=2){
            vsdlss_m3_factor *f=NULL;double eta;
            if(nt>1&&!vsdlss_parallel_enabled())break;
            CHECK(vsdlss_set_num_threads(nt)==VSDLSS_OK);
            { vsdlss_status fs=vsdlss_factorize_m3(A,order,&f); if(fs!=VSDLSS_OK)fprintf(stderr,"order %d nt %d n %lld: %s\n",order,nt,(long long)n,vsdlss_status_string(fs)); CHECK(fs==VSDLSS_OK); }
            CHECK(vsdlss_m3_solve(f,b,nt==1?x1:xt)==VSDLSS_OK);
            CHECK(vsdlss_backward_error(A,nt==1?x1:xt,b,&eta)==VSDLSS_OK&&eta<1e-13);
            if(nt>1)CHECK(memcmp(x1,xt,(size_t)n*sizeof(double))==0);
            vsdlss_m3_factor_free(f);
        }
    }
    /* Strict vs relaxed on the (unpermuted) matrix, 1 vs 4 threads. */
    {
        vsdlss *N=NULL;vsdlss_sn_symbolic *ss=NULL,*rs=NULL;vsdlss_sn_factor *sf=NULL,*rf=NULL,*rf4=NULL;
        vsdlss *SL=NULL,*RL=NULL;
        CHECK(vsdlss_normalize_upper(A,&N)==VSDLSS_OK);
        CHECK(vsdlss_sn_analyze(N,&ss)==VSDLSS_OK&&vsdlss_sn_analyze_relaxed(N,&rs)==VSDLSS_OK);
        CHECK(rs->count<=ss->count&&rs->l_nnz>=ss->l_nnz&&ss->relaxed_zeros==0);
        CHECK(vsdlss_set_num_threads(1)==VSDLSS_OK);
        CHECK(vsdlss_sn_factorize(N,ss,&sf)==VSDLSS_OK&&vsdlss_sn_factorize(N,rs,&rf)==VSDLSS_OK);
        if(vsdlss_parallel_enabled()){
            CHECK(vsdlss_set_num_threads(4)==VSDLSS_OK);
            CHECK(vsdlss_sn_factorize(N,rs,&rf4)==VSDLSS_OK);
            CHECK(memcmp(rf->panel,rf4->panel,(size_t)rs->panel_offset[rs->count]*sizeof(double))==0);
            CHECK(vsdlss_set_num_threads(1)==VSDLSS_OK);
        }
        CHECK(vsdlss_sn_export_L(sf,&SL)==VSDLSS_OK&&vsdlss_sn_export_L(rf,&RL)==VSDLSS_OK);
        /* Every strict entry appears in the relaxed factor with a close value;
         * every extra relaxed entry is an exact zero. */
        for(csi j=0;j<n;j++){
            csi p=RL->p[j];
            for(csi k=SL->p[j];k<SL->p[j+1];k++){
                while(p<RL->p[j+1]&&RL->i[p]<SL->i[k]){CHECK(RL->x[p]==0);p++;}
                CHECK(p<RL->p[j+1]&&RL->i[p]==SL->i[k]);
                CHECK(fabs(RL->x[p]-SL->x[k])<=1e-12*(1+fabs(SL->x[k])));
                p++;
            }
            for(;p<RL->p[j+1];p++)CHECK(RL->x[p]==0);
        }
        vsdlss_spfree(SL);vsdlss_spfree(RL);vsdlss_sn_factor_free(sf);vsdlss_sn_factor_free(rf);
        vsdlss_sn_factor_free(rf4);vsdlss_sn_symbolic_free(ss);vsdlss_sn_symbolic_free(rs);vsdlss_spfree(N);
    }
    free(b);free(x1);free(xt);return 0;
}

int main(void)
{
    static const struct {csi n;int per,blocks,dense;} cases[]={
        {1,0,1,0},{2,1,1,0},{17,2,1,0},{60,3,2,1},{200,2,1,0},{400,4,3,2},
        {900,3,1,0},{1500,5,2,3},{2500,2,5,0},{3000,6,1,4}};
    for(size_t c=0;c<sizeof(cases)/sizeof(*cases);c++){
        vsdlss *A=random_spd(cases[c].n,cases[c].per,cases[c].blocks,cases[c].dense);
        CHECK(A);
        if(one_matrix(A)){fprintf(stderr,"case %zu (n=%lld)\n",c,(long long)cases[c].n);return 1;}
        vsdlss_spfree(A);
    }
    CHECK(vsdlss_set_num_threads(1)==VSDLSS_OK);
    puts("test_supernodal: random SPD, orders 0-5, strict/relaxed, 1/2/4 threads: ALL OK");
    return 0;
}
