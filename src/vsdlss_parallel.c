#include "vsdlss_parallel.h"
static _Thread_local int requested=1;
static _Thread_local int observed=1;
int vsdlss_parallel_enabled(void)
{
#ifdef _OPENMP
    return 1;
#else
    return 0;
#endif
}
vsdlss_status vsdlss_set_num_threads(int threads)
{
    if(threads<1||threads>1024)return VSDLSS_ERR_INVALID;
    if(threads>1&&!vsdlss_parallel_enabled())return VSDLSS_ERR_UNSUPPORTED;
    requested=threads;observed=1;return VSDLSS_OK;
}
int vsdlss_get_num_threads(void){return requested;}
int vsdlss_parallel_last_team_size(void){return observed;}
int vsdlss_parallel_width(double work)
{
#ifdef _OPENMP
    if(!omp_in_parallel()&&work>=100000.0)return requested;
#else
    (void)work;
#endif
    return 1;
}
void vsdlss_parallel_observe(void)
{
#ifdef _OPENMP
    int team=omp_get_num_threads();if(team>observed)observed=team;
#endif
}
