//
// Created by 陈齐斌 on 04/05/2017.
//

#include "../raytracer.h"
#include "components.h"
#include "scene.h"

namespace raytracer
{
#define EPSILON 1e-4

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
			if (tp + t2 < dist && tp + t2 > EPSILON)
			{
				dist = tp + t2;
				return INPRIM;
			}
		}
		else
		{
			if (tp > 0)
			{
				if (tp - t2 < dist && tp - t2 > EPSILON)
				{
					dist = tp - t2;
					return HIT;
				}
			}
		}
	}
	return MISS;
}

float Sphere::Getpdf(const cv::Vec3f &luminairePoint, const cv::Vec3f surfacePoint)
{
	cv::Vec3f omega = cv::normalize(surfacePoint - luminairePoint);

	return (float) (cosf(omega.dot(GetNormal(luminairePoint))) / 2 / M_PI
	                / cv::norm(surfacePoint - luminairePoint, cv::NORM_L2SQR))
					/ (1 - sqrtf((float) (1 - sqRadius / cv::norm(surfacePoint - center, cv::NORM_L2SQR))));
}

cv::Vec3f Sphere::GetRandomPoint()
{
	tt = (tt + 3) % 300;
	return center + radius * cv::normalize(cv::Vec3f(randlist[tt], randlist[tt+1], randlist[tt+2]));
}

HitState Plane::Intersect(const Ray &ray, float &dist)
{
	float dotproduct = normal.dot(ray.GetDirection());
	if (dotproduct != 0)
	{
		float tdist = -(normal.dot(ray.GetOrigin()) + D) / dotproduct;
		if (tdist > EPSILON)
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
