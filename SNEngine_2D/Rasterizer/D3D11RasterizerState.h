#pragma once

class D3D11RasterizerState final
{
public:
	D3D11RasterizerState(class Graphics* graphics);
	~D3D11RasterizerState();

	auto GetResource() const { return state; }

	void Create
	(
		const D3D11_CULL_MODE& cull_mode,  //보이지 않는면
		const D3D11_FILL_MODE& fill_mode
	);
	void Clear();

private:
	ID3D11Device* device = nullptr;
	ID3D11RasterizerState* state = nullptr;

};