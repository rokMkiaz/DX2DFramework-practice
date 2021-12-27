#pragma once
#include "stdafx.h"

struct asBone  //부모 자식관계 저장
{
	int Index; //Parent index가 -1이면 루트
	string Name;

	int Parent;
	Matrix Transform;
};

struct asMesh
{
	string Name;
	int BoneIndex;

	aiMesh* Mesh;

	string MaterialName;

	vector<Model::ModelVertex> Vertices;
	vector<UINT> Indices;
};
