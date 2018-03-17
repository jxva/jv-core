#ifndef _JV_STRING_H_INCLUDED_
#define _JV_STRING_H_INCLUDED_

#include <jv_core.h>

struct jv_string_s {
  size_t len;
  u_char *data;
};

typedef struct {
  jv_string_t key;
  jv_string_t value;
} jv_keyvalue_t;

typedef struct jv_split_s jv_split_t;

struct jv_split_s {
  u_char *data;
  jv_split_t *next;
};

#define jv_string(string) \
  { sizeof(string) - 1, (u_char *) string }

#define jv_null_string \
  { 0, NULL }

#define jv_string_set(string, text) \
  (string)->len = sizeof(text) - 1; \
  (string)->data = (u_char *) text

#define jv_string_null(string) \
  (string)->len = 0;           \
  (string)->data = NULL

#define jv_tolower(c) (u_char)((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#define jv_toupper(c) (u_char)((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)
#define jv_strcmp(s1, s2) strcmp((const char *) s1, (const char *) s2)
#define jv_strncmp(s1, s2, n) strncmp((const char *) s1, (const char *) s2, n)
#define jv_strstr(s1, s2) strstr((const char *) s1, (const char *) s2)
#define jv_strlen(s) (jv_uint_t)strlen((const char *) s)
#define jv_strchr(s1, c) strchr((const char *) s1, (int) c)
#define jv_copy(dst, src, n) (((u_char *) memcpy(dst, src, n)) + (n))
#define jv_move(dst, src, n) (((u_char *) memmove(dst, src, n)) + (n))

u_char *jv_bin2hex(u_char *buf, const byte *data, size_t n);

byte *jv_hex2bin(byte *buf, const u_char *data, size_t n);

u_char *jv_hex_dump(const u_char *src, size_t len);

u_char *jv_strlchr(u_char *p, const u_char *last, const u_char c);

u_char *jv_strlow(u_char *dst, const u_char *src);

u_char *jv_strdup(jv_pool_t *pool, const u_char *s);

u_char *jv_strncpy(u_char *dst, const u_char *src, size_t n);

jv_int_t jv_strcasecmp(const u_char *s1, const u_char *s2);

jv_int_t jv_strncasecmp(const u_char *s1, const u_char *s2, size_t n);

const u_char *jv_strnstr(const u_char *s1, const u_char *s2, size_t n);

const u_char *jv_strstrn(const u_char *s1, const u_char *s2, size_t n);

const u_char *jv_strcasestrn(const u_char *s1, const u_char *s2, size_t n);

const u_char *jv_strlcasestrn(const u_char *s1, const u_char *last, const u_char *s2, size_t n);

jv_int_t jv_atofp(const u_char *s, size_t n, size_t point);

jv_int_t jv_htoi(const u_char *s, size_t n);

u_char *jv_escape_html(u_char *dst, const u_char *src, size_t n);

u_char *jv_ltrim(u_char *dst, const u_char *src);

u_char *jv_rtrim(u_char *dst, const u_char *src);

u_char *jv_trim(u_char *dst, const u_char *src);

u_char *jv_reverse(u_char *dst, const u_char *src);

jv_split_t *jv_split(jv_pool_t *pool, u_char *str, size_t len, const u_char delimiter);

jv_int_t jv_string_index(u_char *s, u_char *t, size_t tlen, size_t pos);

jv_int_t jv_string_kmp(u_char s[], u_char p[], size_t pos, jv_int_t next[]);

u_char *jv_strsep(u_char **s, const u_char *delim);

/* Endian convert: big <-> little */
uint32_t jv_endian_convert(uint32_t i);

#endif /* _JV_STRING_H_INCLUDED_ */
