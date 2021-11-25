#pragma once

struct VertexColor
{
	D3DXVECTOR3 position;
	D3DXCOLOR	color;
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

	VertexColor* vertices = nullptr; 
	ID3D11Buffer* vertex_buffer = nullptr;
	ID3D11InputLayout* input_layout = nullptr;

	uint* indices = nullptr;//ÀÎµ¦½º¹öÆÛ
	ID3D11Buffer* index_buffer = nullptr;

	ID3D11VertexShader* vertex_shader = nullptr;
	ID3DBlob* vs_blob = nullptr;

	ID3D11PixelShader* pixel_shader = nullptr;
	ID3DBlob* ps_blob = nullptr;

	D3DXMATRIX world; 
	D3DXMATRIX view;
	D3DXMATRIX projection;

};