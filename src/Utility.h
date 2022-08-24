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

#include <time.h>

#define LOG(message, ...) TimedLog(message, __VA_ARGS__)

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