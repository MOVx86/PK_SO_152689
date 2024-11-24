#pragma once

// true/false
#define TRUE 1
#define FALSE 0

// proper platform detection
#if defined(__linux__)
    #define PLATFORM_LINUX 1
#else
    #error "Linux distribution required to run!"
#endif

// defined static assertions
#if defined(__GNUC__)
    #define STATIC_ASSERT _Static_assert
#endif

// adding shorter names for data types
// unsigned int types
typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;
// signed int types
typedef signed char         s8;
typedef signed short        s16;
typedef signed int          s32;
typedef signed long long    s64;
// floating point types
typedef float               f32;
typedef double              f64;
// boolean types
typedef _Bool               b8;

// universal
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// linux specific
#if PLATFORM_LINUX == 1
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/ipc.h>
    #include <sys/shm.h>
    #include <sys/sem.h>
    #include <sys/wait.h>
    #include <signal.h>
#else
    #error "Linux platform not detected! Cannot include linux specific libraries!"
#endif