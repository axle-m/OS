#pragma once

// detect architecture

#if defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__)
#define K_ARCH_64BIT 1
#else
#define K_ARCH_32BIT 1
#endif

typedef unsigned char uint8_t;
typedef signed char int8_t;

typedef unsigned short uint16_t;
typedef signed short int16_t;

typedef unsigned int uint32_t;
typedef signed int int32_t;

typedef unsigned long long uint64_t;
typedef signed long long int64_t;

#if defined(K_ARCH_64BIT)

typedef unsigned long long size_t;
typedef long long ssize_t;

typedef unsigned long long uintptr_t;
typedef long long intptr_t;

typedef unsigned long long register_t;

#else /* 32-bit */

typedef unsigned int size_t;
typedef int ssize_t;

typedef unsigned int uintptr_t;
typedef int intptr_t;

typedef unsigned int register_t;

#endif

#define NULL ((void *)0)

#define true 1
#define false 0