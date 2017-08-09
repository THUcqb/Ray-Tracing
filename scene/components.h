//
// Created by 陈齐斌 on 04/05/2017.
//

#ifndef RAYTRACING_COMPONENTS_H
#define RAYTRACING_COMPONENTS_H

#include <opencv2/opencv.hpp>
#include "../engine/engine.h"
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
	Sphere(const char *id, const Radiance &color,
	       const cv::Vec3f& center, float radius,
	       BRDF *brdf = CookTorranceBRDF::CreateCookTorranceBRDF(CookTorranceBRDF::DIFFUSE),
	       bool isLuminaire = false, float reflection = 0)
	: Primitive(id, color, brdf, isLuminaire, reflection), center(center), radius(radius), sqRadius(radius * radius)
	{
		srand((unsigned int) time(0));
	}

//	Override
	Primitive::Type GetType() override { return SPHERE; }

	cv::Vec3f GetNormal(const cv::Vec3f &point) override { return cv::normalize(point - center); }

	HitState Intersect(const Ray& ray, float& dist) override;

//	Luminaire
	float GetPdf(const cv::Vec3f &luminairePoint, const cv::Vec3f surfacePoint) override ;

	cv::Vec3f GetRandomPoint() override ;

};

class Triangle : public Primitive
{
	cv::Vec3f a, b, c;
	cv::Vec3f normal;
	float area, D;
public:
	Triangle(const char *id, const Radiance &color,
	         const cv::Vec3f &a, const cv::Vec3f &b, const cv::Vec3f &c,
	         BRDF *brdf = CookTorranceBRDF::CreateCookTorranceBRDF(CookTorranceBRDF::DIFFUSE), bool isLuminaire = false) : Primitive(id, color, brdf, isLuminaire), a(a), b(b), c(c)
	{
		normal = (b-a).cross(c-a);
		area = (float) cv::norm(normal);
		normal /= area;
		area /= 2;
		D = -normal.dot(a);
		srand((unsigned int) time(0));
	}

	Type GetType() override { return TRIANGLE; }

	cv::Vec3f GetNormal(const cv::Vec3f &point) override { return normal; }

	HitState Intersect(const Ray &ray, float &dist) override;
	//	Luminaire
	float GetPdf(const cv::Vec3f &luminairePoint, const cv::Vec3f surfacePoint) override ;

	cv::Vec3f GetRandomPoint() override ;

};
}   //  namespace raytracer

#endif //RAYTRACING_COMPONENTS_H
