#pragma once
#include"Systems\IExecute.h"
class Vertex_Line : public IExecute
{
public:
	void Initialize() override;
	void Ready() override {};
	void Destroy() override;
	void Update() override;
	void PreRender() override {};
	void Render() override;
	void PostRender() override {};
	void ResizeScreen() override {};

private:
	Shader* shader;

	Vertex vertices[2];
	ID3D11Buffer* vertex_buffer;
};