#pragma once
#include "stdafx.h"

struct asBone  //�θ� �ڽİ��� ����
{
	int Index; //Parent index�� -1�̸� ��Ʈ
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
