#pragma once
#include"IComponent.h"



class TransformComponent final : public IComponent
{
public:
	TransformComponent(class Context* const context, class Actor* const actor, class TransformComponent* const transform);
	~TransformComponent();


	void Initialize() override;	
	void Update() override;	
	void Destroy() override;

	//Local
	auto GetLocalScale() const -> const D3DXVECTOR3& { return local_scale; }
	void SetLocalScale(const D3DXVECTOR3& scale);

	auto GetLocalPosition() const -> const D3DXVECTOR3& { return  local_position; }
	void SetLocalPosition(const D3DXVECTOR3&position);

	auto GetLocalRotation() const -> const D3DXVECTOR3& { return local_rotation; }
	void SetLocalRotation(const D3DXVECTOR3& rotation);

	auto GetLocalMatrix()const -> const D3DXMATRIX& { return local; }


	//World
	auto GetScale()-> const D3DXVECTOR3;
	void SetScale(const D3DXVECTOR3& world_scale);

	auto GetPosition()-> const D3DXVECTOR3;
	void SetPosition(const D3DXVECTOR3& world_position);

	auto GetRotation()-> const D3DXVECTOR3;
	void SetRotation(const D3DXVECTOR3& world_rotation);

	auto GetWorldRotationMatrix() const -> const D3DXMATRIX;
	auto GetWorldMatrix()const -> const D3DXMATRIX& { return world; }

	//방향백터 추출
	auto GetRight() const -> const D3DXVECTOR3;
	auto GetUp() const -> const D3DXVECTOR3;
	auto GetForward() const -> const D3DXVECTOR3;

	//Transform
	bool HasParent() const { return parent ? true : false; }
	bool HasChilds() const { return !childs.empty(); }

	auto GetRoot() -> TransformComponent* const{ return HasParent() ? parent->GetRoot() : this; } //최상위 객체 잦는 함수

	auto GetParent() -> TransformComponent* const { return parent; }

	void SetParent(TransformComponent* const new_parent);

	auto GetChilds() const -> const std::vector<TransformComponent*>& { return childs; }
	auto GetChildFromIndex(const uint& index)->TransformComponent* const;
	auto GetChildCount() const { return static_cast<uint>(childs.size()); }
	void AddChild(TransformComponent* const child);

	void UpdateTransform();



private:
	D3DXVECTOR3 local_scale    = D3DXVECTOR3(1, 1, 1);
	D3DXVECTOR3 local_position = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 local_rotation = D3DXVECTOR3(0, 0, 0);

	D3DXMATRIX local;
	D3DXMATRIX world;

	TransformComponent* parent = nullptr;
	std::vector<TransformComponent*>childs;


};
