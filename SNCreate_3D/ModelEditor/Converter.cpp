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
		aiProcess_ConvertToLeftHanded/*오른손좌표계-> 왼손좌표계*/
		| aiProcess_Triangulate/*삼각형으로 모델을 달라*/
		| aiProcess_GenUVCoords /*UV좌표계 변환*/
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
	//본 정보 읽기
	asBone* bone = new asBone();
	bone->Index = index;
	bone->Parent = parent;
	bone->Name = node->mName.C_Str();//컨스트체 리턴

	Matrix transform(node->mTransformation[0]); //메트릭스 시작주소 받음
	D3DXMatrixTranspose(&bone->Transform, &transform); //전치

	Matrix matParent;
	if (parent < 0)
		D3DXMatrixIdentity(&matParent);
	else
		matParent = bones[parent]->Transform;

	bone->Transform = bone->Transform * matParent; //부모만큼 이동시키기위해 곱해준다.
	bones.push_back(bone);


	//TODO : 매시정보 읽기

	for (UINT i = 0; i < node->mNumChildren; i++)
		ReadBoneData(node->mChildren[i],bones.size() , index);
}

void Converter::ReadMeshData(aiNode* node, int bone)
{
}

void Converter::WritemeshData(wstring savePath)
{
}
