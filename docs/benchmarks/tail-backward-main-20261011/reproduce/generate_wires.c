#define main unused_tail_benchmark_main
#include "../../../../test/bench_tail.c"
#undef main
int main(void)
{
    vsdlss *A=build(1000000,20,200);
    if(!A) return 1;
    FILE *f=fopen("wires-1m.bin","wb");
    const int64_t n=A->n,nz=A->p[A->n];
    if(!f) return 1;
    int ok=fwrite(&n,8,1,f)==1 && fwrite(&nz,8,1,f)==1 &&
        fwrite(A->p,8,(size_t)n+1,f)==(size_t)n+1 &&
        fwrite(A->i,8,(size_t)nz,f)==(size_t)nz &&
        fwrite(A->x,8,(size_t)nz,f)==(size_t)nz;
    if(fclose(f)) ok=0;
    vsdlss_spfree(A); return !ok;
}
