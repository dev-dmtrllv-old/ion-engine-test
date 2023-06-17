#pragma once

#ifdef _WIN32

#ifndef UNICODE
#define UNICODE
#endif 

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#endif

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