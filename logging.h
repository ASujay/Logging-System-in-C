#ifndef __LOGGING__
#define __LOGGING__

#include <stddef.h>
#include <stdio.h>
#include "utils.h"

typedef u32 LogFile;
typedef enum {
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL,
} LogLevel;

/*
 * A global logger config:
 * min_level: Decides the minimum LogLevel
 * log_file: If this is NULL then we have to do console logging
 * log_to_console: 1
 */

typedef struct {
    LogLevel min_level;
    LogFile log_file;
    u8 log_to_console;
} LoggerConfig;

void log_init(LogLevel level, const char *filename, u8 log_to_console);
void log_set_level(LogLevel level);
void log_close(void);

void log_message(LogLevel level, const char *fmt, ...);

#define DEBUG(fmt, ...) log_message(LOG_DEBUG, "[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define INFO(fmt,  ...) log_message(LOG_INFO,  "[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define WARN(fmt,  ...) log_message(LOG_WARN,  "[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define ERROR(fmt, ...) log_message(LOG_ERROR, "[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define FATAL(fmt, ...) log_message(LOG_FATAL, "[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)

#endif
