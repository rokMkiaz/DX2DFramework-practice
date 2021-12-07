#include"stdafx.h"
#include"IComponent.h"
#include"CameraComponent.h"
#include"MeshRendererComponent.h"
#include"MoveScriptComponent.h"
#include"AIScriptComponent.h"

IComponent::IComponent(Context* const context,Actor* const actor, TransformComponent* const transform)
	: context(context), actor(actor), transform(transform)
{

}

template<typename T>
inline constexpr ComponentType IComponent::DeduceComponentType()
{
	return ComponentType::Unknown;
}

//explicit template instantiation
#define REGISTER_COMPONENT_TYPE(T, enum_type) template <> ComponentType IComponent::DeduceComponentType<T>() { return enum_type; }
REGISTER_COMPONENT_TYPE(TransformComponent,    ComponentType::Transform)
REGISTER_COMPONENT_TYPE(CameraComponent,       ComponentType::Camera)
REGISTER_COMPONENT_TYPE(MeshRendererComponent, ComponentType::MeshRenderer)
REGISTER_COMPONENT_TYPE(MoveScriptComponent,   ComponentType::MoveScript)
REGISTER_COMPONENT_TYPE(AIScriptComponent ,    ComponentType::AIScript)
