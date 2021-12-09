#include"stdafx.h"
#include"Renderer.h"
#include"Scene\Scene.h"
#include"Scene\Actor.h"
#include"Scene\Component\CameraComponent.h"
#include"Scene\Component\MeshRendererComponent.h"

Renderer::Renderer(Context* const context)
	:ISubsystem(context)
{
}

Renderer::~Renderer()
{
}

bool Renderer::Initialize()
{
	graphics = context->GetSubsystem<Graphics>();

	graphics->CreateBackBuffer(static_cast<uint>(Settings::Get().GetWidth()), static_cast<uint>(Settings::Get().GetHeight()));

	pipeline = std::make_shared<D3D11_Pipeline>(graphics);

	CreateConstantBuffers();
	CreateRasterizerStates();
	CreateBlendStates();


	return true;
}

void Renderer::Update()
{
	if (!camera)
		return;

	D3DXMatrixTranspose(&cpu_camera_buffer.view, &camera->GetViewMatrix());
	D3DXMatrixTranspose(&cpu_camera_buffer.projection, &camera->GetProjectionMatrix());
	UpdateCameraBuffer();

	graphics->Begin();
	PassMain();
	graphics->End();
}

void Renderer::UpdateRenderables(Scene* const scene)
{
	auto actors = scene->GetActors();
	if (actors.empty())
		return;
	for (const auto& actor : actors)
	{
		auto camera_component = actor->GetComponent<CameraComponent>();
		auto mesh_renderer_component = actor->GetComponent<MeshRendererComponent>();

		if (camera_component)
		{
			renderables[RenderableType::Camera].emplace_back(actor.get());
			camera = camera_component.get();
		}
		if (mesh_renderer_component)
		{
			renderables[RenderableType::Opaque].emplace_back(actor.get());
		}
	}
}



void Renderer::UpdateCameraBuffer()
{

	auto buffer = gpu_camera_buffer->Map<CAMERA_DATA>();
	*buffer = cpu_camera_buffer;
	gpu_camera_buffer->Unmap();
}

void Renderer::UpdateObjectBuffer()
{
	auto buffer = gpu_object_buffer->Map<TRANSFORM_DATA>();
	*buffer = cpu_object_buffer;
	gpu_object_buffer->Unmap();
}

