#pragma once
#include "stdafx.h"
#include"Execute.h"

#include"Graphics.h"

Execute::Execute()
{
	graphics = new Graphics();
	graphics->Initialize();
	graphics->CreateBackBuffer(static_cast<uint>(Settings::Get().GetWidth()),static_cast<uint>(Settings::Get().GetHeight()));
	
	//vertex data
	{
		vertices = new VertexTexture[4];
		vertices[0].position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
		vertices[0].uv = D3DXVECTOR2(0.0f, 1.0f);

		vertices[1].position = D3DXVECTOR3(-0.5f, +0.5f, 0.0f);
		vertices[1].uv = D3DXVECTOR2(0.0f, 0.0f);

		vertices[2].position = D3DXVECTOR3(+0.5f, -0.5f, 0.0f);
		vertices[2].uv = D3DXVECTOR2(1.0f, 1.0f);

		vertices[3].position = D3DXVECTOR3(+0.5f, +0.5f, 0.0f);
		vertices[3].uv = D3DXVECTOR2(1.0f, 0.0f);
	}
	//vertex bufffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = sizeof(VertexTexture) * 4;

		D3D11_SUBRESOURCE_DATA sub_data;
		ZeroMemory(&sub_data, sizeof(D3D11_SUBRESOURCE_DATA));
		sub_data.pSysMem = vertices;

		auto hr = graphics->GetDevice()->CreateBuffer(&desc, &sub_data, &vertex_buffer);
		assert(SUCCEEDED(hr));

	}
	//Index Data
	{
		indices = new uint[6]{ 0,1,2,2,1,3 };

	}
	//Index Buffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = sizeof(uint) * 6;

		D3D11_SUBRESOURCE_DATA sub_data;
		ZeroMemory(&sub_data, sizeof(D3D11_SUBRESOURCE_DATA));
		sub_data.pSysMem = indices;

		auto hr = graphics->GetDevice()->CreateBuffer(&desc, &sub_data, &index_buffer);
		assert(SUCCEEDED(hr));


	}


	//Vertex Shader
	{
		auto hr = D3DX11CompileFromFileA
		(
			"Texture.hlsl",
			nullptr,
			nullptr,
			"VS",
			"vs_5_0",
			0,
			0,
			nullptr,
			&vs_blob,
			nullptr,
			nullptr
		);
		assert(SUCCEEDED(hr));

		hr = graphics->GetDevice()->CreateVertexShader(vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), nullptr, &vertex_shader);
		assert(SUCCEEDED(hr));
	}

	//Input Layout
	{
	/*
		D3D11_INPUT_ELEMENT_DESC layout_desc[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
	*/
		D3D11_INPUT_ELEMENT_DESC layout_desc[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		auto hr = graphics->GetDevice()->CreateInputLayout(layout_desc, 2, vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(), &input_layout);
		assert(SUCCEEDED(hr));
	}

	//Pixel Shader

	{
		auto hr = D3DX11CompileFromFileA
		(
			"Texture.hlsl",
			nullptr,
			nullptr,
			"PS",
			"ps_5_0",
			0,
			0,
			nullptr,
			&ps_blob,
			nullptr,
			nullptr
		);
		assert(SUCCEEDED(hr));

		hr = graphics->GetDevice()->CreatePixelShader(ps_blob->GetBufferPointer(), ps_blob->GetBufferSize(), nullptr, &pixel_shader);
		assert(SUCCEEDED(hr));
	}

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

		std::cout << "View Matrix" << std::endl;
		std::cout << view._11 << " " << view._12 << " " << view._13 << " " << view._14 << std::endl;
		std::cout << view._21 << " " << view._22 << " " << view._23 << " " << view._24 << std::endl;
		std::cout << view._31 << " " << view._32 << " " << view._33 << " " << view._34 << std::endl;
		std::cout << view._41 << " " << view._42 << " " << view._43 << " " << view._44 << std::endl;

		std::cout << std::endl;

		std::cout << "Projection Matrix" << std::endl;
		std::cout << projection._11 << " " << projection._12 << " " << projection._13 << " " << projection._14 << std::endl;
		std::cout << projection._21 << " " << projection._22 << " " << projection._23 << " " << projection._24 << std::endl;
		std::cout << projection._31 << " " << projection._32 << " " << projection._33 << " " << projection._34 << std::endl;
		std::cout << projection._41 << " " << projection._42 << " " << projection._43 << " " << projection._44 << std::endl;

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

	//create constant buffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(TRANSFROM_DATA);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		auto hr = graphics->GetDevice()->CreateBuffer(&desc, nullptr, &gpu_buffer);
		assert(SUCCEEDED(hr));
	}

	//create Rasterizer state
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.FillMode = D3D11_FILL_SOLID; //채우기 또는 채울지
		desc.CullMode = D3D11_CULL_BACK;//Culling할 대상(Front,Back)
		desc.FrontCounterClockwise = false; //앞뒷면 방향(시계,반시계 방향)

		auto hr = graphics->GetDevice()->CreateRasterizerState(&desc, &rasterizer_state);

	}
	//create Shader Resource view
	{
		//auto hr = D3DX11CreateShaderResourceViewFromFileA //신버전에서는 제거됨
		//(
		//	graphics->GetDevice(),
		//	"Free.png",
		//	nullptr,
		//	nullptr,
		//	&shader_resource[0],
		//	nullptr
		//);
		//assert(SUCCEEDED(hr));

	 //  hr = D3DX11CreateShaderResourceViewFromFileA //신버전에서는 제거됨
		//(
		//	graphics->GetDevice(),
		//	"war.png",
		//	nullptr,
		//	nullptr,
		//	&shader_resource[1],
		//	nullptr
		//);
		//assert(SUCCEEDED(hr));
		auto hr = D3DX11CreateShaderResourceViewFromFileA //신버전에서는 제거됨
		(
			graphics->GetDevice(),
			"Free.png",
			nullptr,
			nullptr,
			&shader_resource,
			nullptr
		);
		assert(SUCCEEDED(hr));
	}
	//Create Sampler state
	{
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SAMPLER_DESC));
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; //u좌표가 늘어났을때
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; //v가 늘어났을때
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; //나머지가 늘어났을때
		desc.BorderColor[0] = 1; //외곽선
		desc.BorderColor[1] = 0;
		desc.BorderColor[2] = 0;
		desc.BorderColor[3] = 1;
		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS; //비교
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; //축소 및 확대될때 보정
		desc.MaxAnisotropy = 16; //비등방성 필터링
		desc.MaxLOD = std::numeric_limits<float>::max(); //Level of detail 조정
		desc.MinLOD = std::numeric_limits<float>::min();
		desc.MipLODBias = 0.0f; //오프셋 정보

		auto hr = graphics->GetDevice()->CreateSamplerState(&desc, &sampler_state);
		assert(SUCCEEDED(hr));
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
	SAFE_RELEASE(sampler_state);
	SAFE_RELEASE(shader_resource);
	//SAFE_RELEASE(shader_resource[1]);
	//SAFE_RELEASE(shader_resource[0]);

	SAFE_RELEASE(rasterizer_state);

	SAFE_RELEASE(gpu_buffer);

	SAFE_RELEASE(pixel_shader);
	SAFE_RELEASE(ps_blob);

	SAFE_RELEASE(input_layout);

	SAFE_RELEASE(vertex_shader);
	SAFE_RELEASE(vs_blob);

	SAFE_DELETE_ARRAY(indices);
	SAFE_RELEASE(index_buffer);

	SAFE_RELEASE(vertex_buffer);
	SAFE_DELETE_ARRAY(vertices);

	SAFE_DELETE(graphics);
}

void Execute::Update()
{
	static float radian = 0.0f;
	radian += 0.01f;
	D3DXMATRIX P; //임시 부모행렬
	D3DXMatrixRotationZ(&P, radian);
	//world = world * P;

	D3DXMatrixTranspose(&cpu_buffer.world,&world); //전치행렬변환 함수
	D3DXMatrixTranspose(&cpu_buffer.view, &view);
	D3DXMatrixTranspose(&cpu_buffer.projection, &projection);

	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	graphics->GetDeviceContext()->Map
	(
		gpu_buffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mapped_subresource
	);

	memcpy(mapped_subresource.pData, &cpu_buffer, sizeof(TRANSFROM_DATA));

	graphics->GetDeviceContext()->Unmap(gpu_buffer, 0);
}

void Execute::Render()
{
	uint stride = sizeof(VertexTexture);
	uint offset=0;

	graphics->Begin();
	{
		//IA
		graphics->GetDeviceContext()->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
		graphics->GetDeviceContext()->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);
		graphics->GetDeviceContext()->IASetInputLayout(input_layout);
		graphics->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//VS
		graphics->GetDeviceContext()->VSSetShader(vertex_shader, nullptr, 0);
		graphics->GetDeviceContext()->VSSetConstantBuffers(0, 1, &gpu_buffer);

		//RS
		graphics->GetDeviceContext()->RSSetState(rasterizer_state);

		//PS
		graphics->GetDeviceContext()->PSSetShader(pixel_shader, nullptr, 0);
		graphics->GetDeviceContext()->PSSetShaderResources(0, 1, &shader_resource);
		graphics->GetDeviceContext()->PSSetSamplers(0, 1, &sampler_state);

		//OM
		graphics->GetDeviceContext()->OMSetBlendState(blend_state, nullptr, 0xffffffff);

		graphics->GetDeviceContext()->DrawIndexed(6, 0, 0);
	}
	graphics->End();
}
