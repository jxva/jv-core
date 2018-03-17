#include <jv_core.h>

int main(int argc, const char *argv[]) {
  jv_log_t *log = jv_log_create(NULL, JV_LOG_DEBUG, 0);
  jv_pool_t *pool = jv_pool_create(log, JV_ALLOC_DEFAULT_SIZE);

  jv_pool_destroy(pool);
  jv_log_destroy(log);
  return 0;
}
