#ifndef _JV_TIME_H_INCLUDE_
#define _JV_TIME_H_INCLUDE_

#include <jv_core.h>

typedef jv_uint_t jv_msec_t;
typedef struct tm jv_tm_t;
typedef struct timeval jv_timeval_t;

typedef struct {
  time_t sec;
  jv_msec_t msec;
  jv_int_t gmtoff;
} jv_time_t;

ssize_t jv_format_timeval(jv_timeval_t *tv, char *buf, size_t sz);

ssize_t jv_format_time(time_t sec, char *buf, size_t sz);

jv_msec_t jv_time_update(void);

/*
 * const char* rfc1123fmt = "%a, %d %b %Y %H:%M:%S GMT"
 * Sat, 08 Jun 2013 01:48:51 GMT
 *  */

jv_tm_t *jv_time_parse(const u_char *s, jv_tm_t *tm);

time_t jv_time_translate(const jv_tm_t *tm);

time_t jv_time_convert(const u_char *s);

char *jv_time_gmt(const time_t t, char buf[30]);

jv_tm_t *jv_localtime(jv_tm_t *tm);

#endif /* _JV_TIME_H_INCLUDE_ */
