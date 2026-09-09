#include "vsdlss.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>

static void usage(const char *program)
{
    fprintf(stderr, "usage: %s [-p 0|1|2|3|4] [--demo-rhs] [--disk-budget bytes] [--temp-dir path] <job>\n", program);
}

int main(int argc, char **argv)
{
    int order=0, allow_missing_rhs=0, arg;
    const char *job=NULL, *temp_dir=NULL;
    size_t disk_budget=0;
    int disk_mode=0;
    vsdlss_m4_factor *disk_factor=NULL;
    vsdlss *A=NULL;
    vsdlss_factor *factor=NULL;
    double *rhs=NULL, *solution=NULL;
    double eta=NAN;
    vsdlss_status status=VSDLSS_OK;
    for(arg=1;arg<argc;++arg){
        if(strcmp(argv[arg],"-p")==0){
            if(++arg>=argc){usage(argv[0]);return 2;}
            if(strlen(argv[arg])!=1 || argv[arg][0]<'0' || argv[arg][0]>'4'){usage(argv[0]);return 2;}
            order=argv[arg][0]-'0';
        }else if(strcmp(argv[arg],"--disk-budget")==0){
            char *end; uintmax_t value;
            if(++arg>=argc || argv[arg][0]<'0' || argv[arg][0]>'9'){usage(argv[0]);return 2;}
            errno=0; value=strtoumax(argv[arg],&end,10);
            if(errno || *end || value==0 || value>SIZE_MAX){usage(argv[0]);return 2;}
            disk_budget=(size_t)value; disk_mode=1;
        }else if(strcmp(argv[arg],"--temp-dir")==0){
            if(++arg>=argc){usage(argv[0]);return 2;}
            temp_dir=argv[arg];
        }else if(strcmp(argv[arg],"--demo-rhs")==0){
            allow_missing_rhs=1;
        }else if(argv[arg][0]=='-'){
            usage(argv[0]);return 2;
        }else if(job){
            usage(argv[0]);return 2;
        }else job=argv[arg];
    }
    if(!job || (temp_dir && !disk_mode)){usage(argv[0]);return 2;}
    status=vsdlss_load_job(job,allow_missing_rhs,&A,&rhs);
    if(status!=VSDLSS_OK)goto done;
    solution=(double*)malloc((size_t)A->n*sizeof(double));
    if(!solution){status=VSDLSS_ERR_OOM;goto done;}
    if(disk_mode){
        status=vsdlss_factorize_m4(A,order,disk_budget,temp_dir,&disk_factor);
        if(status!=VSDLSS_OK)goto done;
        status=vsdlss_m4_solve(disk_factor,rhs,solution);
    }else{
        status=vsdlss_factorize(A,order,&factor);
        if(status!=VSDLSS_OK)goto done;
        status=vsdlss_factor_solve(factor,rhs,solution);
    }
    if(status!=VSDLSS_OK)goto done;
    status=vsdlss_backward_error(A,solution,rhs,&eta);
    if(status!=VSDLSS_OK)goto done;
    status=vsdlss_write_solution(job,solution,A->n);
done:
    if(status==VSDLSS_OK)
        printf("VSDLSS %s: n=%lld order=%d backward_error=%.3e\n",
               disk_mode?"M4 disk":"in-memory",(long long)A->n,order,eta);
    else fprintf(stderr,"vsdlss: %s\n",vsdlss_status_string(status));
    vsdlss_m4_factor_free(disk_factor);vsdlss_factor_free(factor);vsdlss_spfree(A);free(rhs);free(solution);
    return status==VSDLSS_OK?0:(int)status+1;
}
