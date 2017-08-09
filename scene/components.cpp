//
// Created by 陈齐斌 on 04/05/2017.
//

#include "../engine/engine.h"
#include "components.h"
#include "scene.h"

namespace raytracer
{
#define EPSILON -1e-5
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

float Sphere::GetPdf(const cv::Vec3f &luminairePoint, const cv::Vec3f surfacePoint)
{
	cv::Vec3f omega = cv::normalize(surfacePoint - luminairePoint);

	return (float) (cosf(omega.dot(GetNormal(luminairePoint))) / 2 / M_PI
	                / cv::norm(surfacePoint - luminairePoint, cv::NORM_L2SQR))
					/ (1 - sqrtf((float) (1 - sqRadius / cv::norm(surfacePoint - center, cv::NORM_L2SQR))));
}

cv::Vec3f Sphere::GetRandomPoint()
{
	return center + radius * cv::normalize(cv::Vec3f(rand() - RAND_MAX / 2, rand() - RAND_MAX / 2, rand() - RAND_MAX / 2));
}

HitState Triangle::Intersect(const Ray &ray, float &dist)
{
	float dot = normal.dot(ray.GetDirection());
	if (dot != 0)
	{
		float tdist = -(normal.dot(ray.GetOrigin()) + D) / dot;
		if (tdist > 0 && tdist < dist)
		{
			cv::Vec3f point = ray.GetOrigin() + tdist * ray.GetDirection();
			if ((b-a).cross(point-a).dot(normal) > EPSILON
			    && (point-a).cross(c-a).dot(normal) > EPSILON
			    && (point-b).cross(a-b).dot(normal) > EPSILON
				&& (c-b).cross(point-b).dot(normal) > EPSILON)
			{
				dist = tdist;
				return HIT;
			}
		}
	}
	return MISS;

}

float Triangle::GetPdf(const cv::Vec3f &luminairePoint, const cv::Vec3f surfacePoint)
{
	return 1 / area;
}

cv::Vec3f Triangle::GetRandomPoint()
{
	float x = (float)rand() / RAND_MAX, y = (float)rand() / RAND_MAX;
	if (x > y)
		std::swap(x, y);
	return a * x + b * (y - x) + c * (1 - y);
}

}
