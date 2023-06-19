#pragma once

#include "pch.hpp"

#define SUBSYSTEM_CTOR(__NAME__) private:\
	__NAME__(Core& core); \
	__NAME__(const __NAME__&) = delete; \
	__NAME__(__NAME__&&) = delete; \
	~__NAME__(); \
	friend class ion::Core;

namespace ion
{
	class Core;

	class SubSystemInterface
	{
	public:
		virtual const char* name() = 0;

	protected:
		virtual void initialize() = 0;
		virtual void dispose() = 0;

		friend class Core;
	};

	
	// struct SubSystemEvent
	// {
	// 	SubSystemEvent(SubSystemInterface& subSystem):subSystem(subSystem) {}

	// 	SubSystemInterface& subSystem;

	// 	enum Events : std::size_t
	// 	{
	// 		INITIALIZE,
	// 		DISPOSE
	// 	};
	// };

	template<typename T>
	class SubSystem : public SubSystemInterface
	{
	protected:
		SubSystem(Core& core) : SubSystemInterface(), core(core) {}
		SubSystem(const SubSystem&) = delete;
		SubSystem(SubSystem&&) = delete;
		virtual ~SubSystem() {};

	public:
		virtual const char* name() override { return typeid(T).name(); };
	
	protected:
		void initialize() override { static_cast<T*>(this)->initialize(); }
		void dispose() override { static_cast<T*>(this)->dispose(); }
	
	public:
		Core& core;

		friend class Core;
	};

	template<typename T>
	concept IsSubSystem = std::is_base_of<SubSystemInterface, T>::value;
}
