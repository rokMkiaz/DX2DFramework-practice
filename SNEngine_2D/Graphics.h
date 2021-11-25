#pragma once


class Graphics final
{
public:
	Graphics();
	~Graphics();

	void Initialize();
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
	D3D11_VIEWPORT viewport                    = { 0 }; 
	D3DXCOLOR clear_color                      = 0xff555566;

};
