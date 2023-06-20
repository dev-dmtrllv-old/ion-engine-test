#include "pch.hpp"
#include "Core.hpp"

int entry()
{
	int returnValue = 0;

	const std::filesystem::path cwd = ion::Core::getCwd();
	const std::string logPath = (cwd / "logs").string();

	ion::Logger::scoped(logPath, [&](ion::Logger& logger)
	{
		ion::Core core(logger);
		returnValue = core.run();
		core.dispose();
		logger.info("Game exited with code", returnValue);
	});


	return returnValue;
}

#ifdef _WIN32
int WINAPI wWinMain([[maybe_unused]] HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance, [[maybe_unused]] PWSTR pCmdLine, [[maybe_unused]] int nCmdShow)
{
	return entry();
}
#else				 													
int main([[maybe_unused]] int argc, [[maybe_unused]] const char** argv)
{
	return entry();
}
#endif