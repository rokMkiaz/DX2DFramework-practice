#include"stdafx.h"
#include"CameraComponent.h"
#include"TransformComponent.h"


CameraComponent::CameraComponent(Actor* const actor, TransformComponent* const transform)
	: IComponent(actor, transform)
{

}

void CameraComponent::Initialize()
{
	D3DXMatrixIdentity(&view);
	D3DXMatrixIdentity(&proj);

	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void CameraComponent::Update()
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}
void CameraComponent::Destroy()
{
}


void CameraComponent::UpdateViewMatrix()
{
	auto position = transform->GetPosition();   //보는 위치 원점 위치
	auto forward = transform->GetForward();// 어디를 볼 것인가.
	auto up = transform->GetUp();	// 위쪽을 가리키는 방향벡터

	D3DXMatrixLookAtLH(&view,&position, &(position+forward), &up);
}

void CameraComponent::UpdateProjectionMatrix()
{
	D3DXMatrixOrthoLH(&proj, Settings::Get().GetWidth(), Settings::Get().GetHeight(),0.0f,1.0f);
}



