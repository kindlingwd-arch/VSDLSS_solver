#define _POSIX_C_SOURCE 200809L
#define _FILE_OFFSET_BITS 64
#include "vsdlss_m4_internal.h"
#include "vsdlss_m3_internal.h"
#include "vsdlss_parallel.h"
#include <float.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAGIC UINT64_C(0x003453534c445356)
#define HASH_INIT UINT64_C(14695981039346656037)
static uint64_t hash_bytes(uint64_t h,const void *v,size_t n)
{
    const unsigned char *p=v;
    for(size_t i=0;i<n;i++){h^=p[i];h*=UINT64_C(1099511628211);}
    return h;
}
static uint64_t decode(const unsigned char *p)
{ uint64_t v=0;for(int i=0;i<8;i++)v|=(uint64_t)p[i]<<(8*i);return v; }
static void encode(unsigned char *p,uint64_t v)
{ for(int i=0;i<8;i++)p[i]=(unsigned char)(v>>(8*i)); }
static int word(FILE *fp,uint64_t *v,int write,uint64_t *hash)
{
    unsigned char b[8];
    if(write){encode(b,*v);if(fwrite(b,1,8,fp)!=8)return 0;}
    else {if(fread(b,1,8,fp)!=8)return 0;*v=decode(b);}
    if(hash)*hash=hash_bytes(*hash,b,8);
    return 1;
}
static int seek_file(FILE *fp,uint64_t offset)
{ return offset<=INT64_MAX&&fseeko(fp,(off_t)offset,SEEK_SET)==0; }
static int portable(void)
{ return sizeof(double)==8&&DBL_MANT_DIG==53&&DBL_MAX_EXP==1024&&sizeof(csi)==8; }
static size_t base_bytes(csi n)
{
    if(n<1||(uint64_t)n>(SIZE_MAX-sizeof(vsdlss_m4_factor)-256)/64)return 0;
    return sizeof(vsdlss_m4_factor)+256+(size_t)n*64;
}
static size_t payload(const vsdlss_disk_panel *b)
{ return (size_t)b->rows*((size_t)b->width+1)*8; }
static csi lookup(const csi *rows,csi n,csi row)
{
    csi lo=0,hi=n;
    while(lo<hi){csi mid=lo+(hi-lo)/2;if(rows[mid]<row)lo=mid+1;else hi=mid;}
    return lo<n&&rows[lo]==row?lo:-1;
}
static vsdlss_status temporary(const char *directory,FILE **out)
{
    if(!directory)directory=getenv("TMPDIR");
    if(!directory||!*directory)directory="/tmp";
    size_t n=strlen(directory);
    if(n>SIZE_MAX-32)return VSDLSS_ERR_OOM;
    char *path=malloc(n+32);if(!path)return VSDLSS_ERR_OOM;
    snprintf(path,n+32,"%s/vsdlss-panel-XXXXXX",directory);
    int fd=mkstemp(path);
    if(fd<0){free(path);return VSDLSS_ERR_IO;}
    if(unlink(path)!=0){close(fd);remove(path);free(path);return VSDLSS_ERR_IO;}
    free(path);*out=fdopen(fd,"w+b");
    if(!*out){close(fd);return VSDLSS_ERR_IO;}
    if(setvbuf(*out,NULL,_IONBF,0)!=0){fclose(*out);*out=NULL;return VSDLSS_ERR_IO;}
    return VSDLSS_OK;
}
static int block_io(vsdlss_m4_factor *f,csi id,void *buffer,int writing,int factored)
{
    vsdlss_disk_panel *b=&f->blocks[id];size_t bytes=payload(b);
    unsigned char *raw=buffer;
    if(!seek_file(f->file,b->offset))return 0;
    if(writing){
        for(size_t i=0;i<bytes;i+=8){uint64_t v;memcpy(&v,raw+i,8);encode(raw+i,v);}
        b->checksum=hash_bytes(HASH_INIT,raw,bytes);
        int ok=fwrite(raw,1,bytes,f->file)==bytes;
        for(size_t i=0;i<bytes;i+=8){uint64_t v=decode(raw+i);memcpy(raw+i,&v,8);}
        if(!ok)return 0;
        f->stats.block_writes++;f->stats.bytes_written+=bytes;
    }else{
        if(fread(raw,1,bytes,f->file)!=bytes||hash_bytes(HASH_INIT,raw,bytes)!=b->checksum)return 0;
        for(size_t i=0;i<bytes;i+=8){uint64_t v=decode(raw+i);memcpy(raw+i,&v,8);}
        f->stats.block_reads++;f->stats.bytes_read+=bytes;
    }
    csi *rows=buffer;double *a=(double *)(rows+b->rows);
    for(uint64_t i=0;i<b->rows;i++){
        if(rows[i]<0||(uint64_t)rows[i]>=(uint64_t)f->n||(i&&rows[i]<=rows[i-1]))return 0;
        if(i<b->width&&(uint64_t)rows[i]!=b->begin+i)return 0;
    }
    for(uint64_t j=0;j<b->width;j++)for(uint64_t i=0;i<b->rows;i++){
        if(!isfinite(a[j*b->rows+i]))return 0;
        if(factored&&i==j&&a[j*b->rows+i]<=0)return 0;
        if(i<j&&a[j*b->rows+i]!=0)return 0;
    }
    return 1;
}
/* Check metadata against the trusted resident layout on every solve/save. */
static int metadata(vsdlss_m4_factor *f,int writing)
{
    vsdlss_disk_panel *last=&f->blocks[f->block_count-1];
    uint64_t h[8]={MAGIC,2,(uint64_t)f->n,(uint64_t)f->block_count,
        (uint64_t)f->width_cap,64+(uint64_t)f->n*8+(uint64_t)f->block_count*40,
        last->offset+payload(last),0},hash=HASH_INIT,v;
    if(!seek_file(f->file,0))return 0;
    for(int i=0;i<8;i++){
        v=h[i];if(!word(f->file,&v,writing,i==7?NULL:&hash))return 0;
        if(!writing&&i<7&&v!=h[i])return 0;
        if(!writing&&i==7)h[7]=v;
    }
    for(csi i=0;i<f->n;i++){
        v=(uint64_t)f->q[i];if(!word(f->file,&v,writing,&hash)||v!=(uint64_t)f->q[i])return 0;
    }
    for(csi i=0;i<f->block_count;i++){
        vsdlss_disk_panel *b=&f->blocks[i];
        uint64_t fields[5]={b->begin,b->width,b->rows,b->offset,b->checksum};
        for(int j=0;j<5;j++){v=fields[j];if(!word(f->file,&v,writing,&hash)||v!=fields[j])return 0;}
    }
    if(writing){if(!seek_file(f->file,56)||!word(f->file,&hash,1,NULL)||fflush(f->file)!=0)return 0;}
    else if(hash!=h[7])return 0;
    return fseeko(f->file,0,SEEK_END)==0&&ftello(f->file)>=0&&(uint64_t)ftello(f->file)==h[6];
}
static void init_stats(vsdlss_m4_factor *f)
{
    f->stats.format_version=2;f->stats.blocks=f->block_count;
    f->stats.numeric_workspace_bytes=f->workspace_bytes;
    for(csi i=0;i<f->block_count;i++){
        if(f->blocks[i].width>1)f->stats.multi_column_blocks++;
        if(f->blocks[i].width>f->stats.max_columns)f->stats.max_columns=f->blocks[i].width;
    }
}
vsdlss_status vsdlss_factorize_m4_ex(const vsdlss *A,int order,size_t budget,
    const char *directory,csi max_columns,vsdlss_m4_factor **out)
{
    vsdlss_m4_factor *f=NULL;vsdlss *B=NULL,*P=NULL,*lower=NULL;
    vsdlss_sn_symbolic *s=NULL;csi *q=NULL,*pinv=NULL,*owner=NULL;
    void *source=NULL,*target=NULL;vsdlss_status st=VSDLSS_ERR_OOM;
    if(!out)return VSDLSS_ERR_INVALID;
    *out=NULL;if(!portable())return VSDLSS_ERR_UNSUPPORTED;
    if(!A||A->n<1||max_columns<1)return VSDLSS_ERR_INVALID;
    size_t base=base_bytes(A->n);
    if(!base||budget<base||(budget-base)/16/(size_t)A->n<2)return VSDLSS_ERR_OOM;
    size_t fit=(budget-base)/16/(size_t)A->n-1;
    csi cap=max_columns<A->n?max_columns:A->n;
    if((uint64_t)cap>fit)cap=(csi)fit;
    st=vsdlss_normalize_upper(A,&B);if(st!=VSDLSS_OK)goto done;
    st=vsdlss_order(B,order,&q,&pinv);if(st!=VSDLSS_OK)goto done;
    st=VSDLSS_ERR_OOM;
    P=vsdlss_symperm(B,pinv,1);if(!P)goto done;
    vsdlss_spfree(B);B=NULL;free(pinv);pinv=NULL;
    st=vsdlss_sn_analyze_compact(P,&s);if(st!=VSDLSS_OK)goto done;
    st=VSDLSS_ERR_OOM;
    lower=vsdlss_transpose(P,1);if(!lower)goto done;
    vsdlss_spfree(P);P=NULL;
    f=calloc(1,sizeof(*f));if(!f)goto done;
    f->n=A->n;f->q=q;q=NULL;f->format_version=2;f->width_cap=cap;
    f->blocks=calloc((size_t)f->n,sizeof(*f->blocks));
    owner=malloc((size_t)f->n*sizeof(csi));if(!f->blocks||!owner)goto done;
    for(csi sn=0;sn<s->count;sn++)for(csi begin=s->column_start[sn];begin<s->column_start[sn+1];){
        csi width=s->column_start[sn+1]-begin;if(width>cap)width=cap;
        vsdlss_disk_panel *b=&f->blocks[f->block_count];
        b->begin=begin;b->width=width;b->rows=s->l_col_ptr[begin+1]-s->l_col_ptr[begin];
        size_t bytes=payload(b);if(bytes>f->max_payload)f->max_payload=bytes;
        for(csi j=begin;j<begin+width;j++)owner[j]=f->block_count;
        f->block_count++;begin+=width;
    }
    f->workspace_bytes=base+2*f->max_payload;
    source=malloc(f->max_payload);target=malloc(f->max_payload);
    if(!source||!target)goto done;
    st=temporary(directory,&f->file);if(st!=VSDLSS_OK)goto done;
    uint64_t offset=64+(uint64_t)f->n*8+(uint64_t)f->block_count*40;
    st=VSDLSS_ERR_IO;
    for(csi id=0;id<f->block_count;id++){
        vsdlss_disk_panel *b=&f->blocks[id];size_t bytes=payload(b);
        if(offset>(uint64_t)INT64_MAX-bytes)goto done;
        b->offset=offset;offset+=bytes;
        memset(source,0,bytes);csi *rows=source;double *a=(double *)(rows+b->rows);
        memcpy(rows,s->l_row_index+s->l_col_ptr[b->begin],(size_t)b->rows*8);
        for(uint64_t j=0;j<b->width;j++)for(csi p=lower->p[b->begin+j];p<lower->p[b->begin+j+1];p++){
            csi r=lookup(rows,(csi)b->rows,lower->i[p]);
            if(r<0){st=VSDLSS_ERR_INVALID;goto done;}
            a[j*b->rows+r]+=lower->x[p];
        }
        if(!block_io(f,id,source,1,0))goto done;
    }
    /* Preparation layouts and matrix are no longer resident in numeric phase. */
    vsdlss_sn_symbolic_free(s);s=NULL;vsdlss_spfree(lower);lower=NULL;
    for(csi id=0;id<f->block_count;id++){
        vsdlss_disk_panel *b=&f->blocks[id];
        st=VSDLSS_ERR_IO;if(!block_io(f,id,source,0,0))goto done;
        csi *rows=source;double *a=(double *)(rows+b->rows);
        st=vsdlss_panel_factor(a,(csi)b->rows,(csi)b->width);if(st!=VSDLSS_OK)goto done;
        st=VSDLSS_ERR_IO;if(!block_io(f,id,source,1,1))goto done;
        for(uint64_t c=b->width;c<b->rows;){
            csi dest=owner[rows[c]];vsdlss_disk_panel *d=&f->blocks[dest];
            if(dest<=id){st=VSDLSS_ERR_INVALID;goto done;}
            if(!block_io(f,dest,target,0,0))goto done;
            csi *dr=target;double *da=(double *)(dr+d->rows);
            uint64_t end=c+1;
            while(end<b->rows&&owner[rows[end]]==dest)end++;
            int nt=vsdlss_parallel_width((double)(end-c)*b->rows*b->width),bad=0;(void)nt;
            VSDLSS_OMP(omp parallel num_threads(nt) if(nt>1) reduction(|:bad))
            {
                VSDLSS_OMP(omp master)
                vsdlss_parallel_observe();
                VSDLSS_OMP(omp for schedule(dynamic,1))
                for(uint64_t sc=c;sc<end;sc++){
                    csi dc=rows[sc]-(csi)d->begin;
                    for(uint64_t r=sc;r<b->rows;r++){
                        csi tr=lookup(dr,(csi)d->rows,rows[r]);
                        if(tr<0){bad|=1;continue;}
                        da[(uint64_t)dc*d->rows+tr]-=vsdlss_panel_dot(a,(csi)b->rows,(csi)b->width,(csi)r,(csi)sc);
                        if(!isfinite(da[(uint64_t)dc*d->rows+tr]))bad|=2;
                    }
                }
            }
            if(bad){st=(bad&1)?VSDLSS_ERR_INVALID:VSDLSS_ERR_NONFINITE;goto done;}
            c=end;
            if(!block_io(f,dest,target,1,0))goto done;
        }
    }
    if(!metadata(f,1))goto done;
    init_stats(f);*out=f;f=NULL;st=VSDLSS_OK;
done:
    free(source);free(target);free(owner);free(q);free(pinv);
    vsdlss_spfree(B);vsdlss_spfree(P);vsdlss_spfree(lower);vsdlss_sn_symbolic_free(s);
    vsdlss_m4_factor_free(f);return st;
}
vsdlss_status vsdlss_factorize_m4(const vsdlss *A,int order,size_t budget,
    const char *directory,vsdlss_m4_factor **out)
{
    size_t base=A?base_bytes(A->n):0;
    if(base&&budget>=base&&(budget-base)/16/(size_t)A->n>=2)
        return vsdlss_factorize_m4_ex(A,order,budget,directory,8,out);
    return vsdlss_factorize_m4_scalar(A,order,budget,directory,out);
}
vsdlss_status vsdlss_m4_panel_solve(vsdlss_m4_factor *f,const double *rhs,double *solution)
{
    for(csi i=0;i<f->n;i++)if(!isfinite(rhs[i]))return VSDLSS_ERR_NONFINITE;
    clearerr(f->file);if(!metadata(f,0))return VSDLSS_ERR_IO;
    void *buffer=malloc(f->max_payload);double *x=malloc((size_t)f->n*8);
    if(!buffer||!x){free(buffer);free(x);return VSDLSS_ERR_OOM;}
    vsdlss_status st=VSDLSS_ERR_IO;
    for(csi i=0;i<f->n;i++)x[i]=rhs[f->q[i]];
    for(int back=0;back<2;back++)for(csi t=0;t<f->block_count;t++){
        csi id=back?f->block_count-1-t:t;vsdlss_disk_panel *b=&f->blocks[id];
        if(!block_io(f,id,buffer,0,1)){st=VSDLSS_ERR_IO;goto done;}
        csi *rows=buffer;
        st=vsdlss_panel_solve((double *)(rows+b->rows),(csi)b->begin,(csi)b->width,
            (csi)(b->rows-b->width),rows+b->width,x,back);
        if(st!=VSDLSS_OK)goto done;
    }
    for(csi i=0;i<f->n;i++)if(!isfinite(x[i])){st=VSDLSS_ERR_NONFINITE;goto done;}
    for(csi i=0;i<f->n;i++)solution[f->q[i]]=x[i];
    st=VSDLSS_OK;
done:free(buffer);free(x);return st;
}
vsdlss_status vsdlss_m4_get_stats(const vsdlss_m4_factor *f,vsdlss_m4_stats *out)
{
    if(!f||!out)return VSDLSS_ERR_INVALID;
    *out=f->stats;
    if(f->format_version!=2){memset(out,0,sizeof(*out));out->format_version=1;
        out->blocks=f->n;out->max_columns=1;out->numeric_workspace_bytes=f->workspace_bytes;}
    return VSDLSS_OK;
}
csi vsdlss_m4_dimension(const vsdlss_m4_factor *f){return f?f->n:0;}

vsdlss_status vsdlss_m4_open(const char *path,size_t budget,vsdlss_m4_factor **out)
{
    vsdlss_m4_factor *f=NULL;void *buffer=NULL;unsigned char *seen=NULL;
    vsdlss_status st=VSDLSS_ERR_IO;uint64_t h[8],v,hash=HASH_INIT;
    if(!out)return VSDLSS_ERR_INVALID;
    *out=NULL;if(!path)return VSDLSS_ERR_INVALID;
    if(!portable())return VSDLSS_ERR_UNSUPPORTED;
    FILE *fp=fopen(path,"rb");if(!fp)return st;
    if(setvbuf(fp,NULL,_IONBF,0)!=0)goto done;
    for(int i=0;i<8;i++)if(!word(fp,&h[i],0,i==7?NULL:&hash))goto done;
    if(h[0]!=MAGIC)goto done;
    if(h[1]!=2){st=VSDLSS_ERR_UNSUPPORTED;goto done;}
    if(!h[2]||h[2]>INT64_MAX||!h[3]||h[3]>h[2]||!h[4]||h[4]>h[2])goto done;
    size_t base=base_bytes((csi)h[2]);
    if(!base||budget<base){st=VSDLSS_ERR_OOM;goto done;}
    if(h[5]!=64+h[2]*8+h[3]*40||h[6]>(uint64_t)INT64_MAX||h[6]<h[5])goto done;
    if(fseeko(fp,0,SEEK_END)!=0||ftello(fp)<0||(uint64_t)ftello(fp)!=h[6]||!seek_file(fp,64))goto done;
    st=VSDLSS_ERR_OOM;f=calloc(1,sizeof(*f));if(!f)goto done;
    f->n=(csi)h[2];f->block_count=(csi)h[3];f->width_cap=(csi)h[4];f->format_version=2;
    f->file=fp;fp=NULL;
    f->q=malloc((size_t)f->n*8);f->blocks=calloc((size_t)f->n,sizeof(*f->blocks));
    seen=calloc((size_t)f->n,1);if(!f->q||!f->blocks||!seen)goto done;
    st=VSDLSS_ERR_IO;
    for(csi i=0;i<f->n;i++){
        if(!word(f->file,&v,0,&hash)||v>=(uint64_t)f->n||seen[v])goto done;
        f->q[i]=(csi)v;seen[v]=1;
    }
    uint64_t begin=0,offset=h[5];
    for(csi i=0;i<f->block_count;i++){
        uint64_t fields[5];for(int j=0;j<5;j++)if(!word(f->file,&fields[j],0,&hash))goto done;
        vsdlss_disk_panel *b=&f->blocks[i];
        b->begin=fields[0];b->width=fields[1];b->rows=fields[2];b->offset=fields[3];b->checksum=fields[4];
        if(b->begin!=begin||!b->width||b->width>h[4]||b->width>h[2]-begin||
            b->rows<b->width||b->rows>h[2]-begin||b->offset!=offset)goto done;
        if(b->rows>SIZE_MAX/8/(b->width+1)){st=VSDLSS_ERR_OOM;goto done;}
        size_t bytes=payload(b);
        if(bytes>(budget-base)/2){st=VSDLSS_ERR_OOM;goto done;}
        if(offset>h[6]||bytes>h[6]-offset)goto done;
        begin+=b->width;offset+=bytes;if(bytes>f->max_payload)f->max_payload=bytes;
    }
    if(begin!=h[2]||offset!=h[6]||hash!=h[7])goto done;
    free(seen);seen=NULL;
    buffer=malloc(f->max_payload);if(!buffer){st=VSDLSS_ERR_OOM;goto done;}
    for(csi i=0;i<f->block_count;i++)if(!block_io(f,i,buffer,0,1))goto done;
    f->workspace_bytes=base+2*f->max_payload;init_stats(f);
    *out=f;f=NULL;st=VSDLSS_OK;
done:
    free(buffer);free(seen);if(fp)fclose(fp);vsdlss_m4_factor_free(f);return st;
}
vsdlss_status vsdlss_m4_save(vsdlss_m4_factor *f,const char *path)
{
    if(!f||!path||!*path)return VSDLSS_ERR_INVALID;
    if(f->format_version!=2)return VSDLSS_ERR_UNSUPPORTED;
    if(!metadata(f,0))return VSDLSS_ERR_IO;
    void *buffer=malloc(f->max_payload);if(!buffer)return VSDLSS_ERR_OOM;
    for(csi i=0;i<f->block_count;i++)if(!block_io(f,i,buffer,0,1)){free(buffer);return VSDLSS_ERR_IO;}
    free(buffer);
    size_t len=strlen(path);if(len>SIZE_MAX-16)return VSDLSS_ERR_OOM;
    char *temp=malloc(len+16);if(!temp)return VSDLSS_ERR_OOM;
    snprintf(temp,len+16,"%s.XXXXXX",path);int fd=mkstemp(temp);
    if(fd<0){free(temp);return VSDLSS_ERR_IO;}
    FILE *dest=fdopen(fd,"wb");vsdlss_status st=VSDLSS_ERR_IO;
    if(!dest){close(fd);goto done;}
    if(!seek_file(f->file,0))goto close_dest;
    unsigned char chunk[4096];size_t n;
    while((n=fread(chunk,1,sizeof(chunk),f->file))>0)if(fwrite(chunk,1,n,dest)!=n)goto close_dest;
    if(ferror(f->file)||fflush(dest)!=0||fsync(fileno(dest))!=0)goto close_dest;
    if(fclose(dest)!=0){dest=NULL;goto done;}dest=NULL;
    if(rename(temp,path)!=0)goto done;
    st=VSDLSS_OK;
close_dest:if(dest)fclose(dest);
done:if(st!=VSDLSS_OK)unlink(temp);free(temp);return st;
}
