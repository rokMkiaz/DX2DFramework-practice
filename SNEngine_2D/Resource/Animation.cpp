#include "stdafx.h"
#include "Animation.h"

Animation::Animation(Context* const context)
	: context(context)
{
}

Animation::~Animation()
{
	keyframes.clear();
	keyframes.shrink_to_fit();
}

bool Animation::SaveToFile(const std::string& path)
{
	Xml::XMLDocument doc;

	Xml::XMLDeclaration* decl = doc.NewDeclaration();
	doc.LinkEndChild(decl);

	Xml::XMLElement* root = doc.NewElement("Animation");
	doc.LinkEndChild(root);

	root->SetAttribute("Name", animation_name.c_str());
	root->SetAttribute("Type", static_cast<uint>(repeat_type));
	root->SetAttribute("TextureSizeX", sprite_texture_size.x);
	root->SetAttribute("TextureSizeY", sprite_texture_size.y);

	for (const auto& keyframe : keyframes)
	{
		Xml::XMLElement* first_element = doc.NewElement("Keyframe");
		root->LinkEndChild(first_element);

		first_element->SetAttribute("OffsetX", keyframe.offset.x);
		first_element->SetAttribute("OffsetY", keyframe.offset.y);
		first_element->SetAttribute("SizeX", keyframe.size.x);
		first_element->SetAttribute("SizeY", keyframe.size.y);
		first_element->SetAttribute("Time", keyframe.time);
	}

	return Xml::XMLError::XML_SUCCESS == doc.SaveFile(path.c_str());
}
bool Animation::LoadFromFile(const std::string& path)
{
	Xml::XMLDocument doc;
	Xml::XMLError error = doc.LoadFile(path.c_str());
	if (error != Xml::XMLError::XML_SUCCESS)
	{
		assert(false);
		return false;
	}

	Xml::XMLElement* root = doc.FirstChildElement();
	animation_name = root->Attribute("Name");
	repeat_type = static_cast<RepeatType>(root->UnsignedAttribute("Type"));
	sprite_texture_size.x = root->FloatAttribute("TextureSizeX");
	sprite_texture_size.y = root->FloatAttribute("TextureSizeY");

	Xml::XMLElement* first_element = root->FirstChildElement();
	for (; first_element != nullptr; first_element = first_element->NextSiblingElement())
	{
		D3DXVECTOR2 offset;
		offset.x = first_element->FloatAttribute("OffsetX");
		offset.x = first_element->FloatAttribute("OffsetY");

		D3DXVECTOR2 size;
		size.x = first_element->FloatAttribute("SizeX");
		size.x = first_element->FloatAttribute("SizeY");

		float time = first_element->FloatAttribute("Time");

		AddKeyframe(offset, size, time);
	}

	return true;
}

void Animation::SetSpriteTexture(const std::string& path)
{
	sprite_texture = std::make_shared<D3D11_Texture>(context->GetSubsystem<Graphics>());
	sprite_texture->Create(path);
}

auto Animation::GetKeyframeFromIndex(const uint& index) -> const Keyframe* const
{
	assert(index < keyframes.size());
	return &keyframes[index];
}

void Animation::AddKeyframe(const Keyframe& keyframe)
{
	keyframes.emplace_back(keyframe);
}

void Animation::AddKeyframe(const D3DXVECTOR2& offset, const D3DXVECTOR2& size, const double& time)
{
	keyframes.emplace_back(offset, size, time);
}
