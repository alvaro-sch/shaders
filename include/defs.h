#ifndef _DEFS_H
#define _DEFS_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifdef DEBUG
# define DBG_PRINT(...) do {fprintf(stderr, "DEBUG: %s:%d:%s(): ", \
    __FILE__, __LINE__, __func__); fprintf(stderr, __VA_ARGS__);} while(0)
# define DBG_ASSERT(expr) do{assert(expr);} while(0)
#else
# define DBG_PRINT(...) do {} while (0)
# define DBG_ASSERT(expr) do {} while (0)
#endif

#define TRY(expr, ...) do {if (!(expr)) fprintf(stderr, __VA_ARGS__);} while (0)

#define PANIC(code, ...) do{fprintf(stderr, "panic! code: %d in %s:%d:%s()\nmessage: ", \
    code, __FILE__, __LINE__, __func__); fprintf(stderr, __VA_ARGS__); exit(code);} while(0)

#endif
