#include "pch.hpp"
#include "core.hpp"

namespace ion
{
    Core::Core(const std::filesystem::path& cwd, Game& game, Logger& logger) :
        cwd_(cwd),
        game_(game),
        logger_(logger)
    {
        logger.info("Running from", cwd_);
    }

    Core::~Core()
    {}

    [[nodiscard]] int Core::run()
    {
        return 0;
    }
} // namespace ion