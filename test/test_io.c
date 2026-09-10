#define _POSIX_C_SOURCE 200809L
#include "vsdlss.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int failures;
#define CHECK(e) do { if (!(e)) { fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #e); failures++; } } while (0)

static int write_file(const char *path, const void *data, size_t size)
{
    FILE *f=fopen(path,"wb");
    if(!f)return 0;
    int ok=fwrite(data,1,size,f)==size;
    if(fclose(f)!=0)ok=0;
    return ok;
}

static void path_of(char *out, size_t cap, const char *job, const char *ext)
{
    snprintf(out,cap,"%s%s",job,ext);
}

static void create_valid_job(const char *job, int with_rhs)
{
    char path[512];
    const char hdr[]="n 2\no 1\n";
    double diag[]={4,3}, off[]={1}, rhs[]={7,-1};
    int32_t from[]={1}, to[]={2};
    path_of(path,sizeof path,job,".hdr"); CHECK(write_file(path,hdr,sizeof(hdr)-1));
    path_of(path,sizeof path,job,".matd"); CHECK(write_file(path,diag,sizeof diag));
    path_of(path,sizeof path,job,".matf"); CHECK(write_file(path,from,sizeof from));
    path_of(path,sizeof path,job,".matt"); CHECK(write_file(path,to,sizeof to));
    path_of(path,sizeof path,job,".mato"); CHECK(write_file(path,off,sizeof off));
    if(with_rhs){path_of(path,sizeof path,job,".rhs"); CHECK(write_file(path,rhs,sizeof rhs));}
}

static void test_load_modes_and_corruption(void)
{
    char dir[]="./vsdlss-io-XXXXXX", job[512], path[512];
    vsdlss *A=NULL; double *rhs=NULL;
    CHECK(mkdtemp(dir)!=NULL);
    snprintf(job,sizeof job,"%s/case",dir);
    create_valid_job(job,1);
    CHECK(vsdlss_load_job(job,0,&A,&rhs)==VSDLSS_OK);
    CHECK(A && rhs && rhs[0]==7 && rhs[1]==-1);
    vsdlss_spfree(A); free(rhs); A=NULL; rhs=NULL;
    path_of(path,sizeof path,job,".rhs"); CHECK(unlink(path)==0);
    CHECK(vsdlss_load_job(job,0,&A,&rhs)==VSDLSS_ERR_IO && !A && !rhs);
    CHECK(vsdlss_load_job(job,1,&A,&rhs)==VSDLSS_OK);
    CHECK(A && rhs && rhs[0]==5 && rhs[1]==4);
    vsdlss_spfree(A); free(rhs); A=NULL; rhs=NULL;
    { double short_rhs=7; path_of(path,sizeof path,job,".rhs"); CHECK(write_file(path,&short_rhs,sizeof short_rhs)); }
    CHECK(vsdlss_load_job(job,1,&A,&rhs)==VSDLSS_ERR_IO && !A && !rhs);
    { int32_t bad=3; path_of(path,sizeof path,job,".matf"); CHECK(write_file(path,&bad,sizeof bad)); }
    CHECK(vsdlss_load_job(job,1,&A,&rhs)==VSDLSS_ERR_INVALID && !A && !rhs);

    {
        const char malformed[]="garbage\nn 2 trailing\no 1 trailing\n";
        create_valid_job(job,1);
        path_of(path,sizeof path,job,".hdr");
        CHECK(write_file(path,malformed,sizeof(malformed)-1));
        CHECK(vsdlss_load_job(job,1,&A,&rhs)==VSDLSS_ERR_INVALID && !A && !rhs);
    }
    const char *suffixes[]={".hdr",".matd",".matf",".matt",".mato",".rhs"};
    for(size_t i=0;i<sizeof(suffixes)/sizeof(suffixes[0]);i++){
        path_of(path,sizeof path,job,suffixes[i]);CHECK(unlink(path)==0);
    }
    CHECK(rmdir(dir)==0);
}

static void test_solution_write_errors(void)
{
    double x[]={2,-1};
    CHECK(vsdlss_write_solution("/no/such/vsdlss/path/case",x,2)==VSDLSS_ERR_IO);
    CHECK(vsdlss_write_solution("ignored",x,0)==VSDLSS_ERR_INVALID);
}

int main(void)
{
    test_load_modes_and_corruption();
    test_solution_write_errors();
    if(failures){fprintf(stderr,"test_io: %d failure(s)\n",failures);return 1;}
    puts("test_io: ALL OK"); return 0;
}
