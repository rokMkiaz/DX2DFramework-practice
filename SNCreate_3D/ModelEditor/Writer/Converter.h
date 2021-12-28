#pragma once
class Converter
{
public:
	Converter();
	~Converter();

	void ReadFile(wstring file);
public:
	void ExportMesh(wstring savePath);

private: //재귀사용
	void ReadBoneData(aiNode* node, int index, int parent);
	void ReadMeshData(aiNode* node, int bone);  //Bone에 Mesh를 입히는 작업
	void WriteMeshData(wstring savePath); //bone,mesh정보를 파일로 출력


private:
	wstring file;

	Assimp::Importer* importer;
	const aiScene* scene; //3D관련 모든내용

	vector<struct asBone* > bones;
	vector<struct asMesh* > meshes;
};