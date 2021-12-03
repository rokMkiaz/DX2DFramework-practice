#pragma once
struct TRANSFORM_DATA
{
	D3DXMATRIX world;
};
struct COLOR_DATA
{
	D3DXCOLOR color;
};
enum class Direction
{
	Up,
	Right,
	Down,
	Left
};

class Rect 
{
public:
	Rect(class Graphics* graphics, const D3DXCOLOR & color);
	virtual ~Rect();

	auto GetScale() const -> const D3DXVECTOR3& { return scale; }
	void SetScale(const D3DXVECTOR3& scale) { this->scale = scale; }

	auto GetPosition() const -> const D3DXVECTOR3 & { return position; }
	void SetPosition(const D3DXVECTOR3& position) { this->position = position; }

	void SetIntersectColor(const D3DXCOLOR& color) { this->intersect_color = color; }

	auto IsActive() const { return is_active; }
	void SetActive(const bool& is_active) { this->is_active = is_active;}

	void Update(); 
	void Render(D3D11_Pipeline* pipeline);

	virtual void Event() = 0;
protected:
	virtual void Move() = 0;

protected :

	D3D11_VertexBuffer* vertex_buffer = nullptr;
	D3D11_InputLayout* input_layout = nullptr;
	D3D11_IndexBuffer* index_buffer = nullptr;
	D3D11_Shader* vertex_shader = nullptr;
	D3D11_Shader* pixel_shader = nullptr;
	D3D11_RasterizerState* rasterizer_state = nullptr;
	D3D11_ConstantBuffer* gpu_buffer = nullptr; //constant buffer
	//D3D11_Texture* texture = nullptr;
	//D3D11_SamplerState* sampler_state = nullptr;
	//D3D11_BlendState* blend_state = nullptr;

	D3D11_ConstantBuffer* color_buffer = nullptr;

	D3DXVECTOR3 position = D3DXVECTOR3(0,0,0);
	D3DXVECTOR3 scale = D3DXVECTOR3(100,100,1);
	D3DXMATRIX world;

	D3DXCOLOR intersect_color = D3DXCOLOR(1, 1, 1, 1);
	bool is_active = true;

};