#ifndef LOG_H_
#define LOG_H_

enum LogLevel {
    Info = 1, Warn, Error
};

void Log(LogLevel logLevel, const char* format, ...);

#endif // LOG_H_
