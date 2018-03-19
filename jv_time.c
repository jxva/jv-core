#include <jv_time.h>

volatile jv_msec_t jv_current_msec;

#define is_leap(year) ((year) % 400 ? ((year) % 100 ? ((year) % 4 ? 0 : 1) : 0) : 1)

static char *weeks[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
static char *months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
        "Aug", "Sep", "Oct", "Nov", "Dec" };

jv_msec_t jv_time_update(void) {

    time_t sec;
    jv_uint_t msec;
    struct timeval tv;

    (void)gettimeofday(&tv, NULL);

    sec = tv.tv_sec;
    msec = tv.tv_usec / 1000;

    jv_current_msec = (jv_msec_t)sec * 1000 + msec;

    return jv_current_msec;
}

static int jv_time_week(const u_char *s) {
    int i, n;

    for (i = 0, n = jv_array_count(weeks); i < n; i++) {
        if (jv_strncmp(s, (u_char *)weeks[i], 3) == 0) {
            return i;
        }
    }

    return -1;

}

static int jv_time_month(const u_char *s) {
    int i, n;

    for (i = 0, n = jv_array_count(months); i < n; i++) {
        if (jv_strncmp(s, (u_char *)months[i], 3) == 0) {
            return i;
        }
    }

    return -1;

}

time_t jv_time_translate(const jv_tm_t *tm) {
    time_t t;
    static int monthtab[12] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273,
            304, 334 };

    /* Years since epoch, converted to days. */
    t = (tm->tm_year - 70) * 365;
    /* Leap days for previous years. */
    t += (tm->tm_year - 69) / 4;
    /* Days for the beginning of this month. */
    t += monthtab[tm->tm_mon];
    /* Leap day for this year. */

    if (tm->tm_mon >= 2 && is_leap(tm->tm_year + 1900)) {
        ++t;
    }
    /* Days since the beginning of this month. */
    t += tm->tm_mday - 1; /* 1-based field */

    t = t * 24 + tm->tm_hour;
    t = t * 60 + tm->tm_min;
    t = t * 60 + tm->tm_sec;
    return t;
}

/*
 * Mon, 28 Sep 1970 06:00:00 GMT
 * 1970-09-28 12:00:00
 * 1970/09/28 12:00:00
 * Fri Aug 28 09:37:46 CST 2013
 *
 * CST = GMT + 3600 * 8 =  UTC + 3600 * 8
 *
 * */
jv_tm_t *jv_time_parse(const u_char *s, jv_tm_t *tm) {
    char str_mon[5], str_wday[5];
    int tm_sec, tm_min, tm_hour, tm_mday, tm_year;

    if (sscanf((char *)s, "%4[a-zA-Z], %d %4[a-zA-Z] %d %d:%d:%d GMT", str_wday, &tm_mday, str_mon, &tm_year, &tm_hour, &tm_min, &tm_sec) == 7) {
        int tm_mon, tm_wday;

        if ((tm_wday = jv_time_week((u_char *)str_wday)) == -1) {
            return NULL;
        }

        if ((tm_mon = jv_time_month((u_char *)str_mon)) == -1) {
            return NULL;
        }

        tm->tm_wday = tm_wday;
        tm->tm_mday = tm_mday;
        tm->tm_mon = tm_mon;
        tm->tm_year = tm_year;
        tm->tm_hour = tm_hour;
        tm->tm_min = tm_min;
        tm->tm_sec = tm_sec;

        if (tm->tm_year > 1900) {
            tm->tm_year -= 1900;
        } else if (tm->tm_year < 70) {
            tm->tm_year += 100;
        }

        /* printf("%d, %d, %d, %d, %d, %d %d\n",tm->tm_year, tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, tm->tm_wday); */

        return tm;
    }
    return NULL;
}

time_t jv_time_convert(const u_char *s){
    struct tm tm;
    if(jv_time_parse(s, &tm)==NULL){
        return 0;
    }
    return jv_time_translate(&tm);
}

char *jv_time_gmt(const time_t t, char buf[30]) {

    static char* rfc1123fmt = "%a, %d %b %Y %H:%M:%S GMT";

    if (strftime(buf, 30, rfc1123fmt, gmtime(&t)) == 0) {
        return NULL;
    }

    return buf;
}


/**
 * reference: https://stackoverflow.com/questions/2408976/struct-timeval-to-printable-format
 **/
ssize_t jv_format_timeval(struct timeval *tv, char *buf, size_t sz) {
  ssize_t written = -1;
  jv_tm_t *t = localtime(&tv->tv_sec); /* UTC gmtime(&tv->tv_sec); */

  if (t) {
    written = (ssize_t) strftime(buf, sz, "%Y-%m-%d %H:%M:%S", t);
    if ((written > 0) && ((size_t) written < sz)) {
      int w = snprintf(buf + written, sz - (size_t) written, ".%06u", (unsigned int) tv->tv_usec);
      written = (w > 0) ? written + w : -1;
    }
  }
  return written;
}

ssize_t jv_format_time(time_t sec, char *buf, size_t sz) {
  ssize_t written = -1;
  jv_tm_t *t = localtime(&sec);

  if (t) {
    written = (ssize_t) strftime(buf, sz, "%Y-%m-%d %H:%M:%S", t);
    /* if ((written > 0) && ((size_t) written < sz)) {
      int w = snprintf(buf + written, sz - (size_t) written,"");
      written = (w > 0) ? written + w : -1;
    } */
  }
  return written;
}

jv_tm_t *jv_localtime(jv_tm_t *tm) {
    time_t now;
#if defined(OS_MINGW) || defined (OS_WIN)
    jv_tm_t  *t;

    now = time(NULL);
    t = localtime(&now);
    *tm = *t;
#else
    now = time(NULL);
    localtime_r(&now, tm);
#endif

    tm->tm_mon++;
    tm->tm_year += 1900;

    return tm;
}
