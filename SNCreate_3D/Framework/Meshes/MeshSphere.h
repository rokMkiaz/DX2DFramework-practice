#pragma once

class MeshSphere : public Mesh
{
public:
	MeshSphere(Shader* shader, float radius, UINT stackCount = 20, UINT sliceCount = 20);
	~MeshSphere();

private:
	void Create() override;

	float radius;

	UINT stackCount;
	UINT sliceCount; //구의 정밀도
};