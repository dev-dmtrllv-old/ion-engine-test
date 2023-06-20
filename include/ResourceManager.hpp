#pragma once

#include "pch.hpp"
#include "SubSystem.hpp"	

namespace ion
{
	class ResourceManager final : public SubSystem<ResourceManager>
	{
		SUBSYSTEM_CTOR(ResourceManager);

	public:
		virtual const char* name() const override;
		virtual void initialize() override;
		virtual void dispose() override;
	};
}