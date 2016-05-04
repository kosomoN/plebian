#include "include\globalsystem.h"
#include <termcolor\termcolor.hpp>

GlobalSystem::GlobalSystem() {
}

GlobalSystem::~GlobalSystem() {
}

void GlobalSystem::Init() {
}

void GlobalSystem::Update() {
}

template <class T>
void GlobalSystem::Log(std::string text, const T& obj) {
	time_t t = time(0);
	struct tm* now = localtime(&t);
	std::string time = "[" + std::to_string(now->tm_hour) + ":" + std::to_string(now->tm_min) + ":" + std::to_string(now->tm_sec) + "] ";
	std::string object = std::string(" | ") + typeid(obj).name();
	termcolor::red;
	std::cout << termcolor::cyan << std::string(time).c_str() << termcolor::reset << std::string(text + object + " \n").c_str() << std::endl;
}

template <class T>
void GlobalSystem::Error(std::string text, const T& obj) {
	time_t t = time(0);
	struct tm* now = localtime(&t);
	std::string time = "[" + std::to_string(now->tm_hour) + ":" + std::to_string(now->tm_min) + ":" + std::to_string(now->tm_sec) + "] ";
	std::string object = std::string(" | ") + typeid(obj).name();
	termcolor::red;
	std::cout << termcolor::red << termcolor::on_white << "ERROR" << termcolor::reset << std::string(time).c_str() << std::string(text + object + " \n").c_str() << std::endl;
}

