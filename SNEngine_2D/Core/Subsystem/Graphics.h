#pragma once
#include"Core\D3D11\Rasterizer\D3D11_Viewport.h"
#include"ISubsystem.h"

class Graphics final : public ISubsystem
{
public:
	Graphics(class Context* contexxt);
	~Graphics();

	bool Initialize() override;
	void Update() override {}

	void CreateBackBuffer(const uint& width, const uint& height);

	ID3D11Device* GetDevice() { return device; }
	ID3D11DeviceContext* GetDeviceContext() { return device_context; }

	void Begin(); //그리기 시작
	void End();


private:
	ID3D11Device* device				       = nullptr;
	ID3D11DeviceContext* device_context        = nullptr;
	IDXGISwapChain* swap_chain			       = nullptr;
	ID3D11RenderTargetView* render_target_view = nullptr;
	D3D11_Viewport viewport                    = D3D11_Viewport::Undefined_viewport; 
	D3DXCOLOR clear_color                      = 0xff555566;

};
