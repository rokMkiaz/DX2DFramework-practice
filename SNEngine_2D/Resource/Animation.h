#pragma once

enum class RepeatType : uint
{
	Once, Loop,
};

struct Keyframe final
{
	Keyframe() = default;
	Keyframe(const D3DXVECTOR2& offset, const D3DXVECTOR2& size, const double& time)
		: offset(offset)
		, size(size)
		, time(time)
	{}

	D3DXVECTOR2 offset{ 0.0f, 0.0f };
	D3DXVECTOR2 size{ 0.0f, 0.0f };
	double time{ 0.0 };
};

class Animation final
{
public:
	Animation(class Context* const context);
	~Animation();

	bool SaveToFile(const std::string& path);
	bool LoadFromFile(const std::string& path);

	auto GetAnimationName() const ->const std::string& { return animation_name; }
	void SetAnimationName(const std::string& name) { this->animation_name = name; }

	auto GetRepeatType() const -> const RepeatType& { return repeat_type; }
	void SetRepeatType(const RepeatType& repeat_type) { this->repeat_type = repeat_type; }

	auto GetSpriteTextureSize() const -> const D3DXVECTOR2& { return sprite_texture_size; }
	void SetSpriteTextureSize(const D3DXVECTOR2& size) { this->sprite_texture_size = size; }

	auto GetSpriteTexture() const -> const std::shared_ptr<class D3D11_Texture>& { return sprite_texture; }
	void SetSpriteTexture(const std::shared_ptr<class D3D11_Texture>& sprite_texture) { this->sprite_texture = sprite_texture; }
	void SetSpriteTexture(const std::string& path);

	auto GetKeyframes() const -> const std::vector<Keyframe>& { return keyframes; }
	void SetKeyframes(const std::vector<Keyframe>& keyframes) { this->keyframes = keyframes; }

	auto GetKeyframeFromIndex(const uint& index) -> const Keyframe* const;
	auto GetKeyframeCount() const -> const uint { return keyframes.size(); }

	void AddKeyframe(const Keyframe& keyframe);
	void AddKeyframe(const D3DXVECTOR2& offset, const D3DXVECTOR2& size, const double& time);

private:
	class Context* context = nullptr;
	RepeatType repeat_type = RepeatType::Loop;
	D3DXVECTOR2 sprite_texture_size = D3DXVECTOR2(1.0f, 1.0f);

	std::string animation_name = "";
	std::shared_ptr<class D3D11_Texture> sprite_texture;
	std::vector<Keyframe> keyframes;
};