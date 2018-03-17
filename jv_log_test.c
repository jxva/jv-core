#include <jv_core.h>

int jv_log_main(int argc, const char *argv[]) {
  
  jv_log_t *log = jv_log_create(NULL, JV_LOG_DEBUG, 0);

  jv_log_debug(log, "Hello, Debug");

  jv_log_debug(log, "%s, %s", "Hello", "Debug");

  jv_log_debug(log, "%s%c %s", "Hello", ',', "Debug");

  jv_log_info(log, "Hello, %s", "Info");

  jv_log_notice(log, "Hello, %s", "World");

  jv_log_warn(log, "Hello, Warn");

  jv_log_error(log, "Hello, %s", "Error");

  jv_log_crit(log, "Hello, %s", "Crit");

  jv_log_alert(log, "Hello, %s", "Alert");

  jv_log_emerg(log, "Hello, %s", "Emerg");

  jv_log_info(log, "Hello, %s", "Info");

  jv_log_destroy(log);

  return 0;
}
