#include <jv_pool.h>

static void *jv_pool_slb(jv_pool_t *pool, size_t size);

static void *jv_pool_alloc_block(jv_pool_t *pool, size_t size);

static void *jv_pool_alloc_large(jv_pool_t *pool, size_t size);

jv_pool_t *jv_pool_create(jv_log_t *log, size_t size) {
  jv_pool_t *pool;
  jv_block_t *block;
  jv_lump_t *lump;
  u_char *cp;

  if (size < JV_ALLOC_MIN_SIZE) {
    size = JV_ALLOC_DEFAULT_SIZE;
  }

  size = jv_align(size, JV_WORD_SIZE / 8);

  cp = malloc(size + sizeof(jv_block_t) + sizeof(jv_pool_t) + sizeof(jv_lump_t));
  if (cp == NULL) {
    jv_log_crit(log, "jv_pool_create() failed");
    return (jv_pool_t *) NULL;
  }

  block = (jv_block_t *) cp;
  pool = (jv_pool_t *) (cp + sizeof(jv_block_t));
  lump = (jv_lump_t *) (cp + sizeof(jv_block_t) + sizeof(jv_pool_t));

  block->size = size;
  block->next = NULL;

  pool->max = size;
  pool->first = pool->last = block;
  pool->pos = pool->lump = lump->next = lump->prev = lump;

  lump->size = size;
  lump->used = 0;

  pool->log = log;

  jv_log_debug(pool->log, "create a new memory pool, size is %lu", (jv_uint_t) size);

  return pool;
}

static void *jv_pool_slb(jv_pool_t *pool, size_t size) {
  if (size <= pool->max) {
    jv_lump_t *p;

    p = pool->pos;
    do {
      if (p->used == 0 && p->size <= pool->max && p->size >= size) { /* first fit */
        if (p->size <= size + 2 * sizeof(jv_lump_t)) {               /* best fit  */
          p->used = 1;
          jv_log_debug(pool->log, "alloc lump memory using best fit, size is: %u", p->size);
          return (void *) (p + 1);
        } else { /* unfit */
          jv_lump_t *lump;

          lump = (jv_lump_t *) ((u_char *) p + p->size - size);
          lump->size = size;

          lump->used = 1;
          lump->next = p->next;
          p->next = lump;

          lump->next->prev = lump;
          lump->prev = p;

          p->size -= size + sizeof(jv_lump_t);
          jv_log_debug(pool->log, "alloc lump memory using unfit, size is: %u", lump->size);
          return (void *) (lump + 1);
        }
      }
      p = p->next;
    } while (p != pool->pos);

    return jv_pool_alloc_block(pool, size);
  } else {
    jv_lump_t *lump;
    jv_block_t *block;

    for (block = pool->first; block != NULL; block = block->next) {
      if (block->size >= size) {
        lump = (jv_lump_t *) ((u_char *) block + sizeof(jv_block_t));
        if (lump->used == 0 && (jv_uint_t)(size * 1.25 / lump->size) == 1) { /* alloc block fit */
          lump->size = block->size;
          lump->used = 1;
          jv_log_debug(pool->log, "alloc block memory using fit, size is: %u", lump->size);
          return (void *) (lump + 1);
        }
      }
    }

    return jv_pool_alloc_large(pool, size);
  }
}

void *jv_pool_alloc(jv_pool_t *pool, size_t size) {
  void *v;

  if (size <= 0) {
    jv_log_error(pool->log, "alloc memory must be greater than zero");
    return NULL;
  }

  if (size > JV_ALLOC_MAX_SIZE) {
    jv_log_alert(pool->log, "alloc memory is too large, allow max memory size is: %u", JV_ALLOC_MAX_SIZE);
    return NULL;
  }

  size = jv_align(size, JV_WORD_SIZE / 8);

  v = jv_pool_slb(pool, size);
  if (v != NULL) {
    jv_memzero(v, size);
  }
  return v;
}

size_t jv_pool_sizeof(jv_pool_t *pool, void *ptr) {
  jv_lump_t *lump;

  if (jv_pool_exist(pool, ptr) == JV_ERROR) {
    return 0;
  }

  lump = (jv_lump_t *) ((u_char *) ptr - sizeof(jv_lump_t));

  if (lump->size % (JV_WORD_SIZE / 8) != 0) {
    return 0;
  }

  return lump->size;
}

jv_int_t jv_pool_exist(jv_pool_t *pool, void *ptr) {
  jv_lump_t *lump;
  size_t l;

  if (pool == NULL || ptr == NULL) {
    return JV_ERROR;
  }

  l = sizeof(jv_lump_t);
  lump = pool->lump;

  do {
    if ((u_char *) lump + l == (u_char *) ptr /*&& lump->size % (JV_WORD_SIZE / 8) != 0*/) {
      return JV_OK;
    }
    lump = lump->next;
  } while (lump != pool->lump);

  jv_log_error(pool->log, "ptr not exist in memroy pool");

  return JV_ERROR;
}

void *jv_pool_realloc(jv_pool_t *pool, void *ptr, size_t size) {
  if (jv_pool_free(pool, ptr) == JV_OK) {
    return jv_pool_alloc(pool, size);
  }
  return NULL;
}

static void *jv_pool_alloc_block(jv_pool_t *pool, size_t size) {
  jv_block_t *block;
  jv_lump_t *tail;
  u_char *cp;

  cp = malloc(pool->max + sizeof(jv_block_t) + sizeof(jv_lump_t));
  if (cp == NULL) {
    jv_log_alert(pool->log, "alloc block memory failed, alloc size is %lu", (jv_uint_t) pool->max);
    return NULL;
  }

  block = (jv_block_t *) cp;
  block->size = pool->max;
  block->next = pool->last->next;

  pool->last->next = block;
  pool->last = block;

  tail = pool->lump->prev;

  if (size > pool->max - 2 * sizeof(jv_lump_t)) {
    jv_lump_t *lump;
    lump = (jv_lump_t *) (cp + sizeof(jv_block_t));
    lump->size = pool->max;
    lump->used = 1;

    lump->next = pool->lump;
    tail->next = lump;

    lump->next->prev = lump;
    lump->prev = tail;

    pool->pos = lump;

    jv_log_debug(pool->log, "alloc a new block with only one lump,  alloc all free memory of ths block to applicant");

    return (void *) (lump + 1);
  } else {
    jv_lump_t *free, *alloc;
    free = (jv_lump_t *) (cp + sizeof(jv_block_t));
    free->size = pool->max - size - sizeof(jv_lump_t);
    free->used = 0;

    alloc = (jv_lump_t *) ((u_char *) free + sizeof(jv_lump_t) + free->size);
    alloc->size = size;
    alloc->used = 1;

    alloc->next = pool->lump;
    free->next = alloc;
    tail->next = free;

    alloc->next->prev = alloc;
    alloc->prev = free;
    free->prev = tail;

    pool->pos = free;

    jv_log_debug(pool->log, "alloc a new block with two lumps, first lump reserved for new block, last one lump alloc to applicant");
    return (void *) (alloc + 1);
  }
}

static void *jv_pool_alloc_large(jv_pool_t *pool, size_t size) {
  jv_block_t *block;
  jv_lump_t *tail, *lump;
  u_char *cp;

  cp = malloc(size + sizeof(jv_block_t) + sizeof(jv_lump_t));
  if (cp == NULL) {
    jv_log_alert(pool->log, "alloc large memory failed, alloc size is %lu", (jv_uint_t) size);
    return NULL;
  }

  block = (jv_block_t *) cp;
  block->size = size;
  block->next = pool->last->next;

  pool->last->next = block;
  pool->last = block;

  tail = pool->lump->prev;

  lump = (jv_lump_t *) (cp + sizeof(jv_block_t));
  lump->size = size;
  lump->used = 1;

  lump->next = pool->lump;
  tail->next = lump;

  lump->next->prev = lump;
  lump->prev = tail;

  pool->pos = lump;

  jv_log_debug(pool->log, "alloc a new large block with only one lump, alloc all free memory of ths large block to applicant");
  return (void *) (lump + 1);
}

jv_int_t jv_pool_free(jv_pool_t *pool, void *ptr) {
  jv_lump_t *prior, *idle;

  if (jv_pool_exist(pool, ptr) == JV_ERROR) {
    return JV_ERROR;
  }

  idle = (jv_lump_t *) ((u_char *) ptr - sizeof(jv_lump_t));

  if (/*idle->used != 1 &&*/ idle->size % (JV_WORD_SIZE / 8) != 0) {
    jv_log_error(pool->log, "free's pointer is not in the memory pool");
    return JV_ERROR;
  }

  idle->used = 0;
  pool->pos = idle;

  if ((u_char *) idle + sizeof(jv_lump_t) + idle->size == (u_char *) idle->next) { /* nearby the next lump */
    if (idle->next->used == 0) {                                                   /* next lump is free，then merge */
      idle->size = idle->size + sizeof(jv_lump_t) + idle->next->size;
      idle->next = idle->next->next;
      idle->next->prev = idle;
      pool->pos = idle;
    }
  }

  prior = idle->prev;

  if ((u_char *) prior + sizeof(jv_lump_t) + prior->size == (u_char *) idle) { /* nearby the previous lump*/
    if (prior->used == 0) {                                                    /*  previous lump is free，then merge */
      prior->size = prior->size + sizeof(jv_lump_t) + idle->size;
      prior->next = prior->next->next;
      prior->next->prev = prior;
      pool->pos = prior;
    }
  }

  return JV_OK;
}

jv_int_t jv_pool_recycle(jv_pool_t *pool, void *ptr) {
  jv_lump_t *lump;

  if (jv_pool_exist(pool, ptr) == JV_ERROR) {
    return JV_ERROR;
  }

  lump = (jv_lump_t *) ((u_char *) ptr - sizeof(jv_lump_t));

  if (lump->size % (JV_WORD_SIZE / 8) != 0) {
    return JV_ERROR;
  }

  lump->used = 0;

  return JV_OK;
}

jv_int_t jv_pool_reset(jv_pool_t *pool) {
  jv_block_t *first, *block, *tmp = NULL;
  jv_lump_t *lump;

  first = pool->first;

  if (pool == NULL) {
    return JV_ERROR;
  }

  lump = pool->lump;
  lump->size = pool->max;
  lump->used = 0;
  lump->next = lump->prev = lump;

  for (block = first->next; block != NULL; block = tmp) {
    tmp = block->next;
    free(block);
  }

  block = first;
  block->size = pool->max;
  block->next = NULL;

  pool->first = pool->last = block;
  pool->pos = lump;

  return JV_OK;
}

void jv_pool_destroy(jv_pool_t *pool) {
  jv_block_t *block, *tmp = NULL;

  jv_log_debug(pool->log, "destory a memory pool, size is %lu", (jv_uint_t) pool->max);

  for (block = pool->first; block != NULL; block = tmp) {
    tmp = block->next;
    free(block);
  }
  pool = NULL;
}

void jv_pool_dump(jv_pool_t *pool, FILE *fd) {
  jv_lump_t *lump;
  jv_block_t *block;
  /* jv_uint_t i; */
  int n;

  if (pool == NULL) {
    return;
  }

  fprintf(fd, "\n┌- - - - - - - - - - - - jv memory pool monitoring - - - - - - - - - - - - -┐\n");
  fprintf(fd, "|- - - - - - - - - - - - jv_pool_lump - - - - - - - - - - - - - - - -  - - -|\n");

  lump = pool->lump;
  do {
    char buf[12];
    n = fprintf(fd, "| lump address: %lu, lump->size: %lu, lump->used: %lu", (unsigned long) lump, (jv_uint_t) lump->size, (jv_uint_t) lump->used);
    sprintf(buf, "%%%d.1s\n", 77 - n);
    fprintf(fd, buf, "|");
    lump = lump->next;
  } while (lump != pool->lump);

  /*
   for (lump = pool->lump, i = 0; i == 0 || lump != pool->lump;
   i++, lump = lump->prev) {
   fprintf(fd, "| lump address:%lu, lump->size:%u, lump->used:%u \n", (unsigned long)lump, lump->size, lump->used);
   }*/

  fprintf(fd, "|- - - - - - - - - - - - jv_pool_block - - - - - - - - - - - - - - - - - - -|\n");

  for (block = pool->first; block != NULL; block = block->next) {
    char buf[12];
    n = fprintf(fd, "| block address: %lu, block->size: %lu", (unsigned long) block, (unsigned long) block->size);
    sprintf(buf, "%%%d.1s\n", 77 - n);
    fprintf(fd, buf, "|");
  }
  fprintf(fd, "└- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - ┘\n\n");
}