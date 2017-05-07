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

public:
	Sphere(const cv::Vec3f& center, float radius, const char *id, bool light = false) :
			Primitive(id, light), center(center), radius(radius),
			sqRadius(radius * radius) {}

	const cv::Vec3f &GetCenter() const { return center; }

	Primitive::Type GetType() override { return SPHERE; }

	HitState Intersect(const Ray& ray, float& dist) override;

	cv::Vec3f GetNormal(cv::Vec3f point) override { return cv::normalize(point - center); }
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

	cv::Vec3f GetNormal(cv::Vec3f point) override { return cv::normalize(normal); }
};

}   //  namespace raytracer

#endif //RAYTRACING_COMPONENTS_H
