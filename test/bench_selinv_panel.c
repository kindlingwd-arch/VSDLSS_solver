/* Panel micro-benchmark: triangular (KV=2) vs inverted diagonal block,
 * forward and backward, hot (one panel) or cold (copies beyond the cache).
 * usage: bench_selinv_panel w e back hot */
#define _POSIX_C_SOURCE 200809L
#include "../src/vsdlss_m3_internal.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+t.tv_nsec*1e-9;}
int main(int argc,char**argv){
  if(argc<5){ puts("usage: bench_selinv_panel w e back hot"); return 1; }
  csi w=atoi(argv[1]),e=atoi(argv[2]); int back=atoi(argv[3]),hot=atoi(argv[4]);
  csi rows=w+e, ps=rows*w; csi copies=hot?1:(csi)(256e6/8/ps)+1;
  double *a=malloc(copies*ps*8),*ai=malloc(copies*ps*8); csi n=w+4*e+16;
  double *x=malloc(n*8),*x0=malloc(n*8); csi *idx=malloc((e+1)*sizeof(csi));
  for(csi c=0;c<copies;c++) for(csi j=0;j<w;j++) for(csi r=0;r<rows;r++)
    a[c*ps+j*rows+r]= r==j?2.0+0.1*j:(r>j?0.01*((r*7+j)%13)/13.0:0);
  memcpy(ai,a,copies*ps*8);
  { vsdlss_sn_factor f; memset(&f,0,sizeof f); csi cs[2]={0,w}, rp[2]={0,e}, po[2]={0,ps};
    f.n=w; f.count=1; f.column_start=cs; f.row_ptr=rp; f.panel_offset=po;
    for(csi c=0;c<copies;c++){ f.panel=ai+c*ps; f.selinv_hi=0; if(vsdlss_sn_selinv(&f,1,w)!=VSDLSS_OK){puts("selinv failed");return 1;} } }
  for(csi t=0;t<e;t++) idx[t]=w+4*t+(t%3);
  for(csi i=0;i<n;i++) x0[i]=1+0.001*i;
  double best[2]={1e9,1e9};
  csi calls=hot?(csi)(2e8/ps)+10:copies;
  for(int rep=0;rep<5;rep++) for(int v=0;v<2;v++){
    memcpy(x,x0,n*8); double t0=now();
    for(csi c=0;c<calls;c++){
      const double *p=(v?ai:a)+(hot?0:c)*ps;
      if(v) vsdlss_panel_solve_inv(p,0,w,e,e?idx:NULL,x,back); else vsdlss_panel_solve(p,0,w,e,e?idx:NULL,x,back);
      if((c&63)==0) memcpy(x,x0,n*8);
    }
    double t=(now()-t0)/calls; if(t<best[v]) best[v]=t;
  }
  printf("w=%lld e=%lld %s %s: triangular %.1f ns, inverse %.1f ns (%.2fx)\n",(long long)w,(long long)e,back?"bwd":"fwd",hot?"hot ":"cold",best[0]*1e9,best[1]*1e9,best[0]/best[1]);
  return 0;}
