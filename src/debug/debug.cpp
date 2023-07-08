// debug.cpp

// Copyright (c) 2023 Yan01h
// GPL-3.0 license

#include "debug.h"

#include <chrono>
#include <cstdarg>
#include <ctime>

#include <stdio.h>

namespace debug
{

void LogToConsole(LogColors color, const char* message, ...)
{
    static HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!consoleHandle)
    {
        MessageBoxA(NULL, "Failed to get console handle for debugging!", "Debug Error",
            MB_ICONERROR | MB_OK);
        VTFV_DEBUG_BREAK();
    }

    switch (color)
    {
    default:
    case white:
        SetConsoleTextAttribute(consoleHandle, 15);
        break;
    case green:
        SetConsoleTextAttribute(consoleHandle, 2);
        break;
    case yellow:
        SetConsoleTextAttribute(consoleHandle, 14);
        break;
    case red:
        SetConsoleTextAttribute(consoleHandle, 12);
        break;
    case darkRed:
        SetConsoleTextAttribute(consoleHandle, 4);
        break;
    }

    auto timeNow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    tm localTime = *localtime(&timeNow);
    printf("[%i:%i:%i] ", localTime.tm_hour, localTime.tm_min, localTime.tm_sec);

    va_list args;
    va_start(args, message);
    vprintf(message, args);
    va_end(args);
    printf("\n");
}

}