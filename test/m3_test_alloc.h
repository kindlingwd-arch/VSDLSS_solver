#ifndef M3_TEST_ALLOC_H
#define M3_TEST_ALLOC_H
#include <stddef.h>
void m3_alloc_reset(void);
void m3_alloc_fail_at(size_t ordinal);
size_t m3_alloc_calls(void);
size_t m3_alloc_live(void);
#endif
