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


	//create world view Projection
	{
		D3DXMatrixIdentity(&world);
		D3DXMatrixIdentity(&view);
		D3DXMatrixIdentity(&projection);

		D3DXVECTOR3 view_Eye = {0,0,0};  //���� ��ġ ���� ��ġ
		D3DXVECTOR3 view_At =  {0,0,1}; // ��� �� ���ΰ�.
		D3DXVECTOR3 view_Up =  {0,1,0}; // ������ ����Ű�� ���⺤��

		D3DXMatrixLookAtLH(&view, &view_Eye, &view_At, &view_Up);
		//D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0)); Diertx 2010 �� Windows SDKȣȯ������ �ٲ�.
		D3DXMatrixOrthoLH(&projection, Settings::Get().GetWidth(), Settings::Get().GetHeight(), 0, 1);
		//D3DXMatrixOrthoOffCenterLH(&view,0,Settings::Get().GetWidth(),0, Settings::Get().GetHeight(),0,1);  ��ǥ�� ����

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
		D3DXMatrixRotationZ(&world_Rotation,static_cast<float>(D3DXToRadian(0))); //2�����̹Ƿ� z�ุ ȸ�������ش�
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
		desc.FillMode = D3D11_FILL_SOLID; //ä��� �Ǵ� ä����
		desc.CullMode = D3D11_CULL_BACK;//Culling�� ���(Front,Back)
		desc.FrontCounterClockwise = false; //�յ޸� ����(�ð�,�ݽð� ����)

		auto hr = graphics->GetDevice()->CreateRasterizerState(&desc, &rasterizer_state);

	}
	//create Shader Resource view
	{
		//auto hr = D3DX11CreateShaderResourceViewFromFileA //�Ź��������� ���ŵ�
		//(
		//	graphics->GetDevice(),
		//	"Free.png",
		//	nullptr,
		//	nullptr,
		//	&shader_resource[0],
		//	nullptr
		//);
		//assert(SUCCEEDED(hr));

	 //  hr = D3DX11CreateShaderResourceViewFromFileA //�Ź��������� ���ŵ�
		//(
		//	graphics->GetDevice(),
		//	"war.png",
		//	nullptr,
		//	nullptr,
		//	&shader_resource[1],
		//	nullptr
		//);
		//assert(SUCCEEDED(hr));
		auto hr = D3DX11CreateShaderResourceViewFromFileA //�Ź��������� ���ŵ�
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
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; //u��ǥ�� �þ����
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; //v�� �þ����
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; //�������� �þ����
		desc.BorderColor[0] = 1; //�ܰ���
		desc.BorderColor[1] = 0;
		desc.BorderColor[2] = 0;
		desc.BorderColor[3] = 1;
		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS; //��
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; //��� �� Ȯ��ɶ� ����
		desc.MaxAnisotropy = 16; //���漺 ���͸�
		desc.MaxLOD = std::numeric_limits<float>::max(); //Level of detail ����
		desc.MinLOD = std::numeric_limits<float>::min();
		desc.MipLODBias = 0.0f; //������ ����

		auto hr = graphics->GetDevice()->CreateSamplerState(&desc, &sampler_state);
		assert(SUCCEEDED(hr));
	}
	//Create Blend State
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
		desc.AlphaToCoverageEnable = false;   //���İ� ����
		desc.IndependentBlendEnable = false; //���带 ���������� ����
		desc.RenderTarget[0].BlendEnable = true; //�ܰ����� �������� ��Ƽ�ٸ���� ����

		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; //��������
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; //������� ���Ļ����� �������ΰ�
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; //�ΰ��� �ȼ����� ���Ѵ�.

		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE; //��濡 �ִ� �Ȼ�����
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; 
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;//Ư�� ��Ʈ���� ������ ������ ����, ������� �̾Ƴ��� ����
	
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
	D3DXMATRIX P; //�ӽ� �θ����
	D3DXMatrixRotationZ(&P, radian);
	//world = world * P;

	D3DXMatrixTranspose(&cpu_buffer.world,&world); //��ġ��ĺ�ȯ �Լ�
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
		graphics->GetDeviceContext()->VSSetConstantBuffers(0, 1, &gpu_buffer);

		//RS
		graphics->GetDeviceContext()->RSSetState(rasterizer_state);

		//PS
		graphics->GetDeviceContext()->PSSetShader(static_cast<ID3D11PixelShader*>(pixel_shader->GetResource()), nullptr, 0);
		graphics->GetDeviceContext()->PSSetShaderResources(0, 1, &shader_resource);
		graphics->GetDeviceContext()->PSSetSamplers(0, 1, &sampler_state);

		//OM
		graphics->GetDeviceContext()->OMSetBlendState(blend_state, nullptr, 0xffffffff);

		graphics->GetDeviceContext()->DrawIndexed(6, 0, 0);
	}
	graphics->End();
}
