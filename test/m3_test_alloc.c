#include "m3_test_alloc.h"

#include <stdlib.h>

void *__real_malloc(size_t);
void *__real_calloc(size_t, size_t);
void *__real_realloc(void *, size_t);
void __real_free(void *);

static size_t calls, fail_at, live;

static int fail_now(void) { calls++; return fail_at && calls == fail_at; }

void m3_alloc_reset(void) { calls=0; fail_at=0; }
void m3_alloc_fail_at(size_t ordinal) { calls=0; fail_at=ordinal; }
size_t m3_alloc_calls(void) { return calls; }
size_t m3_alloc_live(void) { return live; }

void *__wrap_malloc(size_t n)
{
    void *p;
    if (fail_now()) return NULL;
    p=__real_malloc(n); if(p) live++; return p;
}

void *__wrap_calloc(size_t n, size_t z)
{
    void *p;
    if (fail_now()) return NULL;
    p=__real_calloc(n,z); if(p) live++; return p;
}

void *__wrap_realloc(void *old, size_t n)
{
    void *p;
    if (fail_now()) return NULL;
    p=__real_realloc(old,n);
    if(p && !old) live++;
    return p;
}

void __wrap_free(void *p)
{
    if(p) live--;
    __real_free(p);
}
