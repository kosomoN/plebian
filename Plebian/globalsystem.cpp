#include "include/globalsystem.h"
#include <termcolor/termcolor.hpp>
#include <stdarg.h>

template <class T>
void GlobalSystem::LogInfo(const T& obj, const char* format, ...) {
	time_t t = time(0);
	struct tm* now = localtime(&t);
	printf("[%02d:%02d:%02d] INFO  | %s: ", now->tm_hour, now->tm_min, now->tm_sec, typeid(obj).name());
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}

template <class T>
void GlobalSystem::LogWarn(const T& obj, const char* format, ...) {
	time_t t = time(0);
	struct tm* now = localtime(&t);
	std::cout << termcolor::yellow;
	printf("[%02d:%02d:%02d] WARN  | %s: ", now->tm_hour, now->tm_min, now->tm_sec, typeid(obj).name());
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	std::cout << termcolor::reset;
}

template <class T>
void GlobalSystem::LogError(const T& obj, const char* format, ...) {
	time_t t = time(0);
	struct tm* now = localtime(&t);
	std::cout << termcolor::red;
	printf("[%02d:%02d:%02d] ERROR | %s: ", now->tm_hour, now->tm_min, now->tm_sec, typeid(obj).name());
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	std::cout << termcolor::reset;
}
