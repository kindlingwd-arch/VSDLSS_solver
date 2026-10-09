/* Solve time ledger (VSDLSS_SOLVE_LEDGER).
 *
 *   1: mutually exclusive wall-clock phases of each solve, measured on the
 *      calling thread (end to end: a parallel region counts once, by its
 *      wall time).  Phases add up to LG_TOTAL; LG_OTHER is the remainder.
 *      Components solved concurrently (e.g. VDD and GND nets) each mark
 *      their own phases; their sums then exceed the wall time of the
 *      component step and are scaled to it, i.e. split in proportion.
 *   2: additionally, core panel time by panel width bucket, measured with
 *      the TSC on whichever thread runs the panel.  Bucket times are
 *      THREAD-CUMULATIVE: with more than one thread they may exceed the
 *      core wall time and must not be added to wall-clock phases.
 *
 * Not thread safe across concurrent solves; a diagnostic, off by default.
 * With the ledger off every hook is a single predictable branch. */
#ifndef VSDLSS_LEDGER_H
#define VSDLSS_LEDGER_H
#include <stdint.h>

enum {
    LG_SETUP,        /* workspace acquire, plan lookup */
    LG_PERM_GATHER,  /* caller order -> component buffers (two-pass or direct), finite scan */
    LG_RED_FWD,      /* low-degree elimination forward replay */
    LG_CORE_GATHER,  /* component buffer -> core vector */
    LG_CORE_FWD,     /* supernodal forward (L y = b) */
    LG_CORE_BWD,     /* supernodal backward (L^T x = y) */
    LG_CORE_SCATTER, /* core vector -> component buffer, finite scan */
    LG_RED_BWD,      /* low-degree recovery replay */
    LG_WRITEBACK,    /* component buffers -> caller order */
    LG_OTHER,        /* total minus the phases above */
    LG_TOTAL,
    LG_N
};
#define LG_NB 7          /* width buckets: 1-6,7-31,32-63,64-127,128-255,256-511,>=512 */

typedef struct {
    double wall[LG_N];
    double bucket_s[2][LG_NB];          /* [0] forward, [1] backward; thread-cumulative */
    long long bucket_n[2][LG_NB];       /* panel calls */
    double bucket_bytes[2][LG_NB];      /* panel bytes read (values + row indices) */
    long long solves;
} vsdlss_ledger_t;

extern vsdlss_ledger_t vsdlss_ledger;
int vsdlss_ledger_level(void);          /* 0, 1 or 2 (env, cached) */
void vsdlss_ledger_set_level(int level);
void vsdlss_ledger_reset(void);
double vsdlss_ledger_now(void);
double vsdlss_ledger_tsc_sec(void);     /* seconds per TSC tick (calibrated once) */
static inline int vsdlss_ledger_bucket(long long w)
{
    return w <= 6 ? 0 : w <= 31 ? 1 : w <= 63 ? 2 : w <= 127 ? 3 : w <= 255 ? 4 : w <= 511 ? 5 : 6;
}
static inline uint64_t vsdlss_ledger_tsc(void)
{
#if defined(__x86_64__) || defined(__i386__)
    uint32_t lo, hi; __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi)); return ((uint64_t)hi << 32) | lo;
#else
    return (uint64_t)(vsdlss_ledger_now() * 1e9);
#endif
}
/* Atomic: concurrent components (VDD/GND nets) mark from several threads. */
static inline void vsdlss_ledger_add(int ph, double dt)
{
    double old, sum;
    __atomic_load(&vsdlss_ledger.wall[ph], &old, __ATOMIC_RELAXED);
    do sum = old + dt;
    while (!__atomic_compare_exchange(&vsdlss_ledger.wall[ph], &old, &sum, 1,
                                      __ATOMIC_RELAXED, __ATOMIC_RELAXED));
}
/* Adds the time since *t to phase ph and advances *t (level >= 1 only). */
#define LEDGER_MARK(ph, t) do { if (vsdlss_ledger_level()) { double n_ = vsdlss_ledger_now(); \
    vsdlss_ledger_add(ph, n_ - (t)); (t) = n_; } } while (0)
#endif
