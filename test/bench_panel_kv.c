/* Panel-shape micro-benchmark: kernel generation KVA vs KVB (compile-time,
 * default 1 vs 2), one (w, e) shape, cold (working set > cache) or hot.
 *   make bench_panel_kv; ./bench_panel_kv w e back [hot] */
#ifndef KVA
#define KVA 1
#endif
#ifndef KVB
#define KVB 2
#endif
#define _POSIX_C_SOURCE 200809L
#include "../src/vsdlss_m3_internal.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
extern int vsdlss_solve_kv;
vsdlss_status vsdlss_panel_solve(const double*,csi,csi,csi,const csi*,double*,int);
static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+t.tv_nsec*1e-9;}
int main(int argc,char**argv){
  csi w=atoi(argv[1]),e=atoi(argv[2]); int back=atoi(argv[3]); int hot=argc>4?atoi(argv[4]):0;
  csi rows=w+e, ps=rows*w; csi copies=hot?1:(csi)(256e6/8/ps)+1;
  double *a=malloc(copies*ps*8); csi n=w+4*e+16; double *x=malloc(n*8), *x0=malloc(n*8); csi *idx=malloc(e*sizeof(csi));
  for(csi c=0;c<copies;c++) for(csi j=0;j<w;j++) for(csi r=0;r<rows;r++) a[c*ps+j*rows+r]= r==j?2.0+0.1*j:(r>j?0.01*((r*7+j)%13)/13.0:0);
  for(csi t=0;t<e;t++) idx[t]=w+4*t+(t%3);
  for(csi i=0;i<n;i++) x0[i]=1+0.001*i;
  double best[4]={1e9,1e9,1e9,1e9};
  for(int rep=0;rep<5;rep++) for(int kv=KVA;kv<=KVB;kv+=KVB-KVA){
    vsdlss_solve_kv=kv; memcpy(x,x0,n*8);
    double t0=now(); csi calls=hot?(csi)(2e8/ps)+10:copies;
    for(csi c=0;c<calls;c++){ vsdlss_panel_solve(a+(hot?0:c)*ps,0,w,e,idx,x,back); if((c&63)==0) memcpy(x,x0,n*8);}
    double t=(now()-t0)/calls; if(t<best[kv]) best[kv]=t;
  }
  printf("w=%lld e=%lld %s %s: old %.1f ns new %.1f ns (%.2fx) | %.2f GB/s new\n",(long long)w,(long long)e,back?"bwd":"fwd",hot?"hot":"cold",
     best[KVA]*1e9,best[KVB]*1e9,best[KVA]/best[KVB],8.0*ps/best[KVB]/1e9);
  return 0;}
