#define _POSIX_C_SOURCE 200809L
#include "../src/vsdlss_m4_internal.h"
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <signal.h>
#include <dirent.h>
#define CHECK(e) do { if (!(e)) { fprintf(stderr,"FAIL %d: %s\n",__LINE__,#e); return 1; } } while (0)
static uint64_t u64(FILE *fp)
{
    unsigned char b[8]; uint64_t x=0;
    if (fread(b,1,8,fp)!=8) return UINT64_MAX;
    for (int i=0;i<8;i++) x|=(uint64_t)b[i]<<(8*i);
    return x;
}
static int accuracy(void)
{
    const csi n=100;
    vsdlss *A=vsdlss_spalloc(n,n,3*n,1,0);
    double truth[100],rhs[100],x[100],ref[100],dense[10000],L[10000];
    CHECK(A);
    memset(dense,0,sizeof(dense));
    csi nz=0;
    for (csi j=0;j<n;j++) {
        A->p[j]=nz;
        if (j>=10) { A->i[nz]=j-10; A->x[nz++]=-1; dense[(j-10)*n+j]=dense[j*n+j-10]=-1; }
        if (j%10) { A->i[nz]=j-1; A->x[nz++]=-1; dense[(j-1)*n+j]=dense[j*n+j-1]=-1; }
        A->i[nz]=j; A->x[nz++]=5; dense[j*n+j]=5;
    }
    A->p[n]=nz;
    for (int order=0;order<=4;order++) {
        vsdlss_m4_factor *f=NULL;
        vsdlss_factor *m1=NULL;
        CHECK(vsdlss_factorize_m4(A,order,8192,NULL,&f)==VSDLSS_OK);
        CHECK(vsdlss_m4_workspace_bytes(f)<=8192);
        CHECK(vsdlss_factorize(A,order,&m1)==VSDLSS_OK);
        memset(L,0,sizeof(L));
        for (csi j=0;j<n;j++) {
            CHECK(fseeko(f->file,(off_t)f->offset[j],SEEK_SET)==0);
            for (uint64_t p=0;p<f->count[j];p++) {
                uint64_t row=u64(f->file),bits=u64(f->file); double v;
                CHECK(row<(uint64_t)n); memcpy(&v,&bits,8); L[row*n+j]=v;
            }
        }
        for (csi i=0;i<n;i++) for (csi j=0;j<n;j++) {
            double sum=0;
            for (csi k=0;k<n;k++) sum+=L[i*n+k]*L[j*n+k];
            CHECK(isfinite(sum)&&fabs(sum-dense[f->q[i]*n+f->q[j]])<1e-12);
        }
        for (int r=0;r<3;r++) {
            for (csi i=0;i<n;i++) truth[i]=(i%2?-1:1)*(0.2+i*0.03+r);
            CHECK(vsdlss_spmv_sym_upper(A,truth,rhs)==VSDLSS_OK);
            CHECK(vsdlss_factor_solve(m1,rhs,ref)==VSDLSS_OK);
            memcpy(x,rhs,sizeof(x));
            CHECK(vsdlss_m4_solve(f,x,x)==VSDLSS_OK);
            for (csi i=0;i<n;i++) CHECK(isfinite(x[i])&&fabs(x[i]-ref[i])<1e-11&&fabs(x[i]-truth[i])<1e-11);
            double eta;
            CHECK(vsdlss_backward_error(A,x,rhs,&eta)==VSDLSS_OK&&eta<1e-12);
        }
        CHECK(fseeko(f->file,0,SEEK_END)==0);
        CHECK((uint64_t)ftello(f->file)>8192);
        vsdlss_factor_free(m1); vsdlss_m4_factor_free(f);
    }
    vsdlss_spfree(A); return 0;
}
static int failures(void)
{
    csi p[]={0,1,3},i[]={0,0,1}; double a[]={4,1,3};
    vsdlss A={3,2,2,p,i,a,-1}; vsdlss_m4_factor *f=NULL;
    double rhs[]={7,-1},x[]={91,92};
    CHECK(vsdlss_factorize_m4(&A,2,1,NULL,&f)==VSDLSS_ERR_OOM&&f==NULL);
    CHECK(vsdlss_factorize_m4(&A,7,8192,NULL,&f)==VSDLSS_ERR_UNSUPPORTED&&f==NULL);
    CHECK(vsdlss_factorize_m4(&A,2,8192,"/no-such-m4-dir/child",&f)==VSDLSS_ERR_IO&&f==NULL);
    CHECK(vsdlss_factorize_m4(NULL,2,8192,NULL,&f)==VSDLSS_ERR_INVALID&&f==NULL);
    vsdlss huge={0,INT64_MAX,INT64_MAX,p,i,a,-1};
    CHECK(vsdlss_factorize_m4(&huge,2,SIZE_MAX,NULL,&f)==VSDLSS_ERR_OOM&&f==NULL);
    a[2]=0; CHECK(vsdlss_factorize_m4(&A,2,8192,NULL,&f)==VSDLSS_ERR_NOT_POSDEF&&f==NULL);
    a[2]=NAN; CHECK(vsdlss_factorize_m4(&A,2,8192,NULL,&f)==VSDLSS_ERR_NONFINITE&&f==NULL); a[2]=3;
    for (int damage=0;damage<6;damage++) {
        CHECK(vsdlss_factorize_m4(&A,2,8192,NULL,&f)==VSDLSS_OK);
        rhs[0]=INFINITY;
        CHECK(vsdlss_m4_solve(f,rhs,x)==VSDLSS_ERR_NONFINITE&&x[0]==91&&x[1]==92); rhs[0]=7;
        if (damage==0) CHECK(ftruncate(fileno(f->file),10)==0);
        if (damage==1) CHECK(ftruncate(fileno(f->file),(off_t)(f->offset[1]+8))==0);
        if (damage==2) { CHECK(fseeko(f->file,8,SEEK_SET)==0); CHECK(fputc(2,f->file)!=EOF); }
        if (damage==3) { CHECK(fseeko(f->file,48,SEEK_SET)==0); CHECK(fputc(1,f->file)!=EOF); }
        if (damage==4) { CHECK(fseeko(f->file,(off_t)f->offset[0],SEEK_SET)==0); CHECK(fputc(9,f->file)!=EOF); }
        if (damage==5) { CHECK(fseeko(f->file,0,SEEK_END)==0); CHECK(fputc(1,f->file)!=EOF); }
        CHECK(vsdlss_m4_solve(f,rhs,x)==VSDLSS_ERR_IO&&x[0]==91&&x[1]==92);
        vsdlss_m4_factor_free(f); f=NULL;
    }
    /* Kernel-enforced partial write; no production test hooks. */
    pid_t pid=fork(); CHECK(pid>=0);
    if (pid==0) {
        struct rlimit lim={110,110};
        signal(SIGXFSZ,SIG_IGN);
        if (setrlimit(RLIMIT_FSIZE,&lim)!=0) _exit(2);
        vsdlss_status st=vsdlss_factorize_m4(&A,2,8192,NULL,&f);
        _exit(st==VSDLSS_ERR_IO&&f==NULL?0:1);
    }
    int result; CHECK(waitpid(pid,&result,0)==pid&&WIFEXITED(result)&&WEXITSTATUS(result)==0);
    vsdlss_m4_factor_free(NULL); return 0;
}
static int edges_and_cleanup(void)
{
    char directory[]="/tmp/vsdlss-m4-test-XXXXXX";
    csi p[]={0,2,5,6},i[]={0,0,1,0,0,2};
    double a[]={2,2,3,0.25,0.75,2};
    vsdlss A={6,3,3,p,i,a,-1};
    vsdlss_m4_factor *f=NULL;
    double rhs[]={7,-1,6},x[]={0,0,0};
    CHECK(mkdtemp(directory));
    CHECK(vsdlss_factorize_m4(&A,1,8192,directory,&f)==VSDLSS_OK);
    size_t need=vsdlss_m4_workspace_bytes(f);
    CHECK(vsdlss_m4_solve(f,rhs,x)==VSDLSS_OK);
    CHECK(fabs(x[0]-2)<1e-12&&fabs(x[1]+1)<1e-12&&fabs(x[2]-3)<1e-12);
    DIR *dir=opendir(directory); struct dirent *entry;
    CHECK(dir);
    while ((entry=readdir(dir))) CHECK(!strcmp(entry->d_name,".")||!strcmp(entry->d_name,".."));
    CHECK(closedir(dir)==0);
    vsdlss_m4_factor_free(f); f=NULL;
    CHECK(vsdlss_factorize_m4(&A,1,need-1,directory,&f)==VSDLSS_ERR_OOM&&f==NULL);
    CHECK(vsdlss_factorize_m4(&A,1,need,directory,&f)==VSDLSS_OK);
    vsdlss_m4_factor_free(f); f=NULL;
    a[5]=-2;
    CHECK(vsdlss_factorize_m4(&A,1,8192,directory,&f)==VSDLSS_ERR_NOT_POSDEF&&f==NULL);
    CHECK(rmdir(directory)==0);
    csi sp[]={0,1},si[]={0}; double sa[]={9};
    vsdlss single={1,1,1,sp,si,sa,-1}; rhs[0]=18;
    CHECK(vsdlss_factorize_m4(&single,2,8192,NULL,&f)==VSDLSS_OK);
    CHECK(vsdlss_m4_solve(f,rhs,x)==VSDLSS_OK&&fabs(x[0]-2)<1e-12);
    vsdlss_m4_factor_free(f);
    return 0;
}
int main(void)
{
    CHECK(accuracy()==0); CHECK(failures()==0); CHECK(edges_and_cleanup()==0);
    puts("test_m4: ALL OK (8 KiB budget, all orderings, reconstruction, 3 RHS, I/O faults)");
    return 0;
}
