#pragma once


struct TRANSFORM_DATA
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

	D3D11_VertexBuffer* vertex_buffer = nullptr;
	D3D11_InputLayout* input_layout = nullptr;
	D3D11_IndexBuffer* index_buffer = nullptr;
	D3D11_Shader* vertex_shader = nullptr;
	D3D11_Shader* pixel_shader = nullptr;
	D3D11_RasterizerState* rasterizer_state = nullptr;
	D3D11_ConstantBuffer* gpu_buffer = nullptr; //constant buffer
	D3D11_Texture* texture = nullptr;
	D3D11_SamplerState * sampler_state = nullptr;
	D3D11_BlendState* blend_state= nullptr;

	class Camera* camera = nullptr;
	D3DXMATRIX world; 


	//ID3D11ShaderResourceView* shader_resource[2];
};