#include "pch.hpp"
#include "core.hpp"
#include "WindowManager.hpp"

namespace ion
{
	Core::Core(const std::filesystem::path& cwd, Game& game, Logger& logger) :
		cwd_(cwd),
		game_(game),
		logger_(logger)
	{
		registerSystem<WindowManager>();

		logger_.info("Initializing subsystems...");

		for (auto system : systemInitOrder_)
			initializeSystem(system);

	}

	Core::~Core()
	{
		logger_.info("Disposing subsystems...");

		std::reverse(systemInitOrder_.begin(), systemInitOrder_.end());
		for (auto& system : systemInitOrder_)
			disposeSystem(system);
	}

	[[nodiscard]] int Core::run()
	{
		WindowManager& wm = getSystem<WindowManager>();
		wm.startEventLoop();
		return 0;
	}
} // namespace ion