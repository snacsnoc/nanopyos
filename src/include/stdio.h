/*
 * stdio.h
 */
#ifndef __STDIO_H__
#define __STDIO_H__
#include "sys/reent.h"
#define EOF (-1)

#ifndef NULL
#  define NULL (0)
#endif
#if !defined(__FILE_defined)
typedef __FILE FILE;
# define __FILE_defined
#endif
int printf(const char *, ...);

#endif /* __STDIO_H__ */
