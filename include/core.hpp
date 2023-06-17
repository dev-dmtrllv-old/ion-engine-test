#pragma once

#include "game.hpp"
#include "logger.hpp"

namespace ion
{
	class Core final
	{
	public:
		[[nodiscard]] static std::filesystem::path getCwd()
		{
			static bool isInitialized = false;
			static std::filesystem::path cwd;

			if (!isInitialized)
			{
#ifdef _WIN32
				char path[MAX_PATH] = { '\0' };
				GetCurrentDirectoryA(MAX_PATH, path);
#else
				throw std::runtime_error("getCwd() not implemented!");
#endif
				cwd = path;
				isInitialized = true;
			}

			return cwd;
		}

		template<typename T>
		[[nodiscard]] static int load()
		{
			int returnValue = 0;

			const std::filesystem::path cwd = getCwd();
			const std::string logPath = (cwd / "logs").string();

			Logger::scoped(logPath, [&](Logger& logger) {
				const char* gameClassName = typeid(T).name();
				logger.info("Starting game", gameClassName);
				T game = T();
				Core core(cwd, game, logger);
				returnValue = core.run();
			});
			return returnValue;
		}

	private:
		Core(const std::filesystem::path& cwd, Game& game, Logger& logger);
		~Core();

		[[nodiscard]] int run();

		Game& game_;
		Logger& logger_;
		const std::filesystem::path& cwd_;
	};
} // namespace ion