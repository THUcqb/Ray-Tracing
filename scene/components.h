//
// Created by 陈齐斌 on 04/05/2017.
//

#ifndef RAYTRACING_COMPONENTS_H
#define RAYTRACING_COMPONENTS_H

#include <opencv2/opencv.hpp>
#include "scene.h"
namespace raytracer
{

class Ray;

class Sphere : public Primitive
{
private:
	cv::Vec3f center;
	float sqRadius, radius;
	int *randlist;
	int tt;
public:
	Sphere(const cv::Vec3f& center, float radius, const char *id, bool light = false) :
			Primitive(id, light), center(center), radius(radius),
			sqRadius(radius * radius) {
		randlist = new int[300];
		srand((unsigned int) time(0));
		for (int i = 0; i < 300; ++i)
			randlist[i] = rand() - RAND_MAX / 2;
		tt = -3;
	}

	const cv::Vec3f &GetCenter() const { return center; }

	Primitive::Type GetType() override { return SPHERE; }

	HitState Intersect(const Ray& ray, float& dist) override;

	cv::Vec3f GetNormal(const cv::Vec3f &point) override { return cv::normalize(point - center); }

	float GetReflectance(const cv::Vec3f &point, const cv::Vec3f &omega) override
	{
		return material.GetDiffusion();
	};

	float Getpdf(const cv::Vec3f &luminairePoint, const cv::Vec3f surfacePoint);

	cv::Vec3f GetRandomPoint();
};

class Plane : public Primitive
{
	float D;
	cv::Vec3f normal;

public:
	Plane(const cv::Vec3f &normal, float D, const char *id, bool light = false) :
			Primitive(id, light), normal(cv::normalize(normal)), D(D) {}

	Type GetType() override { return PLANE; }

	HitState Intersect(const Ray &ray, float &dist) override;

	cv::Vec3f GetNormal(const cv::Vec3f &point) override { return normal; }

	float GetReflectance(const cv::Vec3f &point, const cv::Vec3f &omega) override
	{
		return material.GetDiffusion();
	}
};

}   //  namespace raytracer

#endif //RAYTRACING_COMPONENTS_H
