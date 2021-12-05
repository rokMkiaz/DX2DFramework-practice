#pragma once
#include "stdafx.h"
#include"Execute.h"
#include"Graphics.h"

#include"Scene\Actor.h"
#include"Scene\Component\TransformComponent.h"
#include"Scene\Component\CameraComponent.h"
#include"Scene\Component\MeshRendererComponent.h"

Execute::Execute()
{

	Graphics::Get().Initialize();
	Graphics::Get().CreateBackBuffer(static_cast<uint>(Settings::Get().GetWidth()),static_cast<uint>(Settings::Get().GetHeight()));


	camera_buffer = new D3D11_ConstantBuffer(graphics);
	camera_buffer->Create<CAMERA_DATA>();

	//Pipe line
	pipeline = new D3D11_Pipeline(graphics);



}

Execute::~Execute()
{
	SAFE_DELETE(pipeline);
	SAFE_DELETE(camera_buffer);

	
}

void Execute::Update()
{
	for (const auto& actor : actors)
		actor->Update();


	auto buffer = camera_buffer->Map<CAMERA_DATA>();
	{
		D3DXMatrixTranspose(&buffer->view, &camera->GetViewMatrix());
		D3DXMatrixTranspose(&buffer->projection, &camera->GetProjectionMatrix());
	}
	camera_buffer->Unmap();

}

void Execute::Render()
{

	Graphics::Get().Begin();
	{
		pipeline->SetConstantBuffer(0, ShaderScope_VS, camera_buffer);
		
		for (const auto& actor : actors)
			actor->Render();
	
		
	}
	Graphics::Get().End();
}
