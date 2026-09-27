#define _POSIX_C_SOURCE 200809L
#include "../src/vsdlss_m3_internal.h"
#include <stdlib.h>
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
 {
  /* Widths 7..40 (the register-resident forward update and the 8-column /
     padded-tail backward dots) against the generic kernel and against the
     previous kernels (vsdlss_solve_v2 = 0): ext across the 16/8/4 register
     blocks, the 128-row forward block and the 1024-entry gather buffer;
     indices mixing consecutive runs and gaps. */
  enum{EMAX=1100,XN=4*EMAX+64};
  double *A=malloc(sizeof(double)*40*(40+EMAX)),*o=malloc(sizeof(double)*XN),
         *x0=malloc(sizeof(double)*XN),*x1=malloc(sizeof(double)*XN),*x2=malloc(sizeof(double)*XN);
  csi *ix=malloc(sizeof(csi)*EMAX);
  CHECK(A&&o&&x0&&x1&&x2&&ix);
  for(int i=0;i<XN;i++)o[i]=sin(0.37*i+0.5);
  csi at=48;for(csi r=0;r<EMAX;r++){ix[r]=at;at+=(r%7==3)?3:1;}
  static const csi exts[]={0,1,2,3,4,5,7,8,11,15,16,17,31,33,63,127,128,129,200,1023,1024,1025,1100};
  int saved=vsdlss_solve_v2;
  for(csi w=7;w<=40;w++)for(size_t ei=0;ei<sizeof exts/sizeof*exts;ei++)for(int back=0;back<2;back++)for(int fault=0;fault<4;fault++){
   csi ext=exts[ei],rows=w+ext;
   for(csi j=0;j<w;j++)for(csi r=0;r<rows;r++)A[j*rows+r]=r<j?0:r==j?2+0.1*j:0.01*cos(r+3*j);
   if(fault==1)A[(w/2)*rows+w/2]=-1;
   if(fault==2)A[(w-1)*rows+w-1]=NAN;
   if(fault==3&&ext)A[(w-1)*rows+rows-1]=INFINITY;
   memcpy(x0,o,sizeof(double)*XN);memcpy(x1,o,sizeof(double)*XN);memcpy(x2,o,sizeof(double)*XN);
   vsdlss_status s0=vsdlss_panel_solve_generic(A,3,w,ext,ext?ix:NULL,x0,back);
   vsdlss_solve_v2=3;vsdlss_status s1=vsdlss_panel_solve(A,3,w,ext,ext?ix:NULL,x1,back);
   vsdlss_solve_v2=0;vsdlss_status s2=vsdlss_panel_solve(A,3,w,ext,ext?ix:NULL,x2,back);
   CHECK(s0==s1&&s0==s2);
   if(s0==VSDLSS_OK||fault==3){CHECK(memcmp(x0,x1,sizeof(double)*XN)==0);CHECK(memcmp(x0,x2,sizeof(double)*XN)==0);}
   /* fused backward (bit 4) with several interleave steps, fallback on faults */
   if(back){ static const int ks[]={1,3,16}; csi fm=vsdlss_fuse_min; int fk=vsdlss_fuse_k; vsdlss_fuse_min=4;
    for(int kk=0;kk<3;kk++){ vsdlss_fuse_k=ks[kk]; memcpy(x1,o,sizeof(double)*XN);
     vsdlss_solve_v2=7; vsdlss_status s3=vsdlss_panel_solve(A,3,w,ext,ext?ix:NULL,x1,back);
     CHECK(s3==s0); if(s0==VSDLSS_OK||fault==3)CHECK(memcmp(x0,x1,sizeof(double)*XN)==0); }
    vsdlss_fuse_min=fm; vsdlss_fuse_k=fk; }
  }
  vsdlss_solve_v2=saved;
  free(A);free(o);free(x0);free(x1);free(x2);free(ix);
  puts("small: widths 7..40, ext 0..1100, new, fused and previous kernels bitwise equal to generic");
 }
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
