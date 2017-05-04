//
// Created by 陈齐斌 on 04/05/2017.
//

#ifndef RAYTRACING_COMPONENTS_H
#define RAYTRACING_COMPONENTS_H

#include <opencv2/opencv.hpp>

namespace raytracer
{
class Ray;

#define DEFAULT_COLOR cv::Scalar(0.2f, 0.2f, 0.2f)
#define DEFAULT_REFLECTION 0
#define DEFAULT_DIFFUSION 0.2f

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
	float reflection;
	float diffusion;

public:
	Material(const cv::Scalar &color = DEFAULT_COLOR, float reflection = DEFAULT_REFLECTION, float diffusion = DEFAULT_DIFFUSION) :
			color(color), reflection(reflection), diffusion(diffusion) {}

//	Getter
	const cv::Scalar &GetColor() const { return color; }

	float GetReflection() const { return reflection; }

	float GetDiffusion() const { return diffusion; }

//	float GetSpecular() { return 1.0f - diffusion; }

//	Setter
	void SetColor(const cv::Scalar &color) { Material::color = color; }

	void SetReflection(float reflection) { Material::reflection = reflection; }

	void SetDiffusion(float diffusion) { Material::diffusion = diffusion; }
};

class Primitive
{
private:
	Material material;
	const char* id;
	bool light;

public:
	enum Type
	{
		PLANE,
		SPHERE
	};

	Primitive(const char *id = nullptr, bool light = false) : id(id), light(light) {}

	virtual ~Primitive() {}

//	Getter
	Material *GetMaterial() { return &material; }

	const char *GetId() const { return id; }

	bool IsLight() const { return light; }

//	Setter
	void SetMaterial(const Material &material) { Primitive::material = material; }

	void SetId(const char *id) { Primitive::id = id; };

//	virtual void SetLight(bool light) { Primitive::light = light; }

//	Interface
	virtual Type GetType() = 0;

	virtual HitState Intersect(const Ray& ray, float& dist) = 0;

	virtual cv::Vec3f GetNormal(cv::Point3f point) = 0;

	virtual cv::Scalar GetColor() { return material.GetColor(); }
};

class Sphere : public Primitive
{
private:
	cv::Point3f center;
	float sqRadius, radius;//, Rradius;

public:
	Sphere(const cv::Point3f& center, float radius, const char *id = nullptr, bool light = false) :
			Primitive(id, light), center(center), radius(radius),
			sqRadius(radius * radius) {}//, Rradius(1.0f / radius) {}

	const cv::Point3f &GetCenter() const { return center; }

	Type GetType() override { return SPHERE; }

	HitState Intersect(const Ray& ray, float& dist) override;

	cv::Vec3f GetNormal(cv::Point3f point) override {return cv::normalize(cv::Vec3f(point - center)); }
};

class Plane : public Primitive
{
	float D;
	cv::Vec3f normal;

public:
	Plane(const cv::Vec3f &normal, float D, const char *id = nullptr, bool light = false) :
			Primitive(id, light), normal(cv::normalize(normal)), D(D) {}

	Type GetType() override { return PLANE; }

	HitState Intersect(const Ray &ray, float &dist) override;

	cv::Vec3f GetNormal(cv::Point3f point) override { return cv::normalize(normal); }
};

}   //  namespace raytracer


#endif //RAYTRACING_COMPONENTS_H
