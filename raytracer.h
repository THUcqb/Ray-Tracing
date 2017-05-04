//
// Created by 陈齐斌 on 04/05/2017.
//

#ifndef RAYTRACING_RAY_H
#define RAYTRACING_RAY_H

#include <opencv2/opencv.hpp>

namespace raytracer
{
class Scene;

class Ray
{
	cv::Point3f ori;
	cv::Vec3f dir;

public:
	Ray(const cv::Point3f &ori = cv::Point3f(0, 0, 0), const cv::Vec3f &dir = cv::Point3f(0, 0, 0)) : ori(ori), dir(normalize(dir)) {}

//	void SetOrigin(const cv::Point3f& cori) { ori = cori; }

//	void SetDirection(const cv::Vec3f& cdir) { dir = normalize(cdir); }

	const cv::Point3f& GetOrigin() const { return ori; }

	const cv::Vec3f& GetDirection() const { return dir; }
};

class Engine
{
private:
	Scene *scene;
	int renderWidth, renderHeight;
	float focalLength, pixelSize;
	cv::Point3f cameraOrigin, canvasCenter;

	void InitRender();

	cv::Scalar RayTrace(const Ray &ray);

	Ray *IndexToRay(int index);
public:
	Engine(Scene *scene);

	cv::Mat *Render();
};

}   // namespace raytracer

#endif //RAYTRACING_RAY_H
