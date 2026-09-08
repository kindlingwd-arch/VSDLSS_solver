#include "vsdlss.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void usage(const char *program)
{
    fprintf(stderr, "usage: %s [-p 0|1|2|3|4] [--demo-rhs] <job>\n", program);
}

int main(int argc, char **argv)
{
    int order=0, allow_missing_rhs=0, arg;
    const char *job=NULL;
    vsdlss *A=NULL;
    vsdlss_factor *factor=NULL;
    double *rhs=NULL, *solution=NULL;
    double eta=NAN;
    vsdlss_status status=VSDLSS_OK;
    for(arg=1;arg<argc;++arg){
        if(strcmp(argv[arg],"-p")==0){
            if(++arg>=argc){usage(argv[0]);return 2;}
            order=atoi(argv[arg]);
        }else if(strcmp(argv[arg],"--demo-rhs")==0){
            allow_missing_rhs=1;
        }else if(argv[arg][0]=='-'){
            usage(argv[0]);return 2;
        }else if(job){
            usage(argv[0]);return 2;
        }else job=argv[arg];
    }
    if(!job){usage(argv[0]);return 2;}
    status=vsdlss_load_job(job,allow_missing_rhs,&A,&rhs);
    if(status!=VSDLSS_OK)goto done;
    solution=(double*)malloc((size_t)A->n*sizeof(double));
    if(!solution){status=VSDLSS_ERR_OOM;goto done;}
    status=vsdlss_factorize(A,order,&factor);
    if(status!=VSDLSS_OK)goto done;
    status=vsdlss_factor_solve(factor,rhs,solution);
    if(status!=VSDLSS_OK)goto done;
    status=vsdlss_backward_error(A,solution,rhs,&eta);
    if(status!=VSDLSS_OK)goto done;
    status=vsdlss_write_solution(job,solution,A->n);
done:
    if(status==VSDLSS_OK)
        printf("VSDLSS M2: n=%lld order=%d backward_error=%.3e\n",
               (long long)A->n,order,eta);
    else fprintf(stderr,"vsdlss: %s\n",vsdlss_status_string(status));
    vsdlss_factor_free(factor);vsdlss_spfree(A);free(rhs);free(solution);
    return status==VSDLSS_OK?0:(int)status+1;
}
