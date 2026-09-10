#include <omp.h>
#include <assert.h>
int main(void){int x=0,y[2]={0,0};
#pragma omp parallel num_threads(2) shared(x,y)
 {
#pragma omp master
  x=42;
#pragma omp barrier
  y[omp_get_thread_num()]=x;
 }
 assert(y[0]==42&&y[1]==42);return 0;
}
