#pragma once
#include "pch.hpp"

namespace ion
{
	class EventBus
	{
	public:
		EventBus();
		EventBus(const EventBus&) = delete;
		EventBus(EventBus&&) = delete;
		~EventBus();


	};
}