//
// Created by 陈齐斌 on 04/05/2017.
//

#ifndef RAYTRACING_SCENE_H
#define RAYTRACING_SCENE_H

#include <opencv2/opencv.hpp>

namespace raytracer
{
#define AMBIENT_RADIANCE Radiance(10, 10, 10)
#define LIGHT_RADIANCE Radiance(50000, 50000, 50000)
#define DEFAULT_RADIANCE Radiance(0, 0, 0)

typedef cv::Scalar_<int > Radiance;

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
	Radiance color;
	float reflection, diffusion, n, roughness;
public:
	Material(const Radiance &color = DEFAULT_RADIANCE, float reflection = DEFAULT_REFLECTION, float diffusion = DEFAULT_DIFFUSION, float n = DEFAULT_REFRACTIVE_INDEX, float roughness = DEFAULT_ROUGHNESS) :
			color(color), reflection(reflection), diffusion(diffusion), n(n), roughness(roughness) {}

//	Getter
	const Radiance &GetColor() const { return color; }

	const float &GetDiffusion() const { return diffusion; }

//	float GetSpecular() { return 1.0f - diffusion; }

//	const float &GetReflection() const { return reflection; }

/*
	float GetN() const { return n; }

	float GetRoughness() const { return roughness; }
*/
//	Setter
	void SetColor(const Radiance &color) { Material::color = color; }

	void SetReflection(float reflection) { Material::reflection = reflection; }

	void SetDiffusion(float diffusion) { Material::diffusion = diffusion; }

	void SetN(float n) { Material::n = n; }

	void SetRoughness(float roughness) { Material::roughness = roughness; }
};

class Primitive
{
private:
	const char* id;
	bool isLuminaire;

protected:
	Material material;

public:
	enum Type
	{
		PLANE,
		SPHERE
	};

	Primitive(const char *id, bool isLuminaire = false);

	virtual ~Primitive();

//	Getter
	Material *GetMaterial() { return &material; }

	const char *GetId() const { return id; }

	bool IsLuminaire() const { return isLuminaire; }

//	Setter
//	void SetMaterial(const Material &material) { Primitive::material = material; }

	void SetId(const char *id);

//	virtual void SetLight(bool light) { Primitive::light = light; }

//	Interface
	virtual Type GetType() = 0;

	virtual cv::Vec3f GetNormal(const cv::Vec3f &point) = 0;

	virtual HitState Intersect(const Ray& ray, float& dist) = 0;

	virtual float GetReflectance(const cv::Vec3f &point, const cv::Vec3f &omega) = 0;

	virtual const Radiance &GetColor() { return material.GetColor(); }
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
