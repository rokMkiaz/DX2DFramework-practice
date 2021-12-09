#include "stdafx.h"
#include "Actor.h"
#include "Component/TransformComponent.h"
#include "Component/MeshRendererComponent.h"

Actor::Actor(class Context* const context)
	:context(context)
{
	AddComponent<TransformComponent>();
}

Actor::~Actor()
{
	components.clear();
	components.shrink_to_fit();
}

void Actor::Initialize()
{
}

void Actor::Update()
{
	if (!is_active)
		return;

	for (const auto& component : components)
	{
		if (!component->IsEnabled())
			continue;

		component->Update();
	}
}

void Actor::Destroy()
{
	for (const auto& component : components)
		component->Destroy();
}

auto Actor::AddComponent(const ComponentType& type) -> const std::shared_ptr<IComponent>
{
	//TODO :
	return std::shared_ptr<IComponent>();
}

bool Actor::HasComponent(const ComponentType& type)
{
	for (const auto& component : components)
	{
		if (component->GetComponentType() == type)
			return true;
	}
	return false;
}
