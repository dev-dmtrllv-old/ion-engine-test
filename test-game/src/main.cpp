#include "pch.hpp"
#include "ion.hpp"
#include "TestGame.hpp"

int __stdcall wWinMain([[maybe_unused]] HINSTANCE hInstance,
					   [[maybe_unused]] HINSTANCE hPrevInstance,
					   [[maybe_unused]] PWSTR pCmdLine,
					   [[maybe_unused]] int nCmdShow)
{
	return ion::Core::load<TestGame>();
}