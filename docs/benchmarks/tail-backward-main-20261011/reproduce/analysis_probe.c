#define main original_phase_main
#include "../../../../test/bench_phase.c"
#undef main
#include "../../../../src/vsdlss_ledger.h"
extern csi vsdlss_reduce_tail_grain;

/* Read-only diagnostic of main production functions; see the companion evidence manifest.
 * No new replay kernel: the timed functions are main's production functions. */
int main(int argc, char **argv)
{
    if(argc!=2) return 2;
    vsdlss *A=read_dump(argv[1]);
    if(!A || vsdlss_set_num_threads(8)!=VSDLSS_OK) return 2;
    vsdlss_m3_factor *F=NULL; double start=now();
    if(vsdlss_factorize_m3(A,6,&F)!=VSDLSS_OK) return 2;
    printf("factor_s=%.6f n=%lld components=%lld chain=%s grain=%s\n",now()-start,(long long)A->n,(long long)F->count,getenv("VSDLSS_CHAIN_ORDER")?getenv("VSDLSS_CHAIN_ORDER"):"0",getenv("VSDLSS_TAIL_GRAIN")?getenv("VSDLSS_TAIL_GRAIN"):"2048");
    fflush(stdout);
    for(csi c=0;c<F->count;c++) {
        const vsdlss_reduction *r=F->component[c].reduction;
        if(!r || !r->blocks || !r->tail_levels || !r->pk || r->records) continue;
        const vsdlss_pk_seg *g=r->pk+r->blocks; const csi cnt=g->count;
        uint32_t *depth=calloc((size_t)r->n,sizeof(uint32_t));
        csi *hist=calloc((size_t)cnt+1,sizeof(csi));
        if(!depth||!hist) return 2;
        csi degs[4]={0},bad=0,core_edges=0,maxdepth=0,o=g->nbn;
        for(csi i=cnt;i>0;) {
            --i; const csi v=vsdlss_pk_vertex(g,i),d=vsdlss_pk_degree(g,i); o-=d; degs[d]++;
            uint32_t dl=0;
            for(csi j=0;j<d;j++) {
                const uint32_t w=g->nb[o+j];
                if(w>=r->count) core_edges++;
                if(!g->head && w>=g->k0 && w<g->k0+cnt && w<=v) bad++;
                if(depth[w]>dl) dl=depth[w];
            }
            depth[v]=++dl; hist[dl]++; if(dl>(uint32_t)maxdepth) maxdepth=dl;
        }
        csi minw=cnt,maxw=0,narrow[4]={0};
        for(csi l=0;l<r->tail_levels;l++) {
            csi width=r->tail_chunk_rec[r->tail_level[l+1]]-r->tail_chunk_rec[r->tail_level[l]];
            if(width<minw) minw=width;
            if(width>maxw) maxw=width;
            for(int t=0;t<4;t++) if(width<2048*((csi)1<<t)) narrow[t]+=width;
        }
        double ideal[4]={0};
        for(csi l=1;l<=maxdepth;l++) for(int t=0;t<4;t++) ideal[t]+=(double)((hist[l]+((csi)1<<t)-1)/((csi)1<<t));
        printf("component=%lld records=%lld tail=%lld tail_pct=%.6f levels=%lld mean=%lld min_width=%lld max_width=%lld core=%lld compact=%d d0=%lld d1=%lld d2=%lld d3=%lld core_edges=%lld bad_dependency=%lld backward_only_depth=%lld backward_only_ideal_8=%.6f narrow_records_2=%lld narrow_records_4=%lld narrow_records_8=%lld\n",
            (long long)c,(long long)r->count,(long long)cnt,100.0*cnt/r->count,(long long)r->tail_levels,(long long)(cnt/r->tail_levels),(long long)minw,(long long)maxw,(long long)r->core_n,!g->head,(long long)degs[0],(long long)degs[1],(long long)degs[2],(long long)degs[3],(long long)core_edges,(long long)bad,(long long)maxdepth,cnt/ideal[3],(long long)narrow[1],(long long)narrow[2],(long long)narrow[3]);
        free(depth);free(hist); if(bad) return 2;
        fflush(stdout);
        vsdlss_pk_seg *seg=malloc((size_t)r->pk_count*sizeof(*seg));
        csi *bp=malloc((size_t)(r->blocks+1)*sizeof(csi));
        double *w0=malloc((size_t)r->n*8),*w=malloc((size_t)r->n*8),*ref=malloc((size_t)r->n*8);
        if(!seg||!bp||!w0||!w||!ref) return 2;
        vsdlss_reduction v=view(r,2,seg,bp);
        for(csi i=0;i<r->n;i++) w0[i]=sin(0.37*(double)i)+0.2;
        vsdlss_set_num_threads(1); memcpy(ref,w0,(size_t)r->n*8);
        if(vsdlss_reduce_forward_inplace(&v,ref,NULL)!=VSDLSS_OK || vsdlss_reduce_backward_inplace(&v,NULL,ref)!=VSDLSS_OK) return 2;
        for(int t=1;t<=8;t*=2) {
            double f[15],b[15]; int ft=0,bt=0;
            for(int k=-2;k<15;k++) {
                memcpy(w,w0,(size_t)r->n*8); vsdlss_set_num_threads(t);
                double s=now(); vsdlss_status st=vsdlss_reduce_forward_inplace(&v,w,NULL); double fd=now()-s;
                ft=vsdlss_parallel_last_team_size(); vsdlss_set_num_threads(t);
                s=now(); vsdlss_status sb=vsdlss_reduce_backward_inplace(&v,NULL,w); double bd=now()-s;
                bt=vsdlss_parallel_last_team_size();
                if(st!=VSDLSS_OK||sb!=VSDLSS_OK||memcmp(w,ref,(size_t)r->n*8)) return 3;
                if(k>=0) {f[k]=fd;b[k]=bd;printf("sample component=%lld threads=%d k=%d fwd_ms=%.9f bwd_ms=%.9f\n",(long long)c,t,k,1e3*fd,1e3*bd);}
            }
            qsort(f,15,8,cmpd);qsort(b,15,8,cmpd);
            printf("TAIL component=%lld threads=%d observed_fwd=%d observed_bwd=%d fwd_min_ms=%.9f fwd_median_ms=%.9f fwd_max_ms=%.9f bwd_min_ms=%.9f bwd_median_ms=%.9f bwd_max_ms=%.9f bitwise_each_sample=1\n",(long long)c,t,ft,bt,1e3*f[0],1e3*f[7],1e3*f[14],1e3*b[0],1e3*b[7],1e3*b[14]);
        }
        /* Change only the existing runtime grain knob on the same factor.
         * Alternate the two settings; this is a diagnostic, not a default change. */
        if(!getenv("VSDLSS_TAIL_GRAIN")) for(int t=4;t<=8;t*=2) {
            double times[2][15]; int team[2]={0};
            for(int k=-2;k<15;k++) for(int pass=0;pass<2;pass++) {
                const int a=(k+2+pass)&1;
                vsdlss_reduce_tail_grain=a?256:2048;
                memcpy(w,w0,(size_t)r->n*8);vsdlss_set_num_threads(t);
                if(vsdlss_reduce_forward_inplace(&v,w,NULL)!=VSDLSS_OK) return 3;
                vsdlss_set_num_threads(t);const double s=now();
                const vsdlss_status st=vsdlss_reduce_backward_inplace(&v,NULL,w);const double dt=now()-s;
                team[a]=vsdlss_parallel_last_team_size();
                if(st!=VSDLSS_OK||memcmp(w,ref,(size_t)r->n*8)) return 3;
                if(k>=0) {times[a][k]=dt;printf("grain_sample component=%lld threads=%d grain=%d k=%d bwd_ms=%.9f\n",(long long)c,t,a?256:2048,k,1e3*dt);}
            }
            for(int a=0;a<2;a++) {qsort(times[a],15,8,cmpd);printf("GRAIN component=%lld threads=%d grain=%d observed_bwd=%d min_ms=%.9f median_ms=%.9f max_ms=%.9f bitwise_each_sample=1\n",(long long)c,t,a?256:2048,team[a],1e3*times[a][0],1e3*times[a][7],1e3*times[a][14]);}
        }
        vsdlss_reduce_tail_grain=2048;
        free(seg);free(bp);free(w0);free(w);free(ref);
    }
    double *rhs=malloc((size_t)A->n*8),*x=malloc((size_t)A->n*8),*ref=malloc((size_t)A->n*8);
    if(!rhs||!x||!ref) return 2;
    for(csi i=0;i<A->n;i++) rhs[i]=sin(0.37*(double)i)+0.2;
    vsdlss_set_num_threads(1);
    if(vsdlss_m3_solve(F,rhs,ref)!=VSDLSS_OK) return 2;
    vsdlss_ledger_set_level(1);
    for(int t=1;t<=8;t*=2) {
        double times[15]; vsdlss_set_num_threads(t);
        for(int k=-2;k<15;k++) {if(k==0) vsdlss_ledger_reset(); double s=now();vsdlss_status st=vsdlss_m3_solve(F,rhs,x);double dt=now()-s;
            if(st!=VSDLSS_OK||memcmp(x,ref,(size_t)A->n*8)) return 3;
            if(k>=0) {times[k]=dt;printf("solve_sample threads=%d k=%d ms=%.9f\n",t,k,1e3*dt);}}
        qsort(times,15,8,cmpd);printf("SOLVE threads=%d min_ms=%.9f median_ms=%.9f max_ms=%.9f bitwise_each_sample=1\n",t,1e3*times[0],1e3*times[7],1e3*times[14]);
        printf("LEDGER threads=%d mean_gather_ms=%.9f mean_red_fwd_ms=%.9f mean_core_fwd_ms=%.9f mean_core_bwd_ms=%.9f mean_red_bwd_ms=%.9f mean_writeback_ms=%.9f mean_total_ms=%.9f\n",t,vsdlss_ledger.wall[LG_PERM_GATHER]*1e3/15,vsdlss_ledger.wall[LG_RED_FWD]*1e3/15,vsdlss_ledger.wall[LG_CORE_FWD]*1e3/15,vsdlss_ledger.wall[LG_CORE_BWD]*1e3/15,vsdlss_ledger.wall[LG_RED_BWD]*1e3/15,vsdlss_ledger.wall[LG_WRITEBACK]*1e3/15,vsdlss_ledger.wall[LG_TOTAL]*1e3/15);
    }
    double eta=0; if(vsdlss_backward_error(A,x,rhs,&eta)!=VSDLSS_OK||!isfinite(eta)||eta>1e-10) return 3;
    printf("backward_error=%.12e\n",eta);
    free(rhs);free(x);free(ref);vsdlss_m3_factor_free(F);vsdlss_spfree(A);return 0;
}
