// DOC-MISSING
// FINISHED

#pragma once

// proper platform detection
#if defined(__linux__)
    #define PLATFORM_LINUX 1
#else
    #error "Linux distribution required to run!"
#endif

#define DEBUG 1
#define USE_SLEEP 1

#define DEFAULT_CAP 1000 // (and limit)
#define DEFAULT_X 0
#define DEFAULT_Y 0
#define DEFAULT_Z 0

// including universal C libraries
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// including linux specific C libraries
#if PLATFORM_LINUX == 1
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/ipc.h>
    #include <mqueue.h>
    #include <fcntl.h>
    #include <sys/stat.h>
    #include <sys/shm.h>
    #include <sys/sem.h>
    #include <sys/wait.h>
    #include <signal.h>
#else
    #error "Linux platform not detected! Cannot include linux specific libraries!"
#endif

// true/false
#define TRUE 1
#define FALSE 0

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
typedef _Bool                b8;