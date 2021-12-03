#include"stdafx.h"
#include"Intersect.h"
#include"Scene\Rendering\Rect.h"

bool Intersect::IsIntersect(Rect* const lhs, Rect* const rhs)
{
	if (!lhs->IsActive() || !rhs->IsActive())
		return false;

	auto lhs_min = lhs->GetPosition() - (lhs->GetScale() * 0.5f);
	auto lhs_max = lhs->GetPosition() + (lhs->GetScale() * 0.5f);

	auto rhs_min = rhs->GetPosition() - (rhs->GetScale() * 0.5f);
	auto rhs_max = rhs->GetPosition() + (rhs->GetScale() * 0.5f);

	if (
		lhs_max.x < rhs_min.x || lhs_min.x > rhs_max.x ||
		lhs_max.y < rhs_min.y || lhs_min.y > rhs_max.y)
		return false;
	else if (
		lhs_min.x < rhs_min.x || lhs_max.x > rhs_max.x ||
		lhs_min.y < rhs_min.y || lhs_max.y > rhs_max.y)
		return true;
	else
		return true;


	return false;
}
