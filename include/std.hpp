#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#include <std/ctype.hpp>
#include <std/string.hpp>

// Structs for stdlib
typedef struct {
    short quot, rem;
} div_t;

typedef struct {
    long quot, rem;
} ldiv_t;

template<class T>
void swap(T x, T y);

template<class T_in, class T_out>
T_out copy(T_in first, T_in last, T_out d_first);

// C++ dynamic memory allocation operators
void *operator new(size_t size);
void *operator new[](size_t size);
void operator delete(void *p);
void operator delete[](void *p);

// C++ placement operators (for rigid memory allocation)
inline void *operator new(size_t, void *p)     throw() { return p; }
inline void *operator new[](size_t, void *p)   throw() { return p; }
inline void  operator delete  (void *, void *) throw() { };
inline void  operator delete[](void *, void *) throw() { };

// "String" functions

char *strcat(char *dest, const char *source);

int strncmp(const char *s1, const char *s2, size_t n);

char * strpbrk(const char *s, const char *accept);
size_t strspn (const char *s, const char *accept);
char * strtok(char *str, const char *delim);
char tolower(char upper);

int putchar(int);

//int printf(const char* __restrict, ...);
int sprintf(char* buffer, const char* fmt, ...);
int vsprintf(char* buffer, const char* format, va_list parameters);

// Long-functions
ldiv_t ldiv (unsigned long numerator, unsigned long denominator );
char* ltoa(unsigned long long value, char* buffer, int radix );

// Integer functions
int itoa(int value, char *sp, int radix);
int atoi(char* str);
