#pragma once
#include"Rect.h"

class Player final : public Rect
{
public:
	Player(class Graphics* graphics,const D3DXCOLOR & color);
	~Player();

protected:
	void Move() override;

private:
};