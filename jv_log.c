#include <jv_log.h>

#define JV_LINE_BUFFER_SIZE 10240

static const char *prioritys[] = {"emerg", "alert", "crit", "error", "warn", "notice", "info", "debug"};

static inline void jv_log_write(jv_log_t *log, jv_uint_t priority, const char *fmt, va_list *args);

static inline void jv_log_write(jv_log_t *log, jv_uint_t priority, const char *fmt, va_list *args) {
  char datetime[20];
  struct tm ptm;
  jv_int_t n;
  char buf[JV_LINE_BUFFER_SIZE];

  strftime(datetime, sizeof(datetime), "%Y-%m-%d %X", jv_localtime(&ptm));

  jv_memzero(buf, JV_LINE_BUFFER_SIZE);

  n = sprintf(buf, "%s %-8s", datetime, prioritys[priority]);
  n += vsprintf(buf + n, fmt, *args);
  n += snprintf(buf + n, 2, "\n");

  fwrite(buf, n, 1, log->fd);

  /*
    fprintf(log->fd, "%-6lu, %s %-8s ", log->line_count, datetime, prioritys[priority]);
    vfprintf(log->fd, fmt, *args);
    fprintf(log->fd, "\n");
  */

  log->line_count++;

  if (log->cache_line == 0 || log->line_count % log->cache_line == 0) {
    fflush(log->fd);
  }
}

jv_log_t *jv_log_create(u_char *filename, jv_uint_t priority, jv_uint_t cache_line) {
  jv_log_t *log = malloc(sizeof(jv_log_t));

  if (log == NULL) {
    return (jv_log_t *) NULL;
  }

  log->fd = filename == NULL ? stdout : fopen((char *) filename, "a+");
  log->priority = priority;
  log->line_count = 0;
  log->cache_line = cache_line;

  return log;
}

void jv_log_emerg(jv_log_t *log, const char *fmt, ...) {
  va_list args;

  if (log->priority < JV_LOG_EMERG) {
    return;
  }

  va_start(args, fmt);
  jv_log_write(log, JV_LOG_EMERG, fmt, &args);
  va_end(args);

  /* jv_log_destroy(log);
   exit(-1); */
}

void jv_log_alert(jv_log_t *log, const char *fmt, ...) {
  va_list args;

  if (log->priority < JV_LOG_ALERT) {
    return;
  }

  va_start(args, fmt);
  jv_log_write(log, JV_LOG_ALERT, fmt, &args);
  va_end(args);
}

void jv_log_crit(jv_log_t *log, const char *fmt, ...) {
  va_list args;

  if (log->priority < JV_LOG_CRIT) {
    return;
  }

  va_start(args, fmt);
  jv_log_write(log, JV_LOG_CRIT, fmt, &args);
  va_end(args);
}

void jv_log_error(jv_log_t *log, const char *fmt, ...) {
  va_list args;

  if (log->priority < JV_LOG_ERROR) {
    return;
  }

  va_start(args, fmt);
  jv_log_write(log, JV_LOG_ERROR, fmt, &args);
  va_end(args);
}

void jv_log_warn(jv_log_t *log, const char *fmt, ...) {
  va_list args;

  if (log->priority < JV_LOG_WARN) {
    return;
  }

  va_start(args, fmt);
  jv_log_write(log, JV_LOG_WARN, fmt, &args);
  va_end(args);
}

void jv_log_notice(jv_log_t *log, const char *fmt, ...) {
  va_list args;

  if (log->priority < JV_LOG_NOTICE) {
    return;
  }

  va_start(args, fmt);
  jv_log_write(log, JV_LOG_NOTICE, fmt, &args);
  va_end(args);
}

void jv_log_info(jv_log_t *log, const char *fmt, ...) {
  va_list args;

  if (log->priority < JV_LOG_INFO) {
    return;
  }

  va_start(args, fmt);
  jv_log_write(log, JV_LOG_INFO, fmt, &args);
  va_end(args);
}

void jv_log_debug(jv_log_t *log, const char *fmt, ...) {
  va_list args;

  if (log->priority < JV_LOG_DEBUG) {
    return;
  }

  va_start(args, fmt);
  jv_log_write(log, JV_LOG_DEBUG, fmt, &args);
  va_end(args);
}

void jv_log_destroy(jv_log_t *log) {
  if (log != (jv_log_t *) NULL) {
    fflush(log->fd);
    fclose(log->fd);
    free(log);
  }
}
