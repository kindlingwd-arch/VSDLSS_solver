/* MKL PARDISO (SPD) on a PG_DUMP system (upper CSC, int64: n, nnz, p, i, x, b).
 *   ./bench_pardiso file ordering(0=AMD,2=METIS,3=parallel METIS) "threads list" [reps]
 *   env: PARDISO_FACT_PAR = iparm[23] (0 classic, 1 two-level factorization);
 *        PARDISO_SOLVE_PAR = "0 1 2": iparm[24] values timed in turn on the same
 *        factor (0 sequential, 1/2 parallel forward/backward solve);
 *        PARDISO_REFINE = iparm[7] (default 0); iparm[6] reports the
 *        refinement steps actually performed. */
#define _POSIX_C_SOURCE 200809L
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void pardisoinit(void *pt, const int *mtype, int *iparm);
void pardiso(void *pt, const int *maxfct, const int *mnum, const int *mtype, const int *phase,
             const int *n, const double *a, const int *ia, const int *ja, int *perm,
             const int *nrhs, int *iparm, const int *msglvl, double *b, double *x, int *error);
void MKL_Set_Num_Threads(int);   /* C interface; the lowercase symbol is the Fortran one (int *) */

#define DBG(s) do{ if(getenv("PDEBUG")) fprintf(stderr,"[dbg] %s\n",s); }while(0)
static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+t.tv_nsec*1e-9;}
static int cmpd(const void *a,const void *b){double x=*(const double*)a,y=*(const double*)b;return (x>y)-(x<y);}
static long peak_rss_mb(void){FILE *f=fopen("/proc/self/status","r");char l[256];long kb=-1;if(!f)return -1;
    while(fgets(l,sizeof l,f))if(!strncmp(l,"VmHWM:",6)){kb=atol(l+6);break;}fclose(f);return kb/1024;}
static const char *ord_name(int o){return o==0?"AMD":o==2?"METIS":o==3?"METIS-par":"other";}

int main(int argc,char **argv)
{
    if(argc<4){fprintf(stderr,"usage: bench_pardiso file ordering threads_list [reps]\n");return 1;}
    int ord=atoi(argv[2]); int reps=argc>4?atoi(argv[4]):7; if(reps>64)reps=64;
    FILE *f=fopen(argv[1],"rb"); if(!f){perror("open");return 1;}
    int64_t n64,nz64; if(fread(&n64,8,1,f)!=1||fread(&nz64,8,1,f)!=1)return 1;
    int64_t *cp=malloc((n64+1)*8),*ci=malloc(nz64*8); double *cx=malloc(nz64*8),*b=malloc(n64*8);
    if(fread(cp,8,n64+1,f)!=(size_t)(n64+1)||fread(ci,8,nz64,f)!=(size_t)nz64||fread(cx,8,nz64,f)!=(size_t)nz64||fread(b,8,n64,f)!=(size_t)n64)return 1;
    fclose(f);
    if(n64>INT32_MAX||nz64>INT32_MAX){puts("too large for 32-bit PARDISO");return 1;}
    const int n=(int)n64;
    /* Upper CSC column j holds rows i<=j; PARDISO wants upper CSR (row i: cols j>=i). */
    int *ia=calloc(n+1,sizeof(int)),*ja=malloc(nz64*sizeof(int)); double *a=malloc(nz64*8);
    for(int64_t k=0;k<nz64;k++) ia[ci[k]+1]++;
    for(int i=0;i<n;i++) ia[i+1]+=ia[i];
    int *pos=malloc(n*sizeof(int)); memcpy(pos,ia,n*sizeof(int));
    for(int64_t j=0;j<n64;j++) for(int64_t k=cp[j];k<cp[j+1];k++){int i=(int)ci[k];ja[pos[i]]=(int)j;a[pos[i]++]=cx[k];}
    free(pos); DBG("csr built");
    double *x=malloc(n64*8);
    const char *e;
    const int fact_par=(e=getenv("PARDISO_FACT_PAR"))?atoi(e):0;
    const int refine=(e=getenv("PARDISO_REFINE"))?atoi(e):0;
    char tbuf[256]; strncpy(tbuf,argv[3],255); tbuf[255]=0;
    char *tsave=NULL;
    for(char *tok=strtok_r(tbuf," ",&tsave);tok;tok=strtok_r(NULL," ",&tsave)){
        int t=atoi(tok); MKL_Set_Num_Threads(t);
        void *pt[64]={0}; int iparm[64]={0}, mtype=2, maxfct=1, mnum=1, nrhs=1, msglvl=0, err=0, phase;
        DBG("init"); pardisoinit(pt,&mtype,iparm); DBG("init done");
        iparm[0]=1; iparm[1]=ord; iparm[34]=1; /* zero-based */ iparm[26]=0;
        iparm[23]=fact_par; iparm[7]=refine;
        double t0=now(); phase=11; DBG("phase 11");
        pardiso(pt,&maxfct,&mnum,&mtype,&phase,&n,a,ia,ja,NULL,&nrhs,iparm,&msglvl,NULL,NULL,&err);
        if(err){printf("analysis error %d\n",err);return 1;}
        double ta=now()-t0; t0=now(); phase=22; DBG("phase 22");
        pardiso(pt,&maxfct,&mnum,&mtype,&phase,&n,a,ia,ja,NULL,&nrhs,iparm,&msglvl,NULL,NULL,&err);
        if(err){printf("factor error %d\n",err);return 1;}
        double tf=now()-t0;
        printf("PARDISO ord=%s threads=%d fact_par=%d | analyze=%.2fs factor=%.2fs total=%.2fs | nnz(L)=%.1fM GFLOP=%.1f | "
               "mem(KB) analysis=%d factor+solve=%d | peak_rss=%ld MB\n",
               ord_name(ord),t,iparm[23],ta,tf,ta+tf,iparm[17]*1e-6,iparm[18]*1e-3,iparm[14],iparm[15]+iparm[16],peak_rss_mb());
        fflush(stdout);
        char sbuf[64]; e=getenv("PARDISO_SOLVE_PAR"); strncpy(sbuf,e?e:"0",63); sbuf[63]=0;
        char *ssave=NULL;
        for(char *sm=strtok_r(sbuf," ",&ssave);sm;sm=strtok_r(NULL," ",&ssave)){
            double ts[64]; phase=33; iparm[24]=atoi(sm);
            for(int r=-1;r<reps;r++){t0=now();pardiso(pt,&maxfct,&mnum,&mtype,&phase,&n,a,ia,ja,NULL,&nrhs,iparm,&msglvl,b,x,&err);
                if(err){printf("  solve error %d (iparm[24]=%d)\n",err,iparm[24]);break;} if(r>=0)ts[r]=now()-t0;}
            if(err){err=0;continue;}
            qsort(ts,reps,8,cmpd);
            /* backward error |b-Ax|inf/(|A|inf|x|inf+|b|inf) from the upper CSC */
            double *r=calloc(n64,8),*rs=calloc(n64,8),rn=0,an=0,xn=0,bn=0;
            for(int64_t j=0;j<n64;j++)for(int64_t k=cp[j];k<cp[j+1];k++){int64_t i=ci[k];double v=cx[k];
                r[i]+=v*x[j];rs[i]+=fabs(v);if(i!=j){r[j]+=v*x[i];rs[j]+=fabs(v);}}
            for(int64_t i=0;i<n64;i++){double d=fabs(b[i]-r[i]);if(d>rn)rn=d;if(rs[i]>an)an=rs[i];if(fabs(x[i])>xn)xn=fabs(x[i]);if(fabs(b[i])>bn)bn=fabs(b[i]);}
            free(r);free(rs);
            { const char *xp=getenv("XDUMP"); if(xp){ FILE *xf=fopen(xp,"wb"); if(!xf||fwrite(x,8,(size_t)n64,xf)!=(size_t)n64){puts("xdump failed");return 1;} fclose(xf);} }
            printf("  solve_par=%d refine_steps=%d | solve med=%.4fs min=%.4fs | berr=%.1e | peak_rss=%ld MB\n",
                   iparm[24],iparm[6],ts[reps/2],ts[0],rn/(an*xn+bn),peak_rss_mb());
            fflush(stdout);
        }
        phase=-1; pardiso(pt,&maxfct,&mnum,&mtype,&phase,&n,NULL,ia,ja,NULL,&nrhs,iparm,&msglvl,NULL,NULL,&err);
    }
    return 0;
}
