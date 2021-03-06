#include <assert.h>
#include <jv_pool.h>

static jv_log_t *_log;

void test1(void) {
  jv_pool_t *pool;
  void *a, *b, *c, *d, *e, *f, *g, *h, *i, *j;
  jv_uint_t base;

  jv_lump_t *lump;
  u_char *s;

  pool = jv_pool_create(_log, JV_POOL_DEFAULT_SIZE, 1);

  base = (jv_uint_t) pool - sizeof(jv_block_t);

  printf("base: %lu, lump size: %lu\n\n", (jv_uint_t) base, (jv_uint_t) sizeof(jv_lump_t));
  printf("pool: %lu, idle: %lu, block: %lu, block->size: %lu, block->next: %lu, lump: %lu, lump->size: %lu, lump->next: %lu\n",
         (jv_uint_t) pool - base, (jv_uint_t) pool->idle - base, (jv_uint_t) pool->first - base, (jv_uint_t) pool->first->size,
         (jv_uint_t) pool->first->next, (jv_uint_t) pool->lump - base, (jv_uint_t) pool->lump->size, (jv_uint_t) pool->lump->next - base);

  /* jv_pool_dump(pool, stdout); */

  assert((u_char *) pool->first + sizeof(jv_block_t) == (u_char *) pool);
  assert((u_char *) pool->first + sizeof(jv_block_t) + sizeof(jv_pool_t) == (u_char *) pool->lump);
  assert((u_char *) pool + sizeof(jv_pool_t) == (u_char *) (pool->lump));

  a = jv_pool_alloc(pool, 40);

  printf("pool: %lu, idle: %lu, block: %lu, block->size: %lu, block->next: %lu, lump: %lu, lump->size: %lu, lump->next: %lu\n",
         (jv_uint_t) pool - base, (jv_uint_t) pool->idle - base, (jv_uint_t) pool->first - base, (jv_uint_t) pool->first->size,
         (jv_uint_t) pool->first->next, (jv_uint_t) pool->lump - base, (jv_uint_t) pool->lump->size, (jv_uint_t) pool->lump->next - base);

  printf("a address: %lu, idle: %lu\n", (jv_uint_t) a - base, (jv_uint_t) pool->idle - base);

  assert(jv_pool_sizeof(pool, a) == 40);
  assert((u_char *) a == (u_char *) pool->lump->next + sizeof(jv_lump_t));

  b = jv_pool_alloc(pool, 56);

  printf("pool: %lu, idle: %lu, block: %lu, block->size: %lu, block->next: %lu, lump: %lu, lump->size: %lu, lump->next: %lu\n",
         (jv_uint_t) pool - base, (jv_uint_t) pool->idle - base, (jv_uint_t) pool->first - base, (jv_uint_t) pool->first->size,
         (jv_uint_t) pool->first->next, (jv_uint_t) pool->lump - base, (jv_uint_t) pool->lump->size, (jv_uint_t) pool->lump->next - base);

  printf("b address: %lu, idle: %lu\n", (jv_uint_t) b - base, (jv_uint_t) pool->idle - base);

  assert(jv_pool_sizeof(pool, b) == 56);
  assert((u_char *) b == (u_char *) pool->lump->next + sizeof(jv_lump_t));

  c = jv_pool_alloc(pool, 32);

  printf("pool: %lu, idle: %lu, block: %lu, block->size: %lu, block->next: %lu, lump: %lu, lump->size: %lu, lump->next: %lu\n",
         (jv_uint_t) pool - base, (jv_uint_t) pool->idle - base, (jv_uint_t) pool->first - base, (jv_uint_t) pool->first->size,
         (jv_uint_t) pool->first->next, (jv_uint_t) pool->lump - base, (jv_uint_t) pool->lump->size, (jv_uint_t) pool->lump->next - base);

  printf("c address: %lu, idle: %lu\n", (jv_uint_t) c - base, (jv_uint_t) pool->idle - base);

  assert(jv_pool_sizeof(pool, c) == 32);
  assert((u_char *) c == (u_char *) pool->lump->next + sizeof(jv_lump_t));

  d = jv_pool_alloc(pool, 8);

  printf("pool: %lu, idle: %lu, block: %lu, block->size: %lu, block->next: %lu, lump: %lu, lump->size: %lu, lump->next: %lu\n",
         (jv_uint_t) pool - base, (jv_uint_t) pool->idle - base, (jv_uint_t) pool->first - base, (jv_uint_t) pool->first->size,
         (jv_uint_t) pool->first->next, (jv_uint_t) pool->lump - base, (jv_uint_t) pool->lump->size, (jv_uint_t) pool->lump->next - base);

  printf("d address: %lu, idle: %lu\n", (jv_uint_t) d - base, (jv_uint_t) pool->idle - base);

  assert(jv_pool_sizeof(pool, d) == 8);
  assert((u_char *) d == (u_char *) pool->lump->next + sizeof(jv_lump_t));

  e = jv_pool_alloc(pool, 4);

  printf("pool: %lu, idle: %lu, block: %lu, block->size: %lu, block->next: %lu, lump: %lu, lump->size: %lu, lump->next: %lu\n",
         (jv_uint_t) pool - base, (jv_uint_t) pool->idle - base, (jv_uint_t) pool->first - base, (jv_uint_t) pool->first->size,
         (jv_uint_t) pool->first->next, (jv_uint_t) pool->lump - base, (jv_uint_t) pool->lump->size, (jv_uint_t) pool->lump->next - base);

  printf("e address: %lu, idle: %lu\n", (jv_uint_t) e - base, (jv_uint_t) pool->idle - base);

  assert(jv_pool_sizeof(pool, e) == jv_align(4, JV_WORD_SIZE / 8));
  assert((u_char *) e == (u_char *) pool->lump->next + sizeof(jv_lump_t));

  f = jv_pool_alloc(pool, 24);

  printf("pool: %lu, idle: %lu, block: %lu, block->size: %lu, block->next: %lu, lump: %lu, lump->size: %lu, lump->next: %lu\n",
         (jv_uint_t) pool - base, (jv_uint_t) pool->idle - base, (jv_uint_t) pool->first - base, (jv_uint_t) pool->first->size,
         (jv_uint_t) pool->first->next, (jv_uint_t) pool->lump - base, (jv_uint_t) pool->lump->size, (jv_uint_t) pool->lump->next - base);

  printf("f address: %lu, idle: %lu\n", (jv_uint_t) f - base, (jv_uint_t) pool->idle - base);

  assert(jv_pool_sizeof(pool, f) == 24);
  assert((u_char *) f == (u_char *) pool->lump->next + sizeof(jv_lump_t));

  g = jv_pool_alloc(pool, 104);

  printf("pool: %lu, idle: %lu, block: %lu, block->size: %lu, block->next: %lu, lump: %lu, lump->size: %lu, lump->next: %lu\n",
         (jv_uint_t) pool - base, (jv_uint_t) pool->idle - base, (jv_uint_t) pool->first - base, (jv_uint_t) pool->first->size,
         (jv_uint_t) pool->first->next, (jv_uint_t) pool->lump - base, (jv_uint_t) pool->lump->size, (jv_uint_t) pool->lump->next - base);

  printf("g address: %lu, idle: %lu\n", (jv_uint_t) g - base, (jv_uint_t) pool->idle - base);

  assert(jv_pool_sizeof(pool, g) == 104);
  assert((u_char *) g == (u_char *) pool->lump->next + sizeof(jv_lump_t));

  h = jv_pool_alloc(pool, 1); /* 12 */
  assert(jv_pool_sizeof(pool, h) == jv_align(1, JV_WORD_SIZE / 8));

  i = jv_pool_alloc(pool, 310);
  assert(jv_pool_sizeof(pool, i) == jv_align(310, JV_WORD_SIZE / 8));

  j = jv_pool_alloc(pool, 12312);
  assert(jv_pool_sizeof(pool, j) == jv_align(12312, JV_WORD_SIZE / 8));

  jv_pool_dump(pool, stdout);

  assert(jv_pool_free(pool, j) == JV_OK);

  j = jv_pool_alloc(pool, 9000);

  assert(jv_pool_sizeof(pool, j) == jv_align(9000, JV_WORD_SIZE / 8));

  assert(jv_pool_free(pool, h) == JV_OK);

  assert(jv_pool_free(pool, c) == JV_OK);

  assert(jv_pool_free(pool, f) == JV_OK);

  assert(jv_pool_free(pool, a) == JV_OK);

  assert(jv_pool_free(pool, e) == JV_OK);

  assert(jv_pool_free(pool, d) == JV_OK);

  assert(jv_pool_free(pool, b) == JV_OK);

  assert(jv_pool_free(pool, g) == JV_OK);

  assert(jv_pool_free(pool, i) == JV_OK);

  assert(jv_pool_free(pool, j) == JV_OK);

  jv_pool_dump(pool, stdout);

  h = jv_pool_alloc(pool, 36);
  assert(jv_pool_sizeof(pool, h) == jv_align(36, JV_WORD_SIZE / 8));

  assert(jv_pool_free(pool, h) == JV_OK);

  s = malloc(4);
  assert(jv_pool_sizeof(pool, s) == 0);
  assert(jv_pool_free(pool, s) == JV_ERROR);
  free(s);

  lump = (jv_lump_t *) ((u_char *) pool->first + sizeof(jv_pool_t) + sizeof(jv_block_t));

  assert(lump->used == 0);
  assert(lump->size == pool->size);

  jv_pool_destroy(pool);
}

void test2(void) {
  jv_pool_t *pool;
  unsigned i;

  pool = jv_pool_create(_log, 1024 * 16, 1);

  srand(time(NULL));
  for (i = 0; i < 10000; i++) {
    jv_uint_t j = rand() % 1001;
    jv_uint_t k = rand() % 10001;
    size_t size = j * k;
    jv_lump_t *lump = jv_pool_alloc(pool, size);
    /* printf("allocate memory size: %lu\n", j); */
    if (lump == NULL) {
      printf("allocate memory error: %lu\n", (jv_uint_t) size);
      break;
    }
    /*printf("allocate memory size:%lu,  %lu, %lu\n", (jv_uint_t)lump, jv_align(size, JV_WORD_SIZE / 8), (jv_uint_t) jv_pool_sizeof(pool, lump));*/
    /* assert(jv_pool_sizeof(pool, lump) == jv_align(size, JV_WORD_SIZE / 8)); */
    assert(jv_pool_free(pool, lump) == JV_OK);
  }
  /* jv_pool_dump(pool,stdout); */
  jv_pool_destroy(pool);
}

void test3(void) {
  jv_pool_t *pool;
  unsigned i;

  pool = jv_pool_create(_log, 1024 * 16, 1);

  srand(time(NULL));
  for (i = 0; i < 10000; i++) {
    jv_uint_t j = rand() % 1001;
    jv_lump_t *lump = jv_pool_alloc(pool, j);
    /* printf("allocate memory size: %lu\n", j); */
    if (lump == NULL) {
      printf("allocate memory error: %u\n", i);
      break;
    }
    assert(jv_pool_free(pool, lump) == JV_OK);
  }
  /* jv_pool_dump(pool,stdout); */
  jv_pool_destroy(pool);
}

void test4(void) {
  jv_pool_t *pool;
  unsigned i, *s;
  pool = jv_pool_create(_log, 1024 * 16, 1);

  srand(time(NULL));
  for (i = 0; i < 50000; i++) {
    jv_uint_t j = rand() % 1001 + 1;
    assert((s = jv_pool_alloc(pool, j)) != NULL);
    assert(jv_pool_free(pool, s) == JV_OK);
  }
  jv_pool_destroy(pool);
}

void test5(void) {
  jv_pool_t *pool;

  pool = jv_pool_create(_log, 1024 * 16, 1);

  jv_pool_alloc(pool, 432141);
  jv_pool_alloc(pool, 10000);
  jv_pool_alloc(pool, 10000);

  assert(jv_pool_free(pool, jv_pool_alloc(pool, 432141)) == JV_OK);
  assert(jv_pool_free(pool, jv_pool_alloc(pool, 4)) == JV_OK);
  assert(jv_pool_free(pool, jv_pool_alloc(pool, 634)) == JV_OK);
  assert(jv_pool_free(pool, jv_pool_alloc(pool, 10000)) == JV_OK);
  assert(jv_pool_free(pool, jv_pool_alloc(pool, 10000)) == JV_OK);
  assert(jv_pool_free(pool, jv_pool_alloc(pool, 324542)) == JV_OK);
  /*assert(jv_pool_free(pool, jv_pool_alloc(pool, 0x1fffffff)) == JV_OK);*/
  assert(jv_pool_free(pool, jv_pool_alloc(pool, 41233)) == JV_OK);

  /* jv_pool_alloc(pool, 0x1fffffff); */

  /* jv_pool_dump(pool,stdout); */
  jv_pool_destroy(pool);
}

void test6(void) {
  jv_pool_t *pool;
  char *a, *b, *c, *d;

  pool = jv_pool_create(_log, 128, 1);

  /* printf("sizeof(jv_pool_t): %lu\n",sizeof(jv_pool_t)); */
  printf("pool: %lu, idle: %lu, block: %lu, block->size: %lu, block->next: %lu, lump: %lu, lump->size: %lu, lump->next: %lu\n", (jv_uint_t) pool,
         (jv_uint_t) pool->idle, (jv_uint_t) pool->first, (jv_uint_t) pool->first->size, (jv_uint_t) pool->first->next, (jv_uint_t) pool->lump,
         (jv_uint_t) pool->lump->size, (jv_uint_t) pool->lump->next);

  jv_pool_dump(pool, stdout);

  assert((u_char *) pool->first + sizeof(jv_block_t) == (u_char *) pool);
  assert((u_char *) pool->first + sizeof(jv_block_t) + sizeof(jv_pool_t) == (u_char *) pool->lump);
  assert((u_char *) pool + sizeof(jv_pool_t) == (u_char *) (pool->lump));

  a = jv_pool_alloc(pool, 40);

  b = jv_pool_alloc(pool, 56);

  c = jv_pool_alloc(pool, 32);

  jv_pool_dump(pool, stdout);

  d = jv_pool_realloc(pool, c, 48);

  assert(c - 16 == d);

  jv_pool_dump(pool, stdout);

  assert(jv_pool_recycle(pool, a) == JV_OK);

  assert(jv_pool_recycle(pool, b) == JV_OK);

  assert(jv_pool_recycle(pool, c) == JV_ERROR);

  assert(jv_pool_recycle(pool, d) == JV_OK);

  jv_pool_dump(pool, stdout);

  jv_pool_destroy(pool);
}

void test7(void) {
#define C 10000
  jv_pool_t *pool;
  jv_lump_t *lump[C];
  unsigned i;

  pool = jv_pool_create(_log, 1024 * 16, 1);

  srand(time(NULL));
  for (i = 0; i < C; i++) {
    jv_uint_t j = rand() % 5 + 1;
    jv_uint_t k = (rand() % 1024 * 3) + 1;
    assert((lump[i] = jv_pool_alloc(pool, j * k)) != NULL);
  }

  printf("---- pool monitor, block count: %u, lump count: %d\n", pool->block_count, pool->lump_count);

  for (i = 0; i < C; i++) {
    assert(jv_pool_free(pool, lump[i]) == JV_OK);
  }

  printf("---- pool monitor, block count: %u, lump count: %d\n", pool->block_count, pool->lump_count);

  /* jv_pool_dump(pool,stdout); */

  jv_pool_destroy(pool);
}

void test8(void) {
  jv_pool_t *pool;

  pool = jv_pool_create(_log, 1024 * 16, 1);

  jv_pool_dump(pool, stdout);

  assert(jv_pool_reset(pool) == JV_OK);

  jv_pool_alloc(pool, 4432);
  jv_pool_alloc(pool, 412);
  jv_pool_alloc(pool, 3);

  jv_pool_dump(pool, stdout);

  assert(jv_pool_reset(pool) == JV_OK);

  jv_pool_dump(pool, stdout);
  jv_pool_destroy(pool);
}

void test9(void) {
  jv_pool_t *pool;

  pool = jv_pool_create(_log, JV_POOL_MIN_SIZE, 1);

  jv_pool_alloc(pool, 4432);
  jv_pool_alloc(pool, 412);
  jv_pool_alloc(pool, 3);
  jv_pool_alloc(pool, 12);

  jv_pool_dump(pool, stdout);

  assert(jv_pool_reset(pool) == JV_OK);

  jv_pool_dump(pool, stdout);
  jv_pool_destroy(pool);
}

void test10(void) {
  jv_pool_t *pool;

  pool = jv_pool_create(_log, JV_POOL_MAX_SIZE, 1);

  jv_pool_dump(pool, stdout);

  jv_pool_alloc(pool, 4432);
  jv_pool_alloc(pool, 412);
  jv_pool_alloc(pool, 3);
  jv_pool_alloc(pool, 12);

  /*assert(jv_pool_reset(pool) == JV_OK);*/

  jv_pool_dump(pool, stdout);
  jv_pool_destroy(pool);
}

int main(int argc, char *argv[]) {
  _log = jv_log_create(NULL, JV_LOG_DEBUG, 0);

  printf("test1\n");
  test1();
  printf("test2\n");
  test2();
  printf("test3\n");
  test3();
  printf("test4\n");
  test4();
  printf("test5\n");
  test5();
  printf("test6\n");
  test6();
  printf("test7\n");
  test7();
  printf("test8\n");
  test8();
  printf("test9\n");
  test9();
  printf("test10\n");
  test10();
  return 0;
}
