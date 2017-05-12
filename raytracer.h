//
// Created by 陈齐斌 on 04/05/2017.
//

#ifndef RAYTRACING_RAY_H
#define RAYTRACING_RAY_H

#include <opencv2/opencv.hpp>

namespace raytracer
{
typedef cv::Scalar_<int > Radiance;

class Scene;

class Primitive;

class Ray
{
	cv::Vec3f ori, dir;

public:
	Ray(const cv::Vec3f &ori = cv::Vec3f(0, 0, 0), const cv::Vec3f &dir = cv::Vec3f(0, 0, 0)) : ori(ori), dir(normalize(dir)) {}

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

	Primitive *Hit(Ray ray, float &dist);

	Radiance RayTrace(int depth, Ray ray, float dist, Primitive *surface);

#define MAX_DEPTH 2
	Radiance map[121][101][201][MAX_DEPTH];
	bool set[121][101][201][MAX_DEPTH];
public:
	Engine(Scene *scene);

	virtual ~Engine();

	cv::Mat Render();
};

}   // namespace raytracer

#endif //RAYTRACING_RAY_H
