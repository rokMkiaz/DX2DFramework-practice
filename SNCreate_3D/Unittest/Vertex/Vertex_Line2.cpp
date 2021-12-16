#include"stdafx.h"
#include"Vertex_Line2.h"

void Vertex_Line2::Initialize()
{
	shader = new Shader{ L"01_Vertex.fx" };

	vertices[0].Position = Vector3{ 0.0f,0.0f,0.0f };
	vertices[1].Position = Vector3{ 1.0f,0.0f,0.0f };
	vertices[2].Position = Vector3{ 0.0f,0.5f,0.0f };
	vertices[3].Position = Vector3{ 1.0f,0.5f,0.0f };

	vertices[4].Position = Vector3{ 0.0f,0.9f,0.0f };
	vertices[5].Position = Vector3{ 1.0f,0.5f,0.0f };

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = sizeof(Vertex) * 6;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subre_resource = { 0 };
	subre_resource.pSysMem = vertices;

	Check(D3D::GetDevice()->CreateBuffer(&desc,&subre_resource,&vertex_buffer))
}


void Vertex_Line2::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertex_buffer);
}

void Vertex_Line2::Update()
{
}

void Vertex_Line2::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	shader->Draw(1, 0, 6);
}

