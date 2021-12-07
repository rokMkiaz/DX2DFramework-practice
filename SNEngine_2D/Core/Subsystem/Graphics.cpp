#pragma once
#include"stdafx.h"
#include"Graphics.h"


Graphics::Graphics(Context* contexxt)
    :ISubsystem(context)
{
}

Graphics::~Graphics()
{
    SAFE_RELEASE(render_target_view);
    SAFE_RELEASE(device);
    SAFE_RELEASE(device_context);
    SAFE_RELEASE(swap_chain);

}

bool Graphics::Initialize()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
    desc.BufferDesc.Width                 = 0;
    desc.BufferDesc.Height                = 0;
    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.BufferDesc.Format                = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferDesc.ScanlineOrdering      = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    desc.BufferDesc.Scaling               = DXGI_MODE_SCALING_UNSPECIFIED;

   
    desc.BufferCount         = 1;
    desc.BufferUsage         = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    desc.SampleDesc.Count   = 1;
    desc.SampleDesc.Quality = 0;

    desc.OutputWindow       = Settings::Get().GetWindowHandle();
    desc.Windowed           = TRUE;
    desc.SwapEffect         = DXGI_SWAP_EFFECT_DISCARD;


    std::vector<D3D_FEATURE_LEVEL>feature_levels
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };

    auto hr = D3D11CreateDeviceAndSwapChain
    (
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        feature_levels.data(),
        1,
        D3D11_SDK_VERSION,
        &desc,
        &swap_chain,
        &device,
        nullptr,
        &device_context
    );
    assert(SUCCEEDED(hr));

    return true;
}

void Graphics::CreateBackBuffer(const uint& width, const uint& height)
{
    auto hr = swap_chain->ResizeBuffers
    (
        0,
        width,
        height,
        DXGI_FORMAT_UNKNOWN,
        0
    );
    assert(SUCCEEDED(hr));

    ID3D11Texture2D* back_buffer = nullptr;
    hr = swap_chain->GetBuffer
    (
        0,
        __uuidof(ID3D11Texture2D),
        reinterpret_cast<void**>(&back_buffer)
    );
    assert(SUCCEEDED(hr));

    hr = device->CreateRenderTargetView
    (
        back_buffer,
        nullptr,
        &render_target_view
    );
    assert(SUCCEEDED(hr));

    viewport.x= 0.0f;
    viewport.y= 0.0f;
    viewport.width =  static_cast<float>(width);
    viewport.height = static_cast<float>(height);
    viewport.min_depth = 0.0f;
    viewport.max_depth = 1.0f;


    SAFE_RELEASE(back_buffer);
}

void Graphics::Begin() 
{
    device_context->OMSetRenderTargets(1,&render_target_view,nullptr);
    D3D11_VIEWPORT d3d11_viewport;
    d3d11_viewport.TopLeftX         = viewport.x;
    d3d11_viewport.TopLeftY         =  viewport.y;
    d3d11_viewport.Width            = viewport.width;
    d3d11_viewport.Height           = viewport.height;
    d3d11_viewport.MinDepth         = viewport.min_depth;
    d3d11_viewport.MaxDepth         = viewport.max_depth;
    device_context->RSSetViewports(1, &d3d11_viewport);


    device_context->ClearRenderTargetView(render_target_view, clear_color);
}

void Graphics::End()
{
    auto hr= swap_chain->Present(1,0);
    assert(SUCCEEDED(hr));
}
