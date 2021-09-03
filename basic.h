#ifndef _BASIC_H_
#define _BASIC_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <time.h>

#define global      static 
#define internal    static 

#define INTERNAL    static inline

typedef unsigned char   u8;
typedef char            i8;
typedef u_int16_t       u16;
typedef int16_t         i16;
typedef u_int32_t       u32;
typedef u_int64_t       u64;
typedef int             i32;
typedef long            i64;
typedef float           f32;
typedef double          f64;

#define WORD    512
#define KB      (WORD * 2)
#define MB      (KB * 1024)
#define GB      (MB * 1024)

// Used with struct declaration to avoid padding
#define NOPADDING __attribute__((packed)) 

// Used with function declaration to force inlining
#define FORCEINLINE __attribute__((always_inline))

#define ERROR {\
    fprintf(stderr, "[(%s:%d): %s] %s\n",__FILE__, __LINE__, __func__, strerror(errno)); \
    exit(0);                                    \
}

#define eprint(fmt, ...) {\
    fprintf(stderr, "[(%s:%d): %s] " fmt "\n",__FILE__, __LINE__, __func__, ##__VA_ARGS__); \
    exit(0);                                    \
}

//  Buffer type (hopefully this will help in avoiding buffer overflows)
typedef struct {

    void *array;
    u64 capacity;

} buffer_t;

#define buffer_init(parray, capacity) { .array = parray, .capacity = capacity}

int randint(int min, int max)
{
    srand(time(NULL));
    return (rand() % (max - min + 1)) + min;
}

#endif // _BASIC_H_
