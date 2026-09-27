/* Link-only diagnostic wrappers. Serial attribution avoids summing overlapping
 * worker times. Buckets are actual factor widths, not original graph degrees. */
#define _POSIX_C_SOURCE 200809L
#include "../src/vsdlss_m3_internal.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
static int enabled,done,sample_offset;
static unsigned long long samples[2][7];
static double clock_cost;
static double panel_time[2][7],diag_time[7],core_time,reduce_time[2];
static unsigned long long calls[2][7];
static int bucket(csi w){return w<=6?0:w<32?1:w<64?2:w<128?3:w<256?4:w<512?5:6;}
double solve_profile_clock(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+t.tv_nsec*1e-9;}
void solve_profile_diag_add(long long w,double t){diag_time[bucket(w)]+=t;}
vsdlss_status profile_panel_solve(const double*,csi,csi,csi,const csi*,double*,int);
vsdlss_status __real_vsdlss_panel_solve(const double*,csi,csi,csi,const csi*,double*,int);
vsdlss_status __wrap_vsdlss_panel_solve(const double*a,csi b,csi w,csi e,const csi*idx,double*x,int back){
 if(!enabled)return __real_vsdlss_panel_solve(a,b,w,e,idx,x,back);
 int k=bucket(w);unsigned long long call=++calls[back][k];
 if(w<64 && ((call+(unsigned)sample_offset)&63))return __real_vsdlss_panel_solve(a,b,w,e,idx,x,back);
 double t=solve_profile_clock();vsdlss_status s=w>=64?profile_panel_solve(a,b,w,e,idx,x,back):__real_vsdlss_panel_solve(a,b,w,e,idx,x,back);
 double elapsed=solve_profile_clock()-t-clock_cost;
 panel_time[back][k]+=elapsed>0?elapsed:0;samples[back][k]++;return s;
}
vsdlss_status __real_vsdlss_sn_solve_inplace(const vsdlss_sn_factor*,double*);
vsdlss_status __wrap_vsdlss_sn_solve_inplace(const vsdlss_sn_factor*f,double*x){
 if(!enabled)return __real_vsdlss_sn_solve_inplace(f,x);
 double t=solve_profile_clock();vsdlss_status s=__real_vsdlss_sn_solve_inplace(f,x);core_time+=solve_profile_clock()-t;return s;
}
vsdlss_status __real_vsdlss_reduce_forward_inplace(const vsdlss_reduction*,double*,double*);
vsdlss_status __wrap_vsdlss_reduce_forward_inplace(const vsdlss_reduction*r,double*x,double*s){
 if(!enabled)return __real_vsdlss_reduce_forward_inplace(r,x,s);
 double t=solve_profile_clock();vsdlss_status st=__real_vsdlss_reduce_forward_inplace(r,x,s);reduce_time[0]+=solve_profile_clock()-t;return st;
}
vsdlss_status __real_vsdlss_reduce_backward_inplace(const vsdlss_reduction*,const double*,double*);
vsdlss_status __wrap_vsdlss_reduce_backward_inplace(const vsdlss_reduction*r,const double*s,double*x){
 if(!enabled)return __real_vsdlss_reduce_backward_inplace(r,s,x);
 double t=solve_profile_clock();vsdlss_status st=__real_vsdlss_reduce_backward_inplace(r,s,x);reduce_time[1]+=solve_profile_clock()-t;return st;
}
static int cmp(const void*a,const void*b){double x=*(const double*)a,y=*(const double*)b;return (x>y)-(x<y);}
vsdlss_status __real_vsdlss_m3_solve(const vsdlss_m3_factor*,const double*,double*);
vsdlss_status __wrap_vsdlss_m3_solve(const vsdlss_m3_factor*f,const double*b,double*x){
 if(done)return __real_vsdlss_m3_solve(f,b,x);
 done=1;
 if(vsdlss_get_num_threads()!=1){fprintf(stderr,"profiling requires threads=1\n");return VSDLSS_ERR_INVALID;}
 unsigned long long count[7]={0},nodes[7]={0},bytes[7]={0};csi maxw=0,core_n=0,reduced=0;
 for(csi c=0;c<f->count;c++){
  reduced+=f->component[c].reduction->count;core_n+=f->component[c].reduction->core_n;
  const vsdlss_sn_factor*n=f->component[c].numeric;if(!n)continue;
  for(csi s=0;s<n->count;s++){csi w=n->column_start[s+1]-n->column_start[s],e=n->row_ptr[s+1]-n->row_ptr[s];int k=bucket(w);count[k]++;nodes[k]+=w;bytes[k]+=(unsigned long long)w*(w+e)*8;if(w>maxw)maxw=w;}
 }
 printf("PROFILE_STRUCTURE n=%lld core_n=%lld reduced=%lld max_width=%lld\n",(long long)f->n,(long long)core_n,(long long)reduced,(long long)maxw);
 for(int k=0;k<7;k++)printf("PROFILE_SHAPE bucket=%d count=%llu columns=%llu bytes=%llu\n",k,count[k],nodes[k],bytes[k]);
 double *reference=malloc((size_t)f->n*8);if(!reference)return VSDLSS_ERR_OOM;
 vsdlss_status st=__real_vsdlss_m3_solve(f,b,reference);if(st!=VSDLSS_OK){free(reference);return st;}
 double clocks[1001];for(int i=0;i<1001;i++){double t=solve_profile_clock();clocks[i]=solve_profile_clock()-t;}qsort(clocks,1001,8,cmp);clock_cost=clocks[500];printf("PROFILE_CLOCK seconds=%.9f small_panel_sampling=1/64\n",clock_cost);
 double base[7];
 for(int r=0;r<7;r++){double t=solve_profile_clock();st=__real_vsdlss_m3_solve(f,b,x);base[r]=solve_profile_clock()-t;if(st!=VSDLSS_OK){free(reference);return st;}}
 qsort(base,7,8,cmp);printf("PROFILE_BASELINE median=%.9f min=%.9f max=%.9f\n",base[3],base[0],base[6]);
 for(int r=0;r<7;r++){
  memset(panel_time,0,sizeof panel_time);memset(diag_time,0,sizeof diag_time);memset(calls,0,sizeof calls);memset(samples,0,sizeof samples);sample_offset=r*17;core_time=reduce_time[0]=reduce_time[1]=0;enabled=1;
  double t=solve_profile_clock();st=__real_vsdlss_m3_solve(f,b,x);double total=solve_profile_clock()-t;enabled=0;
  if(st!=VSDLSS_OK||memcmp(reference,x,(size_t)f->n*8)){fprintf(stderr,"profile changed solution\n");free(reference);return VSDLSS_ERR_INVALID;}
  printf("PROFILE_RUN run=%d total=%.9f core=%.9f reduce_fwd=%.9f reduce_back=%.9f other=%.9f checked=bitwise\n",r,total,core_time,reduce_time[0],reduce_time[1],total-core_time-reduce_time[0]-reduce_time[1]);
  for(int side=0;side<2;side++)for(int k=0;k<7;k++)if(samples[side][k])panel_time[side][k]*=(double)calls[side][k]/samples[side][k];
  for(int k=0;k<7;k++)printf("PROFILE_BUCKET run=%d bucket=%d fwd=%.9f back=%.9f back_diag=%.9f calls_fwd=%llu calls_back=%llu\n",r,k,panel_time[0][k],panel_time[1][k],diag_time[k],calls[0][k],calls[1][k]);
 }
 free(reference);fflush(stdout);return st;
}
