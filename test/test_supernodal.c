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
        /* exact minimum degree builds explicit cliques: keep it to small n */
        if(order==3&&n>1000)continue;
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

/* Power-grid-like graph: G x G junctions, wires of 7 segments with taps,
 * pendant per junction, grounded pads; `copies` disjoint copies; shuffled.
 * Large enough to take the BFS renumbering and blocked-reduction paths. */
static vsdlss *powergrid(csi G,int copies)
{
    csi per=G*G+2*G*(G-1)*9+G*G, n=per*copies, cap=3*n, m=0;
    csi *ea=malloc((size_t)cap*sizeof(csi)),*eb=malloc((size_t)cap*sizeof(csi)),*perm=malloc((size_t)n*sizeof(csi));
    double *ew=malloc((size_t)cap*sizeof(double)),*diag=calloc((size_t)n,sizeof(double));
    csi *cnt=calloc((size_t)n+1,sizeof(csi));
    vsdlss *A;
    if(!ea||!eb||!perm||!ew||!diag||!cnt)return NULL;
    for(int cp=0;cp<copies;cp++){
        csi base=cp*per,next=base+G*G;
        for(csi y=0;y<G;y++)for(csi x=0;x<G;x++)for(int dir=0;dir<2;dir++){
            csi x2=x+(dir==0),y2=y+(dir==1),prev=base+y*G+x;
            if(x2>=G||y2>=G)continue;
            for(int c=0;c<7;c++){
                csi node=next++;
                ea[m]=prev;eb[m]=node;ew[m++]=0.5+0.1*(double)(next%13);
                if(c==1||c==4){csi tap=next++;ea[m]=node;eb[m]=tap;ew[m++]=0.2;diag[tap]+=1e-3;}
                prev=node;
            }
            ea[m]=prev;eb[m]=base+y2*G+x2;ew[m++]=1.0;
        }
        for(csi y=0;y<G;y++)for(csi x=0;x<G;x++){
            csi pnode=next++;ea[m]=base+y*G+x;eb[m]=pnode;ew[m++]=1.3;
            if(x%8==0&&y%8==0)diag[pnode]+=10;
        }
    }
    for(csi i=0;i<n;i++)perm[i]=i;
    for(csi i=n-1;i>0;i--){csi j=(csi)(next_rand()%(unsigned)(i+1)),t=perm[i];perm[i]=perm[j];perm[j]=t;}
    A=vsdlss_spalloc(n,n,n+m,1,0);if(!A)return NULL;
    double *d2=calloc((size_t)n,sizeof(double));
    for(csi k=0;k<m;k++){csi a=perm[ea[k]],b=perm[eb[k]];cnt[a>b?a:b]++;d2[a]+=ew[k];d2[b]+=ew[k];}
    for(csi i=0;i<n;i++)d2[perm[i]]+=diag[i];
    A->p[0]=0;for(csi j=0;j<n;j++){A->p[j+1]=A->p[j]+cnt[j]+1;cnt[j]=A->p[j];}
    for(csi k=0;k<m;k++){csi a=perm[ea[k]],b=perm[eb[k]],col=a>b?a:b;A->i[cnt[col]]=a<b?a:b;A->x[cnt[col]++]=-ew[k];}
    for(csi j=0;j<n;j++){A->i[cnt[j]]=j;A->x[cnt[j]++]=d2[j];}
    free(ea);free(eb);free(ew);free(perm);free(diag);free(cnt);free(d2);
    /* columns were filled in edge order: sort them */
    vsdlss *N=NULL;if(vsdlss_normalize_upper(A,&N)!=VSDLSS_OK)return NULL;
    vsdlss_spfree(A);return N;
}

/* Two supply nets (VDD and GND): the extracted netlist splits into two
 * components of the same size, which is what the parallel component pass is
 * for.  The union-find path must reproduce the serial seed scan exactly --
 * same component ids, same BFS order, same adjacency -- for any thread count,
 * because the renumbering it feeds decides fill and arithmetic order. */
static int two_supply_nets(void)
{
    vsdlss *A=powergrid(40,2);CHECK(A);
    csi n=A->n,saved=vsdlss_components_uf_min;
    vsdlss_components *cs=NULL;vsdlss_wgraph *gs=NULL;

    CHECK(vsdlss_set_num_threads(1)==VSDLSS_OK);
    vsdlss_components_uf_min=(csi)1<<40;          /* force the serial scan */
    CHECK(vsdlss_components_build_graph(A,&cs,&gs)==VSDLSS_OK);
    CHECK(cs->count==2);
    CHECK(cs->offset[1]-cs->offset[0]==cs->offset[2]-cs->offset[1]);
    CHECK(cs->offset[2]==n);

    if(vsdlss_parallel_enabled()) for(int nt=2;nt<=4;nt*=2){
        vsdlss_components *cp=NULL;vsdlss_wgraph *gp=NULL;csi nz=gs->ptr[n];
        CHECK(vsdlss_set_num_threads(nt)==VSDLSS_OK);
        vsdlss_components_uf_min=1;               /* force the union-find path */
        CHECK(vsdlss_components_build_graph(A,&cp,&gp)==VSDLSS_OK);
        CHECK(cp->count==cs->count);
        CHECK(memcmp(cp->offset,cs->offset,(size_t)(cs->count+1)*sizeof(csi))==0);
        CHECK(memcmp(cp->vertices,cs->vertices,(size_t)n*sizeof(csi))==0);
        CHECK(memcmp(cp->component_of,cs->component_of,(size_t)n*sizeof(csi))==0);
        CHECK(memcmp(cp->local_of,cs->local_of,(size_t)n*sizeof(csi))==0);
        CHECK(memcmp(cp->order,cs->order,(size_t)n*sizeof(csi))==0);
        CHECK(memcmp(gp->ptr,gs->ptr,(size_t)(n+1)*sizeof(csi))==0);
        CHECK(memcmp(gp->idx,gs->idx,(size_t)nz*sizeof(csi))==0);
        CHECK(memcmp(gp->val,gs->val,(size_t)nz*sizeof(double))==0);
        CHECK(memcmp(gp->diag,gs->diag,(size_t)n*sizeof(double))==0);
        vsdlss_components_free(cp);vsdlss_wgraph_free(gp);
    }
    vsdlss_components_uf_min=saved;
    vsdlss_components_free(cs);vsdlss_wgraph_free(gs);vsdlss_spfree(A);
    CHECK(vsdlss_set_num_threads(1)==VSDLSS_OK);
    return 0;
}

static int large_powergrid(void)
{
    /* one component of ~150k (blocked reduction + BFS renumbering), plus a
     * second copy so the component loop and gather maps are exercised */
    vsdlss *A=powergrid(85,2);CHECK(A);CHECK(A->n/2>=((csi)1<<17));
    csi n=A->n,nrhs=3;
    double *b=malloc((size_t)n*nrhs*sizeof(double)),*x1=malloc((size_t)n*nrhs*sizeof(double));
    double *xt=malloc((size_t)n*nrhs*sizeof(double)),*xm=malloc((size_t)n*nrhs*sizeof(double));
    CHECK(b&&x1&&xt&&xm);
    for(csi i=0;i<n*nrhs;i++)b[i]=cos(0.001*(double)i)-0.5;
    for(int order=0;order<=5;order+=5){
        for(int nt=1;nt<=4;nt*=2){
            vsdlss_m3_factor *f=NULL;double eta;
            if(nt>1&&!vsdlss_parallel_enabled())break;
            CHECK(vsdlss_set_num_threads(nt)==VSDLSS_OK);
            CHECK(vsdlss_factorize_m3(A,order,&f)==VSDLSS_OK);
            CHECK(f->count==2&&f->component[0].gather!=NULL);
            double *x=nt==1?x1:xt;
            for(csi r=0;r<nrhs;r++)CHECK(vsdlss_m3_solve(f,b+r*n,x+r*n)==VSDLSS_OK);
            for(csi r=0;r<nrhs;r++)CHECK(vsdlss_backward_error(A,x+r*n,b+r*n,&eta)==VSDLSS_OK&&eta<1e-13);
            CHECK(vsdlss_m3_solve_many(f,nrhs,b,n,xm,n)==VSDLSS_OK);
            CHECK(memcmp(xm,x,(size_t)(n*nrhs)*sizeof(double))==0);
            if(nt>1)CHECK(memcmp(x1,xt,(size_t)(n*nrhs)*sizeof(double))==0);
            vsdlss_m3_factor_free(f);
        }
    }
    /* reduced M4 goes through the same renumbered preprocessing */
    { vsdlss_m4_reduced_factor *f=NULL;double eta;
      CHECK(vsdlss_set_num_threads(1)==VSDLSS_OK);
      CHECK(vsdlss_factorize_m4_reduced(A,5,(size_t)64<<20,NULL,&f)==VSDLSS_OK);
      CHECK(vsdlss_m4_reduced_solve(f,b,xt)==VSDLSS_OK);
      CHECK(vsdlss_backward_error(A,xt,b,&eta)==VSDLSS_OK&&eta<1e-13);
      vsdlss_m4_reduced_free(f); }
    free(b);free(x1);free(xt);free(xm);vsdlss_spfree(A);return 0;
}

int main(int argc,char **argv)
{
    int only_large=argc>1&&argv[1][0]=='L';
    static const struct {csi n;int per,blocks,dense;} cases[]={
        {1,0,1,0},{2,1,1,0},{17,2,1,0},{60,3,2,1},{200,2,1,0},{400,4,3,2},
        {900,3,1,0},{1500,5,2,3},{2000,2,5,0},{1200,6,1,4}};
    for(size_t c=0;!only_large&&c<sizeof(cases)/sizeof(*cases);c++){
        vsdlss *A=random_spd(cases[c].n,cases[c].per,cases[c].blocks,cases[c].dense);
        CHECK(A);
        if(one_matrix(A)){fprintf(stderr,"case %zu (n=%lld)\n",c,(long long)cases[c].n);return 1;}
        vsdlss_spfree(A);
    }
    CHECK(two_supply_nets()==0);
    CHECK(large_powergrid()==0);
    CHECK(vsdlss_set_num_threads(1)==VSDLSS_OK);
    puts("test_supernodal: random SPD, orders 0-5, strict/relaxed, 1/2/4 threads, power-grid reorder/blocked reduction, two supply nets: ALL OK");
    return 0;
}
