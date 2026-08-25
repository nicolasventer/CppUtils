#include "print.h"

#ifdef __cplusplus
#include <cstdio>
#else
#include <stdio.h> // IWYU pragma: keep
#endif

#ifndef LOGGER_H
#define LOGGER_H

#define DEBUG 0
#define INFO  1
#define WARN  2
#define ERROR 3
#define FATAL 4

#include "logger_config.h" // IWYU pragma: keep

#ifndef LOG_LEVEL

#if defined(SET_LOG_LEVEL_FATAL)
#define LOG_LEVEL FATAL
#elif defined(SET_LOG_LEVEL_ERROR)
#define LOG_LEVEL ERROR
#elif defined(SET_LOG_LEVEL_WARN)
#define LOG_LEVEL WARN
#elif defined(SET_LOG_LEVEL_INFO)
#define LOG_LEVEL INFO
#elif defined(SET_LOG_LEVEL_DEBUG)
#define LOG_LEVEL DEBUG
#else
#define LOG_LEVEL DEBUG
#endif

#endif

#define P_LOG_RESET	 "\033[0m"
#define P_LOG_WHITE	 "\033[37m"
#define P_LOG_BLUE	 "\033[34m"
#define P_LOG_ORANGE "\033[38;5;208m"
#define P_LOG_YELLOW "\033[33m"
#define P_LOG_RED	 "\033[31m"
#define P_LOG_RED_BG "\033[41m"

#define P_LOG_DEBUG P_LOG_WHITE
#define P_LOG_INFO	P_LOG_BLUE
#define P_LOG_WARN	P_LOG_ORANGE
#define P_LOG_ERROR P_LOG_RED
#define P_LOG_FATAL P_LOG_RED_BG

inline int* logPathOffset(void)
{
	static int value = 0;
	return &value;
}

#define INIT_LOG_PATH_OFFSET() *logPathOffset() = strlen(__FILE__) - strlen(__FILE_NAME__)

static inline const char* stripPrefix(const char* s) { return s + *logPathOffset(); }

#define P_LOG_IMPL(level, ...)                                                \
	do                                                                        \
	{                                                                         \
		printf("%5s%s:%d: ", P_LOG_##level, stripPrefix(__FILE__), __LINE__); \
		PRINT(__VA_ARGS__);                                                   \
		printf("%s\n", P_LOG_RESET);                                          \
	} while (0)

#define LOG_DEBUG(...) P_LOG_IMPL(DEBUG, __VA_ARGS__)
#define LOG_INFO(...)  P_LOG_IMPL(INFO, __VA_ARGS__)
#define LOG_WARN(...)  P_LOG_IMPL(WARN, __VA_ARGS__)
#define LOG_ERROR(...) P_LOG_IMPL(ERROR, __VA_ARGS__)
#define LOG_FATAL(...) P_LOG_IMPL(FATAL, __VA_ARGS__)

#define LOG(level, ...) LOG_##level(__VA_ARGS__)

#if LOG_LEVEL > DEBUG
#undef LOG_DEBUG
#define LOG_DEBUG(...)
#endif
#if LOG_LEVEL > INFO
#undef LOG_INFO
#define LOG_INFO(...)
#endif
#if LOG_LEVEL > WARN
#undef LOG_WARN
#define LOG_WARN(...)
#endif
#if LOG_LEVEL > ERROR
#undef LOG_ERROR
#define LOG_ERROR(...)
#endif
#if LOG_LEVEL > FATAL
#undef LOG_FATAL
#define LOG_FATAL(...)
#endif

#if defined(LOG_DISABLE)
#undef LOG
#define LOG(level, ...)
#endif

// should be used like this: LOG_DEBUG("myBool", AS_BOOL(myBool))
#define AS_BOOL(x) ((x) ? "true" : "false")

#endif // LOGGER_H