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

	class Camera* camera = nullptr;
	class D3D11_ConstantBuffer* camera_buffer = nullptr;
	D3D11_Pipeline* pipeline = nullptr;

	std::vector<class Actor*> actors;
};