#include"stdafx.h"
#include"Actor.h"

Actor::Actor()
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
	for (const auto& component : components)
		component->Initialize();
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
		component->Destory();
}

auto Actor::AddComponent(const ComponentType& type) -> std::shared_ptr<IComponent>
{
	//TODO : 
	return std::shared_ptr<IComponent>();
}

bool Actor::HasComponent(const ComponentType& type)
{
	for (const auto& component : components)
	{
		if (component->GetComponetType() == type)
			return true;

	}
	return false;
}
