#pragma once
#include "stdafx.h"
#include"Execute.h"

#include"Graphics.h"

Execute::Execute()
{
	graphics = new Graphics();
	graphics->Initialize();
	graphics->CreateBackBuffer(static_cast<uint>(Settings::Get().GetWidth()),static_cast<uint>(Settings::Get().GetHeight()));
	
	//vertex,Index data
	
	geometry.AddVertex(D3D11_VertexTexture(D3DXVECTOR3(-0.5f, -0.5f, 0.0f), D3DXVECTOR2(0.0f, 1.0f)));
	geometry.AddVertex(D3D11_VertexTexture(D3DXVECTOR3(-0.5f, +0.5f, 0.0f), D3DXVECTOR2(0.0f, 0.0f)));
	geometry.AddVertex(D3D11_VertexTexture(D3DXVECTOR3(+0.5f, -0.5f, 0.0f), D3DXVECTOR2(1.0f, 1.0f)));
	geometry.AddVertex(D3D11_VertexTexture(D3DXVECTOR3(+0.5f, +0.5f, 0.0f), D3DXVECTOR2(1.0f, 0.0f)));
	geometry.AddIndex(0);	geometry.AddIndex(1);	geometry.AddIndex(2);
	geometry.AddIndex(2);	geometry.AddIndex(1);	geometry.AddIndex(3);

	//vertex bufffer
	vertex_buffer = new D3D11_VertexBuffer(graphics);
	vertex_buffer->Create(geometry.GetVertices());

	//Index Buffer
	index_buffer = new D3D11_IndexBuffer(graphics);
	index_buffer->Create(geometry.GetIndices());
	


	//Vertex Shader
	vertex_shader = new D3D11_Shader(graphics);
	vertex_shader->Create(ShaderScope_VS, "Texture.hlsl");
	
	//Input Layout
	input_layout = new D3D11_InputLayout(graphics);
	input_layout->Create(D3D11_VertexTexture::descs, D3D11_VertexTexture::count, vertex_shader->GetShaderBlob());



	//Pixel Shader
	pixel_shader = new D3D11_Shader(graphics);
	pixel_shader->Create(ShaderScope_PS, "Texture.hlsl");


	//create constant buffer
	gpu_buffer = new D3D11_ConstantBuffer(graphics);
	gpu_buffer->Create<TRANSFORM_DATA>();

	//create Rasterizer state
	rasterizer_state = new D3D11RasterizerState(graphics);
	rasterizer_state->Create(D3D11_CULL_BACK, D3D11_FILL_SOLID);

	//create Shader Resource view
	texture = new D3D11_Texture(graphics);
	texture->Create("Free.png");

	//Create Sampler state
	sampler_state= new D3D11_SamplerState(graphics);
	sampler_state->Create(D3D11_FILTER_MIN_MAG_MIP_LINEAR,D3D11_TEXTURE_ADDRESS_CLAMP);

	//create world view Projection
	{
		D3DXMatrixIdentity(&world);
		D3DXMatrixIdentity(&view);
		D3DXMatrixIdentity(&projection);

		D3DXVECTOR3 view_Eye = {0,0,0};  //보는 위치 원점 위치
		D3DXVECTOR3 view_At =  {0,0,1}; // 어디를 볼 것인가.
		D3DXVECTOR3 view_Up =  {0,1,0}; // 위쪽을 가리키는 방향벡터

		D3DXMatrixLookAtLH(&view, &view_Eye, &view_At, &view_Up);
		//D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0)); Diertx 2010 및 Windows SDK호환문제로 바뀜.
		D3DXMatrixOrthoLH(&projection, Settings::Get().GetWidth(), Settings::Get().GetHeight(), 0, 1);
		//D3DXMatrixOrthoOffCenterLH(&view,0,Settings::Get().GetWidth(),0, Settings::Get().GetHeight(),0,1);  좌표계 선택

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






	//Create Blend State
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
		desc.AlphaToCoverageEnable = false;   //알파값 제거
		desc.IndependentBlendEnable = false; //블랜드를 독립적으로 진행
		desc.RenderTarget[0].BlendEnable = true; //외곽선을 바탕색과 안티앨리얼싱 해줌

		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; //원본색상
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; //어느정도 알파색으로 섞을것인가
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; //두개의 픽셀색을 더한다.

		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE; //배경에 있는 픽샐색상
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; 
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;//특정 비트값을 가지는 이진수 나열, 어떤색상을 뽑아낼지 설정
	
		auto hr = graphics->GetDevice()->CreateBlendState(&desc, &blend_state);
		assert(SUCCEEDED(hr));
	
	}

}

Execute::~Execute()
{
	SAFE_RELEASE(blend_state);

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
	static float radian = 0.0f;
	radian += 0.01f;
	D3DXMATRIX P; //임시 부모행렬
	D3DXMatrixRotationZ(&P, radian);
	//world = world * P;
	auto buffer = gpu_buffer->Map<TRANSFORM_DATA>();
	{
		D3DXMatrixTranspose(&buffer->world,&world); //전치행렬변환 함수
		D3DXMatrixTranspose(&buffer->view, &view);
		D3DXMatrixTranspose(&buffer->projection, &projection);
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
		graphics->GetDeviceContext()->IASetIndexBuffer(index_buffer->GetResourc(), DXGI_FORMAT_R32_UINT, index_buffer->GetOffset());
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
		graphics->GetDeviceContext()->OMSetBlendState(blend_state, nullptr, 0xffffffff);

		graphics->GetDeviceContext()->DrawIndexed(6, 0, 0);
	}
	graphics->End();
}
