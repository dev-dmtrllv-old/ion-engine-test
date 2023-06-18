#pragma once

#include "game.hpp"
#include "logger.hpp"
#include "SubSystem.hpp"
#include "hash.hpp"

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

		template<typename T>
		[[nodiscard]] static int load()
		{
			int returnValue = 0;

			const std::filesystem::path cwd = getCwd();
			const std::string logPath = (cwd / "logs").string();

			Logger::scoped(logPath, [&](Logger& logger)
				{
					const char* gameClassName = typeid(T).name();
					logger.info("Starting game class", gameClassName);
					T game = T();
					Core core(cwd, game, logger);
					returnValue = core.run();
					logger.info("Game exited with code", returnValue);
				});
			return returnValue;
		}

	private:
		Core(const std::filesystem::path& cwd, Game& game, Logger& logger);
		~Core();

		[[nodiscard]] int run();

		template<IsSubSystem T>
		void registerSystem()
		{
			const char* systemName = typeid(T).name();
			const Hash hash = Hasher::hashType<T>();
			if (systems_.contains(hash))
				throw std::runtime_error(std::format("{} is already registered!", systemName));
			logger_.debug("Registering {}", systemName);
			T* system = new T(*this);
			systems_.emplace(hash, system);
			systemInitOrder_.emplace_back(system);
		}

		void initializeSystem(SubSystemInterface* system)
		{
			logger_.info("Initializing", system->name());
			system->initialize();
		}

		void disposeSystem(SubSystemInterface* system)
		{
			logger_.info("Disposing", system->name());
			system->dispose();
		}

		template<IsSubSystem T>
		[[nodiscard]] T& getSystem()
		{
			const char* systemName = typeid(T).name();
			const Hash hash = Hasher::hashType<T>();
			if (!systems_.contains(hash))
				throw std::runtime_error(std::format("{} is not registered!", systemName));
			return *static_cast<T*>((systems_.at(hash)));
		}

		public:
			inline Logger& logger() { return logger_; }

		private:

		Game& game_;
		Logger& logger_;
		const std::filesystem::path& cwd_;

		std::unordered_map<Hash, SubSystemInterface*> systems_;
		std::vector<SubSystemInterface*> systemInitOrder_;
	};
} // namespace ion