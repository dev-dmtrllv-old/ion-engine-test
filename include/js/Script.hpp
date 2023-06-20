#pragma once

#include "pch.hpp"

namespace ion
{
	class Script
	{
	public:
		Script(const char* scriptPath);

	public:
		const char* path;
	};
}