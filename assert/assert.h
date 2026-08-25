#ifndef ASSERT_H
#define ASSERT_H

#ifdef __cplusplus
#include <cstdio>
#include <cstdlib>
#else
#include <stdio.h>
#include <stdlib.h>
#endif

#if defined(__cplusplus) && __cplusplus >= 202002L
#define P_ASSERT_UNLIKELY [[unlikely]]
#else
#define P_ASSERT_UNLIKELY
#endif

#define ASSERT(condition, message, ...)                                                                                \
	do                                                                                                                 \
	{                                                                                                                  \
		P_ASSERT_UNLIKELY if (!(condition))                                                                            \
		{                                                                                                              \
			(void)fprintf(stderr, "%s:%d: '%s' failed, " message "\n", __FILE__, __LINE__, #condition, ##__VA_ARGS__); \
			(void)exit(EXIT_FAILURE);                                                                                  \
		}                                                                                                              \
	} while (0)

#endif // ASSERT_H
