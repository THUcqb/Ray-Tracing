//
// Created by 陈齐斌 on 04/05/2017.
//

#ifndef RAYTRACING_SCENE_H
#define RAYTRACING_SCENE_H

#include <opencv2/opencv.hpp>
#include "brdf/CookTorranceBRDF.h"

namespace raytracer
{
#define AMBIENT_RADIANCE Radiance(10, 10, 10)
#define LIGHT_RADIANCE Radiance(15000, 15000, 15000)
#define DEFAULT_RADIANCE Radiance(0, 0, 0)

typedef cv::Scalar_<int > Radiance;

class Ray;

enum HitState
{
	HIT,
	MISS,
	INPRIM
};

class Primitive
{
private:
	const char* id;
	Radiance color;
	BRDF *brdf;
	bool isLuminaire;
	float reflection;
public:
	enum Type
	{
		PLANE,
		SPHERE,
		TRIANGLE
	};

	Primitive(const char *id, const Radiance &color, BRDF *brdf = CookTorranceBRDF::CreateCookTorranceBRDF(CookTorranceBRDF::DIFFUSE), bool isLuminaire = false, float reflection = 0);

	virtual ~Primitive();

//	Getter
	const char *GetId() const { return id; }

	const Radiance &GetColor() { return color; }
	const Radiance &GetRadiance() { return color; }

	BRDF *GetBRDF() const { return brdf; }

	bool IsLuminaire() const { return isLuminaire; }

	float GetReflection() const { return reflection; }

//	Interface
	virtual Type GetType() = 0;

	virtual cv::Vec3f GetNormal(const cv::Vec3f &point) = 0;

	virtual HitState Intersect(const Ray& ray, float& dist) = 0;

	//	Luminaire
	virtual float GetPdf(const cv::Vec3f &luminairePoint, const cv::Vec3f surfacePoint) = 0;

	virtual cv::Vec3f GetRandomPoint() = 0;

};

class Scene
{
	Primitive **primitives;
	int numPrimitives;

public:
	Scene();

	virtual ~Scene();

	//  Getter
	int GetNumPrimitives() const { return numPrimitives; }

	Primitive *GetPrimitives(int index) const
	{
		if (index < numPrimitives)
			return primitives[index];
		return nullptr;
	}

	void InitScene();
};

}

#endif //RAYTRACING_SCENE_H
