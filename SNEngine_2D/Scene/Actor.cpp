#include"stdafx.h"
#include"Actor.h"
#include"Component\TransformComponent.h"
#include"Component\MeshRendererComponent.h"

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

void Actor::Render(D3D11_Pipeline* const pipeline)
{
	if (!is_active)
		return;

	if(auto mesh_render = GetComponent<MeshRendererComponent>())
	{
		D3D11_PipelineState pipeline_state;
		pipeline_state.input_layout = mesh_render->GetInputLayout().get();
		pipeline_state.primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		pipeline_state.vertex_shader = mesh_render->GetVertexShader().get();
		pipeline_state.pixel_shader = mesh_render->GetPixelShader().get();


		if (pipeline->Begin(pipeline_state))
		{
			pipeline->SetVertexBuffer(mesh_render->GetVertexBuffer().get());
			pipeline->SetIndexBuffer(mesh_render->GetIndexBuffer().get());

			transform->UpdateConstantBuffer();
			pipeline->SetConstantBuffer(1, ShaderScope_VS, transform->GetConstantBuffer().get());
	
			pipeline->DrawIndexed(mesh_render->GetIndexBuffer()->GetCount(), mesh_render->GetIndexBuffer()->GetOffset(), mesh_render->GetVertexBuffer()->GetOffset());

			pipeline->End();
		}
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

