/**
 * string.h
 */
#ifndef ___STRING_H___
#define ___STRING_H___

#include "sys/types.h"

/*
 * True string functions
 */
int strcmp(const char *, const char *);

size_t strlen(const char *);

int strncmp(const char *, const char *, size_t);

char *strchr(const char *, int);

/*
 * Memory routines
 */
void *memcpy(void *, const void *, size_t);

void *memset(void *, int, size_t);

int memcmp(const void *, const void *, size_t);

void *memmove(void *, const void *, size_t);

#endif   /* #ifndef ___STRING_H___ */
