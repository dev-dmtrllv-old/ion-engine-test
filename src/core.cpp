#include "pch.hpp"
#include "Core.hpp"
#include "hash.hpp"
#include "WindowManager.hpp"
#include "ResourceManager.hpp"

namespace ion
{
	Core::Core(const std::filesystem::path& cwd, Game& game, Logger& logger) :
		cwd_(cwd),
		game_(game),
		logger_(logger),
		eventHandlers_()
	{
		registerSystem<WindowManager>();
		registerSystem<ResourceManager>();

		logger_.info("Initializing subsystems...");

		for (auto system : systemInitOrder_)
			initializeSystem(system);

	}

	Core::~Core()
	{}

	[[nodiscard]] int Core::run()
	{
		WindowManager& wm = getSystem<WindowManager>();
		wm.startEventLoop();
		return 0;
	}

	void Core::dispose()
	{
		logger_.info("Disposing subsystems...");

		std::reverse(systemInitOrder_.begin(), systemInitOrder_.end());

		for (auto& system : systemInitOrder_)
			disposeSystem(system);

		systemInitOrder_.clear();
		systems_.clear();
	}

	void Core::emitEvent(const Hash eventType, const Event& event)
	{
		if (!eventHandlers_.contains(eventType))
			return;

		auto vec = eventHandlers_.at(eventType);

		for (const auto& data : vec)
			data.handler(event, data.externalData);
	}

	void Core::registerEventHandler(const Hash eventType, EventHandler handler, void* externalData)
	{
		if (!eventHandlers_.contains(eventType))
			eventHandlers_.insert(std::pair<Hash, std::vector<EventHandlerData>>(eventType, std::vector<EventHandlerData>()));

		EventHandlerData data = {
			handler,
			externalData
		};

		eventHandlers_.at(eventType).emplace_back(data);
	}

	void Core::removeEventHandler(const Hash eventType, EventHandler handler)
	{
		if (!eventHandlers_.contains(eventType))
			return;

		auto vec = eventHandlers_.at(eventType);

		const auto iter = std::find_if(vec.begin(), vec.end(), [&](const EventHandlerData& data)
		{
			return data.handler == handler;
		});

		if(iter != vec.end())
			vec.erase(iter); 
	}
} // namespace ion