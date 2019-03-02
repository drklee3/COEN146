#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

/**
 * @brief Logs messages based on given format
 * 
 * @param tag     log message type (error, info, debug)
 * @param message content to show
 * @param args    formatting arguments
 */
void log_format(const char* tag, const char* message, va_list args) {
    // get current time
    char buff[16];
    time_t now = time(0);
    strftime(buff, 40, "%H:%M:%S", localtime(&now));

    pthread_t id = pthread_self();

    // print to stderr if on error duh
    FILE* target = stdout;
    if (strcmp(tag, "ERROR")) {
        target = stderr;
    }
    
    // [thread id] [time] [type] message
    fprintf(target, "[TID %zx] [%s] [%s] ",
            id, buff, tag);
    vprintf(message, args);
    printf("\n");
}

/**
 * @brief Logs an error message
 * 
 * @param message error message
 * @param ...     formatting arguments
 */
void log_error(const char* message, ...) {
    va_list args;
    va_start(args, message);
    log_format(RED "ERROR" RESET, message, args);
    va_end(args);
}

/**
 * @brief Logs an info message
 * 
 * @param message info message
 * @param ...     formatting arguments
 */
void log_info(const char* message, ...) {
    va_list args;
    va_start(args, message);
    log_format(CYN "INFO" RESET, message, args);
    va_end(args);
}

/**
 * @brief Logs a debug message
 * 
 * @param message debug message
 * @param ...     formatting arguments
 */
void log_debug(const char* message, ...) {
    va_list args;
    va_start(args, message);
    log_format(YEL "DEBUG" RESET, message, args);
    va_end(args);
}
