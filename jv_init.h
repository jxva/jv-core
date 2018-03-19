#ifndef _JV_INIT_H_INCLUDE_
#define _JV_INIT_H_INCLUDE_

#include <jv_config.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h> /* gettimeofday */
#include <time.h>
/* #include <unistd.h> */

#define JV_INFTIM -1 /* wait forever */

#define LF (u_char) 10
#define CR (u_char) 13
#define CRLF "\x0d\x0a"

#ifndef SHUT_RDWR
#define SHUT_RD 0x00
#define SHUT_WR 0x01
#define SHUT_RDWR 0x02
#endif

#endif
