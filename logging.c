#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "logging.h"

static const char *log_colors[] = { 
    [LOG_DEBUG] = "\033[36m",
    [LOG_INFO]  = "\033[32m",
    [LOG_WARN]  = "\033[33m",
    [LOG_ERROR] = "\033[31m",
    [LOG_FATAL] = "\033[1;31m",
};

#define RESET   "\x1b[0m"

static LoggerConfig log_config;

void log_init(LogLevel level, const char *filename, u8 log_to_console){
    log_config.min_level = level;
    log_config.log_to_console = log_to_console;
    // open the logfile
    i32 log_file;
    i32 flags = O_CREAT | O_APPEND | O_TRUNC | O_RDWR;
    i32 permissions = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    if(filename == NULL){
        log_file = open("termemu.log", flags, permissions);
    } else {
        log_file = open(filename, flags, permissions);
    }

    if(log_file == -1){
        printf("Error opening log file.\n");
        exit(1);
    }
    log_config.log_file = log_file;
}

void log_set_level(LogLevel level) {
    log_config.min_level = level;
}

void log_close(void) {
    if(close(log_config.log_file) == -1){
        printf("Error closing the log file.");
        exit(1);
    }
}

void log_message(LogLevel level, const char *fmt, ...){
    if(level >= log_config.min_level) {
        // write to the file
        va_list arg_list;
        va_start(arg_list, fmt);
        
        // build the format string along with colors
        char log_string[1024];
        memset(log_string, 0, sizeof(log_string));
        vsnprintf(log_string, sizeof(log_string), fmt, arg_list);
        if(write(log_config.log_file, log_string, sizeof(log_string)) != sizeof(log_string)) {
            fprintf(stderr, "%sError writing to log file.%s\n", log_colors[LOG_FATAL], RESET);
        }
        if(log_config.log_to_console) {
            char console_log_string[1024];
            memset(console_log_string, 0, sizeof(console_log_string));
            snprintf(console_log_string, 
                    sizeof(console_log_string), 
                    "%s%s%s\n", 
                    log_colors[level],
                    fmt,
                    RESET);
            vfprintf(stderr, console_log_string, arg_list);
        }
        va_end(arg_list);
    }
}
