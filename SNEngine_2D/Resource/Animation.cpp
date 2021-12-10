#include"stdafx.h"
#include"Animation.h"

Animation::Animation(Context* const context)
	:context(context)
{
}

Animation::~Animation()
{
	keyframes.clear();
	keyframes.shrink_to_fit();
}

void Animation::SetSpriteTexture(const std::string& path)
{
	sprite_texture = std::make_shared<D3D11_Texture>(context->GetSubsystem<Graphics>());
	sprite_texture->Create(path);
}

auto Animation::GetKeyFrameFromIndex(const uint& index) -> const Keyframe* const
{
	assert(index < keyframes.size());

	return &keyframes[index];
}

void Animation::AddKeyFrame(const Keyframe& keyframe)
{
	keyframes.emplace_back(keyframe);
}

void Animation::AddKeyFrame(const D3DXVECTOR2& offset, const D3DXVECTOR2& size, const double& time)
{
	keyframes.emplace_back(offset, size, time);
}
