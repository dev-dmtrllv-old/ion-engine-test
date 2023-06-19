#include "ResourceManager.hpp"

namespace ion
{
	ResourceManager::ResourceManager(Core& core): SubSystem(core)
	{

	}

	ResourceManager::~ResourceManager()
	{

	}

	const char* ResourceManager::name() const
	{
		return "ResourceManager";
	}

	void ResourceManager::initialize() {}

	void ResourceManager::dispose() {}
}