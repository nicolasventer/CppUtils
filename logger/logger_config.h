#ifndef LOGGER_CONFIG_H
#define LOGGER_CONFIG_H

// here put code like: #define LOG_LEVEL WARN or #define LOG_DISABLE or #define SET_LOG_LEVEL_FATAL
// here is also where you can define custom log levels,
// with something like: #define CUSTOM #define P_LOG_CUSTOM "\033[32m" #define LOG_CUSTOM(...) P_LOG_IMPL(CUSTOM, __VA_ARGS__)

#define SET_LOG_LEVEL_DEBUG

#endif /* LOGGER_CONFIG_H */
