#define _POSIX_C_SOURCE 200809L
#include "../src/vsdlss_m3_internal.h"
#include <string.h>
#include <time.h>
#define CHECK(e) do {if(!(e)){fprintf(stderr,"small: line %d\n",__LINE__);return 1;}}while(0)
static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+1e-9*t.tv_nsec;}
int main(void){
 double a[6*80],original[160],x[160],y[160];csi idx[64];
 for(csi r=0;r<64;r++)idx[r]=20+2*r;
 for(int i=0;i<160;i++)original[i]=sin(i+0.5);
 for(csi w=1;w<=6;w++)for(csi ext=0;ext<=64;ext++)for(int back=0;back<2;back++)for(int fault=0;fault<4;fault++){
  csi rows=w+ext;memset(a,0,sizeof(a));
  for(csi j=0;j<w;j++)for(csi r=j;r<rows;r++)a[j*rows+r]=r==j?2:0.01*cos(r+j);
  if(fault==1)a[0]=0;
  if(fault==2)a[0]=NAN;
  if(fault==3&&ext)a[rows-1]=INFINITY;
  memcpy(x,original,sizeof(x));memcpy(y,original,sizeof(y));
  vsdlss_status s=vsdlss_panel_solve_generic(a,3,w,ext,idx,x,back);
  CHECK(s==vsdlss_panel_solve(a,3,w,ext,idx,y,back));
  CHECK(memcmp(x,y,sizeof(x))==0);
 }
 puts("small: all widths, directions, ext 0..64, strided indices, faults bitwise OK");
 puts("width generic_ms specialized_ms speedup (50000 calls, ext=16)");
 for(csi w=1;w<=6;w++){
  csi rows=w+16;memset(a,0,sizeof(a));
  for(csi j=0;j<w;j++)for(csi r=j;r<rows;r++)a[j*rows+r]=r==j?2:0.01;
  double elapsed[2];
  for(int mode=0;mode<2;mode++){
   double start=now();
   for(int rep=0;rep<50000;rep++){
    memcpy(x,original,sizeof(x));
    vsdlss_status s=mode?vsdlss_panel_solve(a,3,w,16,idx,x,rep%2):vsdlss_panel_solve_generic(a,3,w,16,idx,x,rep%2);
    CHECK(s==VSDLSS_OK);
   }
   elapsed[mode]=now()-start;
  }
  printf("%lld %.3f %.3f %.2f\n",(long long)w,elapsed[0]*1000,elapsed[1]*1000,elapsed[0]/elapsed[1]);
 }
 return 0;
}
