#include "pch.hpp"
#include "core.hpp"

namespace ion
{
	Core::Core(Game& game, Logger& logger):
        game_(game),
        logger_(logger)
    {

    }

	Core::~Core()
    {

    }

    [[nodiscard]] int Core::run()
    {
        return 0;
    }
} // namespace ion