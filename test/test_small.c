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
 /* Packed panels (vsdlss_sn_factor layout, 32-bit indices) against the
    full-layout kernels: same status and bits for every dispatch path
    (narrow kernels, SIMD backward, generic, heap gather above 1024 rows). */
 {
  enum{MW=40,ME=1100,NX=2*ME+64};
  static double full[MW*(MW+ME)],packed[MW*(MW+ME)],px[NX],py[NX],po[NX];
  static csi fidx[ME]; static vsdlss_sni pidx[ME];
  const csi widths[]={1,2,3,4,5,6,7,8,9,13,40}, exts[]={0,1,3,17,64,130,1100};
  /* external rows lie below the panel columns [5, 5+w), as in a factor */
  for(csi r=0;r<ME;r++){fidx[r]=5+MW+2*r;pidx[r]=(vsdlss_sni)fidx[r];}
  for(int i=0;i<NX;i++)po[i]=sin(0.7*i+0.1);
  for(size_t wi=0;wi<sizeof widths/sizeof *widths;wi++)for(size_t ei=0;ei<sizeof exts/sizeof *exts;ei++)
  for(int back=0;back<2;back++)for(int fault=0;fault<4;fault++)for(int gen=0;gen<2;gen++){
   const csi w=widths[wi],ext=exts[ei],rows=w+ext;
   memset(full,0,sizeof(full));
   for(csi j=0;j<w;j++)for(csi r=j;r<rows;r++)full[j*rows+r]=r==j?2+0.1*j:0.01*cos(r+3*j);
   if(fault==1)full[(w-1)*rows+w-1]=0;
   if(fault==2)full[0]=NAN;
   if(fault==3&&ext)full[rows-1]=INFINITY;
   for(csi j=0;j<w;j++){
    for(csi r=j;r<w;r++)packed[VSDLSS_SN_DCOL(j,w)+r-j]=full[j*rows+r];
    for(csi r=0;r<ext;r++)packed[w*(w+1)/2+j*ext+r]=full[j*rows+w+r];
   }
   memcpy(px,po,sizeof(px));memcpy(py,po,sizeof(py));
   vsdlss_status sf=gen?vsdlss_panel_solve_generic(full,5,w,ext,fidx,px,back):vsdlss_panel_solve(full,5,w,ext,fidx,px,back);
   vsdlss_status sp=gen?vsdlss_sn_panel_solve_generic(packed,5,w,ext,pidx,py,back):vsdlss_sn_panel_solve(packed,5,w,ext,pidx,py,back);
   CHECK(sf==sp);
   CHECK(memcmp(px,py,sizeof(px))==0);
  }
  puts("small: packed panels bitwise equal to full panels (all dispatch paths, faults)");
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
