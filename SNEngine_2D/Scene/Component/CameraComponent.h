#pragma once
#include"IComponent.h"



class CameraComponent final : public IComponent
{
public:
	CameraComponent(class Context* const context, class Actor * const actor, class TransformComponent* const transform);
	~CameraComponent() = default;

	void Initialize() override;
	void Update() override;
	void Destroy() override;

	auto GetViewMatrix() const -> const D3DXMATRIX& { return view; }
	auto GetProjectionMatrix() const -> const D3DXMATRIX& { return proj; }


private:
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();

private:
	D3DXMATRIX view;
	D3DXMATRIX proj;

};