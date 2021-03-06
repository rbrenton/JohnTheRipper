/*
 * This file is part of John the Ripper password cracker,
 * Copyright (c) 1996-99,2005,2009,2011,2013,2015 by Solar Designer
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 *
 * There's ABSOLUTELY NO WARRANTY, express or implied.
 */

/*
 * Things common to many ciphertext formats.
 */

#ifndef _JOHN_COMMON_H
#define _JOHN_COMMON_H

#if !defined(_OPENCL_COMPILER)
#include "arch.h"
#include "memory.h"
#endif

#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif
#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif

#if !defined(_OPENCL_COMPILER)

#if ARCH_INT_GT_32
typedef unsigned short ARCH_WORD_32;
typedef unsigned int ARCH_WORD_64;
#else
typedef unsigned int ARCH_WORD_32;
typedef unsigned long long ARCH_WORD_64;
#endif

/* ONLY use this to check alignments of even power of 2 (2, 4, 8, 16, etc) byte counts (CNT).
   The cast to void* MUST be done, due to C spec. http://stackoverflow.com/a/1898487 */
#define is_aligned(PTR, CNT) ((((ARCH_WORD)(const void *)(PTR))&(CNT-1))==0)

#ifdef __GNUC__
#if __GNUC__ >= 5
#define MAYBE_INLINE __attribute__((gnu_inline)) inline
#elif __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7) || defined(__INTEL_COMPILER)
#define MAYBE_INLINE __attribute__((always_inline)) inline
#elif __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1)
#define MAYBE_INLINE __attribute__((always_inline))
#else
#define MAYBE_INLINE __inline__
#endif
#elif __STDC_VERSION__ >= 199901L
#define MAYBE_INLINE inline
#else
#define MAYBE_INLINE
#endif

#if ((__GNUC__ == 2) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 2)
#define CC_CACHE_ALIGN \
	__attribute__ ((aligned (MEM_ALIGN_CACHE)))
#else
#define CC_CACHE_ALIGN			/* nothing */
#endif

/*
 * This "shift" is the number of bytes that may be inserted between arrays the
 * size of which would be a multiple of cache line size (some power of two) and
 * that might be accessed simultaneously.  The purpose of the shift is to avoid
 * cache bank conflicts with such accesses, actually allowing them to proceed
 * simultaneously.  This number should be a multiple of the machine's word size
 * but smaller than cache line size.
 */
#define CACHE_BANK_SHIFT		ARCH_SIZE

/*
 * ASCII <-> binary conversion tables.
 */
#define DIGITCHARS   "0123456789"
#define HEXCHARS_lc  DIGITCHARS"abcdef"
#define HEXCHARS_uc  DIGITCHARS"ABCDEF"
#define HEXCHARS_all DIGITCHARS"abcdefABCDEF"
#define BASE64_CRYPT "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"

extern const char itoa64[64]; /* crypt(3) base64 - not MIME Base64! */
extern unsigned char atoi64[0x100];
extern const char itoa16[16];
extern unsigned char atoi16[0x100], atoi16l[0x100], atoi16u[0x100];
extern const char itoa16u[16]; // uppercase

/*
 * Initializes the tables.
 */
extern void common_init(void);

/**************************************************************
 * added 'common' helper functions for things used in valid() *
 **************************************************************/

/* is string full valid hex string */
int ishex(const char *q);
/* Same as ishex(), BUT will still return true for an odd length string */
int ishex_oddOK(const char *q);
/* is string full valid hex string (only upper case letters) */
int ishexuc(const char *q);
/* is string full valid hex string (only lower case letters) */
int ishexlc(const char *q);
/* same as ishexuc/lc except odd length is ok */
int ishexuc_oddOK(const char *q);
int ishexlc_oddOK(const char *q);
/* provide a length field, so return true if 'n' bytes of the string are hex */
/* the n is length q, so there is no need for a 'odd' field. If checking for */
/* a 49 byte string, simply specify 49 */
int ishexn(const char *q, int n);
int ishexucn(const char *q, int n);
int ishexlcn(const char *q, int n);
/* length of hex. if non-hex chars found, then negative length of valid hex */
int hexlen(const char *q);
int hexlenl(const char *q); /* lower cased only */
int hexlenu(const char *q); /* upper cased only */
/* is this a valid string for atoi() ONLY positive numbers are valid */
int isdec(const char *q);
/* is this a valid string for atoi() */
int isdec_negok(const char *q);
/* is this a valid string for atou()?  atou() func == sprintf("%x",&val) */
int isdecu(const char *q);
/* provides the length of the base64 string.  See base64_convert.c for that
 * function. If the string is not 'pure', then the return is -1*length */
int base64_mime_len(const char *q);
int base64_crypt_len(const char *q);
int base64_mime_du_len(const char *q);  /* mime, -_ instead of +/ */

#endif

#endif
