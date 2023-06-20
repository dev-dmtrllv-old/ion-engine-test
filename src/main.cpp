#include "pch.hpp"
#include "Core.hpp"

[[nodiscard]] std::filesystem::path getCwd()
{
	static bool isInitialized = false;
	static std::filesystem::path cwd;

	if (!isInitialized)
	{
		char path[MAX_PATH] = { '\0' };
#ifdef _WIN32
		GetCurrentDirectoryA(MAX_PATH, path);
#else
		if (getcwd(path, sizeof(path)) == NULL)
			throw std::runtime_error("getcwd() failed!");
#endif
		cwd = path;
		isInitialized = true;
	}

	return cwd;
}

int entry(const std::vector<std::string> args)
{
	int returnValue = 0;

	const std::filesystem::path cwd = getCwd();
	const std::string logPath = (cwd / "logs").string();

	ion::Logger::scoped(logPath, [&](ion::Logger& logger)
	{
		std::string argsString = "";
		for(const auto& arg : args)
			argsString += std::format(" {}", arg);
		logger.info("Running game with args", argsString);
		
		ion::Core core(cwd, logger);
		// returnValue = core.run();
		core.dispose();
		logger.info("Game exited with code", returnValue);
	});

	return returnValue;
}

#ifdef _WIN32
int WINAPI wWinMain([[maybe_unused]] HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance,
	[[maybe_unused]] PWSTR pCmdLine, [[maybe_unused]] int nCmdShow)
{
	return entry({});
}
#else
int main([[maybe_unused]] int argc, [[maybe_unused]] const char** argv)
{
	return entry({});

#endif