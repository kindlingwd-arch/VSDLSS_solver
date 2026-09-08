#include "vsdlss.h"

#include <errno.h>
#include <stdint.h>
#include <string.h>

static vsdlss_status make_path(char *path, size_t capacity,
                               const char *job, const char *suffix)
{
    int length;
    if (!path || !job || !suffix || !*job) return VSDLSS_ERR_INVALID;
    length = snprintf(path, capacity, "%s%s", job, suffix);
    return length < 0 || (size_t)length >= capacity ? VSDLSS_ERR_INVALID : VSDLSS_OK;
}

static vsdlss_status read_exact(const char *path, void *buffer,
                                size_t count, size_t width)
{
    FILE *file;
    int extra;
    if (width && count > SIZE_MAX / width) return VSDLSS_ERR_INVALID;
    file = fopen(path, "rb");
    if (!file) return VSDLSS_ERR_IO;
    if (fread(buffer, width, count, file) != count) { fclose(file); return VSDLSS_ERR_IO; }
    extra = fgetc(file);
    if (extra != EOF || ferror(file) || fclose(file) != 0) return VSDLSS_ERR_IO;
    return VSDLSS_OK;
}

static vsdlss_status read_header(const char *job, csi *n, csi *offdiag)
{
    char path[1024], line[256], key;
    FILE *file;
    long long value;
    int got_n = 0, got_o = 0;
    vsdlss_status status = make_path(path, sizeof path, job, ".hdr");
    if (status != VSDLSS_OK) return status;
    file = fopen(path, "r");
    if (!file) return VSDLSS_ERR_IO;
    while (fgets(line, sizeof line, file)) {
        const char *cursor=line;
        char *end;
        while(*cursor==' '||*cursor=='\t'||*cursor=='\r'||*cursor=='\n')++cursor;
        if(!*cursor)continue;
        key=*cursor++;
        if(!strchr("nomakzrcxu",key)){fclose(file);return VSDLSS_ERR_INVALID;}
        while(*cursor==' '||*cursor=='\t')++cursor;
        if(!*cursor){fclose(file);return VSDLSS_ERR_INVALID;}
        errno=0;
        value=strtoll(cursor,&end,10);
        if(errno==ERANGE||end==cursor){fclose(file);return VSDLSS_ERR_INVALID;}
        while(*end==' '||*end=='\t'||*end=='\r'||*end=='\n')++end;
        if(*end){fclose(file);return VSDLSS_ERR_INVALID;}
        if (key == 'n') { if (got_n) { fclose(file); return VSDLSS_ERR_INVALID; } *n = (csi)value; got_n = 1; }
        if (key == 'o') { if (got_o) { fclose(file); return VSDLSS_ERR_INVALID; } *offdiag = (csi)value; got_o = 1; }
    }
    if (ferror(file) || fclose(file) != 0) return VSDLSS_ERR_IO;
    if (!got_n || !got_o || *n < 1 || *offdiag < 0 ||
        (uint64_t)*n > SIZE_MAX / sizeof(double) ||
        (uint64_t)*offdiag > SIZE_MAX / sizeof(double)) return VSDLSS_ERR_INVALID;
    return VSDLSS_OK;
}

vsdlss_status vsdlss_load_job(const char *job, int allow_missing_rhs,
                              vsdlss **A, double **rhs)
{
    char path[1024];
    csi n = 0, offdiag = 0, k, nnz, *counts = NULL, *position = NULL;
    int32_t *from = NULL, *to = NULL;
    double *diag = NULL, *values = NULL, *loaded_rhs = NULL;
    vsdlss *raw = NULL, *normalized = NULL;
    vsdlss_status status;
    if (!A || !rhs || (allow_missing_rhs != 0 && allow_missing_rhs != 1)) return VSDLSS_ERR_INVALID;
    *A = NULL; *rhs = NULL;
    status = read_header(job, &n, &offdiag);
    if (status != VSDLSS_OK) return status;
    if ((uint64_t)offdiag > SIZE_MAX / sizeof(int32_t) || offdiag > INT64_MAX - n)
        return VSDLSS_ERR_INVALID;
    diag = (double *)malloc((size_t)n * sizeof(double));
    from = (int32_t *)malloc((size_t)(offdiag ? offdiag : 1) * sizeof(int32_t));
    to = (int32_t *)malloc((size_t)(offdiag ? offdiag : 1) * sizeof(int32_t));
    values = (double *)malloc((size_t)(offdiag ? offdiag : 1) * sizeof(double));
    counts = (csi *)calloc((size_t)n, sizeof(csi));
    position = (csi *)malloc((size_t)n * sizeof(csi));
    loaded_rhs = (double *)malloc((size_t)n * sizeof(double));
    if (!diag || !from || !to || !values || !counts || !position || !loaded_rhs) {
        status = VSDLSS_ERR_OOM; goto cleanup;
    }
    if ((status=make_path(path,sizeof path,job,".matd"))!=VSDLSS_OK ||
        (status=read_exact(path,diag,(size_t)n,sizeof(double)))!=VSDLSS_OK) goto cleanup;
    if ((status=make_path(path,sizeof path,job,".matf"))!=VSDLSS_OK ||
        (status=read_exact(path,from,(size_t)offdiag,sizeof(int32_t)))!=VSDLSS_OK ||
        (status=make_path(path,sizeof path,job,".matt"))!=VSDLSS_OK ||
        (status=read_exact(path,to,(size_t)offdiag,sizeof(int32_t)))!=VSDLSS_OK ||
        (status=make_path(path,sizeof path,job,".mato"))!=VSDLSS_OK ||
        (status=read_exact(path,values,(size_t)offdiag,sizeof(double)))!=VSDLSS_OK) goto cleanup;
    for (k=0;k<n;++k) counts[k]=1;
    for (k=0;k<offdiag;++k) {
        csi r=(csi)from[k]-1, c=(csi)to[k]-1, col;
        if (r<0 || r>=n || c<0 || c>=n || r==c) { status=VSDLSS_ERR_INVALID; goto cleanup; }
        col=r>c?r:c;
        if (counts[col]==INT64_MAX) { status=VSDLSS_ERR_INVALID; goto cleanup; }
        ++counts[col];
    }
    nnz=n+offdiag;
    raw=vsdlss_spalloc(n,n,nnz,1,0);
    if(!raw){status=VSDLSS_ERR_OOM;goto cleanup;}
    raw->p[0]=0;
    for(k=0;k<n;++k){raw->p[k+1]=raw->p[k]+counts[k];position[k]=raw->p[k];}
    for(k=0;k<n;++k){csi at=position[k]++;raw->i[at]=k;raw->x[at]=diag[k];}
    for(k=0;k<offdiag;++k){
        csi r=(csi)from[k]-1,c=(csi)to[k]-1,col=r>c?r:c,row=r<c?r:c,at=position[col]++;
        raw->i[at]=row;raw->x[at]=values[k];
    }
    status=vsdlss_normalize_upper(raw,&normalized);
    if(status!=VSDLSS_OK)goto cleanup;
    status=make_path(path,sizeof path,job,".rhs");
    if(status!=VSDLSS_OK)goto cleanup;
    {
        FILE *probe=fopen(path,"rb");
        if(probe){fclose(probe);status=read_exact(path,loaded_rhs,(size_t)n,sizeof(double));}
        else if(errno==ENOENT && allow_missing_rhs){
            double *ones=(double *)malloc((size_t)n*sizeof(double));
            if(!ones){status=VSDLSS_ERR_OOM;goto cleanup;}
            for(k=0;k<n;++k)ones[k]=1.0;
            status=vsdlss_spmv_sym_upper(normalized,ones,loaded_rhs);
            free(ones);
        } else status=VSDLSS_ERR_IO;
    }
    if(status!=VSDLSS_OK)goto cleanup;
    for(k=0;k<n;++k)if(!isfinite(loaded_rhs[k])){status=VSDLSS_ERR_NONFINITE;goto cleanup;}
    *A=normalized;normalized=NULL;*rhs=loaded_rhs;loaded_rhs=NULL;status=VSDLSS_OK;
cleanup:
    free(diag);free(from);free(to);free(values);free(counts);free(position);free(loaded_rhs);
    vsdlss_spfree(raw);vsdlss_spfree(normalized);
    return status;
}

vsdlss_status vsdlss_write_solution(const char *job, const double *solution, csi n)
{
    char path[1024];
    FILE *file;
    csi k;
    vsdlss_status status;
    if (!solution || n < 1 || (uint64_t)n > SIZE_MAX / sizeof(double)) return VSDLSS_ERR_INVALID;
    for (k=0;k<n;++k) if(!isfinite(solution[k])) return VSDLSS_ERR_NONFINITE;
    status=make_path(path,sizeof path,job,".rsl");
    if(status!=VSDLSS_OK)return status;
    file=fopen(path,"wb");
    if(!file)return VSDLSS_ERR_IO;
    if(fwrite(solution,sizeof(double),(size_t)n,file)!=(size_t)n || fclose(file)!=0)return VSDLSS_ERR_IO;
    return VSDLSS_OK;
}
