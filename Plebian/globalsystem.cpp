#include "globalsystem.h"

#include <stdarg.h>
#include <termcolor/termcolor.hpp>

void LogInfo(const char* format, ...) {
	time_t t = time(0);
	struct tm* now = localtime(&t);
	printf("[%02d:%02d:%02d]  INFO : ", now->tm_hour, now->tm_min, now->tm_sec);
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	std::cout << std::endl;
}

void LogWarn(const char* format, ...) {
	time_t t = time(0);
	struct tm* now = localtime(&t);
	std::cout << termcolor::yellow;
	printf("[%02d:%02d:%02d]  WARN : ", now->tm_hour, now->tm_min, now->tm_sec);
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	std::cout << std::endl << termcolor::reset;
}

void LogError(const char* format, ...) {
	time_t t = time(0);
	struct tm* now = localtime(&t);
	std::cout << termcolor::red;
	printf("[%02d:%02d:%02d] ERROR : ", now->tm_hour, now->tm_min, now->tm_sec);
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	std::cout << std::endl << termcolor::reset;
}