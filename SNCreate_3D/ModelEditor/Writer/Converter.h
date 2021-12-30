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
	void ReadSkinData(); //Skinning�� ���� ���������� ����
	void WriteMeshData(wstring savePath); //bone,mesh������ ���Ϸ� ���


/// ////////////////////////////////////////////////////////////////////////////

public:
	void ExportMaterial(wstring savePath , bool bOverwrite = true );

private:
	void ReadMaterialData();
	void WriteMaterialData(wstring savePath);
	string WriteTexture(string saveFolder, string file);


/// ///////////////////////////////////////////////////////////////////////////


private:
	wstring file;

	Assimp::Importer* importer;
	const aiScene* scene; //3D���� ��系��

	vector<struct asBone* > bones;
	vector<struct asMesh* > meshes;
	vector<struct asMaterial*> materials;
};