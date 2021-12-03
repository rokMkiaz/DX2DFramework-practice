#include"stdafx.h"
#include"Monster.h"

Monster::Monster(Graphics* graphics,const D3DXCOLOR& color) : Rect(graphics,color)
{
}

Monster::~Monster()
{
}

void Monster::Move()
{
	static Direction direction = Direction::Right;

	static std::random_device rd;
	static std::mt19937 mt(rd());   //메르센 소스를 이용한 난수생성기
	static std::uniform_int_distribution<int> rand(0, 3); //정규화된 범위

	static auto cur_time = std::chrono::system_clock::now();
	static auto check_time = std::chrono::system_clock::now();

	cur_time = std::chrono::system_clock::now();
	std::chrono::duration<double, std::milli> work_time = cur_time - check_time;


	switch (direction)
	{
	case Direction::Up:    position.y++; break;
	case Direction::Right: position.x++; break;
	case Direction::Down:  position.y--; break;
	case Direction::Left:  position.x--; break;
	}

	if (work_time.count() >= 1000.0)
	{
		auto new_dir = static_cast<Direction>(rand(mt));

		if (direction == new_dir)
		{
			int dir_num = static_cast<int>(new_dir);
			dir_num = (dir_num <= 1) ? dir_num + 2 : dir_num - 2;
			direction = static_cast<Direction>(dir_num);
		}
		else direction = new_dir;


		check_time = std::chrono::system_clock::now();
	}
}
