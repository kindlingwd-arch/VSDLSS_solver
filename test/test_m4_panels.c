#define _POSIX_C_SOURCE 200809L
#include "../src/vsdlss_m4_internal.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <signal.h>
#define CHECK(e) do{if(!(e)){fprintf(stderr,"FAIL panels %d: %s\n",__LINE__,#e);return 1;}}while(0)
static int dense(void)
{
    const csi n=24;
    vsdlss *A=vsdlss_spalloc(n,n,n*(n+1)/2,1,0);
    double truth[24],rhs[24],x[24],ref[24];
    CHECK(A); csi p=0;
    for(csi j=0;j<n;j++) {
        A->p[j]=p;
        for(csi i=0;i<=j;i++){A->i[p]=i;A->x[p++]=i==j?30:0.2/(1+j-i);}
        truth[j]=(j%2?-1:1)*(1+j*0.1);
    }
    A->p[n]=p;
    CHECK(vsdlss_spmv_sym_upper(A,truth,rhs)==VSDLSS_OK);
    vsdlss_factor *m1=NULL;
    CHECK(vsdlss_factorize(A,2,&m1)==VSDLSS_OK);
    CHECK(vsdlss_factor_solve(m1,rhs,ref)==VSDLSS_OK);
    uint64_t scalar_reads=0;
    for(csi width=1;width<=8;width*=2) {
        vsdlss_m4_factor *f=NULL,*opened=NULL;
        CHECK(vsdlss_factorize_m4_ex(A,2,16384,NULL,width,&f)==VSDLSS_OK);
        vsdlss_m4_stats stats;
        CHECK(vsdlss_m4_get_stats(f,&stats)==VSDLSS_OK);
        CHECK(stats.format_version==2&&stats.max_columns==(uint64_t)width);
        CHECK(width==1||stats.multi_column_blocks>0);
        CHECK(stats.numeric_workspace_bytes<=16384);
        if(width==1)scalar_reads=stats.block_reads;
        if(width==8){CHECK(stats.block_reads<scalar_reads);
            printf("dense numeric block reads: width1=%llu width8=%llu\n",
                (unsigned long long)scalar_reads,(unsigned long long)stats.block_reads);}

        for(int r=0;r<3;r++) {
            memcpy(x,rhs,sizeof(x));
            CHECK(vsdlss_m4_solve(f,x,x)==VSDLSS_OK);
            for(csi i=0;i<n;i++) CHECK(isfinite(x[i])&&fabs(x[i]-truth[i])<1e-12&&fabs(x[i]-ref[i])<1e-12);
        }
        char directory[]="/tmp/vsdlss-panels-XXXXXX",path[100];
        CHECK(mkdtemp(directory)); snprintf(path,sizeof(path),"%s/factor",directory);
        CHECK(vsdlss_m4_save(f,path)==VSDLSS_OK);
        vsdlss_m4_factor_free(f); f=NULL;
        CHECK(vsdlss_m4_open(path,16384,&opened)==VSDLSS_OK);
        CHECK(vsdlss_m4_dimension(opened)==n);
        CHECK(vsdlss_m4_solve(opened,rhs,x)==VSDLSS_OK);
        for(csi i=0;i<n;i++)CHECK(fabs(x[i]-truth[i])<1e-12);
        vsdlss_m4_factor_free(opened);opened=NULL;
        CHECK(vsdlss_m4_open(path,1,&opened)==VSDLSS_ERR_OOM&&opened==NULL);
        FILE *fp=fopen(path,"r+b");CHECK(fp);
        CHECK(fseeko(fp,-1,SEEK_END)==0);int byte=fgetc(fp);CHECK(byte!=EOF);
        CHECK(fseeko(fp,-1,SEEK_END)==0&&fputc(byte^1,fp)!=EOF&&fclose(fp)==0);
        CHECK(vsdlss_m4_open(path,16384,&opened)==VSDLSS_ERR_IO&&opened==NULL);
        CHECK(unlink(path)==0&&rmdir(directory)==0);
    }
    vsdlss_factor_free(m1);vsdlss_spfree(A);
    return 0;
}

static int sparse(void)
{
    const csi n=64;vsdlss *A=vsdlss_spalloc(n,n,3*n,1,0);
    double truth[64],rhs[64],x[64],dense[4096],L[4096];
    CHECK(A);memset(dense,0,sizeof(dense));csi p=0;
    for(csi j=0;j<n;j++){
        A->p[j]=p;
        if(j>=8){A->i[p]=j-8;A->x[p++]=-1;dense[j*n+j-8]=dense[(j-8)*n+j]=-1;}
        if(j%8){A->i[p]=j-1;A->x[p++]=-1;dense[j*n+j-1]=dense[(j-1)*n+j]=-1;}
        A->i[p]=j;A->x[p++]=5;dense[j*n+j]=5;truth[j]=(j%2?-1:1)*(0.7+j*0.01);
    }
    A->p[n]=p;CHECK(vsdlss_spmv_sym_upper(A,truth,rhs)==VSDLSS_OK);
    for(int order=0;order<=4;order++){
        vsdlss_m4_factor *f=NULL;vsdlss_m4_stats stats;
        CHECK(vsdlss_factorize_m4_ex(A,order,32768,NULL,8,&f)==VSDLSS_OK);
        CHECK(vsdlss_m4_get_stats(f,&stats)==VSDLSS_OK&&stats.multi_column_blocks>0);
        CHECK(vsdlss_m4_solve(f,rhs,x)==VSDLSS_OK);
        double eta;CHECK(vsdlss_backward_error(A,x,rhs,&eta)==VSDLSS_OK&&eta<1e-12);
        memset(L,0,sizeof(L));
        /* Independent little-endian decoder, not the production block reader. */
        for(csi b=0;b<f->block_count;b++){
            vsdlss_disk_panel *d=&f->blocks[b];uint64_t rows[64];
            CHECK(fseeko(f->file,(off_t)d->offset,SEEK_SET)==0);
            for(uint64_t k=0;k<d->rows*(d->width+1);k++){
                unsigned char bytes[8];uint64_t bits=0;
                CHECK(fread(bytes,1,8,f->file)==8);
                for(int c=0;c<8;c++)bits|=(uint64_t)bytes[c]<<(8*c);
                if(k<d->rows)rows[k]=bits;
                else{uint64_t v=k-d->rows;double value;memcpy(&value,&bits,8);
                    L[rows[v%d->rows]*n+d->begin+v/d->rows]=value;}
            }
        }
        for(csi i=0;i<n;i++)for(csi j=0;j<n;j++){
            double v=0;for(csi k=0;k<n;k++)v+=L[i*n+k]*L[j*n+k];
            CHECK(isfinite(v)&&fabs(v-dense[f->q[i]*n+f->q[j]])<1e-12);
        }
        CHECK(fseeko(f->file,(off_t)(f->blocks[0].offset+8),SEEK_SET)==0);
        int byte=fgetc(f->file);CHECK(byte!=EOF);
        CHECK(fseeko(f->file,(off_t)(f->blocks[0].offset+8),SEEK_SET)==0&&fputc(byte^1,f->file)!=EOF);
        for(csi j=0;j<n;j++)x[j]=123;
        CHECK(vsdlss_m4_solve(f,rhs,x)==VSDLSS_ERR_IO);
        for(csi j=0;j<n;j++)CHECK(x[j]==123);
        vsdlss_m4_factor_free(f);
    }
    vsdlss_m4_factor *f=NULL;
    size_t budget=sizeof(*f)+256+64*n+16*n*3;
    CHECK(vsdlss_factorize_m4_ex(A,2,budget,NULL,64,&f)==VSDLSS_OK);
    vsdlss_m4_stats stats;CHECK(vsdlss_m4_get_stats(f,&stats)==VSDLSS_OK);
    CHECK(stats.max_columns==2&&stats.numeric_workspace_bytes<=budget);
    CHECK(vsdlss_m4_solve(f,rhs,x)==VSDLSS_OK);
    vsdlss_m4_factor_free(f);vsdlss_spfree(A);return 0;
}
static int faults(void)
{
    csi p[]={0,1,3},i[]={0,0,1};double values[]={4,1,3};
    vsdlss A={3,2,2,p,i,values,-1};vsdlss_m4_factor *f=NULL,*opened=NULL;
    double rhs[]={7,-1},x[]={91,92};
    CHECK(vsdlss_factorize_m4_ex(&A,2,8192,NULL,0,&f)==VSDLSS_ERR_INVALID&&f==NULL);
    values[2]=0;CHECK(vsdlss_factorize_m4_ex(&A,2,8192,NULL,8,&f)==VSDLSS_ERR_NOT_POSDEF&&f==NULL);
    values[2]=3;
    CHECK(vsdlss_factorize_m4_ex(&A,2,8192,NULL,8,&f)==VSDLSS_OK);
    rhs[0]=NAN;CHECK(vsdlss_m4_solve(f,rhs,x)==VSDLSS_ERR_NONFINITE&&x[0]==91&&x[1]==92);rhs[0]=7;
    char directory[]="/tmp/vsdlss-panel-faults-XXXXXX",path[100];
    CHECK(mkdtemp(directory));snprintf(path,sizeof(path),"%s/factor",directory);
    for(int damage=0;damage<4;damage++){
        CHECK(vsdlss_m4_save(f,path)==VSDLSS_OK);
        FILE *fp=fopen(path,"r+b");CHECK(fp);
        if(damage==0)CHECK(ftruncate(fileno(fp),12)==0);
        if(damage==1){CHECK(fseeko(fp,8,SEEK_SET)==0&&fputc(99,fp)!=EOF);}
        if(damage==2){CHECK(fseeko(fp,64,SEEK_SET)==0&&fputc(1,fp)!=EOF);}
        if(damage==3){CHECK(fseeko(fp,0,SEEK_END)==0&&fputc(0,fp)!=EOF);}
        CHECK(fclose(fp)==0);
        vsdlss_status st=vsdlss_m4_open(path,8192,&opened);
        CHECK(st==(damage==1?VSDLSS_ERR_UNSUPPORTED:VSDLSS_ERR_IO)&&opened==NULL);
    }
    FILE *fp=fopen(path,"wb");CHECK(fp&&fwrite("keep",1,4,fp)==4&&fclose(fp)==0);
    for(int saving=0;saving<2;saving++){
        pid_t pid=fork();CHECK(pid>=0);
        if(pid==0){
            struct rlimit lim={130,130};signal(SIGXFSZ,SIG_IGN);
            if(setrlimit(RLIMIT_FSIZE,&lim)!=0)_exit(2);
            vsdlss_status st;
            if(saving)st=vsdlss_m4_save(f,path);
            else{vsdlss_m4_factor *bad=NULL;st=vsdlss_factorize_m4_ex(&A,2,8192,directory,8,&bad);
                if(bad)_exit(3);}
            _exit(st==VSDLSS_ERR_IO?0:1);
        }
        int result;CHECK(waitpid(pid,&result,0)==pid&&WIFEXITED(result)&&WEXITSTATUS(result)==0);
    }
    char preserved[4];fp=fopen(path,"rb");
    CHECK(fp&&fread(preserved,1,4,fp)==4&&memcmp(preserved,"keep",4)==0&&fclose(fp)==0);
    CHECK(unlink(path)==0&&rmdir(directory)==0);
    vsdlss_m4_factor_free(f);return 0;
}
int main(void){CHECK(dense()==0);CHECK(sparse()==0);CHECK(faults()==0);puts("test_m4_panels: ALL OK");return 0;}
