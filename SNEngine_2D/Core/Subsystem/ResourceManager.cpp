#include "stdafx.h"
#include "ResourceManager.h"

ResourceManager::ResourceManager(Context* const context)
	: ISubsystem(context)
{
	auto root_directory = GetAssetDirectory();

	RegisterDirectory(AssetType::Animation, root_directory + "Animation/");
	RegisterDirectory(AssetType::Animation, root_directory + "Shader/");
	RegisterDirectory(AssetType::Animation, root_directory + "Texture/");
}

bool ResourceManager::Initialize()
{
	return true;
}

bool ResourceManager::HasResource(const std::string& resource_name, const ResourceType& resource_type)
{
	for (const auto& resource : resource_groups[resource_type])
	{
		if (resource->GetResourceName() == resource_name)
			return true;
	}
	return false;
}

auto ResourceManager::GetAssetDirectory(const AssetType& type) -> const std::string
{
	return asset_directories[type];
}

void ResourceManager::RegisterDirectory(const AssetType& type, const std::string& directory)
{
	asset_directories[type] = directory;
}
