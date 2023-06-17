#pragma once

#ifdef _WIN32
#define GAME_ENTRY(__GAME_CLASS__)                                                                      \
	int WINAPI wWinMain([[maybe_unused]] HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance, \
						[[maybe_unused]] PWSTR pCmdLine, [[maybe_unused]] int nCmdShow)                 \
	{                                                                                                   \
		return ion::Core::load<__GAME_CLASS__>();														\
	}
#else
// clang-format off
#define GAME_ENTRY(__GAME_CLASS__)					 													\
int main([[maybe_unused]] int argc, [[maybe_unused]] const char** argv)                                 \
{                                                                                                       \
    return ion::Core::load<__GAME_CLASS__>();															\
}
// clang-format on
#endif

namespace ion
{
	class Game
	{
	public:
		Game() = default;
		~Game();
	};
} // namespace ion