#include "vsdlss_m3_internal.h"
#include "vsdlss_parallel.h"

#include <string.h>

#define VSDLSS_SOLVE_BLK 128
#define VSDLSS_SOLVE_GATHER 1024

vsdlss_status vsdlss_panel_factor(double *a, csi rows, csi width)
{
    if (!a || width<1 || rows<width) return VSDLSS_ERR_INVALID;
    /* Factor the diagonal block first. External rows only read this block,
       so each row tile can complete all columns without column barriers.
       Keep the original increasing-k subtraction order for each element. */
    for(csi j=0;j<width;j++) {
        double d=a[j*rows+j];
        for(csi k=0;k<j;k++)d-=a[k*rows+j]*a[k*rows+j];
        if(!isfinite(d))return VSDLSS_ERR_NONFINITE;
        if(d<=0)return VSDLSS_ERR_NOT_POSDEF;
        d=sqrt(d);a[j*rows+j]=d;
        for(csi k=0;k<j;k++) {
            double coeff=a[k*rows+j];
            for(csi i=j+1;i<width;i++)a[j*rows+i]-=a[k*rows+i]*coeff;
        }
        for(csi i=j+1;i<width;i++) {
            a[j*rows+i]/=d;
            if(!isfinite(a[j*rows+i]))return VSDLSS_ERR_NONFINITE;
        }
    }
    const csi tile=32,ext=rows-width,tiles=ext/tile+(ext%tile!=0);
    int nt=vsdlss_parallel_width((double)ext*width*width),bad=0;
    if(tiles==0)return VSDLSS_OK;
    if(nt>tiles)nt=(int)tiles;
    (void)nt;
    VSDLSS_OMP(omp parallel num_threads(nt) if(nt>1) reduction(|:bad))
    {
        VSDLSS_OMP(omp master)
        vsdlss_parallel_observe();
        VSDLSS_OMP(omp for schedule(static))
        for(csi t=0;t<tiles;t++) {
            csi first=width+t*tile,n=rows-first;
            if(n>tile)n=tile;
            double v[32];
            for(csi j=0;j<width;j++) {
                for(csi i=0;i<n;i++)v[i]=a[j*rows+first+i];
                for(csi k=0;k<j;k++) {
                    double coeff=a[k*rows+j];
                    for(csi i=0;i<n;i++)v[i]-=a[k*rows+first+i]*coeff;
                }
                for(csi i=0;i<n;i++) {
                    v[i]/=a[j*rows+j];
                    if(!isfinite(v[i]))bad=1;
                    a[j*rows+first+i]=v[i];
                }
            }
        }
    }
    return bad?VSDLSS_ERR_NONFINITE:VSDLSS_OK;
}

/* Blocked external-block update.  The old kernel evaluated one
 * vsdlss_panel_dot per output element, so every element walked the panel with
 * stride `rows` and no output reuse: pure memory traffic, nothing to vectorise
 * and nothing for a second thread to overlap.  This version keeps the panel
 * layout but inverts the loops into rank-1 updates over a small accumulator
 * tile, so the k loop streams contiguous column segments and each loaded value
 * feeds VSDLSS_PANEL_UPDATE_CB outputs.
 *
 * Accumulation order is unchanged: every accumulator starts at +0 and takes
 * k = 0..width-1 in increasing order, exactly the sequence vsdlss_panel_dot
 * performs, so results stay bitwise identical to the previous kernel and to
 * each other for any thread count or blocking.
 *
 * Returns a bit mask: 1 = target slot outside the permitted range,
 * 2 = non-finite result. */
#define VSDLSS_UPD_CB VSDLSS_PANEL_UPDATE_CB
#define VSDLSS_UPD_RB 256

int vsdlss_panel_update_range(const double *a,csi rows,csi width,csi ext,
                              csi cfirst,csi clast,const csi *targets,
                              double *panel,csi lo_slot,csi hi_slot)
{
    double acc[VSDLSS_UPD_RB*VSDLSS_UPD_CB];
    const double *base=a+width; /* external rows of column 0 */
    int bad=0;
    for(csi c0=cfirst;c0<clast;c0+=VSDLSS_UPD_CB) {
        csi cb=clast-c0; if(cb>VSDLSS_UPD_CB)cb=VSDLSS_UPD_CB;
        for(csi r0=c0;r0<ext;r0+=VSDLSS_UPD_RB) {
            csi rn=ext-r0; if(rn>VSDLSS_UPD_RB)rn=VSDLSS_UPD_RB;
            memset(acc,0,(size_t)rn*(size_t)cb*sizeof(double));
            for(csi k=0;k<width;k++) {
                const double *pk=base+k*rows;
                const double *src=pk+r0;
                for(csi c=0;c<cb;c++) {
                    double cv=pk[c0+c];
                    double *dst=acc+c*rn;
                    for(csi i=0;i<rn;i++)dst[i]+=src[i]*cv;
                }
            }
            for(csi c=0;c<cb;c++) {
                csi col=c0+c,u=col*ext-col*(col-1)/2;
                csi rs=r0>col?r0:col,rend=r0+rn;
                const double *src=acc+c*rn;
                for(csi row=rs;row<rend;row++) {
                    csi target=targets[u+row-col];
                    if(target<lo_slot||target>=hi_slot){bad|=1;continue;}
                    panel[target]-=src[row-r0];
                    if(!isfinite(panel[target]))bad|=2;
                }
            }
        }
    }
    return bad;
}

double vsdlss_panel_dot(const double *a,csi rows,csi width,csi i,csi j)
{
    double v=0;
    /* Preserve left-to-right accumulation, including the initial +0. */
#define DOT(K) v+=a[(K)*rows+i]*a[(K)*rows+j]
    if(width>=1 && width<=6) {
        DOT(0); if(width>=2){DOT(1);} if(width>=3){DOT(2);}
        if(width>=4){DOT(3);} if(width>=5){DOT(4);} if(width>=6){DOT(5);}
    } else for(csi k=0;k<width;k++) v+=a[k*rows+i]*a[k*rows+j];
#undef DOT
    return v;
}

vsdlss_status vsdlss_panel_solve_generic(const double *a,csi begin,csi width,
                                csi ext,const csi *index,double *x,int back)
{
    csi rows=width+ext;
    int nt=vsdlss_parallel_width((double)width*ext),bad=0;(void)nt;
    /* External rows have no dependency on J until the triangular solve ends.
       Reverse solve applies the external contribution before solving J^T. */
    if(!back)for(csi j=0;j<width;j++){
        double d=a[j*rows+j];
        if(!isfinite(d)||d<=0)return VSDLSS_ERR_INVALID;
        x[begin+j]/=d;
        if(!isfinite(x[begin+j]))return VSDLSS_ERR_NONFINITE;
        for(csi r=j+1;r<width;r++)x[begin+r]-=a[j*rows+r]*x[begin+j];
    }
    /* External phase.  Both directions keep the per-element accumulation
       order of the original element-at-a-time loops and only change which
       values are reused, so every result is bitwise unchanged.
       Forward: hold one block of external destinations in registers and sweep
       j outward, turning the stride-`rows` panel reads into contiguous ones.
       Reverse: gather the external x values once instead of re-reading them
       through index[] for every column of J. */
    if(!back) {
        const csi blk=VSDLSS_SOLVE_BLK,nblocks=ext/blk+(ext%blk!=0);
        int fnt=nt; if(fnt>nblocks)fnt=(int)nblocks;
        if(fnt<1)fnt=1;
        (void)fnt;
        VSDLSS_OMP(omp parallel num_threads(fnt) if(fnt>1) reduction(|:bad))
        {
            VSDLSS_OMP(omp master)
            vsdlss_parallel_observe();
            VSDLSS_OMP(omp for schedule(static))
            for(csi b=0;b<nblocks;b++){
                csi t0=b*blk,t1=t0+blk,t;
                double v[VSDLSS_SOLVE_BLK];
                if(t1>ext)t1=ext;
                for(t=t0;t<t1;t++)v[t-t0]=x[index[t]];
                for(csi j=0;j<width;j++){
                    double c=x[begin+j];
                    const double *col=a+j*rows+width;
                    for(t=t0;t<t1;t++)v[t-t0]-=col[t]*c;
                }
                for(t=t0;t<t1;t++){
                    x[index[t]]=v[t-t0];
                    if(!isfinite(v[t-t0]))bad=1;
                }
            }
        }
    } else if(ext) {
        double stack_gather[VSDLSS_SOLVE_GATHER];
        double *xg=ext<=VSDLSS_SOLVE_GATHER?stack_gather:
                   (double*)malloc((size_t)ext*sizeof(double));
        if(!xg) {
            /* Allocation-free fallback; identical arithmetic, more reads. */
            VSDLSS_OMP(omp parallel num_threads(nt) if(nt>1) reduction(|:bad))
            {
                VSDLSS_OMP(omp master)
                vsdlss_parallel_observe();
                VSDLSS_OMP(omp for schedule(static))
                for(csi t=0;t<width;t++){
                    double v=x[begin+t];
                    for(csi r=0;r<ext;r++)v-=a[t*rows+width+r]*x[index[r]];
                    x[begin+t]=v;if(!isfinite(v))bad=1;
                }
            }
        } else {
            for(csi r=0;r<ext;r++)xg[r]=x[index[r]];
            VSDLSS_OMP(omp parallel num_threads(nt) if(nt>1) reduction(|:bad))
            {
                VSDLSS_OMP(omp master)
                vsdlss_parallel_observe();
                VSDLSS_OMP(omp for schedule(static))
                for(csi t=0;t<width;t++){
                    double v=x[begin+t];
                    const double *row_t=a+t*rows+width;
                    for(csi r=0;r<ext;r++)v-=row_t[r]*xg[r];
                    x[begin+t]=v;if(!isfinite(v))bad=1;
                }
            }
            if(xg!=stack_gather)free(xg);
        }
    }
    if(bad)return VSDLSS_ERR_NONFINITE;
    if(back)for(csi j=width;j-- >0;){
        double d=a[j*rows+j],v=x[begin+j];
        if(!isfinite(d)||d<=0)return VSDLSS_ERR_INVALID;
        for(csi r=j+1;r<width;r++)v-=a[j*rows+r]*x[begin+r];
        x[begin+j]=v/d;
        if(!isfinite(x[begin+j]))return VSDLSS_ERR_NONFINITE;
    }
    return VSDLSS_OK;
}

#include "vsdlss_small_solve.inc"

vsdlss_status vsdlss_panel_solve(const double *a,csi begin,csi width,
                                csi ext,const csi *index,double *x,int back)
{
    /* Keep large external-row work on the existing parallel path. */
    if(vsdlss_parallel_width((double)width*ext)>1)
        return vsdlss_panel_solve_generic(a,begin,width,ext,index,x,back);
    switch(width) {
#define CASE(N) case N: return solve_##N(a,begin,ext,index,x,back)
        CASE(1); CASE(2); CASE(3); CASE(4); CASE(5); CASE(6);
#undef CASE
        default: return vsdlss_panel_solve_generic(a,begin,width,ext,index,x,back);
    }
}
