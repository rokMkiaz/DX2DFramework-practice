#pragma once
class Converter
{
public:
	Converter();
	~Converter();

	void ReadFile(wstring file);
public:
	void ExportMesh(wstring savePath);

private: //��ͻ��
	void ReadBoneData(aiNode* node, int index, int parent);
	void ReadMeshData(aiNode* node, int bone);  //Bone�� Mesh�� ������ �۾�
	void WriteMeshData(wstring savePath); //bone,mesh������ ���Ϸ� ���


private:
	wstring file;

	Assimp::Importer* importer;
	const aiScene* scene; //3D���� ��系��

	vector<struct asBone* > bones;
	vector<struct asMesh* > meshes;
};