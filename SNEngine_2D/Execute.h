#pragma once

struct VertexColor
{
	D3DXVECTOR3 position;
	D3DXCOLOR	color;
};
struct VertexTexture
{
	D3DXVECTOR3 position;
	D3DXVECTOR2 uv; //texcoord
};
struct TRANSFROM_DATA
{
	D3DXMATRIX world;
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

	VertexTexture* vertices = nullptr; 
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


	TRANSFROM_DATA cpu_buffer;
	ID3D11Buffer* gpu_buffer = nullptr; //constant buffer

	ID3D11RasterizerState* rasterizer_state = nullptr;

	ID3D11ShaderResourceView* shader_resource = nullptr;

	ID3D11SamplerState * sampler_state = nullptr;
	ID3D11BlendState* blend_state= nullptr;
	//ID3D11ShaderResourceView* shader_resource[2];
};