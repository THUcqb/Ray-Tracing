//
// Created by 陈齐斌 on 04/05/2017.
//

#include "components.h"
#include "../raytracer.h"

namespace raytracer
{

HitState Sphere::Intersect(const Ray &ray, float &dist)
{
	cv::Vec3f l = center - ray.GetOrigin();

	float tp = l.dot(ray.GetDirection());

	float t2 = sqRadius - (l.dot(l) - tp * tp);

	if (t2 > 0)
	{
		t2 = cv::sqrt(t2);
		if (l.dot(l) < sqRadius)
		{
			if (tp + t2 < dist)
			{
				dist = tp + t2;
				return INPRIM;
			}
		}
		else
		{
			if (tp > 0)
			{
				if (tp - t2< dist)
				{
					dist = tp - t2;
					return HIT;
				}
			}
		}
	}
	return MISS;
}

HitState Plane::Intersect(const Ray &ray, float &dist)
{
	float dotproduct = normal.dot(ray.GetDirection());
	if (dotproduct != 0)
	{
		float tdist = -(normal.dot(ray.GetOrigin()) + D) / dotproduct;
		if (tdist > 0)
		{
			if (tdist < dist)
			{
				dist = tdist;
				return HIT;
			}
		}
	}
	return MISS;
}


}
