#define _POSIX_C_SOURCE 200809L
#include "vsdlss_ledger.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

vsdlss_ledger_t vsdlss_ledger;
static int level = -1;

int vsdlss_ledger_level(void)
{
    if (level < 0) { const char *e = getenv("VSDLSS_SOLVE_LEDGER"); level = e ? atoi(e) : 0; if (level < 0) level = 0; }
    return level;
}
void vsdlss_ledger_set_level(int l) { level = l < 0 ? 0 : l; }
void vsdlss_ledger_reset(void) { memset(&vsdlss_ledger, 0, sizeof vsdlss_ledger); }
double vsdlss_ledger_now(void)
{
    struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + t.tv_nsec * 1e-9;
}
double vsdlss_ledger_tsc_sec(void)
{
    static double s = 0;
    if (s == 0) {
        double t0 = vsdlss_ledger_now(); uint64_t c0 = vsdlss_ledger_tsc(), c1; double t1;
        do { t1 = vsdlss_ledger_now(); } while (t1 - t0 < 0.05);
        c1 = vsdlss_ledger_tsc();
        s = (t1 - t0) / (double)(c1 - c0);
    }
    return s;
}
