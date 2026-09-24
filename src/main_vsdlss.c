#define _POSIX_C_SOURCE 200809L
#include "vsdlss.h"
#include "vsdlss_text_io.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>

static double seconds_now(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (double)t.tv_sec + (double)t.tv_nsec * 1e-9;
}

static void usage(const char *program)
{
    fprintf(stderr, "usage: %s [-p 0|1|2|3|4|5] [--text-dump dir] [--demo-rhs] [--disk-budget bytes] [--temp-dir path] [--block-cols n] [--save-factor path] [--load-factor path] [--m3] [--m4-reduced] [--dag] [--threads n] <job>\n", program);
    fprintf(stderr, "  -p: 0/5 = AMD (default, recommended), 1 = RCM, 2 = natural, 3 = minimum degree, 4 = MLD\n");
    fprintf(stderr, "  --text-dump: read diag.txt, data.txt, b_vector.txt, x_vector.txt from dir; <job> is the .rsl output prefix\n");
}

int main(int argc, char **argv)
{
    int order=0, allow_missing_rhs=0, arg;
    const char *job=NULL, *temp_dir=NULL, *text_dump=NULL;
    size_t disk_budget=0;
    int disk_mode=0, m3_mode=0, reduced_mode=0;
    csi block_cols=8;
    int threads=1;
    int explicit_cols=0;
    const char *save_factor=NULL, *load_factor=NULL;
    vsdlss_m3_factor *m3_factor=NULL;
    vsdlss_m4_factor *disk_factor=NULL;
    vsdlss_m4_reduced_factor *reduced_factor=NULL;
    vsdlss *A=NULL;
    vsdlss_factor *factor=NULL;
    double *rhs=NULL, *solution=NULL, *reference=NULL;
    double eta=NAN, relative_residual=NAN, max_error=NAN, relative_error=NAN;
    double load_start=0, load_end=0, prep_start=0, prep_end=0;
    double factor_start=0, factor_end=0, solve_end=0;
    vsdlss_status status=VSDLSS_OK;
    for(arg=1;arg<argc;++arg){
        if(strcmp(argv[arg],"-p")==0){
            if(++arg>=argc){usage(argv[0]);return 2;}
            if(strlen(argv[arg])!=1 || argv[arg][0]<'0' || argv[arg][0]>'5'){usage(argv[0]);return 2;}
            order=argv[arg][0]-'0';
        }else if(strcmp(argv[arg],"--disk-budget")==0){
            char *end; uintmax_t value;
            if(++arg>=argc || argv[arg][0]<'0' || argv[arg][0]>'9'){usage(argv[0]);return 2;}
            errno=0; value=strtoumax(argv[arg],&end,10);
            if(errno || *end || value==0 || value>SIZE_MAX){usage(argv[0]);return 2;}
            disk_budget=(size_t)value; disk_mode=1;
        }else if(strcmp(argv[arg],"--dag")==0){
            vsdlss_set_dag_enabled(1);
        }else if(strcmp(argv[arg],"--threads")==0){
            char *end; long value;
            if(++arg>=argc){usage(argv[0]);return 2;}
            errno=0;value=strtol(argv[arg],&end,10);
            if(errno||end==argv[arg]||*end||value<1||value>1024){usage(argv[0]);return 2;}
            threads=(int)value;
        }else if(strcmp(argv[arg],"--block-cols")==0){
            char *end; uintmax_t value;
            if(++arg>=argc || argv[arg][0]<'0' || argv[arg][0]>'9'){usage(argv[0]);return 2;}
            errno=0; value=strtoumax(argv[arg],&end,10);
            if(errno || *end || value==0 || value>INT64_MAX){usage(argv[0]);return 2;}
            block_cols=(csi)value; explicit_cols=1;
        }else if(strcmp(argv[arg],"--save-factor")==0){
            if(++arg>=argc){usage(argv[0]);return 2;} save_factor=argv[arg];
        }else if(strcmp(argv[arg],"--load-factor")==0){
            if(++arg>=argc){usage(argv[0]);return 2;} load_factor=argv[arg];
        }else if(strcmp(argv[arg],"--m4-reduced")==0){
            reduced_mode=1;
        }else if(strcmp(argv[arg],"--m3")==0){
            m3_mode=1;
        }else if(strcmp(argv[arg],"--temp-dir")==0){
            if(++arg>=argc){usage(argv[0]);return 2;}
            temp_dir=argv[arg];
        }else if(strcmp(argv[arg],"--text-dump")==0){
            if(++arg>=argc || text_dump){usage(argv[0]);return 2;}
            text_dump=argv[arg];
        }else if(strcmp(argv[arg],"--demo-rhs")==0){
            allow_missing_rhs=1;
        }else if(argv[arg][0]=='-'){
            usage(argv[0]);return 2;
        }else if(job){
            usage(argv[0]);return 2;
        }else job=argv[arg];
    }
    if((reduced_mode&&(!disk_mode||m3_mode||save_factor||load_factor||explicit_cols)) || !job || ((temp_dir||save_factor||load_factor||explicit_cols) && !disk_mode) ||
       (m3_mode&&disk_mode) || (load_factor&&(save_factor||explicit_cols||temp_dir)) ||
       (text_dump&&allow_missing_rhs)){usage(argv[0]);return 2;}
    load_start=seconds_now();
    status=vsdlss_set_num_threads(threads);
    if(status!=VSDLSS_OK)goto done;
    if(text_dump)status=vsdlss_load_text_dump(text_dump,&A,&rhs,&reference);
    else status=vsdlss_load_job(job,allow_missing_rhs,&A,&rhs);
    if(status!=VSDLSS_OK)goto done;
    load_end=seconds_now();
    prep_start=load_end;
    solution=(double*)malloc((size_t)A->n*sizeof(double));
    if(!solution){status=VSDLSS_ERR_OOM;goto done;}
    prep_end=seconds_now();
    factor_start=prep_end;
    if(reduced_mode){
        status=vsdlss_factorize_m4_reduced(A,order,disk_budget,temp_dir,&reduced_factor);
        factor_end=seconds_now();
        if(status!=VSDLSS_OK)goto done;
        status=vsdlss_m4_reduced_solve(reduced_factor,rhs,solution);
        solve_end=seconds_now();
    }else if(disk_mode){
        if(load_factor)status=vsdlss_m4_open(load_factor,disk_budget,&disk_factor);
        else if(explicit_cols||save_factor)
            status=vsdlss_factorize_m4_ex(A,order,disk_budget,temp_dir,block_cols,&disk_factor);
        else status=vsdlss_factorize_m4(A,order,disk_budget,temp_dir,&disk_factor);
        factor_end=seconds_now();
        if(status!=VSDLSS_OK)goto done;
        if(vsdlss_m4_dimension(disk_factor)!=A->n){status=VSDLSS_ERR_INVALID;goto done;}
        status=vsdlss_m4_solve(disk_factor,rhs,solution);
        solve_end=seconds_now();
        if(status==VSDLSS_OK&&save_factor)status=vsdlss_m4_save(disk_factor,save_factor);
    }else if(m3_mode){
        status=vsdlss_factorize_m3(A,order,&m3_factor);
        factor_end=seconds_now();
        if(status!=VSDLSS_OK)goto done;
        status=vsdlss_m3_solve(m3_factor,rhs,solution);
        solve_end=seconds_now();
    }else{
        status=vsdlss_factorize(A,order,&factor);
        factor_end=seconds_now();
        if(status!=VSDLSS_OK)goto done;
        status=vsdlss_factor_solve(factor,rhs,solution);
        solve_end=seconds_now();
    }
    if(status!=VSDLSS_OK)goto done;
    status=vsdlss_backward_error(A,solution,rhs,&eta);
    if(status!=VSDLSS_OK)goto done;
    if(reference){
        csi i;
        double *ax=(double*)malloc((size_t)A->n*sizeof(double)), bnorm=0, refnorm=0;
        if(!ax){status=VSDLSS_ERR_OOM;goto done;}
        status=vsdlss_spmv_sym_upper(A,solution,ax);
        if(status!=VSDLSS_OK){free(ax);goto done;}
        relative_residual=0;max_error=0;
        for(i=0;i<A->n;++i){
            double difference=fabs(ax[i]-rhs[i]), error=fabs(solution[i]-reference[i]);
            if(difference>relative_residual)relative_residual=difference;
            if(error>max_error)max_error=error;
            if(fabs(rhs[i])>bnorm)bnorm=fabs(rhs[i]);
            if(fabs(reference[i])>refnorm)refnorm=fabs(reference[i]);
        }
        relative_residual/=fmax(1.0,bnorm);
        relative_error=max_error/fmax(1.0,refnorm);
        free(ax);
    }
    status=vsdlss_write_solution(job,solution,A->n);
done:
    if(status==VSDLSS_OK)
        printf("VSDLSS %s: n=%lld order=%d backward_error=%.3e\n",
               reduced_mode?"M4 reduced":disk_mode?"M4 disk":(m3_mode?"M3":"in-memory"),(long long)A->n,order,eta);
    else fprintf(stderr,"vsdlss: %s\n",vsdlss_status_string(status));
    if(status==VSDLSS_OK){
        printf("load_ms=%.6f prepare_ms=%.6f factor_ms=%.6f solve_ms=%.6f factor_solve_ms=%.6f\n",
               1000*(load_end-load_start),1000*(prep_end-prep_start),
               1000*(factor_end-factor_start),1000*(solve_end-factor_end),
               1000*(solve_end-factor_start));
        if(reference)printf("relative_residual_inf=%.17g max_abs_error_vs_x=%.17g relative_error_vs_x_inf=%.17g\n",
                            relative_residual,max_error,relative_error);
    }
    if(status==VSDLSS_OK&&disk_factor){
        vsdlss_m4_stats stats;
        vsdlss_m4_get_stats(disk_factor,&stats);
        printf("disk_format=%d blocks=%" PRIu64 " multi_column_blocks=%" PRIu64
               " max_columns=%" PRIu64 " workspace=%zu block_reads=%" PRIu64
               " block_writes=%" PRIu64 "\n",stats.format_version,stats.blocks,
               stats.multi_column_blocks,stats.max_columns,stats.numeric_workspace_bytes,
               stats.block_reads,stats.block_writes);
    }
    if(status==VSDLSS_OK)printf("threads_requested=%d max_team_used=%d\n",threads,vsdlss_parallel_last_team_size());
    vsdlss_m4_reduced_free(reduced_factor);
    vsdlss_m3_factor_free(m3_factor);
    vsdlss_m4_factor_free(disk_factor);vsdlss_factor_free(factor);vsdlss_spfree(A);free(rhs);free(solution);free(reference);
    return status==VSDLSS_OK?0:(int)status+1;
}
