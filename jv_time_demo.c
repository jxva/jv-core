#include <assert.h>
#include <jv_core.h>

int main(int argc, const char *argv[]) {
  jv_tm_t tm;
  jv_tm_t *tm2;
  time_t t;
  char buf[30];

  /*
   *  "Sat, 27 Apr 2013 09:06:19 GMT";
   * */
  const u_char *s = (u_char *) "Sat, 8 Jun 2013 14:17:19 GMT";
  jv_time_parse(s, &tm);

  t = time(NULL);

  tm2 = localtime(&t);

  printf("%lu, %lu, %lu\n", jv_time_translate(&tm), t, jv_strlen((char *) s));

  printf("%d, %d, %d, %d, %d, %d %d\n", tm2->tm_year, tm2->tm_mon, tm2->tm_mday, tm2->tm_hour, tm2->tm_min, tm2->tm_sec, tm2->tm_wday);

  printf("%s\n", jv_time_gmt(t, buf));
  return 0;
}
