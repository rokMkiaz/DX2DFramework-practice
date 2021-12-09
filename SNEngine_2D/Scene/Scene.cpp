#pragma once
#include "stdafx.h"
#include"Scene.h"


#include"Scene\Actor.h"
#include"Scene\Component\TransformComponent.h"
#include"Scene\Component\CameraComponent.h"
#include"Scene\Component\MeshRendererComponent.h"
#include "Scene\Component\MoveScriptComponent.h"
#include"Scene\Component\AIScriptComponent.h"

Scene::Scene(Context* const context )
	:context(context)
{
	renderer = context->GetSubsystem<Renderer>();

	auto camera = CreateActor();
	camera->SetName("MainCamera");
	camera->AddComponent<CameraComponent>();

	auto player = CreateActor();
	player->SetName("Player");
	player->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3{ 100.0f, 100.0f, 1.0f });
	player->GetComponent<TransformComponent>()->SetPosition(D3DXVECTOR3{ +100.0f, 0.0f, 0.0f });
	player->AddComponent<MeshRendererComponent>();
	player->AddComponent<MoveScriptComponent>();

	auto monster = CreateActor();
	monster->SetName("Monster");
	monster->GetComponent<TransformComponent>()->SetScale(D3DXVECTOR3{ 100.0f, 100.0f, 1.0f });
	monster->GetComponent<TransformComponent>()->SetPosition(D3DXVECTOR3{ -100.0f, 0.0f, 0.0f });
	monster->AddComponent<MeshRendererComponent>();
	monster->AddComponent<AIScriptComponent>();
}

Scene::~Scene()
{
	actors.clear();
	actors.shrink_to_fit();
	
}

void Scene::Update()
{
	for (const auto& actor : actors)
		actor->Update();

	if (is_update)
	{
		renderer->UpdateRenderables(this);
		is_update = false;
	}
}

auto Scene::CreateActor(const bool& is_active) -> const std::shared_ptr<class Actor>
{
	auto actor = std::make_shared<Actor>(context);
	actor->SetActive(is_active);
	AddActor(actor);

	return actor;
}

void Scene::AddActor(const std::shared_ptr<class Actor>& actor)
{
	actors.emplace_back(actor);
}
