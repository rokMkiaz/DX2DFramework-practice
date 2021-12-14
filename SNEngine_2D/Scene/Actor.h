#pragma once
#include "Component/IComponent.h"

class Actor final
{
public:
	Actor(class Context* const context);
	~Actor();

	void Initialize();
	void Update();
	void Destroy();

	/*
		Property
	*/
	auto GetName() const -> const std::string& { return name; }
	void SetName(const std::string& name) { this->name = name; }

	auto IsActive() const { return is_active; }
	void SetActive(const bool& is_active) { this->is_active = is_active; }

	auto GetTransform() const -> const std::shared_ptr<class TransformComponent>& { return transform; }
	auto GetTransform_Raw() const -> class TransformComponent* { return transform.get(); }

	/*
		Component
	*/
	auto AddComponent(const ComponentType& type) -> const std::shared_ptr<IComponent>;

	template <typename T>
	auto AddComponent() -> const std::shared_ptr<T>;

	template <typename T>
	auto GetComponent() -> const std::shared_ptr<T>;

	template <typename T>
	auto GetComponent_Raw()->T*;

	template <typename T>
	auto GetComponents() -> const std::vector<std::shared_ptr<T>>;

	auto GetAllComponents() const -> const std::vector<std::shared_ptr<IComponent>>& { return components; }

	bool HasComponent(const ComponentType& type);

	template <typename T>
	bool HasComponent();

	template <typename T>
	void RemoveComponent();

private:
	class Context* context = nullptr;

	std::string name;
	bool is_active = true;

	std::shared_ptr<class TransformComponent> transform;
	std::vector<std::shared_ptr<IComponent>> components;
};

template<typename T>
inline auto Actor::AddComponent() -> const std::shared_ptr<T>
{
	static_assert(std::is_base_of<IComponent, T>::value, "Provider type does not implement IComponent");

	auto type = IComponent::DeduceComponentType<T>();

	if (HasComponent(type))
		return GetComponent<T>();

	components.emplace_back
	(
		std::make_shared<T>
		(
			context,
			this,
			transform.get()
			)
	);

	auto new_component = std::static_pointer_cast<T>(components.back());
	new_component->Initialize();
	new_component->SetComponentType(type);

	if constexpr (std::is_same<T, class TransformComponent>::value)
		transform = new_component;

	return new_component;
}

template<typename T>
inline auto Actor::GetComponent() -> const std::shared_ptr<T>
{
	static_assert(std::is_base_of<IComponent, T>::value, "Provider type does not implement IComponent");

	auto type = IComponent::DeduceComponentType<T>();
	for (const auto& component : components)
	{
		if (component->GetComponentType() == type)
			return std::static_pointer_cast<T>(component);
	}
	return nullptr;
}

template<typename T>
inline auto Actor::GetComponent_Raw() -> T*
{
	static_assert(std::is_base_of<IComponent, T>::value, "Provider type does not implement IComponent");

	auto type = IComponent::DeduceComponentType<T>();
	for (const auto& component : components)
	{
		if (component->GetComponentType() == type)
			return std::static_pointer_cast<T>(component).get();
	}
	return nullptr;
}

template<typename T>
inline auto Actor::GetComponents() -> const std::vector<std::shared_ptr<T>>
{
	static_assert(std::is_base_of<IComponent, T>::value, "Provider type does not implement IComponent");

	auto type = IComponent::DeduceComponentType<T>();

	std::vector<std::shared_ptr<T>> temp_compoents;
	for (const auto& component : components)
	{
		if (component->GetComponentType() != type)
			continue;

		temp_compoents.emplace_back(std::static_pointer_cast<T>(component));
	}
	return temp_compoents;
}

template<typename T>
inline bool Actor::HasComponent()
{
	static_assert(std::is_base_of<IComponent, T>::value, "Provider type does not implement IComponent");

	return HasComponent(IComponent::DeduceComponentType<T>());
}

template<typename T>
inline void Actor::RemoveComponent()
{
	static_assert(std::is_base_of<IComponent, T>::value, "Provider type does not implement IComponent");

	auto type = IComponent::DeduceComponentType<T>();

	for (auto iter = components.begin(); iter != components.end(); )
	{
		auto component = *iter;
		if (component->GetComponentType() == type)
		{
			component->Destroy();
			component.reset();
			iter = components.erase(iter);
		}
		else
			iter++;
	}
}
