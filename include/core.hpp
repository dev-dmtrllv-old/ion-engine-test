#pragma once

#include "Game.hpp"
#include "Logger.hpp"
#include "SubSystem.hpp"
#include "hash.hpp"

namespace ion
{
	struct Event
	{
		SubSystemInterface& subSystem;
	};

	using EventHandler = void(*)(const Event&, void* data);
	struct EventHandlerData
	{
		EventHandler handler;
		void* externalData;
	};

	class Core final
	{
	public:
		Core(const std::filesystem::path& cwd, Logger& logger);
		~Core();

		[[nodiscard]] int run();

	private:
		template<IsSubSystem T>
		void registerSystem()
		{
			const char* systemName = typeid(T).name();
			const Hash hash = Hasher::hashType<T>();
			if (systems_.contains(hash))
				throw std::runtime_error(std::format("{} is already registered!", systemName));
			logger_.debug("Registering", systemName);
			T* system = new T(*this);
			systems_.emplace(hash, system);
			systemInitOrder_.emplace_back(system);
		}

		void initializeSystem(SubSystemInterface* system)
		{
			logger_.info("Initializing", system->name());
			system->initialize();
			emitEvent(Hasher::hash("INITIALIZED"), Event{ *system });
		}
		
		void disposeSystem(SubSystemInterface* system)
		{
			logger_.info("Disposing", system->name());
			system->dispose();
			emitEvent(Hasher::hash("DISPOSED"), Event{ *system });
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
		void dispose();

		void emitEvent(const Hash eventType, const Event& event);

	public:
		inline Logger& logger() { return logger_; }
		void registerEventHandler(const Hash eventType, EventHandler handler, void* externalData);
		void removeEventHandler(const Hash eventType, EventHandler handler);

	private:
		const std::filesystem::path& cwd_;
		Logger& logger_;

		std::unordered_map<Hash, SubSystemInterface*> systems_;
		std::vector<SubSystemInterface*> systemInitOrder_;
		std::unordered_map<Hash, std::vector<EventHandlerData>> eventHandlers_;
	};
} // namespace ion