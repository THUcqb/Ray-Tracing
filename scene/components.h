//
// Created by 陈齐斌 on 04/05/2017.
//

#ifndef RAYTRACING_COMPONENTS_H
#define RAYTRACING_COMPONENTS_H

#include <opencv2/opencv.hpp>

namespace raytracer
{
class Ray;
#define AMBIENT_COLOR cv::Scalar(0.04, 0.04, 0.04)
#define LIGHT_COLOR cv::Scalar(1, 1, 1)
#define DEFAULT_COLOR cv::Scalar(0, 0, 0)

#define DEFAULT_REFLECTION 0
#define DEFAULT_DIFFUSION 0.2

#define DEFAULT_REFRACTIVE_INDEX 1.2
#define DEFAULT_ROUGHNESS 1.2

#define EPSILON 1e-4

enum HitState
{
	HIT,
	MISS,
	INPRIM
};

class Material
{
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

	Type GetType() override { return SPHERE; }

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
