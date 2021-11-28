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
	D3DXVECTOR3 view_Eye = { 0,0,0 };  //���� ��ġ ���� ��ġ
	D3DXVECTOR3 view_At = { 0,0,1 }; // ��� �� ���ΰ�.
	D3DXVECTOR3 view_Up = { 0,1,0 }; // ������ ����Ű�� ���⺤��

	D3DXMatrixLookAtLH(&view,&view_Eye, &view_At, &view_Up);
}

void Camera::UpdateProjectionMatrix()
{
	D3DXMatrixOrthoLH(&proj, Settings::Get().GetWidth(), Settings::Get().GetHeight(),0.0f,1.0f);
}
