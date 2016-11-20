#include "log.h"

#include <ctime>
#include <iostream>
#include <cstdarg>
#include <termcolor/termcolor.hpp>

void Log(LogLevel logLevel, const char* format, ...) {
    std::time_t t = std::time(nullptr);
    tm now;
    localtime_s(&now, &t);
    switch(logLevel) {
    case Info:
        printf("[%02d:%02d:%02d]  INFO : ", now.tm_hour, now.tm_min, now.tm_sec);
        break;
    case Warn:
        std::cout << termcolor::yellow;
        printf("[%02d:%02d:%02d]  WARN : ", now.tm_hour, now.tm_min, now.tm_sec);
        break;
    case Error:
        std::cout << termcolor::red;
        printf("[%02d:%02d:%02d] ERROR : ", now.tm_hour, now.tm_min, now.tm_sec);
        break;
    case Debug:
        std::cout << termcolor::cyan;
        printf("[%02d:%02d:%02d] DEBUG : ", now.tm_hour, now.tm_min, now.tm_sec);
        break;
    }
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    std::cout << std::endl << termcolor::white;
}