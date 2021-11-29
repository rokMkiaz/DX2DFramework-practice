#pragma once
#include "stdafx.h"
#include"Execute.h"
#include"Graphics.h"

#include "Scene\Camera.h"

Execute::Execute()
{
	graphics = new Graphics();
	graphics->Initialize();
	graphics->CreateBackBuffer(static_cast<uint>(Settings::Get().GetWidth()),static_cast<uint>(Settings::Get().GetHeight()));
	
	//vertex,Index data
	D3D11_Geometry< D3D11_VertexTexture>geometry;
	Geometry_Generator::CreateQuad(geometry);


	//vertex bufffer
	vertex_buffer = new D3D11_VertexBuffer(graphics);
	vertex_buffer->Create(geometry.GetVertices());

	//Index Buffer
	index_buffer = new D3D11_IndexBuffer(graphics);
	index_buffer->Create(geometry.GetIndices());
	
	//Vertex Shader
	vertex_shader = new D3D11_Shader(graphics);
	vertex_shader->Create(ShaderScope_VS, "../_Asset/Shader/Texture.hlsl");
	
	//Input Layout
	input_layout = new D3D11_InputLayout(graphics);
	input_layout->Create(D3D11_VertexTexture::descs, D3D11_VertexTexture::count, vertex_shader->GetShaderBlob());

	//Pixel Shader
	pixel_shader = new D3D11_Shader(graphics);
	pixel_shader->Create(ShaderScope_PS, "../_Asset/Shader/Texture.hlsl");

	//create constant buffer
	gpu_buffer = new D3D11_ConstantBuffer(graphics);
	gpu_buffer->Create<TRANSFORM_DATA>();

	//create Rasterizer state
	rasterizer_state = new D3D11_RasterizerState(graphics);
	rasterizer_state->Create(D3D11_CULL_BACK, D3D11_FILL_SOLID);

	//create Shader Resource view
	texture = new D3D11_Texture(graphics);
	texture->Create("../_Asset/Texture/Free.png");

	//Create Sampler state
	sampler_state= new D3D11_SamplerState(graphics);
	sampler_state->Create(D3D11_FILTER_MIN_MAG_MIP_LINEAR,D3D11_TEXTURE_ADDRESS_CLAMP);

	//Create Blend State
	blend_state = new D3D11_BlendState(graphics);
	blend_state->Create(true);

	camera = new Camera();
	//create world 
	{
		D3DXMatrixIdentity(&world);

		//world
		D3DXMATRIX world_Scale;
		D3DXMatrixScaling(&world_Scale,500,500,1 );
		D3DXMATRIX world_Rotation;
		D3DXMatrixRotationZ(&world_Rotation,static_cast<float>(D3DXToRadian(0))); //2차원이므로 z축만 회전시켜준다
		D3DXMATRIX world_Translation;
		D3DXMatrixTranslation(&world_Translation, 0, 0, 0);

		//Scale * Rotation * Translation* (Rotation(idle) * Parents)
		world = world_Scale * world_Rotation * world_Translation;

	}
}

Execute::~Execute()
{
	SAFE_DELETE(camera)
	SAFE_DELETE(blend_state);
	SAFE_DELETE(sampler_state);
	SAFE_DELETE(texture);
	SAFE_DELETE(rasterizer_state);
	SAFE_DELETE(gpu_buffer);
	SAFE_DELETE(pixel_shader);
	SAFE_DELETE(input_layout);
	SAFE_DELETE(vertex_shader);
	SAFE_DELETE(index_buffer);
	SAFE_DELETE(vertex_buffer);
	SAFE_DELETE(graphics);
}

void Execute::Update()
{
	camera->Update();

	auto buffer = gpu_buffer->Map<TRANSFORM_DATA>();
	{
		D3DXMatrixTranspose(&buffer->world,&world); //전치행렬변환 함수
		D3DXMatrixTranspose(&buffer->view, &camera->GetViewMatrix());
		D3DXMatrixTranspose(&buffer->projection, &camera->GetProjectionMatrix());
	}
	gpu_buffer->Unmap();


}

void Execute::Render()
{

	graphics->Begin();
	{
		//IA
		ID3D11Buffer* buffers[] = { vertex_buffer->GetResource() };

		graphics->GetDeviceContext()->IASetVertexBuffers(0, 1, buffers, &vertex_buffer->GetStride(), &vertex_buffer->GetOffset());
		graphics->GetDeviceContext()->IASetIndexBuffer(index_buffer->GetResource(), DXGI_FORMAT_R32_UINT, index_buffer->GetOffset());
		graphics->GetDeviceContext()->IASetInputLayout(input_layout->GetResource());
		graphics->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//VS
		graphics->GetDeviceContext()->VSSetShader(static_cast<ID3D11VertexShader*>(vertex_shader->GetResource()), nullptr, 0);
		ID3D11Buffer* cbuffers[] = { gpu_buffer->GetResource() };
		graphics->GetDeviceContext()->VSSetConstantBuffers(0, 1, cbuffers);

		//RS
		graphics->GetDeviceContext()->RSSetState(rasterizer_state->GetResource());

		//PS
		graphics->GetDeviceContext()->PSSetShader(static_cast<ID3D11PixelShader*>(pixel_shader->GetResource()), nullptr, 0);
		
		ID3D11ShaderResourceView* shader_resources[] = { texture->GetResource() };
		graphics->GetDeviceContext()->PSSetShaderResources(0, 1, shader_resources);

		ID3D11SamplerState* samplers[]{ sampler_state->GetResource() };
		graphics->GetDeviceContext()->PSSetSamplers(0, 1, samplers);

		//OM
		float factor = blend_state->GetBlendFactor();
		float blend_factor[] = { factor ,factor ,factor ,factor };
		graphics->GetDeviceContext()->OMSetBlendState(blend_state->GetResource(), blend_factor, blend_state->GetSampleMask());

		graphics->GetDeviceContext()->DrawIndexed(6, 0, 0);
	}
	graphics->End();
}
