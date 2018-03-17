#ifndef _JV_POOL_H_INCLUDED_
#define _JV_POOL_H_INCLUDED_

#include <jv_core.h>

#define JV_ALLOC_MIN_SIZE 256

#define JV_ALLOC_DEFAULT_SIZE 0x4000 /* 0x4000  = 1024 * 16 = 16384 */

/**
 *  (2<<31) = 2147483648
 *  (2<<63) = 9223372036854775808s
 **/
#define JV_ALLOC_MAX_SIZE (2147483648 - JV_ALLOC_MIN_SIZE)

typedef struct jv_block_s jv_block_t;
typedef struct jv_lump_s jv_lump_t;

struct jv_block_s {
  jv_block_t *next;
  size_t size;
};

struct jv_lump_s {
  jv_lump_t *prev;
  jv_lump_t *next;
  unsigned size : 31;
  unsigned used : 1;
};

struct jv_pool_s {
  jv_log_t *log;
  size_t max; /* block max size */
  jv_block_t *first;
  jv_block_t *last;
  jv_lump_t *lump;
  jv_lump_t *pos; /* current idle lump's position */
};

jv_pool_t *jv_pool_create(jv_log_t *log, size_t size);

void *jv_pool_alloc(jv_pool_t *pool, size_t size);

void *jv_pool_realloc(jv_pool_t *pool, void *ptr, size_t size);

jv_int_t jv_pool_free(jv_pool_t *pool, void *ptr);

size_t jv_pool_sizeof(jv_pool_t *pool, void *ptr);

jv_int_t jv_pool_exist(jv_pool_t *pool, void *ptr);

jv_int_t jv_pool_recycle(jv_pool_t *pool, void *ptr);

jv_int_t jv_pool_reset(jv_pool_t *pool);

#define jv_pool_each_lump(pool, lump, i) \
  \
for(lump = (pool)->lump, i = 0; i == 0 || lump != (pool)->lump; i++, lump = lump->prev)

#define jv_pool_each_block(pool, block, i) \
  \
for(block = (pool)->first, i = 0; block != NULL; i++, block = block->next)

void jv_pool_destroy(jv_pool_t *pool);

void jv_pool_dump(jv_pool_t *pool, FILE *fd);

#endif /* _JV_POOL_H_INCLUDED_ */
