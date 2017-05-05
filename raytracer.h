//
// Created by 陈齐斌 on 04/05/2017.
//

#ifndef RAYTRACING_RAY_H
#define RAYTRACING_RAY_H

#include <opencv2/opencv.hpp>

namespace raytracer
{
class Scene;

class Primitive;

class Ray
{
	cv::Vec3f ori, dir;

public:
	Ray(const cv::Vec3f &ori = cv::Vec3f(0, 0, 0), const cv::Vec3f &dir = cv::Vec3f(0, 0, 0)) : ori(ori), dir(normalize(dir)) {}

//	void SetOrigin(const cv::Vec3f& cori) { ori = cori; }

//	void SetDirection(const cv::Vec3f& cdir) { dir = normalize(cdir); }

	const cv::Vec3f& GetOrigin() const { return ori; }

	const cv::Vec3f& GetDirection() const { return dir; }
};

class Engine
{
private:
	Scene *scene;
	int renderWidth, renderHeight;
	float focalLength, pixelSize;
	cv::Vec3f cameraOrigin, canvasCenter;

	void InitRender();

	Ray IndexToRay(int index);

	float D(float alpha, float roughness);

	float G(cv::Vec3f L, cv::Vec3f N, cv::Vec3f H, cv::Vec3f V);

	float ρ(float θ, float φ);

	cv::Scalar RayTrace(Ray ray, int depth = 0);

	Primitive *Hit(Ray ray, float &dist);

public:
	Engine(Scene *scene);

	virtual ~Engine();

	cv::Mat Render();
};

}   // namespace raytracer

#endif //RAYTRACING_RAY_H
