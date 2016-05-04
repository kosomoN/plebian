#include "include/globalsystem.h"
#include <termcolor/termcolor.hpp>
#include <stdarg.h>

template <class T>
void GlobalSystem::Log(const T& obj, const char* format, ...) {
	time_t t = time(0);
	struct tm* now = localtime(&t);
	std::string time = "[" + std::to_string(now->tm_hour) + ":" + std::to_string(now->tm_min) + ":" + std::to_string(now->tm_sec) + "] ";
	std::string object = std::string(" | ") + typeid(obj).name();
	std::cout << termcolor::cyan << std::string(time).c_str() << termcolor::reset;
	va_list args;
	va_start(args, format);
	vprintf(std::string("	" + std::string(format) + object).c_str(), args);
	va_end(args);
}

template <class T>
void GlobalSystem::LogError(const T& obj, const char* format, ...) {
	time_t t = time(0);
	struct tm* now = localtime(&t);
	std::string time = "[" + std::to_string(now->tm_hour) + ":" + std::to_string(now->tm_min) + ":" + std::to_string(now->tm_sec) + "] ";
	std::string object = std::string(" | ") + typeid(obj).name();
	termcolor::red;
	std::cout << termcolor::red << termcolor::on_white << "ERROR" << termcolor::reset;
	va_list args;
	va_start(args, format);
	vprintf(std::string("	" + std::string(format) + object).c_str(), args);
	va_end(args);
}

