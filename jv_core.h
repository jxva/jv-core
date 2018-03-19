#ifndef _JV_CORE_H_INCLUDE_
#define _JV_CORE_H_INCLUDE_

#include <jv_init.h>

/* typedef enum { JV_ERROR = -1, JV_OK = 0 } jv_bool_t; */

#define JV_OK 0
#define JV_ERROR -1
#define JV_AGAIN -2
#define JV_BUSY -3
#define JV_DONE -4
#define JV_DECLINED -5
#define JV_ABORT -6

#ifndef intptr_t
#define intptr_t long
#define uintptr_t unsigned long
#endif

#ifndef _UINT64_T
/* #if JV_PLATFORM_WORD == 32 */
#ifdef __i386__
typedef signed long long int64_t;
typedef unsigned long long uint64_t;
#else /*  __x86_64__ */
typedef signed long int64_t;
typedef unsigned long uint64_t;
#endif
#endif

typedef intptr_t jv_int_t;
typedef uintptr_t jv_uint_t;

typedef unsigned char u_char;

typedef unsigned char byte;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef struct jv_log_s jv_log_t;
typedef struct jv_pool_s jv_pool_t;
typedef struct jv_string_s jv_string_t;

#define jv_abs(a) (((a) >= 0) ? (a) : -(a))
#define jv_max(a, b) ((a > b) ? (a) : (b))
#define jv_min(a, b) ((a < b) ? (a) : (b))
#define jv_is_odd(x) (((x) &0x1) == 1)
#define jv_is_even(x) (((x) &0x1) == 0)

/* = a /2 + b/2 */
#define jv_average(a, b) (((a) & (b)) + (((a) ^ (b)) >> 1))
#define jv_array_count(a) ((size_t)(sizeof(a) / sizeof(*(a))))

#define jv_ctoi(c) ((uint8_t)((c) -0x30))
#define jv_itoc(i) ((u_char)((i) + 0x30))

#define jv_align(d, a) (((d) + (a - 1)) & ~(a - 1))
#define jv_align_ptr(p, a) (u_char *) (((uintptr_t)(p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))

#define jv_memzero(buf, n) (void *) memset(buf, 0, n)
#define jv_memset(buf, c, n) (void *) memset(buf, c, n)
#define jv_memcpy(dst, src, n) (void *) memcpy(dst, src, n)
#define jv_memmove(dst, src, n) (void *) memmove(dst, src, n)

#define jv_memcmp(s1, s2, n) memcmp(s1, s2, n)

#include <jv_log.h>
#include <jv_pool.h>
#include <jv_string.h>
#include <jv_time.h>

#endif /* _JV_CORE_H_INCLUDE_ */
