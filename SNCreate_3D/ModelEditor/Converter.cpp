#include"stdafx.h"
#include"Converter.h"
#include"Types.h"

Converter::Converter()
{
	importer = new Assimp::Importer();
}

Converter::~Converter()
{
	SafeDelete(importer);
}

void Converter::ReadFile()
{
	this->file = L"../../_Assets/" + file;

	scene = importer->ReadFile
	(
		String::ToString(this->file),
		aiProcess_ConvertToLeftHanded/*��������ǥ��-> �޼���ǥ��*/
		| aiProcess_Triangulate/*�ﰢ������ ���� �޶�*/
		| aiProcess_GenUVCoords /*UV��ǥ�� ��ȯ*/
		| aiProcess_GenNormals
		| aiProcess_CalcTangentSpace
	);
	assert(scene != NULL);
}

void Converter::ExportMesh(wstring savePath)
{
	savePath = L"../../_Models/" + savePath + L".mesh";

	ReadBoneData(scene->mRootNode, -1, -1);
}

void Converter::ReadBoneData(aiNode* node, int index, int parent)
{
	//�� ���� �б�
	asBone* bone = new asBone();
	bone->Index = index;
	bone->Parent = parent;
	bone->Name = node->mName.C_Str();//����Ʈü ����

	Matrix transform(node->mTransformation[0]); //��Ʈ���� �����ּ� ����
	D3DXMatrixTranspose(&bone->Transform, &transform); //��ġ

	Matrix matParent;
	if (parent < 0)
		D3DXMatrixIdentity(&matParent);
	else
		matParent = bones[parent]->Transform;

	bone->Transform = bone->Transform * matParent; //�θ�ŭ �̵���Ű������ �����ش�.
	bones.push_back(bone);


	//TODO : �Ž����� �б�

	for (UINT i = 0; i < node->mNumChildren; i++)
		ReadBoneData(node->mChildren[i],bones.size() , index);
}

void Converter::ReadMeshData(aiNode* node, int bone)
{
}

void Converter::WritemeshData(wstring savePath)
{
}
