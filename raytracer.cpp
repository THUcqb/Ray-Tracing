//
// Created by 陈齐斌 on 04/05/2017.
//

#include <cv.h>
#include "raytracer.h"
#include "scene/scene.h"
#include "scene/components.h"

namespace raytracer
{

Engine::Engine(Scene *scene) : scene(scene)
{
	if (scene != nullptr)
		scene->InitScene();
}

void Engine::InitRender()
{
	renderHeight = 480 * 4;
	renderWidth = 640 * 4;
	focalLength = 0.2;
	pixelSize = 0.0001;  // or less
	cameraOrigin = cv::Point3f(0, -1, -5);
	canvasCenter = cv::Point3f(0, -1, -5 + focalLength);
}

Ray *Engine::IndexToRay(int idx)
{
	int y = idx / renderWidth, x = idx % renderWidth;

	cv::Point3f dest = cv::Vec3f(canvasCenter) + cv::Vec3f((x - renderWidth / 2) * pixelSize, (y - renderHeight / 2) * pixelSize, 0);

	return new Ray(cameraOrigin, dest - cameraOrigin);
}

cv::Scalar Engine::RayTrace(const Ray &ray)
{
	float dist = INFINITY;
	Primitive *hitprim = nullptr;
	for (int i = 0; i < scene->GetNumPrimitives(); ++i)
	{
		Primitive *prim = scene->GetPrimitives(i);
		if (prim == nullptr) continue;
		float tdist = INFINITY;
		if (prim->Intersect(ray, tdist) == raytracer::HIT)
		{
			if (tdist < dist)
			{
				dist = tdist;
				hitprim = prim;
			}
		}

	}
	if (hitprim != nullptr)
	{
		if (hitprim->IsLight())
			return cv::Scalar(255, 255, 255);
		else
		{
			cv::Scalar color;
			cv::Point3f point = (cv::Vec3f)ray.GetOrigin() + dist * ray.GetDirection();
			cv::Vec3f N = hitprim->GetNormal(point);
			for (int i = 0; i < scene->GetNumPrimitives(); i++)
			{
				Primitive *light = scene->GetPrimitives(i);
				if (light == nullptr) continue;
				if (light->IsLight())
				{
					cv::Vec3f L = cv::normalize((cv::Vec3f)point - (cv::Vec3f)((Sphere *)light)->GetCenter());
					if (light->GetMaterial()->GetDiffusion() > 0)
					{
						float dot = -L.dot(N);
						if (dot > 0)
						{
							double diff = 10 * dot * light->GetMaterial()->GetDiffusion();

							color += diff / 256 * (hitprim->GetMaterial()->GetColor().mul(light->GetMaterial()->GetColor()));
						}
					}
				}
			}
			return color;
		}
	}

	return BACKGROUND_COLOR;
}

cv::Mat *Engine::Render()
{
	if (scene != nullptr)
	{
		InitRender();
		cv::Mat *colorMat = new cv::Mat(renderHeight, renderWidth, CV_8UC3);

		for (int idx = 0; idx < renderHeight * renderWidth; ++idx)
		{
			Ray *ray = IndexToRay(idx);
			colorMat->row(idx / renderWidth).col(idx % renderWidth) = RayTrace(*ray);
			delete ray;
		}

		cv::resize(*colorMat, *colorMat, cv::Size(renderWidth / 4, renderHeight / 4), 0, 0);

		return colorMat;
	}
	return nullptr;
}

}
