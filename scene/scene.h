//
// Created by 陈齐斌 on 04/05/2017.
//

#ifndef RAYTRACING_SCENE_H
#define RAYTRACING_SCENE_H

#include <opencv2/opencv.hpp>

namespace raytracer
{
#define AMBIENT_COLOR cv::Scalar(0.04, 0.04, 0.04)
#define LIGHT_COLOR cv::Scalar(1, 1, 1)
#define DEFAULT_COLOR cv::Scalar(0, 0, 0)

class Ray;

enum HitState
{
	HIT,
	MISS,
	INPRIM
};

class Material
{
#define DEFAULT_REFLECTION 0
#define DEFAULT_DIFFUSION 0.2
#define DEFAULT_REFRACTIVE_INDEX 1.2
#define DEFAULT_ROUGHNESS 1.2

private:
	cv::Scalar color;
	float reflection, diffusion, n, roughness;
public:
	Material(const cv::Scalar &color = DEFAULT_COLOR, float reflection = DEFAULT_REFLECTION, float diffusion = DEFAULT_DIFFUSION, float n = DEFAULT_REFRACTIVE_INDEX, float roughness = DEFAULT_ROUGHNESS) :
			color(color), reflection(reflection), diffusion(diffusion), n(n), roughness(roughness) {}

//	Getter
	const cv::Scalar &GetColor() const { return color; }

	const float &GetReflection() const { return reflection; }

	const float &GetDiffusion() const { return diffusion; }

	float GetSpecular() { return 1.0f - diffusion; }

	float GetN() const { return n; }

	float GetRoughness() const { return roughness; }

//	Setter
	void SetColor(const cv::Scalar &color) { Material::color = color; }

	void SetReflection(float reflection) { Material::reflection = reflection; }

	void SetDiffusion(float diffusion) { Material::diffusion = diffusion; }

	void SetN(float n) { Material::n = n; }

	void SetRoughness(float roughness) { Material::roughness = roughness; }
};

class Primitive
{
private:
	Material material;
	const char* id;
	bool isLight;

public:
	enum Type
	{
		PLANE,
		SPHERE
	};

	Primitive(const char *id, bool isLight = false);

	virtual ~Primitive();

//	Getter
	Material *GetMaterial() { return &material; }

	const char *GetId() const { return id; }

	bool IsLight() const { return isLight; }

//	Setter
	void SetMaterial(const Material &material) { Primitive::material = material; }

	void SetId(const char *id);

//	virtual void SetLight(bool light) { Primitive::light = light; }

//	Interface
	virtual Type GetType() = 0;

	virtual HitState Intersect(const Ray& ray, float& dist) = 0;

	virtual cv::Vec3f GetNormal(cv::Vec3f point) = 0;

	virtual const cv::Scalar &GetColor() { return material.GetColor(); }
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
