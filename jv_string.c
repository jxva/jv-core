#include <jv_string.h>

static void jv_kmp_next(u_char p[], size_t plen, jv_int_t next[]);

u_char *jv_bin2hex(u_char *buf, const byte *data, size_t n) {
  /*
  size_t i;

  for (i = 0; i < len; ++i) {
    char msg[3];
    snprintf(msg, 3, "%02X", (unsigned char) data[i]);
    strncat(buf, msg, 3);
  }

  return buf;
  */

  u_char *s = buf;
  static u_char hex[] = "0123456789abcdef";
  while (n--) {
    *buf++ = hex[(*data >> 4) & 0xf];
    *buf++ = hex[*data++ & 0xf];
  }
  return s;
}

byte *jv_hex2bin(byte *buf, const u_char *data, size_t n) {
  byte *s =buf;
  unsigned j = 0, t;
  while (n > 0) {
    t = *data++;
    if ((t >= '0') && (t <= '9'))
      j = (t - '0') << 4;
    else if ((t >= 'a') && (t <= 'f'))
      j = (t - 'a' + 10) << 4;
    else if ((t >= 'A') && (t <= 'F'))
      j = (t - 'A' + 10) << 4;

    t = *data++;
    if ((t >= '0') && (t <= '9'))
      j ^= (t - '0');
    else if ((t >= 'a') && (t <= 'f'))
      j ^= (t - 'a' + 10);
    else if ((t >= 'A') && (t <= 'F'))
      j ^= (t - 'A' + 10);

    *buf++ = j;
    n -= 2;
  }
  return s;
}


u_char *jv_hex_dump(const u_char *src, size_t len) {
  /*
  static u_char buf[4096];
  size_t i;

  jv_memzero(buf, 4096);

  for (i = 0; i < len; ++i) {
    char msg[4];
    snprintf(msg, 4, "%02X ", (unsigned char) src[i]);
    strncat(buf, msg, 4);
  }
  */
  static u_char buf[4096]; /* len*3+1 */
  static u_char hex[] = "0123456789abcdef";
  size_t i;

  if (len * 3 + 1 > 4096) {
    return NULL;
  }

  for (i = 0; i < len; i++) {
    buf[3 * i] = hex[*src >> 4];
    buf[3 * i + 1] = hex[*src++ & 0xf];
    buf[3 * i + 2] = ' ';
  }

  buf[len * 3] = '\0';

  return buf;
}

u_char *jv_strlchr(u_char *p, const u_char *last, const u_char c) {
  while (p < last) {
    if (*p == '\0')
      return NULL;
    if (*p == c)
      return p;
    p++;
  }
  return NULL;
}

u_char *jv_strlow(u_char *dst, const u_char *src) {
  u_char *tmp = dst;
  do {
    (*dst++) = jv_tolower(*src);
  } while (*src++);
  return tmp;
}

u_char *jv_strdup(jv_pool_t *pool, const u_char *s) {
  size_t n = jv_strlen(s) + 1;
  u_char *dst = jv_pool_alloc(pool, sizeof(u_char) * n);
  if (dst == NULL) {
    return NULL;
  }
  jv_memcpy(dst, s, n);
  return dst;
}

u_char *jv_strncpy(u_char *dst, const u_char *src, size_t n) {
  u_char *tmp = dst;
  while (n--) {
    if ((*dst++ = *src++) == '\0') {
      return tmp;
    }
  }
  *dst = '\0';
  return tmp;
}

jv_int_t jv_strcasecmp(const u_char *s1, const u_char *s2) {
  jv_uint_t c1, c2;
  for (;;) {
    c1 = (jv_uint_t) *s1++;
    c2 = (jv_uint_t) *s2++;

    c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
    c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;
    if (c1 == c2) {
      if (c1) {
        continue;
      }
      return 0;
    }
    return c1 - c2;
  }
  return -1; /* the statement is unreachable for ever */
}

jv_int_t jv_strncasecmp(const u_char *s1, const u_char *s2, size_t n) {
  jv_uint_t c1, c2;
  while (n) {
    c1 = (jv_uint_t) *s1++;
    c2 = (jv_uint_t) *s2++;
    c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
    c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;
    if (c1 == c2) {
      if (c1) {
        n--;
        continue;
      }
      return 0;
    }

    return c1 - c2;
  }
  return 0;
}

const u_char *jv_strnstr(const u_char *s1, const u_char *s2, size_t n) {
  u_char c1, c2 = *s2++;
  size_t len = jv_strlen(s2);
  do {
    do {
      if (n-- == 0) {
        return NULL;
      }
      c1 = *s1++;
      if (c1 == 0) {
        return NULL;
      }
    } while (c1 != c2);
    if (len > n) {
      return NULL;
    }
  } while (jv_strncmp(s1, s2, len) != 0);
  return --s1;
}

const u_char *jv_strstrn(const u_char *s1, const u_char *s2, size_t n) {
  u_char c1 = *s1, c2 = *s2;
  do {
    while (c1 != c2) {
      if ((c1 = *++s1) == 0) {
        return NULL;
      }
    }
  } while (jv_strncmp(s1, s2, n) != 0);
  return s1;
}

const u_char *jv_strcasestrn(const u_char *s1, const u_char *s2, size_t n) {
  jv_uint_t c1 = (jv_uint_t) *s1, c2 = (jv_uint_t) *s2;
  c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
  c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;
  do {
    while (c1 != c2) {
      c1 = (jv_uint_t) * ++s1;
      c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
      if (c1 == 0) {
        return NULL;
      }
    }
  } while (jv_strncasecmp(s1, s2, n) != 0);
  return s1;
}

/*
 * jv_strlcasestrn() is intended to search for static substring
 * with known length in string until the argument last. The argument n
 * must be length of the second substring length.
 */

/*
 const u_char s1[]="  fd Sa h = Fa elLo  ";
 const u_char s2[]=" fA ";
 const u_char *last=s1+15;
 const u_char *tmp;
 printf("sss:%s,d:%d,sX:%X\n",s1,jv_strlen(s1),(unsigned int)s1);

 tmp=jv_strlcasestrn(s1,last,s2,jv_strlen(s2));
 if(tmp!=NULL){
 printf("tmp:%s,d:%d,tmpX:%X\n",tmp,jv_strlen(tmp),(unsigned int)tmp);
 printf("sss:%s,d:%d,sX:%X\n",s1,jv_strlen(s1),(unsigned int)s1);
 }
 从字符串s1的前last个字符中搜索s2前n个字符
 如果搜索成功将返回匹配s2的前n个字符的s1指针
 否则，到last个字符未搜索到s2的前n个字符将返回NULL
 注意：n<=jv_strlen(s2)
 */
const u_char *jv_strlcasestrn(const u_char *s1, const u_char *last, const u_char *s2, size_t n) {
  jv_uint_t c1, c2 = (jv_uint_t) *s2++;
  c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;
  last -= n - 1;
  do {
    do {
      if (s1 >= last) {
        return NULL;
      }
      c1 = (jv_uint_t) *s1++;
      c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
    } while (c1 != c2);
  } while (jv_strncasecmp(s1, s2, n - 1) != 0);
  return --s1;

  /*
   the way is effective too.
   jv_uint_t  c1, c2;
   c1 = (jv_uint_t) *s1;
   c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
   c2 = (jv_uint_t) *s2;
   c2 = (c2 >= 'A' && c2 <= 'Z') ? (c2 | 0x20) : c2;
   do {
   while(c1!= c2){
   if(s1>=last)return NULL;
   c1 = (jv_uint_t)*++s1;
   c1 = (c1 >= 'A' && c1 <= 'Z') ? (c1 | 0x20) : c1;
   if(c1==0){
   return NULL;
   }
   }
   printf("s1:%s,s2:%s\n",s1,s2);
   } while (jv_strncasecmp(s1, s2, n) != 0);
   return s1;
   */
}

/*
 parse a fixed point number, e.g., jv_atofp("10.5", 4, 2) returns 1050
 (1) n表示截取s的前n位
 (2) .点号将替换为0
 (3) point取点号后point-1位
 解说不是太规则，可以参见单元测试
 */
jv_int_t jv_atofp(const u_char *s, size_t n, size_t point) {
  jv_int_t value;
  jv_uint_t dot;
  if (n == 0) {
    return JV_ERROR;
  }
  dot = 0;
  for (value = 0; n--; s++) {
    if (point == 0) {
      return JV_ERROR;
    }
    if (*s == '.') {
      if (dot) {
        return JV_ERROR;
      }
      dot = 1;
      continue;
    }
    if (*s < '0' || *s > '9') {
      return JV_ERROR;
    }
    value = value * 10 + (*s - '0');
    point -= dot;
  }
  while (point--) {
    value = value * 10;
  }
  return value < 0 ? JV_ERROR : value;
}
/*
 将s的前n个hex字符串转换为十进制整型
 jv_htoi((u_char *)"-5A")  :    -90;
 jv_htoi((u_char *)"40")   :    64;
 */
jv_int_t jv_htoi(const u_char *s, size_t n) {
  u_char c, ch;
  jv_int_t value, sign;
  if (s[0] == '-') {
    sign = -1;
    n--;
    s++;
  } else {
    sign = 1;
  }
  for (value = 0; n--; s++) {
    ch = *s;
    if (ch >= '0' && ch <= '9') {
      value = value * 16 + (ch - '0');
      continue;
    }
    c = (u_char)(ch | 0x20);
    if (c >= 'a' && c <= 'f') {
      value = value * 16 + (c - 'a' + 10);
      continue;
    }
    return 0;
  }
  return sign * value;
}

/*
 将src的前n个字符中的html字符进行转义
 const u_char s[]="<font color=\"red\"><b>Hello,World</b></font>";
 u_char dst[120];
 u_char *tmp;
 printf("sss:%s,d:%d,sX:%X\n",s,jv_strlen(s),(unsigned int)s);

 tmp=jv_escape_html(dst,s,jv_strlen(s));
 if(tmp!=NULL){
 printf("tmpX:%X,dstX:%X\n",(unsigned int)tmp,(unsigned int)dst);
 printf("tmp:%s,d:%d\n",tmp,jv_strlen(tmp));
 printf("dst:%s,d:%d\n",dst,jv_strlen(dst));
 printf("sss:%s,d:%d,sX:%X\n",s,jv_strlen(s),(unsigned int)s);
 }
 */
u_char *jv_escape_html(u_char *dst, const u_char *src, size_t n) {
  u_char ch, *tmp = dst;
  while (n--) {
    ch = *src++;
    switch (ch) {
      case '\0':
        *dst = ch;
        return tmp;
      case '<':
        *dst++ = '&';
        *dst++ = 'l';
        *dst++ = 't';
        *dst++ = ';';
        break;
      case '>':
        *dst++ = '&';
        *dst++ = 'g';
        *dst++ = 't';
        *dst++ = ';';
        break;
      case '&':
        *dst++ = '&';
        *dst++ = 'a';
        *dst++ = 'm';
        *dst++ = 'p';
        *dst++ = ';';
        break;
      case '"':
        *dst++ = '&';
        *dst++ = 'q';
        *dst++ = 'u';
        *dst++ = 'o';
        *dst++ = 't';
        *dst++ = ';';
        break;
      default:
        *dst++ = ch;
        break;
    }
  }
  *dst = '\0';
  return tmp;
}

u_char *jv_ltrim(u_char *dst, const u_char *src) {
  u_char *tmp = dst;
  while (*src == ' ' || *src == '\t') {
    src++;
  }
  while (*src != '\0') {
    *dst++ = *src++;
  }
  *dst = '\0';
  return tmp;
}

u_char *jv_rtrim(u_char *dst, const u_char *src) {
  jv_int_t i, len = jv_strlen(src) - 1;
  while (src[len] == ' ' || src[len] == '\t') {
    len--;
  }
  for (i = 0; i <= len; i++) {
    *dst++ = src[i];
  }
  *dst = '\0';
  return dst - i;
}

u_char *jv_trim(u_char *dst, const u_char *src) {
  u_char *tmp = dst;
  jv_int_t i = 0, len = jv_strlen(src);
  while (src[i] == ' ' || src[i] == '\t') {
    i++;
  }

  do {
    len--;
  } while (src[len] == ' ' || src[len] == '\t');

  while (i <= len) {
    *dst++ = src[i++];
  }
  *dst = '\0';
  return tmp;
}

u_char *jv_reverse(u_char *dst, const u_char *src) {
  size_t n, len;
  n = len = jv_strlen(src);
  while (len) {
    *dst++ = src[--len];
  }
  *dst = '\0';
  return dst - n;
}

/*
 char *str = "10,20,30";
 char *arr[3];
 const char *del = ",";
 int i = 0;
 jv_split(arr, str, del);

 while(i<3)
 printf("%s\n", *(arr+i++));

 void jv_split(u_char **arr, u_char *str, const u_char *delimiter) {
 u_char *s = (u_char *)strtok((char *)str, (char *)delimiter);
 while (s != NULL) {
 *arr++ = s;
 s = (u_char *)strtok(NULL, (char *)delimiter);
 }
 } */

/**
 *   traverse data
 *
 *   tmp=first->next;
 *   while(tmp->next!=NULL){
 *       printf("%s\n",tmp->data);
 *        tmp=tmp->next;
 *   }
 *
 *   release memory
 *
 *   for(temp=first; first->next!=NULL; first=temp){
 *          temp = first->next;
 *          printf("%ld\n",(long)first);
 *          // free(first);
 *          jv_pool_free(pool, first);
 *   }
 */
jv_split_t *jv_split(jv_pool_t *pool, u_char *str, size_t len, const u_char delimiter) {
  unsigned i, j;
  jv_split_t *first, *last;

  first = jv_pool_alloc(pool, 2 * sizeof(jv_split_t));
  last = first + 1;

  first->data = last->data = NULL;
  first->next = last->next = NULL;

  for (i = 0, j = 0; i < len; i++) {
    if (str[i] == delimiter || str[i] == '\0') {
      jv_split_t *split = jv_pool_alloc(pool, sizeof(jv_split_t) + sizeof(u_char) * (i - j + 1));

      u_char *data = (u_char *) split + sizeof(jv_split_t);
      jv_strncpy(data, str + j, i - j);
      split->data = data;

      if (first->next == NULL) {
        first->next = split;
        split->next = last;
        last = split;
      } else {
        split->next = last->next;
        last->next = split;
        last = split;
      }
      printf("SS: %s\n", data);
      j = i + 1;
    }
  }
  return first;
}

jv_int_t jv_string_index(u_char *s, u_char *t, size_t tlen, size_t pos) {
  jv_uint_t i = pos, j = 0;
  while (s[i] != 0 && j < tlen) {
    if (s[i] == t[j]) {
      ++i;
      ++j;
    } else {
      i = i - j + 1;
      j = 0;
    }
  }
  return (jv_int_t)(j >= tlen) ? (jv_int_t)(i - tlen) : -1;
}

static void jv_kmp_next(u_char p[], size_t plen, jv_int_t next[]) {
  jv_uint_t i = 0;
  jv_int_t j = -1;
  next[0] = -1;
  while (i < plen) {
    if (j == -1 || p[i] == p[j]) {
      ++i;
      ++j;
      if (p[i] != p[j]) {
        next[i] = j;
      } else {
        next[i] = next[j];
      }
    } else {
      j = next[j];
    }
  }
}

/*
 char s1[] = "ababcabcacbab";
 char p1[] = "abcac";
 int *next1 = malloc(sizeof(p1) * sizeof(int));
 printf("found index at %d\n", jv_string_kmp(s1, p1, 0, next1));
 free(next1);

 printf("..............................................\n");

 char s2[] = "00010000010000100";
 char p2[] = "000001";
 int *next2 = malloc(sizeof(p2) * sizeof(int));
 printf("found index at %d\n", jv_string_kmp(s2, p2, 0, next2));
 free(next2);

 printf("..............................................\n");

 char s3[] = "abcabaabcaabac";
 char p3[] = "abaabcac";
 int *next3 = malloc(sizeof(p3) * sizeof(int));
 printf("found index at %d\n", jv_string_kmp(s3, p3, 0, next3));
 free(next3);
 */
jv_int_t jv_string_kmp(u_char s[], u_char p[], size_t pos, jv_int_t next[]) {
  jv_int_t j = 0, len = jv_strlen(p);
  jv_uint_t i = pos;

  jv_kmp_next(p, len, next);

  while (s[i] != 0 && j < len) {
    if (j == -1 || s[i] == p[j]) {
      i++;
      j++;
    } else {
      j = next[j];
    }
  }
  return (jv_int_t)(j >= len) ? (jv_int_t)(i - len) : -1;
}

u_char *jv_strsep(u_char **s, const u_char *delim) {
  u_char *tmp;
  const u_char *spanp;
  int c, sc;
  u_char *tok;
  if ((tmp = *s) == NULL)
    return (NULL);
  for (tok = tmp;;) {
    c = *tmp++;
    spanp = delim;
    do {
      if ((sc = *spanp++) == c) {
        if (c == 0)
          s = NULL;
        else
          tmp[-1] = 0;
        *s = tmp;
        return (tok);
      }
    } while (sc != 0);
  }
  /* NOTREACHED */
}

uint32_t jv_endian_convert(uint32_t i) {
  uint32_t a = 0;
  a |= (i << 24) & 0xff000000;
  a |= (i << 8) & 0x00ff0000;
  a |= (i >> 8) & 0x0000ff00;
  a |= (i >> 24) & 0x000000ff;
  return a;
}
