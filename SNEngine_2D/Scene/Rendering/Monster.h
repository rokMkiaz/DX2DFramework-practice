#pragma once
#include"Rect.h"

class Monster final : public Rect
{
public: 
	Monster(class Graphics* graphics,const D3DXCOLOR& color);
	~Monster();

	void Event() final override;

protected:
	void Move() override;

private:

	Stopwatch stopwatch;
	Direction direction = Direction::Left;

};