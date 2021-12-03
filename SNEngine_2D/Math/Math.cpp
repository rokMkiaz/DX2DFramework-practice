#include "stdafx.h"
#include "Math.h"

static std::random_device rd;
static std::mt19937 mt(rd());

auto Math::Random(const int& min, const int& max) -> const int
{
	return std::uniform_int_distribution<int>{min, max}(mt);
}

auto Math::Random(const float& min, const float& max) -> const float
{
	return std::uniform_real_distribution<float>{min, max}(mt);
}
