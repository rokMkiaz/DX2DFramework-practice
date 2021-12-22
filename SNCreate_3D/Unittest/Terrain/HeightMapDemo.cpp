#include "stdafx.h"
#include "HeightMapDemo.h"

void HeightMapDemo::Initialize()
{

	Context::Get()->GetCamera()->RotationDegree(20, 69, 0);
	Context::Get()->GetCamera()->Position(-34, 29, 6.0f);

	shader = new Shader(L"19_Terrain.fx");


	terrain = new Terrain(shader, L"Terrain/Gray256.png");
	terrain->Pass(1);
	
}

void HeightMapDemo::Destroy()
{
	SafeDelete(shader);

	SafeDelete(terrain);

}

void HeightMapDemo::Update()
{
	terrain->Update();
}


void HeightMapDemo::Render()
{
	terrain->Render();
}


