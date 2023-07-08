// debug.h

// Copyright (c) 2023 Yan01h
// GPL-3.0 license

#pragma once

#ifdef VTFV_WINDOWS
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>

#   define VTFV_DEBUG_BREAK() __debugbreak()
#endif

#ifdef VTFV_DEBUG
#   define VTFV_LOG_INFO(message, ...) debug::LogToConsole(debug::white, message, __VA_ARGS__)
#   define VTFV_LOG_WARNING(message, ...) debug::LogToConsole(debug::yellow, message, __VA_ARGS__)
#   define VTFV_LOG_ERROR(message, ...) debug::LogToConsole(debug::red, message, __VA_ARGS__)
#   define VTFV_LOG_FATAL(message, ...) debug::LogToConsole(debug::darkRed, message, __VA_ARGS__)

#   define VTFV_ASSERT(cond, message, ...) if (!(cond)) { \
        VTFV_LOG_FATAL("Assertion failed in File '%s' at Line %i! Message: '" message "'", \
        __FILE__, __LINE__, __VA_ARGS__); VTFV_DEBUG_BREAK(); }
#else
#   define VTFV_LOG_INFO(message, ...)
#   define VTFV_LOG_WARNING(message, ...)
#   define VTFV_LOG_ERROR(message, ...)
#   define VTFV_LOG_FATAL(message, ...)

#   define VTFV_ASSERT(cond, message, ...)
#endif

namespace debug
{

enum LogColors
{
    white = 0,
    green,
    yellow,
    red,
    darkRed
};

void LogToConsole(LogColors color, const char* message, ...);

}