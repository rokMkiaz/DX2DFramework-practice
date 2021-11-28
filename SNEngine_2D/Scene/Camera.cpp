#include"stdafx.h"
#include"Camera.h"

Camera::Camera()
{
	D3DXMatrixIdentity(&view);
	D3DXMatrixIdentity(&proj);

	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void Camera::Update()
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void Camera::UpdateViewMatrix()
{
	D3DXVECTOR3 view_Eye = { 0,0,0 };  //보는 위치 원점 위치
	D3DXVECTOR3 view_At = { 0,0,1 }; // 어디를 볼 것인가.
	D3DXVECTOR3 view_Up = { 0,1,0 }; // 위쪽을 가리키는 방향벡터

	D3DXMatrixLookAtLH(&view,&view_Eye, &view_At, &view_Up);
}

void Camera::UpdateProjectionMatrix()
{
	D3DXMatrixOrthoLH(&proj, Settings::Get().GetWidth(), Settings::Get().GetHeight(),0.0f,1.0f);
}
