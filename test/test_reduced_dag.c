#define _POSIX_C_SOURCE 200809L
#include "../src/vsdlss_m3_internal.h"
#include <string.h>
#include <time.h>
static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+1e-9*t.tv_nsec;}
#define CHECK(e) do{if(!(e)){fprintf(stderr,"reduced/dag line %d\n",__LINE__);return 1;}}while(0)
static vsdlss *grid(int side){
 csi n=side*side*side,p=0;vsdlss *a=vsdlss_spalloc(n,n,4*n,1,0);if(!a)return NULL;
 for(csi j=0;j<n;j++){
  a->p[j]=p;
  if(j>=side*side){a->i[p]=j-side*side;a->x[p++]=-1;}
  if((j/side)%side){a->i[p]=j-side;a->x[p++]=-1;}
  if(j%side){a->i[p]=j-1;a->x[p++]=-1;}
  a->i[p]=j;a->x[p++]=7;
 }a->p[n]=p;return a;
}
int main(void){
 /* Three components: isolated vertex, degree-1 tail attached to K5,
    and a two-node path. Mixed empty and nonempty reduced cores. */
 vsdlss *a=vsdlss_spalloc(9,9,30,1,0);CHECK(a);csi p=0;
 for(csi j=0;j<9;j++){
  a->p[j]=p;
  for(csi i=0;i<j;i++)if((i>=1&&i<=5&&j<=5)||(i==5&&j==6)||(i==7&&j==8)){
   a->i[p]=i;a->x[p++]=-1;
  }
  a->i[p]=j;a->x[p++]=8;
 }a->p[9]=p;
 double truth[9],rhs[9],x[9];for(int i=0;i<9;i++)truth[i]=0.3+i;
 CHECK(vsdlss_spmv_sym_upper(a,truth,rhs)==VSDLSS_OK);
 for(int order=0;order<=4;order++){
  vsdlss_m4_reduced_factor *f=NULL;
  CHECK(vsdlss_factorize_m4_reduced(a,order,65536,NULL,&f)==VSDLSS_OK);
  CHECK(f->count==3);csi disks=0;for(csi c=0;c<f->count;c++)if(f->component[c].disk)disks++;
  CHECK(disks==1);
  for(int r=0;r<3;r++){
   memcpy(x,rhs,sizeof(x));CHECK(vsdlss_m4_reduced_solve(f,x,x)==VSDLSS_OK);
   for(int i=0;i<9;i++)CHECK(fabs(x[i]-truth[i])<1e-12);
  }
  double bad[9];memcpy(bad,rhs,sizeof(bad));bad[8]=NAN;
  for(int i=0;i<9;i++)x[i]=123;
  CHECK(vsdlss_m4_reduced_solve(f,bad,x)==VSDLSS_ERR_NONFINITE);
  for(int i=0;i<9;i++)CHECK(x[i]==123);
  vsdlss_m4_reduced_free(f);
 }
 vsdlss_m4_reduced_factor *f=NULL;
 CHECK(vsdlss_factorize_m4_reduced(a,2,0,NULL,&f)!=VSDLSS_OK&&f==NULL);
 vsdlss_spfree(a);
 a=vsdlss_spalloc(10,10,30,1,0);CHECK(a);p=0;
 for(csi j=0;j<10;j++){
  a->p[j]=p;for(csi i=(j/5)*5;i<=j;i++){a->i[p]=i;a->x[p++]=i==j?8:-1;}
 }a->p[10]=p;
 CHECK(vsdlss_factorize_m4_reduced(a,2,65536,NULL,&f)==VSDLSS_OK);
 CHECK(f->count==2&&f->component[0].disk&&f->component[1].disk);
 CHECK(vsdlss_m4_workspace_bytes(f->component[0].disk)+vsdlss_m4_workspace_bytes(f->component[1].disk)<=65536);
 vsdlss_m4_reduced_free(f);f=NULL;
 a->x[a->p[10]-1]=-1;
 CHECK(vsdlss_factorize_m4_reduced(a,2,65536,NULL,&f)==VSDLSS_ERR_NOT_POSDEF&&f==NULL);
 vsdlss_spfree(a);
 a=vsdlss_spalloc(1,1,1,1,0);CHECK(a);a->p[0]=0;a->p[1]=1;a->i[0]=0;a->x[0]=2;
 CHECK(vsdlss_factorize_m4_reduced(a,2,0,NULL,&f)==VSDLSS_OK);
 double scalar=6;CHECK(vsdlss_m4_reduced_solve(f,&scalar,&scalar)==VSDLSS_OK&&scalar==3);
 vsdlss_m4_reduced_free(f);vsdlss_spfree(a);
 a=grid(8);CHECK(a);csi n=a->n;
 double *b=malloc(n*sizeof(double)),*ref=malloc(n*sizeof(double)),*out=malloc(n*sizeof(double));CHECK(b&&ref&&out);
 for(csi i=0;i<n;i++)b[i]=sin(i+0.2);
 vsdlss_sn_symbolic *s=NULL;vsdlss_sn_factor *one=NULL,*many=NULL;
 CHECK(vsdlss_sn_analyze(a,&s)==VSDLSS_OK&&s->count>1);
 CHECK(vsdlss_set_num_threads(1)==VSDLSS_OK);
 double start=now();
 CHECK(vsdlss_sn_factorize(a,s,&one)==VSDLSS_OK);
 printf("serial numeric_ms=%.3f supernodes=%lld\n",1000*(now()-start),(long long)s->count);
 CHECK(vsdlss_sn_solve(one,b,ref)==VSDLSS_OK);
 vsdlss_set_dag_enabled(1);
 if(vsdlss_parallel_enabled())for(int nt=2;nt<=4;nt*=2){
  CHECK(vsdlss_set_num_threads(nt)==VSDLSS_OK);
  double start=now();
  CHECK(vsdlss_sn_factorize(a,s,&many)==VSDLSS_OK);
  printf("DAG threads=%d numeric_ms=%.3f supernodes=%lld\n",nt,1000*(now()-start),(long long)s->count);
  CHECK(vsdlss_parallel_last_team_size()>1);
  CHECK(memcmp(one->panel,many->panel,(size_t)s->panel_offset[s->count]*sizeof(double))==0);
  CHECK(vsdlss_sn_solve(many,b,out)==VSDLSS_OK);
  CHECK(memcmp(ref,out,n*sizeof(double))==0);
  double eta;CHECK(vsdlss_backward_error(a,out,b,&eta)==VSDLSS_OK&&eta<1e-12);
  vsdlss_sn_factor_free(many);many=NULL;
 }
 a->x[a->p[n-1+1]-1]=-1;
 CHECK(vsdlss_sn_factorize(a,s,&many)==VSDLSS_ERR_NOT_POSDEF&&many==NULL);
 vsdlss_sn_factor_free(one);vsdlss_sn_symbolic_free(s);vsdlss_spfree(a);free(b);free(ref);free(out);
 CHECK(vsdlss_set_num_threads(1)==VSDLSS_OK);
 vsdlss_set_dag_enabled(0);
 puts("reduced M4 and single-component DAG: ALL OK");return 0;
}
