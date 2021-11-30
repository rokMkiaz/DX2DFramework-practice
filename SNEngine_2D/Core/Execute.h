#pragma once

struct CAMERA_DATA
{
	D3DXMATRIX view;
	D3DXMATRIX projection;
};



class Execute	final
{

public:
	Execute();
	~Execute();

	void Update();
	void Render();

private:
	class Graphics* graphics = nullptr;
	class Camera* camera = nullptr;
	class D3D11_ConstantBuffer* camera_buffer = nullptr;
	D3D11_Pipeline* pipeline = nullptr;

	class Rect* rect = nullptr;
	class Rect* rect2 = nullptr;
};