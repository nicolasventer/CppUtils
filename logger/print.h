#ifndef PRINT_H
#define PRINT_H

#ifdef __cplusplus
#include <cstdio>
#include <iostream>
#else
#include <stdio.h>
#endif

#include "print_config.h" // IWYU pragma: keep

#if defined(__clang__)
#define P_PRINT_FMT_DIAG_PUSH()                      \
	_Pragma("clang diagnostic push");                \
	_Pragma("clang diagnostic ignored \"-Wformat\"")
#define P_PRINT_FMT_DIAG_POP() _Pragma("clang diagnostic pop")
#elif defined(__GNUC__)
#define P_PRINT_FMT_DIAG_PUSH()                    \
	_Pragma("GCC diagnostic push");                \
	_Pragma("GCC diagnostic ignored \"-Wformat\"")
#define P_PRINT_FMT_DIAG_POP() _Pragma("GCC diagnostic pop")
#else
#define P_PRINT_FMT_DIAG_PUSH()
#define P_PRINT_FMT_DIAG_POP()
#endif

#ifdef __cplusplus
#define P_PRINT(x) std::cout << x
#else
#define P_PRINT(x)                                                           \
	P_PRINT_FMT_DIAG_PUSH()                                                  \
	(void)_Generic((x),                                                      \
		int: printf("%d", (x)),                                              \
		unsigned: printf("%u", (x)),                                         \
		long: printf("%ld", (x)),                                            \
		double: printf("%g", (x)),                                           \
		float: printf("%g", (x)),                                            \
		char: printf("%c", (x)),                                             \
		char*: printf("%s", (x)),                                            \
		const char*: printf("%s", (x)),                                      \
		void*: printf("%p", (x)),                                            \
		default: ((void)fprintf(stderr, "P_PRINT: unsupported type\n"), 0)); \
	P_PRINT_FMT_DIAG_POP()
#endif

// PRINT(a, b, …) expands to P_PRINT for each argument (1..8). Empty PRINT() is undefined.
#define PRINT(...)                                              \
	do                                                          \
	{                                                           \
		P_PRINT_SELECT(P_PRINT_NARG(__VA_ARGS__))(__VA_ARGS__); \
	} while (0)

#define PRINT_LN(...) PRINT(__VA_ARGS__, "\n")

#ifndef P_PRINT_SPACE
#define P_PRINT_SPACE() printf(" ")
#endif

#define P_PRINT_VA_NARG_(...) P_PRINT_ARG_N(__VA_ARGS__)

#define P_PRINT_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N

#define P_PRINT_RSEQ_N()   8, 7, 6, 5, 4, 3, 2, 1, 0
#define P_PRINT_NARG(...)  P_PRINT_VA_NARG_(__VA_ARGS__, P_PRINT_RSEQ_N())
#define P_PRINT_GLUE(a, b) a##b
#define P_PRINT_SELECT(n)  P_PRINT_GLUE(P_PRINT_, n)

#define P_PRINT_1(_1) P_PRINT(_1)
#define P_PRINT_2(_1, _2) \
	P_PRINT(_1);          \
	P_PRINT_SPACE();      \
	P_PRINT(_2)
#define P_PRINT_3(_1, _2, _3) \
	P_PRINT(_1);              \
	P_PRINT_SPACE();          \
	P_PRINT(_2);              \
	P_PRINT_SPACE();          \
	P_PRINT(_3)
#define P_PRINT_4(_1, _2, _3, _4) \
	P_PRINT(_1);                  \
	P_PRINT_SPACE();              \
	P_PRINT(_2);                  \
	P_PRINT_SPACE();              \
	P_PRINT(_3);                  \
	P_PRINT_SPACE();              \
	P_PRINT(_4)
#define P_PRINT_5(_1, _2, _3, _4, _5) \
	P_PRINT(_1);                      \
	P_PRINT_SPACE();                  \
	P_PRINT(_2);                      \
	P_PRINT_SPACE();                  \
	P_PRINT(_3);                      \
	P_PRINT_SPACE();                  \
	P_PRINT(_4);                      \
	P_PRINT_SPACE();                  \
	P_PRINT(_5)
#define P_PRINT_6(_1, _2, _3, _4, _5, _6) \
	P_PRINT(_1);                          \
	P_PRINT_SPACE();                      \
	P_PRINT(_2);                          \
	P_PRINT_SPACE();                      \
	P_PRINT(_3);                          \
	P_PRINT_SPACE();                      \
	P_PRINT(_4);                          \
	P_PRINT_SPACE();                      \
	P_PRINT(_5);                          \
	P_PRINT_SPACE();                      \
	P_PRINT(_6)
#define P_PRINT_7(_1, _2, _3, _4, _5, _6, _7) \
	P_PRINT(_1);                              \
	P_PRINT_SPACE();                          \
	P_PRINT(_2);                              \
	P_PRINT_SPACE();                          \
	P_PRINT(_3);                              \
	P_PRINT_SPACE();                          \
	P_PRINT(_4);                              \
	P_PRINT_SPACE();                          \
	P_PRINT(_5);                              \
	P_PRINT_SPACE();                          \
	P_PRINT(_6);                              \
	P_PRINT_SPACE();                          \
	P_PRINT(_7)
#define P_PRINT_8(_1, _2, _3, _4, _5, _6, _7, _8) \
	P_PRINT(_1);                                  \
	P_PRINT_SPACE();                              \
	P_PRINT(_2);                                  \
	P_PRINT_SPACE();                              \
	P_PRINT(_3);                                  \
	P_PRINT_SPACE();                              \
	P_PRINT(_4);                                  \
	P_PRINT_SPACE();                              \
	P_PRINT(_5);                                  \
	P_PRINT_SPACE();                              \
	P_PRINT(_6);                                  \
	P_PRINT_SPACE();                              \
	P_PRINT(_7);                                  \
	P_PRINT_SPACE();                              \
	P_PRINT(_8)

#endif // PRINT_H
