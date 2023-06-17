#pragma once

#include "game.hpp"
#include "logger.hpp"

namespace ion
{
	class Core final
	{
	public:
		template<typename T>
		[[nodiscard]] static int load()
		{
			int returnValue = 0;
			Logger::scoped("D:\\src\\c++\\ion-engine\\logs", [&](Logger& logger)
            {
                const char* gameClassName = typeid(T).name();
                logger.info("Starting game", gameClassName);
				T game = T();
				Core core(game, logger);
				returnValue = core.run();
			});
            return returnValue;
		}

	private:
		Core(Game& game, Logger& logger);
		~Core();

		[[nodiscard]] int run();

		Game& game_;
        Logger& logger_;
	};
} // namespace ion