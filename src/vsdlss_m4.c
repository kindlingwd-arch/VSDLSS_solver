#define _POSIX_C_SOURCE 200809L
#define _FILE_OFFSET_BITS 64
#include "vsdlss_m4_internal.h"
#include <float.h>
#include <string.h>
#include <unistd.h>

/* All disk integers and binary64 bits are explicitly little endian. */
static int put64(FILE *f, uint64_t x)
{
    unsigned char b[8];
    for (int i=0;i<8;i++) b[i]=(unsigned char)(x>>(8*i));
    return fwrite(b,1,8,f)==8;
}
static int get64(FILE *f, uint64_t *x)
{
    unsigned char b[8];
    if (fread(b,1,8,f)!=8) return 0;
    *x=0;
    for (int i=0;i<8;i++) *x|=(uint64_t)b[i]<<(8*i);
    return 1;
}
static int seek64(FILE *f, uint64_t off)
{
    return off<=INT64_MAX && fseeko(f,(off_t)off,SEEK_SET)==0;
}
static int record_read(FILE *f, uint64_t *row, double *value)
{
    uint64_t bits;
    if (!get64(f,row)||!get64(f,&bits)) return 0;
    memcpy(value,&bits,8);
    return isfinite(*value);
}
static int record_write(FILE *f, uint64_t row, double value)
{
    uint64_t bits;
    memcpy(&bits,&value,8);
    return put64(f,row)&&put64(f,bits);
}
static int metadata(vsdlss_m4_factor *f, int writing)
{
    /* magic, version, byte-order tag, dimension, record width, completion */
    uint64_t h[6]={UINT64_C(0x003453534c445356),1,UINT64_C(0x0102030405060708),
                   (uint64_t)f->n,16,1}, x;
    if (!seek64(f->file,0)) return 0;
    for (int i=0;i<6;i++) {
        if (writing) { if (!put64(f->file,h[i])) return 0; }
        else if (!get64(f->file,&x)||x!=h[i]) return 0;
    }
    for (csi j=0;j<f->n;j++) {
        uint64_t v[3]={(uint64_t)f->q[j],f->offset[j],f->count[j]};
        for (int k=0;k<3;k++) {
            if (writing) { if (!put64(f->file,v[k])) return 0; }
            else if (!get64(f->file,&x)||x!=v[k]) return 0;
        }
    }
    return 1;
}
void vsdlss_m4_factor_free(vsdlss_m4_factor *f)
{
    if (!f) return;
    if (f->file) fclose(f->file);
    free(f->q); free(f->offset); free(f->count); free(f->blocks); free(f);
}
size_t vsdlss_m4_workspace_bytes(const vsdlss_m4_factor *f)
{
    return f?f->workspace_bytes:0;
}
vsdlss_status vsdlss_factorize_m4_scalar(const vsdlss *A, int order,
    size_t budget, const char *directory, vsdlss_m4_factor **out)
{
    vsdlss_m4_factor *f=NULL;
    vsdlss *B=NULL;
    csi *q=NULL,*pinv=NULL;
    double *w=NULL;
    char *path=NULL;
    vsdlss_status st;
    uint64_t end;
    if (!out) return VSDLSS_ERR_INVALID;
    *out=NULL;
    if (sizeof(double)!=8||DBL_MANT_DIG!=53||DBL_MAX_EXP!=1024)
        return VSDLSS_ERR_UNSUPPORTED;
    if (!A || A->n<1) return VSDLSS_ERR_INVALID;
    /* Reject impossible dimensions before validators touch n+1 or p[n]. */
    if ((uint64_t)A->n>(SIZE_MAX-sizeof(*f)-128)/40 ||
        (uint64_t)A->n>(uint64_t)(INT64_MAX-48)/24)
        return VSDLSS_ERR_OOM;
    st=vsdlss_validate_upper_csc(A);
    if (st!=VSDLSS_OK) return st;
    size_t required=sizeof(*f)+128+(size_t)A->n*40;
    if (budget<required) return VSDLSS_ERR_OOM;
    st=vsdlss_normalize_upper(A,&B);
    if (st!=VSDLSS_OK) goto done;
    st=vsdlss_order(B,order,&q,&pinv);
    if (st!=VSDLSS_OK) goto done;
    st=VSDLSS_ERR_OOM;
    f=calloc(1,sizeof(*f));
    if (!f) goto done;
    f->n=B->n; f->q=q; q=NULL; f->workspace_bytes=required;
    f->offset=calloc((size_t)f->n,sizeof(uint64_t));
    f->count=calloc((size_t)f->n,sizeof(uint64_t));
    w=malloc((size_t)f->n*sizeof(double));
    if (!f->offset||!f->count||!w) goto done;
    if (!directory) directory=getenv("TMPDIR");
    if (!directory || !*directory) directory="/tmp";
    size_t len=strlen(directory);
    if (len>SIZE_MAX-24) goto done;
    path=malloc(len+24);
    if (!path) goto done;
    snprintf(path,len+24,"%s/vsdlss-m4-XXXXXX",directory);
    st=VSDLSS_ERR_IO;
    int fd=mkstemp(path);
    if (fd<0) goto done;
    /* Unlink immediately: cleanup also works after process termination. */
    if (unlink(path)!=0) { close(fd); remove(path); goto done; }
    f->file=fdopen(fd,"w+b");
    if (!f->file) { close(fd); goto done; }
    if (setvbuf(f->file,NULL,_IONBF,0)!=0) goto done;
    free(path); path=NULL;
    end=48+(uint64_t)f->n*24;
    /* Leave completion/header unpublished until every column is written. */
    for (csi j=0;j<f->n;j++) {
        memset(w,0,(size_t)f->n*sizeof(double));
        for (csi col=0;col<B->n;col++)
            for (csi p=B->p[col];p<B->p[col+1];p++) {
                csi a=pinv[B->i[p]],b=pinv[col];
                if (a==j&&b>=j) w[b]=B->x[p];
                else if (b==j&&a>=j) w[a]=B->x[p];
            }
        for (csi k=0;k<j;k++) {
            uint64_t row; double v,ljk=0;
            if (!seek64(f->file,f->offset[k])) goto done;
            for (uint64_t p=0;p<f->count[k];p++) {
                if (!record_read(f->file,&row,&v)) goto done;
                if (row==(uint64_t)j) { ljk=v; break; }
                if (row>(uint64_t)j) break;
            }
            if (ljk==0) continue;
            if (!seek64(f->file,f->offset[k])) goto done;
            for (uint64_t p=0;p<f->count[k];p++) {
                if (!record_read(f->file,&row,&v)||row>=(uint64_t)f->n) goto done;
                if (row>=(uint64_t)j) w[row]-=v*ljk;
            }
        }
        if (!isfinite(w[j])) { st=VSDLSS_ERR_NONFINITE; goto done; }
        if (w[j]<=0) { st=VSDLSS_ERR_NOT_POSDEF; goto done; }
        double diag=sqrt(w[j]);
        f->offset[j]=end;
        if (!seek64(f->file,end)) goto done;
        for (csi i=j;i<f->n;i++) {
            double v=i==j?diag:w[i]/diag;
            if (!isfinite(v)) { st=VSDLSS_ERR_NONFINITE; goto done; }
            if (v==0) continue;
            if (end>(uint64_t)INT64_MAX-16||!record_write(f->file,(uint64_t)i,v)) goto done;
            end+=16; f->count[j]++;
        }
    }
    if (!metadata(f,1)||fflush(f->file)!=0) goto done;
    *out=f; f=NULL; st=VSDLSS_OK;
done:
    free(path); free(w); free(q); free(pinv); vsdlss_spfree(B);
    vsdlss_m4_factor_free(f);
    return st;
}
vsdlss_status vsdlss_m4_solve(vsdlss_m4_factor *f,
    const double *rhs, double *solution)
{
    double *w;
    vsdlss_status st=VSDLSS_ERR_IO;
    if (!f||!f->file||!rhs||!solution) return VSDLSS_ERR_INVALID;
    if (f->format_version==2) return vsdlss_m4_panel_solve(f,rhs,solution);
    for (csi i=0;i<f->n;i++) if (!isfinite(rhs[i])) return VSDLSS_ERR_NONFINITE;
    clearerr(f->file);
    if (!metadata(f,0)) return st;
    uint64_t expected=f->offset[f->n-1]+16*f->count[f->n-1];
    if (fseeko(f->file,0,SEEK_END)!=0 || ftello(f->file)<0 ||
        (uint64_t)ftello(f->file)!=expected) return st;
    w=malloc((size_t)f->n*sizeof(double));
    if (!w) return VSDLSS_ERR_OOM;
    for (csi i=0;i<f->n;i++) w[i]=rhs[f->q[i]];
    for (int pass=0;pass<2;pass++) for (csi t=0;t<f->n;t++) {
        csi j=pass?f->n-1-t:t;
        uint64_t row,prev=(uint64_t)j;
        double diag,v;
        if (!seek64(f->file,f->offset[j])||
            !record_read(f->file,&row,&diag)||row!=(uint64_t)j||diag<=0) goto done;
        if (!pass) w[j]/=diag;
        for (uint64_t p=1;p<f->count[j];p++) {
            if (!record_read(f->file,&row,&v)||row<=prev||row>=(uint64_t)f->n) goto done;
            prev=row;
            if (pass) w[j]-=v*w[row]; else w[row]-=v*w[j];
        }
        if (pass) w[j]/=diag;
        if (!isfinite(w[j])) { st=VSDLSS_ERR_NONFINITE; goto done; }
    }
    for (csi i=0;i<f->n;i++) solution[f->q[i]]=w[i];
    st=VSDLSS_OK;
done:
    free(w); return st;
}
