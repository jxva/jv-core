#ifndef _JV_CONFIG_H_INCLUDE_
#define _JV_CONFIG_H_INCLUDE_

#ifdef __x86_64__
#define JV_WORD_SIZE 64
#else
#define JV_WORD_SIZE 32
#endif

#if defined(__linux__) || defined(__linux)
#define OS_LINUX
#elif defined(__CYGWIN__)
#define OS_CYGWIN
#elif defined(__APPLE__)
#define OS_MACOSX
#elif defined(__FreeBSD__) || defined(__DragonFly__)
#define OS_FREEBSD
#elif defined sun && (defined __svr4__ || defined __SVR4)
#define OS_SOLARIS
#elif defined(__MINGW32__)
#define OS_MINGW
#define OS_MINGW32
#elif defined(__MINGW64__)
#define OS_MINGW
#define OS_MINGW64
#elif defined(_WIN32)
#define OS_WIN
#define OS_WIN32
#elif defined(_WIN64)
#define OS_WIN
#define OS_WIN64
#else
#define OS_POSIX
#endif

#endif /* _JV_CONFIG_H_INCLUDE_ */
