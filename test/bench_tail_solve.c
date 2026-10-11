/* Same input generator and same harness for both production-code versions. */
#define main unused_replay_benchmark_main
#include "bench_tail.c"
#undef main
#include "../src/vsdlss_simd.h"
/* GNU ld wrappers reset observation at the replay entry, so a larger core
 * team cannot masquerade as a tail team. Both versions link these wrappers;
 * the only timed instrumentation is a few TLS loads/stores per replay. */
static _Thread_local int forward_team, backward_team;
vsdlss_status __real_vsdlss_reduce_forward_inplace(const vsdlss_reduction *,double *,double *);
vsdlss_status __real_vsdlss_reduce_backward_inplace(const vsdlss_reduction *,const double *,double *);
vsdlss_status __wrap_vsdlss_reduce_forward_inplace(const vsdlss_reduction *r,double *x,double *s)
{
    vsdlss_set_num_threads(vsdlss_get_num_threads());
    const vsdlss_status st=__real_vsdlss_reduce_forward_inplace(r,x,s);
    forward_team=vsdlss_parallel_last_team_size(); return st;
}
vsdlss_status __wrap_vsdlss_reduce_backward_inplace(const vsdlss_reduction *r,const double *s,double *x)
{
    vsdlss_set_num_threads(vsdlss_get_num_threads());
    const vsdlss_status st=__real_vsdlss_reduce_backward_inplace(r,s,x);
    backward_team=vsdlss_parallel_last_team_size(); return st;
}
int main(void)
{
    const csi n=2000000; const int reps=21;
    const char *grain=getenv("VSDLSS_TAIL_GRAIN");
    if(grain && strcmp(grain,"2048")) { fputs("benchmark requires VSDLSS_TAIL_GRAIN unset or 2048\n",stderr); return 1; }
    printf("grain=2048 simd=%d\n",vsdlss_simd_enabled());
    if(vsdlss_set_num_threads(8)!=VSDLSS_OK) return 1;
    vsdlss *A=build(n,20,200); if(!A) return 1;
    vsdlss_m3_factor *f=NULL;
    double start=now();
    if(!checked(vsdlss_factorize_m3(A,6,&f),"factor")) return 1;
    printf("factor_s=%.6f components=%lld\n",now()-start,(long long)f->count);
    if(f->count!=1) { fputs("benchmark expects one component\n",stderr); return 1; }
    int capacity=0;
    for(csi c=0;c<f->count;c++) {
        const vsdlss_reduction *r=f->component[c].reduction;
        const csi tail=r->blocks?r->pk[r->blocks].count:0;
        printf("component=%lld tail=%lld levels=%lld records_per_level=%lld core=%lld relabel=%d\n",
            (long long)c,(long long)tail,(long long)r->tail_levels,
            (long long)(r->tail_levels?tail/r->tail_levels:0),(long long)r->core_n,
            r->blocks&&!r->pk[r->blocks].head);
        if(!r->tail_levels || tail/r->tail_levels<2048*2 || r->pk[r->blocks].head) { fputs("full solve lacks active relabelled tail\n",stderr); return 1; }
        capacity=(int)(tail/r->tail_levels/2048);
    }
    double *b=malloc((size_t)n*8),*x=malloc((size_t)n*8),*ref=malloc((size_t)n*8);
    if(!b||!x||!ref) return 1;
    for(csi i=0;i<n;i++) b[i]=sin(0.37*(double)i)+0.2;
    vsdlss_set_num_threads(1);
    if(!checked(vsdlss_m3_solve(f,b,ref),"reference")) return 1;
    for(int t=1;t<=8;t*=2) {
        double times[reps];
        for(int k=-2;k<reps;k++) {
            if(!checked(vsdlss_set_num_threads(t),"set threads")) return 1;
            const double s=now(); const vsdlss_status st=vsdlss_m3_solve(f,b,x); const double dt=now()-s;
            if(!checked(st,"solve") || memcmp(x,ref,(size_t)n*8)) { fputs("solve differs\n",stderr); return 1; }
            const int expected=t<capacity?t:capacity;
            if(forward_team!=expected || backward_team!=expected) { fputs("unexpected tail team size or missing instrumentation\n",stderr); return 1; }
            if(k>=0) times[k]=dt;
        }
        qsort(times,reps,sizeof(double),cmpd);
        printf("SOLVE threads=%d observed_tail_fwd=%d observed_tail_bwd=%d min_ms=%.6f median_ms=%.6f max_ms=%.6f bitwise_same=1\n",
            t,forward_team,backward_team,times[0]*1e3,times[reps/2]*1e3,times[reps-1]*1e3);
    }
    double eta;
    if(!checked(vsdlss_backward_error(A,x,b,&eta),"residual") || !isfinite(eta) || eta>1e-10) return 1;
    uint64_t h=1469598103934665603ULL; const unsigned char *p=(const unsigned char *)x;
    for(size_t i=0;i<(size_t)n*8;i++) {h^=p[i];h*=1099511628211ULL;}
    printf("xhash=%016llx backward_error=%.9e\n",(unsigned long long)h,eta);
    free(b);free(x);free(ref);vsdlss_m3_factor_free(f);vsdlss_spfree(A);return 0;
}
