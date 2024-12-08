#pragma once

// Detect platform
#if defined(WIN32) || defined(_WINDOWS) || defined(_WIN32)
#if !defined(PLATFORM_WIN)
#define PLATFORM_WIN
#endif
#define CORE_WINDOWS
#define CORE_WINDOWS_API
#define PLATFORM_DESKTOP
#elif defined(__ANDROID__)
#if !defined(PLATFORM_ANDROID)
#define PLATFORM_ANDROID
#endif
#define CORE_LINUX
#define CORE_LINUX_API
#define PLATFORM_MOBILE
#elif defined(__EMSCRIPTEN__)
#if !defined(PLATFORM_EMSCRIPTEN)
#define PLATFORM_EMSCRIPTEN
#endif
#define CORE_LINUX
#define CORE_LINUX_API
#define PLATFORM_WEB
#else
#if !defined(PLATFORM_LINUX)
#define PLATFORM_LINUX
#endif
#define CORE_LINUX
#define CORE_LINUX_API
#define PLATFORM_DESKTOP
#endif



typedef unsigned char u8;
typedef signed char s8;
typedef char c8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef long long s64;
typedef float f32;
typedef double f64;

#define GLSL(src) "#version 460 core\n" #src




#if defined(WIN32) || defined(_WINDOWS) || defined(_WIN32)

#pragma warning(disable : 4514) // unreferenced inline removed
#pragma warning(disable : 4820) // padding added
#pragma warning(disable : 4828) // bytes padding added


#endif



#if defined(_DEBUG)
#include <assert.h>

#define DEBUG_BREAK_IF(condition)                                                      \
    if (condition)                                                                     \
    {                                                                                  \
        fprintf(stderr, "Debug break: %s at %s:%d\n", #condition, __FILE__, __LINE__); \
        std::exit(EXIT_FAILURE);                                                       \
    }

#define ASSERT(condition, message)                                 \
    do                                                             \
    {                                                              \
        if (!(condition))                                          \
        {                                                          \
            fprintf(stderr, "[%s:%d] Assert failed in %s(): %s\n", \
                    __FILE__, __LINE__, __func__, message);        \
            abort();                                               \
        }                                                          \
    } while (false)

// Indicates that we know execution should never reach this point in the
// program. In debug mode, we assert this fact because it's a bug to get here.
//
// In release mode, we use compiler-specific built in functions to tell the
// compiler the code can't be reached. This avoids "missing return" warnings
// in some cases and also lets it perform some optimizations by assuming the
// code is never reached.
#define UNREACHABLE()                                                        \
    do                                                                       \
    {                                                                        \
        fprintf(stderr, "[%s:%d] This code should not be reached in %s()\n", \
                __FILE__, __LINE__, __func__);                               \
        abort();                                                             \
    } while (false)

#else
#define DEBUG_BREAK_IF(_CONDITION_)

#define ASSERT(condition, message) \
    do                             \
    {                              \
    } while (false)

// Tell the compiler that this part of the code will never be reached.
#if defined(_MSC_VER)

#define UNREACHABLE() __assume(0)
#elif (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5))
#define UNREACHABLE() __builtin_unreachable()
#else
#define UNREACHABLE()
#endif

#endif

#include <time.h>
#include <climits>
#define GL_GLEXT_PROTOTYPES
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#include <cstring>
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

void LogError( const char *msg, ... );
void LogInfo( const char *msg, ... );
void LogWarning( const char *msg, ... );