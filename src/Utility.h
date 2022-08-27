// Utility.h
/*
                     GNU GENERAL PUBLIC LICENSE
                       Version 3, 29 June 2007
 Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.
 ...
*/

#pragma once

#include <chrono>
#include <cstdio>
#include <string>

#include <time.h>

#define LOG(message, ...) TimedLog(message, __VA_ARGS__)

struct GLFWwindow;

template<typename... Args>
void TimedLog(const char* message, Args... args)
{
	std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm t;
	localtime_s(&t, &time);
	printf("[%i:%i:%i] ", t.tm_hour, t.tm_min, t.tm_sec);
	printf(message, args...);
	printf("\n");
}

namespace VTFViewer {

	typedef unsigned int uint;

	void GLFWDropCallback(GLFWwindow* window, int count, const char** paths);

	void GLFWErrorCallback(int error, const char* description);

	bool FileHasExtension(char* file, const char* extension);

	int GetFileSizeFromPath(char* path); // In KB

	std::string GetFileNameFromPath(char* path);
}