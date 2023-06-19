#pragma once

#ifdef _WIN32
#define _WIN32_WINNT 0x0500 // build for Windows 2000 or higher
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#elif defined(linux) || defined(__unix__)

#include <unistd.h>
#include <X11/X.h>
#include <X11/Xlib.h>
// #include <wayland-client.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#define MAX_PATH PATH_MAX

#endif

#include <stdio.h>
#include <stdlib.h>

#include <filesystem>
#include <iostream>
#include <fstream>
#include <optional>
#include <string_view>
#include <cassert>
#include <exception>
#include <format>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <chrono>
#include <atomic>
#include <array>
#include <unordered_map>
#include <vector>
#include <string>

constexpr struct
{
#ifdef NDEBUG
    const bool isDebug = false;
    const bool isRelease = true;
#else
    const bool isDebug = true;
    const bool isRelease = false;
#endif
} env;

#define CHECK_NULL(__STATEMENT__) [&]{ \
auto returnValue = __STATEMENT__;  \
if(returnValue == nullptr) \
    throw std::runtime_error(std::format("{} is NULL!", #__STATEMENT__));\
return returnValue; \
}();