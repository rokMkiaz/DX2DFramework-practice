#include"stdafx.h"
#include"Rect.h"

Rect::Rect(Graphics* graphics)
{
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
	sampler_state = new D3D11_SamplerState(graphics);
	sampler_state->Create(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP);

	//Create Blend State
	blend_state = new D3D11_BlendState(graphics);
	blend_state->Create(true);

	//create world 
	{
		D3DXMatrixIdentity(&world);

		//world
		D3DXMATRIX world_Scale;
		D3DXMatrixScaling(&world_Scale, scale.x, scale.y, scale.z);
		
		//D3DXMATRIX world_Rotation;
		//D3DXMatrixRotationZ(&world_Rotation, static_cast<float>(D3DXToRadian(0))); //2차원이므로 z축만 회전시켜준다
		
		D3DXMATRIX world_Translation;
		D3DXMatrixTranslation(&world_Translation, position.x, position.y, position.z);

		//Scale * Rotation * Translation* (Rotation(idle) * Parents)
		world = world_Scale /* world_Rotation*/ * world_Translation;

	}



}

Rect::~Rect()
{
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
}

void Rect::Update()
{
	D3DXMATRIX world_Scale;
	D3DXMatrixScaling(&world_Scale, scale.x, scale.y, scale.z);

	D3DXMATRIX world_Translation;
	D3DXMatrixTranslation(&world_Translation, position.x, position.y, position.z);

	world = world_Scale  * world_Translation;



	auto buffer = gpu_buffer->Map<TRANSFORM_DATA>();
	{
		D3DXMatrixTranspose(&buffer->world, &world); //전치행렬변환 함수
	}
	gpu_buffer->Unmap();
}

void Rect::Render(D3D11_Pipeline* pipeline)
{
	D3D11_PipelineState pipeline_state;
	pipeline_state.input_layout = input_layout;
	pipeline_state.primitive_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	pipeline_state.vertex_shader = vertex_shader;
	pipeline_state.pixel_shader = pixel_shader;
	pipeline_state.rasterizer_state = rasterizer_state;
	pipeline_state.blend_state = blend_state;

	if (pipeline->Begin(pipeline_state))
	{
		pipeline->SetVertexBuffer(vertex_buffer);
		pipeline->SetIndexBuffer(index_buffer);
		pipeline->SetConstantBuffer(1, ShaderScope_VS, gpu_buffer);
		pipeline->SetShaderResource(0, ShaderScope_PS, texture);
		pipeline->SetSamplerState(0, ShaderScope_PS, sampler_state);
		pipeline->DrawIndexed(index_buffer->GetCount(), index_buffer->GetOffset(), vertex_buffer->GetOffset());

		pipeline->End();
	}
}
