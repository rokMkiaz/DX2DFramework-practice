#include"stdafx.h"
#include"Monster.h"

Monster::Monster(Graphics* graphics,const D3DXCOLOR& color) : Rect(graphics,color)
{
}

Monster::~Monster()
{
}

void Monster::Event()
{
	is_active = false;
}

void Monster::Move()
{

	if (is_active)
	{
		switch (direction)
		{
		case Direction::Up:    position.y++; break;
		case Direction::Right: position.x++; break;
		case Direction::Down:  position.y--; break;
		case Direction::Left:  position.x--; break;
		}



		if (stopwatch.GetElapsedTimeSec() >= 1.0f)
		{
			auto new_dir = static_cast<Direction>(Math::Random(0, 3));

			if (direction == new_dir)
			{
				int dir_num = static_cast<int>(new_dir);
				dir_num = (dir_num <= 1) ? dir_num + 2 : dir_num - 2;
				direction = static_cast<Direction>(dir_num);
			}
			else direction = new_dir;


			stopwatch.Start();
		}
	}
}
