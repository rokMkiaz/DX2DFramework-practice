#pragma once
#include"../ISubsystem.h"
#include"Renderer_Types.h"
#include"Renderer_ConstantBuffers.h"


class Renderer final : public ISubsystem
{
public:
	Renderer(class Context* const context);
	~Renderer();

	bool Initialize() override;
	void Update() override;
	
	void UpdateRenderables(class Scene* const scene);

private:
	void CreateConstantBuffers();
	void CreateRasterizerStates();
	void CreateBlendStates();

	void UpdateCameraBuffer();
	void UpdateObjectBuffer();
	void UpdateAnimationBuffer();

private:
	void PassMain();

private:
	class Graphics* graphics = nullptr;
	class CameraComponent* camera = nullptr;
	std::shared_ptr<class D3D11_Pipeline> pipeline;

	CAMERA_DATA cpu_camera_buffer;
	std::shared_ptr<class D3D11_ConstantBuffer> gpu_camera_buffer;

	TRANSFORM_DATA cpu_object_buffer;
	std::shared_ptr<class D3D11_ConstantBuffer> gpu_object_buffer;

	ANIMATION_DATA cpu_animation_buffer;
	std::shared_ptr<class D3D11_ConstantBuffer> gpu_animation_buffer;

private:

	std::map<RasterizerStateType, std::shared_ptr<class D3D11_RasterizerState>> rasterizers;
	std::map<BlendStateType, std::shared_ptr<class D3D11_BlendState>> blend_states;

	std::unordered_map<RenderableType, std::vector<class Actor*>> renderables;
};
