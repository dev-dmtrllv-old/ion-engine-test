#include "pch.hpp"
#include "js/ScriptManager.hpp"
#include "Core.hpp"
#include "v8.h"
#include "libplatform/libplatform.h"
#include "js/Env.hpp"

namespace ion::js
{
	ScriptManager::ScriptManager(Core& core) : SubSystem(core),
		platform_(v8::platform::NewDefaultPlatform())
	{

	}

	ScriptManager::~ScriptManager()
	{
		
	}

	[[nodiscard]] Env& ScriptManager::createEnv()
	{
		return *envs_.emplace_back(new Env());
	}

	[[nodiscard]] Env& ScriptManager::mainEnv()
	{
		assert(envs_.size() > 0);
		return *envs_.at(0);
	}

	const char* ScriptManager::name() const
	{
		return "ScriptManager";
	}

	void ScriptManager::initialize()
	{
		v8::V8::InitializePlatform(platform_.get());
		v8::V8::Initialize();
		envs_.emplace_back(new Env());
	}

	void ScriptManager::dispose()
	{
		for (Env* env : envs_)
			env->~Env();
		
		envs_.clear();

		v8::V8::Dispose();
		v8::V8::DisposePlatform();
	}
}