#include"stdafx.h"
#include"D3D11RasterizerState.h"

D3D11RasterizerState::D3D11RasterizerState(Graphics* graphics)
{
	device = graphics->GetDevice();
}

D3D11RasterizerState::~D3D11RasterizerState()
{
	Clear();
}

void D3D11RasterizerState::Create(const D3D11_CULL_MODE& cull_mode, const D3D11_FILL_MODE& fill_mode)
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
	desc.CullMode = cull_mode;
	desc.FillMode = fill_mode;
	desc.FrontCounterClockwise = false;

	auto hr = device->CreateRasterizerState(&desc, &state);
	assert(SUCCEEDED(hr));
}

void D3D11RasterizerState::Clear()
{
	SAFE_RELEASE(state);
}
