#pragma once

#include "pch.h"
#include "log.h"

#if defined WIN32
    #ifdef WIN_EXPORT
        #define API __declspec(dllexport)
    #else
        #define API __declspec(dllimport)
    #endif
    #define NOT_EXPORTED
#elif defined GNU_EXPORT
    #define API __attribute__((visibility("default")))
    #define NOT_EXPORTED __attribute__((visibility("hidden")))
#else
    #define API
    #define NOT_EXPORTED
#endif

namespace Alkahest
{
#define AKST_DEBUG_BREAK() asm("int $3")

#ifdef DEBUG
#define AKST_ENG_ASSERT(expr) \
    if (expr) {} \
    else \
    { \
        logError("Engine assertion failed! Expression: {} at {}:{}", #expr, __FILE__, __LINE__); \
        AKST_DEBUG_BREAK(); \
    }
#else
#define AKST_ENG_ASSERT(expr)
#endif

#define AKST_ASSERT(expr) \
    if (expr) {} \
    else \
    { \
        logError("Assertion failed! Expression: {} at {}:{}", #expr, __FILE__, __LINE__); \
        AKST_DEBUG_BREAK(); \
    }
}
