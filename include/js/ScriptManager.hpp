#pragma once

#include "pch.hpp"
#include "SubSystem.hpp"
#include "v8.h"
#include "libplatform/libplatform.h"
#include "js/Env.hpp"

namespace ion::js
{
	class ScriptManager final : public SubSystem<ScriptManager>
	{
		SUBSYSTEM_CTOR(ScriptManager);

	public:
		virtual const char* name() const override;
		virtual void initialize() override;
		virtual void dispose() override;

		[[nodiscard]] Env& createEnv();
		[[nodiscard]] Env& mainEnv();

	private:
		std::unique_ptr<v8::Platform> platform_;
		std::vector<Env*> envs_;
	};
}