/**
 * types.h
 */
#ifndef __TYPES_H__
#define __TYPES_H__

#include <stdint.h>
#include <stddef.h>

#ifndef _USECONDS_T_DEFINED
#define _USECONDS_T_DEFINED
typedef unsigned int useconds_t;
#endif

typedef unsigned short ushort_t;
typedef unsigned char uchar_t;
typedef unsigned int uint_t;
typedef unsigned long ulong_t;
typedef unsigned long long ulonglong_t;


typedef uint32_t mode_t;    // Used for file permissions
typedef uint32_t uid_t;     // User ID
typedef uint32_t gid_t;     // Group ID
typedef int32_t pid_t;      // Process ID
typedef uint32_t ino_t;     // Inode number
typedef int64_t off_t;      // File size and offset
typedef uint64_t dev_t;     // Device number
typedef uint32_t nlink_t;   // Number of links

#ifdef __aarch64__
typedef int64_t mp_int_t;
typedef uint64_t mp_uint_t;
typedef long mp_off_t;
#else
// Definitions for other architectures
typedef int mp_int_t;
typedef unsigned int mp_uint_t;
typedef long mp_off_t;
#endif

#ifndef _SSIZE_T_DEFINED
#define _SSIZE_T_DEFINED
typedef int64_t ssize_t;
#endif

#ifndef _SIZE_T
#  define _SIZE_T
typedef unsigned long size_t;
#endif

#endif /* __TYPES_H__ */
