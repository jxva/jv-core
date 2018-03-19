#include <jv_log.h>

#define JV_LOG_LINE_BUFFER_SIZE 10240

static const char *prioritys[] = {"emerg", "alert", "crit", "error", "warn", "notice", "info", "debug"};

static inline void jv_log_out(FILE *fd, const char *fmt, va_list *args);

static inline void jv_log_write(jv_log_t *log, jv_uint_t priority, const char *fmt, va_list *args);

static inline void jv_log_write(jv_log_t *log, jv_uint_t priority, const char *fmt, va_list *args) {
  char datetime[20];
  struct tm ptm;
  jv_int_t n;
  char buf[JV_LOG_LINE_BUFFER_SIZE];

  strftime(datetime, sizeof(datetime), "%Y-%m-%d %X", jv_localtime(&ptm));

  jv_memzero(buf, JV_LOG_LINE_BUFFER_SIZE);

  n = sprintf(buf, "%s %-8s", datetime, prioritys[priority]);
  n += vsprintf(buf + n, fmt, *args);
  n += snprintf(buf + n, 2, "\n");

  fwrite(buf, n, 1, log->fd);

  /*
    fprintf(log->fd, "%-6lu, %s %-8s ", log->count, datetime, prioritys[priority]);
    vfprintf(log->fd, fmt, *args);
    fprintf(log->fd, "\n");
  */

  log->count++;

  if (log->mode == JV_LOG_FLUSH_MODE || log->count % 10 == 0) {
    fflush(log->fd);
  }
}

static inline void jv_log_out(FILE *fd, const char *fmt, va_list *args) {
  char datetime[20];
  struct tm ptm;
  jv_int_t n;
  char buf[JV_LOG_LINE_BUFFER_SIZE];

  strftime(datetime, sizeof(datetime), "%Y-%m-%d %X", jv_localtime(&ptm));

  jv_memzero(buf, JV_LOG_LINE_BUFFER_SIZE);

  n = sprintf(buf, "%s %-8s", datetime, fd == stdout ? "stdout" : "stderr");
  n += vsprintf(buf + n, fmt, *args);
  n += snprintf(buf + n, 2, "\n");

  fwrite(buf, n, 1, fd);

  /*fprintf(fd, buf);*/
  fflush(fd);
}

jv_log_t *jv_log_create(u_char *filename, unsigned priority, unsigned mode) {
  jv_log_t *log = malloc(sizeof(jv_log_t));

  if (log == NULL) {
    jv_log_stderr("jv_log_create() failed");
    return (jv_log_t *) NULL;
  }

  if(filename==NULL) {
    log->fd = stdout;
  } else {
    FILE *fd;
    if((fd = fopen((char *) filename, "a+"))==NULL) {
      free(log);
      jv_log_stderr("open file '%s' failed: %s", (char *)filename, strerror(errno));
      return (jv_log_t *) NULL;
    }
    log->fd = fd;
  }

  log->priority = priority;
  log->count = 0;
  log->mode = mode == JV_LOG_FLUSH_MODE ? JV_LOG_FLUSH_MODE : JV_LOG_CACHE_MODE;

  return log;
}

void jv_log_stdout(const char *fmt, ...) {
#ifdef DEBUG
  va_list args;

  va_start(args, fmt);
  jv_log_out(stdout, fmt, &args);
  va_end(args);
#endif
}

void jv_log_stderr(const char *fmt, ...) {
  va_list args;

  va_start(args, fmt);
  jv_log_out(stderr, fmt, &args);
  va_end(args);
}

void jv_log_emerg(jv_log_t *log, const char *fmt, ...) {
  va_list args;

  if (log->priority < JV_LOG_EMERG) {
    return;
  }

  va_start(args, fmt);
  jv_log_write(log, JV_LOG_EMERG, fmt, &args);
  va_end(args);
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
