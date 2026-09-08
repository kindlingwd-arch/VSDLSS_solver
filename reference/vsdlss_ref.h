#ifndef VSDLSS_REF_H
#define VSDLSS_REF_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <setjmp.h>
#include <time.h>
#include <sys/times.h>
#include <errno.h>
#include <float.h>
struct __jmp_buf_tag;
typedef struct __jmp_buf_tag __jmp_buf_tag;
struct tms;
typedef struct tms tms;
typedef unsigned char  undefined;
typedef unsigned char  undefined1;
typedef unsigned short undefined2;
typedef unsigned int   undefined4;
typedef unsigned long long undefined8;
typedef unsigned char  byte;
typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;
typedef unsigned long  ulong;
typedef long long      longlong;
typedef unsigned long long ulonglong;
typedef unsigned long long uint128 __attribute__((aligned(16)));
typedef unsigned char * code;
typedef unsigned char  bytearray;
/* Ghidra decompiler math-macro artifacts */
#ifdef NAN
#undef NAN
#endif
#define NAN(x)   isnan(x)
#ifndef SQRT
#define SQRT(x)  sqrt(x)
#endif
#ifndef FABS
#define FABS(x)  fabs(x)
#endif
#ifndef ABS
#define ABS(x)   fabs(x)
#endif
#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif
/* tcmalloc types referenced by the vendor code */
typedef void FreeList;
typedef void ThreadCache;
typedef void CentralFreeList;
typedef void Span;
typedef void PageHeap;
typedef void PageMap;
/* Ghidra odd-width / concat / sub / carry macros */
typedef int64_t  int7;
typedef uint64_t uint7;
typedef int      int3;
#define CONCAT16(a,b) ((((unsigned long long)(a)) << 48) | ((unsigned long long)(b) & 0xFFFFFFFFFFFFFFULL))
#define CONCAT17(a,b) ((((unsigned long long)(a)) << 56) | ((unsigned long long)(b) & 0xFFFFFFFFFFFFFFFFULL))
#define CONCAT24(a,b) ((((unsigned long long)(a)) << 32) | ((unsigned long long)(b) & 0xFFFFFFFFULL))
#define CONCAT31(a,b) ((((unsigned long long)(a)) << 8)  | ((unsigned long long)(b) & 0xffULL))
#define CONCAT35(a,b) ((((unsigned long long)(a)) << 40) | ((unsigned long long)(b) & 0xffffffffffULL))
#define CONCAT42(a,b) ((((unsigned long long)(a)) << 16) | ((unsigned long long)(b) & 0xffffULL))
#define CONCAT44(a,b) ((((unsigned long long)(a)) << 32) | ((unsigned long long)(b) & 0xffffffffULL))
#define CONCAT62(a,b) ((((unsigned long long)(a)) << 16) | ((unsigned long long)(b) & 0xffffULL))
#define CONCAT71(a,b) ((((unsigned long long)(a)) << 8)  | ((unsigned long long)(b) & 0xffULL))
#define SUB41(v,n)    ((unsigned char)(((unsigned long long)(v)) >> (8*(n))))
#define SUB164(v,n)   ((unsigned int)(((unsigned long long)(v)) >> (8*(n))))
#define SUB168(v,n)   ((unsigned long long)(((unsigned long long)(v)) >> (8*(n))))
#define CARRY1(a,b)   ((((unsigned long long)(a)) + (unsigned long long)(b)) > 255 ? 1 : 0)
#define CARRY8(a,b)   (((((unsigned long long)(a)) + (unsigned long long)(b)) >> 64) & 1)
#define SBORROW4(a,b) (((unsigned int)(a)) < ((unsigned int)(b)))
#define SBORROW8(a,b) (((unsigned long long)(a)) < ((unsigned long long)(b)))
extern void *__cdecl_malloc_wrap(size_t);
#include "vsdlss_globals.h"
#endif
