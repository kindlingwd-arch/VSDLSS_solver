/* Internal dense-node solve replay.
 *
 *   ./bench_sn_replay dump <matrix.bin> <order> <out.sn>
 *   ./bench_sn_replay run  <in.sn> [reps] [x_out.bin]
 *
 * dump: read a PG_DUMP matrix (bench_pg_solve / bench_pg_profile format:
 * n, nnz, p[n+1], i[nnz], x[nnz], ...), factor it with vsdlss_factorize_m3
 * and write the numeric supernodal factor of every component's core: column
 * ranges, row lists, dense panels and the solve tree / source blocks.
 *
 * run: load the dump and time ONLY the triangular solves over those dense
 * panels -- no reduction replay, no permutation, no gather / write-back:
 *   - forward  : vsdlss_panel_solve(back = 0) over supernodes ascending
 *   - backward : vsdlss_panel_solve(back = 1) over supernodes descending
 *   - per width class (1-6, 7-32, 33-128, >128): the same passes restricted
 *     to that class (the other panels are skipped), so their shares show
 * Single thread only: the tree-parallel path needs the factor's cached solve
 * tree, which is private to the numeric module (bench_pg_solve covers it).
 * The dump is independent of the code version, so two builds replaying the
 * same file compare kernels on bitwise identical data.  x_out.bin receives
 * the serial forward+backward result (all components concatenated) for an
 * accuracy comparison between builds. */
#define _POSIX_C_SOURCE 200809L
#include "../src/vsdlss_m3_internal.h"
#include "../src/vsdlss_simd.h"
#include <string.h>
#include <time.h>

static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+t.tv_nsec*1e-9;}
static void die(const char *s){fprintf(stderr,"bench_sn_replay: %s\n",s);exit(1);}
static void wr(FILE *f,const void *p,size_t z,size_t n){if(n&&fwrite(p,z,n,f)!=n)die("write failed");}
static void rd(FILE *f,void *p,size_t z,size_t n){if(n&&fread(p,z,n,f)!=n)die("read failed (truncated dump?)");}
static void *xmalloc(size_t n){void *p=malloc(n?n:1);if(!p)die("out of memory");return p;}
#define MAGIC 0x31504D4453534C56ULL   /* "VLSSDMP1" */

static int dump(const char *in,int order,const char *out)
{
    FILE *f=fopen(in,"rb");if(!f)die("cannot open matrix");
    int64_t n,nz;rd(f,&n,8,1);rd(f,&nz,8,1);
    vsdlss *A=vsdlss_spalloc(n,n,nz,1,0);if(!A)die("alloc matrix");
    rd(f,A->p,8,(size_t)n+1);rd(f,A->i,8,(size_t)nz);rd(f,A->x,8,(size_t)nz);fclose(f);
    vsdlss_m3_factor *F=NULL;double t=now();
    if(vsdlss_factorize_m3(A,order,&F)!=VSDLSS_OK)die("factorization failed");
    printf("# factor order=%d time=%.2fs components=%lld\n",order,now()-t,(long long)F->count);
    vsdlss_spfree(A);
    FILE *o=fopen(out,"wb");if(!o)die("cannot open output");
    int64_t magic=(int64_t)MAGIC,nc=0;
    for(csi c=0;c<F->count;c++)if(F->component[c].numeric)nc++;
    wr(o,&magic,8,1);wr(o,&nc,8,1);
    for(csi c=0;c<F->count;c++){
        const vsdlss_sn_factor *s=F->component[c].numeric;if(!s)continue;
        const csi K=s->count,nrow=s->row_ptr[K],np=s->panel_offset[K],nb=s->blk_ptr?s->blk_ptr[K]:0;
        int64_t h[6]={s->n,K,s->l_nnz,nrow,np,s->blk_ptr?nb:-1};
        wr(o,h,8,6);
        wr(o,s->column_start,8,(size_t)K+1);wr(o,s->row_ptr,8,(size_t)K+1);wr(o,s->row_index,8,(size_t)nrow);
        wr(o,s->panel_offset,8,(size_t)K+1);wr(o,s->panel,8,(size_t)np);
        int64_t hp=s->sn_parent!=NULL;wr(o,&hp,8,1);
        if(hp)wr(o,s->sn_parent,8,(size_t)K);
        if(s->blk_ptr){wr(o,s->blk_ptr,8,(size_t)K+1);wr(o,s->blk_src,8,(size_t)nb);
                       wr(o,s->blk_first,8,(size_t)nb);wr(o,s->blk_end,8,(size_t)nb);}
        printf("# component %lld: core n=%lld supernodes=%lld nnzL=%.1fM panel=%.1f MB\n",(long long)c,
               (long long)s->n,(long long)K,s->l_nnz*1e-6,np*8e-6);
    }
    fclose(o);vsdlss_m3_factor_free(F);return 0;
}

static vsdlss_sn_factor *load_one(FILE *f)
{
    int64_t h[6];rd(f,h,8,6);
    vsdlss_sn_factor *s=calloc(1,sizeof(*s));if(!s)die("alloc");
    const csi K=h[1],nrow=h[3],np=h[4],nb=h[5];
    s->n=h[0];s->count=K;s->l_nnz=h[2];
    s->column_start=xmalloc(((size_t)K+1)*8);rd(f,s->column_start,8,(size_t)K+1);
    s->row_ptr=xmalloc(((size_t)K+1)*8);rd(f,s->row_ptr,8,(size_t)K+1);
    s->row_index=xmalloc((size_t)nrow*8);rd(f,s->row_index,8,(size_t)nrow);
    s->panel_offset=xmalloc(((size_t)K+1)*8);rd(f,s->panel_offset,8,(size_t)K+1);
    s->panel=xmalloc((size_t)np*8);rd(f,s->panel,8,(size_t)np);
    int64_t hp;rd(f,&hp,8,1);
    if(hp){s->sn_parent=xmalloc((size_t)K*8);rd(f,s->sn_parent,8,(size_t)K);}
    if(nb>=0){
        s->blk_ptr=xmalloc(((size_t)K+1)*8);rd(f,s->blk_ptr,8,(size_t)K+1);
        s->blk_src=xmalloc((size_t)nb*8);rd(f,s->blk_src,8,(size_t)nb);
        s->blk_first=xmalloc((size_t)nb*8);rd(f,s->blk_first,8,(size_t)nb);
        s->blk_end=xmalloc((size_t)nb*8);rd(f,s->blk_end,8,(size_t)nb);
    }
    return s;
}

/* One pass over the panels of s; cls < 0: all widths, else only class cls. */
static int wclass(csi w){return w<=6?0:w<=32?1:w<=128?2:3;}
static const char *cname[4]={"1-6","7-32","33-128",">128"};
static void pass(const vsdlss_sn_factor *s,double *x,int back,int cls)
{
    for(csi t=0;t<s->count;t++){
        const csi q=back?s->count-1-t:t;
        const csi b=s->column_start[q],w=s->column_start[q+1]-b,e=s->row_ptr[q+1]-s->row_ptr[q];
        if(cls>=0&&wclass(w)!=cls)continue;
        if(vsdlss_panel_solve(s->panel+s->panel_offset[q],b,w,e,e?s->row_index+s->row_ptr[q]:NULL,x,back)!=VSDLSS_OK)
            die("panel solve failed");
    }
}
static uint64_t fnv(const double *x,csi n){uint64_t h=1469598103934665603ULL;const unsigned char *p=(const unsigned char*)x;
    for(size_t i=0;i<(size_t)n*8;i++){h^=p[i];h*=1099511628211ULL;}return h;}

static int run(const char *in,int reps,const char *xout)
{
    FILE *f=fopen(in,"rb");if(!f)die("cannot open dump");
    int64_t magic,nc;rd(f,&magic,8,1);rd(f,&nc,8,1);if(magic!=(int64_t)MAGIC)die("not a supernode dump");
    vsdlss_sn_factor **S=xmalloc((size_t)nc*sizeof(*S));csi ntot=0,maxn=0;
    for(int64_t c=0;c<nc;c++){S[c]=load_one(f);ntot+=S[c]->n;if(S[c]->n>maxn)maxn=S[c]->n;}
    fclose(f);
    /* width statistics: supernodes and stored panel entries per class */
    double cnt[4]={0},ent[4]={0},all=0;
    for(int64_t c=0;c<nc;c++)for(csi q=0;q<S[c]->count;q++){
        csi w=S[c]->column_start[q+1]-S[c]->column_start[q],e=S[c]->row_ptr[q+1]-S[c]->row_ptr[q];
        double z=(double)w*(w+1)/2+(double)w*e;cnt[wclass(w)]++;ent[wclass(w)]+=z;all+=z;}
    printf("# dump: components=%lld core n=%lld simd=%d\n",(long long)nc,(long long)ntot,vsdlss_simd_enabled());
    for(int k=0;k<4;k++)printf("#   width %-7s supernodes=%-9.0f share of L=%5.1f%%\n",cname[k],cnt[k],100*ent[k]/all);
    double **x0=xmalloc((size_t)nc*sizeof(double*)),*x=xmalloc((size_t)maxn*8);
    for(int64_t c=0;c<nc;c++){x0[c]=xmalloc((size_t)S[c]->n*8);
        for(csi i=0;i<S[c]->n;i++)x0[c][i]=1.0+0.25*sin(0.37*(double)i+(double)c);}
    if(vsdlss_set_num_threads(1)!=VSDLSS_OK)die("threads");
    /* rows: -1 = all widths, 0..3 = width class; min over reps, summed over components */
    printf("scope\tforward_ms\tbackward_ms\n");
    for(int cls=-1;cls<4;cls++){
        double tf=0,tb=0;
        for(int64_t c=0;c<nc;c++){
            double bf=1e30,bb=1e30;
            for(int r=-1;r<reps;r++){
                memcpy(x,x0[c],(size_t)S[c]->n*8);
                double t0=now();pass(S[c],x,0,cls);double t1=now();pass(S[c],x,1,cls);double t2=now();
                if(r>=0){if(t1-t0<bf)bf=t1-t0;if(t2-t1<bb)bb=t2-t1;}
            }
            tf+=bf;tb+=bb;
        }
        printf("%s\t%.3f\t%.3f\n",cls<0?"all":cname[cls],tf*1e3,tb*1e3);
    }
    /* result hashes: forward only, forward+backward (serial) */
    uint64_t hf=1469598103934665603ULL,hs=hf;FILE *xo=xout?fopen(xout,"wb"):NULL;
    for(int64_t c=0;c<nc;c++){
        memcpy(x,x0[c],(size_t)S[c]->n*8);pass(S[c],x,0,-1);hf^=fnv(x,S[c]->n);
        pass(S[c],x,1,-1);hs^=fnv(x,S[c]->n);if(xo)wr(xo,x,8,(size_t)S[c]->n);
    }
    if(xo)fclose(xo);
    printf("# hash forward=%016llx forward+backward=%016llx\n",(unsigned long long)hf,(unsigned long long)hs);
    return 0;
}

int main(int argc,char **argv)
{
    if(argc>=5&&!strcmp(argv[1],"dump"))return dump(argv[2],atoi(argv[3]),argv[4]);
    if(argc>=3&&!strcmp(argv[1],"run"))
        return run(argv[2],argc>3?atoi(argv[3]):7,argc>4?argv[4]:NULL);
    fprintf(stderr,"usage: %s dump <matrix.bin> <order> <out.sn> | run <in.sn> [reps] [x_out.bin]\n",argv[0]);
    return 2;
}
