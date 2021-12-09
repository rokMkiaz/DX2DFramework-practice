#pragma once 
#include"ISubsystem.h"

class SceneManager final : public ISubsystem
{
public:
	SceneManager(class Context* context);
	~SceneManager();

	bool Initialize() override;
	void Update() override;

	auto GetAllScenes() const -> const std::map<std::string, std::shared_ptr<class Scene>>& { return scenes; }

	auto GetCurrentScene() -> class Scene* const;
	void SetCurrentScene(const std::string& scene_name);

	auto RegisterScene(const std::string& scene_name) -> class Scene* const;



private:
	std::weak_ptr<class Scene> current_scene;
	std::map<std::string, std::shared_ptr<class Scene>> scenes;
};