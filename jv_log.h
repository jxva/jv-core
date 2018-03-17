#ifndef _JV_LOG_H_INCLUDE_
#define _JV_LOG_H_INCLUDE_

#include <jv_core.h>

#define JV_LOG_EMERG 0 /* program will exit(-1) */
#define JV_LOG_ALERT 1
#define JV_LOG_CRIT 2
#define JV_LOG_ERROR 3
#define JV_LOG_WARN 4
#define JV_LOG_NOTICE 5
#define JV_LOG_INFO 6
#define JV_LOG_DEBUG 7

struct jv_log_s {
  FILE *fd;
  jv_uint_t line_count;
  unsigned priority : 4;
  unsigned cache_line : 28;
};

jv_log_t *jv_log_create(u_char *filename, jv_uint_t priority, jv_uint_t cache_line);

void jv_log_emerg(jv_log_t *log, const char *fmt, ...);

void jv_log_alert(jv_log_t *log, const char *fmt, ...);

void jv_log_crit(jv_log_t *log, const char *fmt, ...);

void jv_log_error(jv_log_t *log, const char *fmt, ...);

void jv_log_warn(jv_log_t *log, const char *fmt, ...);

void jv_log_notice(jv_log_t *log, const char *fmt, ...);

void jv_log_info(jv_log_t *log, const char *fmt, ...);

void jv_log_debug(jv_log_t *log, const char *fmt, ...);

void jv_log_destroy(jv_log_t *log);

/* for c99
#define jv_log_debug(log, ...)    \
  if ((log)->priority >= JV_LOG_DEBUG) \
  jv_log(log, JV_LOG_DEBUG, __VA_ARGS__) */

#endif /* _JV_LOG_H_INCLUDE_ */
