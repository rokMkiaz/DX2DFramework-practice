#pragma once
#include"Rect.h"

class Player final : public Rect
{
public:
	Player(class Graphics* graphics,const D3DXCOLOR & color);
	~Player();

	void Event() override;

protected:
	void Move() override;

private:
};