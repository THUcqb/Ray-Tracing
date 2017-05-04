//
// Created by 陈齐斌 on 04/05/2017.
//

#include <cv.h>
#include "raytracer.h"
#include "scene/scene.h"
#include "scene/components.h"

namespace raytracer
{
#define SSAA 1
Engine::Engine(Scene *scene) : scene(scene)
{
	if (scene != nullptr)
		scene->InitScene();
}

Engine::~Engine()
{
}

void Engine::InitRender()
{
	renderHeight = 480 * SSAA;
	renderWidth = 640 * SSAA;
	focalLength = 0.2f;
	pixelSize = 0.0004f / SSAA;  // or less
	cameraOrigin = cv::Vec3f(0, -1, -5);
	canvasCenter = cv::Vec3f(0, -1, -5 + focalLength);
}

Ray Engine::IndexToRay(int idx)
{

	int y = idx / renderWidth, x = idx % renderWidth;

	cv::Vec3f dest = canvasCenter + cv::Vec3f((x - renderWidth / 2) * pixelSize, (y - renderHeight / 2) * pixelSize, 0);

	return Ray(cameraOrigin, dest - cameraOrigin);
}

Primitive *Engine::Hit(Ray ray, float &dist)
{
	//	Find the hit Primitive
	Primitive *hit = nullptr;
	for (int i = 0; i < scene->GetNumPrimitives(); ++i)
	{
		Primitive *prim = scene->GetPrimitives(i);

		float t_dist = INFINITY;
		if (prim->Intersect(ray, t_dist) == raytracer::HIT)
		{
			if (t_dist < dist)
			{
				dist = t_dist;
				hit = prim;
			}
		}
	}
	return hit;
}

cv::Scalar Engine::RayTrace(Ray ray, int depth)
{
	if (depth > 3)
	{
		return DEFAULT_COLOR;
	}

	float dist = INFINITY;
	Primitive *hit = Hit(ray, dist);

//	If hit something
	if (hit != nullptr)
	{
		if (hit->IsLight())
		{
			return hit->GetColor();
		}
		else
		{
			cv::Scalar color = DEFAULT_COLOR;

			cv::Vec3f point = ray.GetOrigin() + dist * ray.GetDirection();

//			Normal vector, view vector
			cv::Vec3f N = hit->GetNormal(point), V = -ray.GetDirection();

			for (int i = 0; i < scene->GetNumPrimitives(); i++)
			{
				Primitive *light = scene->GetPrimitives(i);
				cv::Scalar I = hit->GetMaterial()->GetColor().mul(light->GetMaterial()->GetColor());

				if (light->IsLight())
				{
					cv::Vec3f L = cv::normalize(point - ((Sphere *)light)->GetCenter());
					if (Hit(Ray(point, -L), dist = INFINITY) != light) continue;
					double dot = -N.ddot(L);
					if (dot > 0)
					{
						cv::Vec3f R = L - 2 * N.dot(L) * N;

//					    Diffusion
						color += 0.5 * hit->GetMaterial()->GetDiffusion() * -N.ddot(L) * I;

//					    Specular
						color += 1.0 * hit->GetMaterial()->GetSpecular() * pow(V.dot(R), 30) * I;

					}

				}
			}

//			Reflection
			cv::Vec3f VR = 2 * N.dot(V) * N - V;
			color += 1.0 * hit->GetMaterial()->GetReflection() * hit->GetMaterial()->GetColor().mul(RayTrace(Ray(point, VR), depth + 1));

			return color;
		}
	}

	return DEFAULT_COLOR;
}

cv::Mat Engine::Render()
{
	InitRender();

	cv::Mat colorMat(renderHeight, renderWidth, CV_8UC3);

	if (scene != nullptr)
	{

		for (int idx = 0; idx < renderHeight * renderWidth; ++idx)
		{
			Ray &&ray = IndexToRay(idx);

			cv::Scalar &&color = RayTrace(ray) + AMBIENT_COLOR;
			colorMat.row(idx / renderWidth).col(idx % renderWidth) = 255.0 * color;
		}

	}
	else
	{
		std::cout << "Scene isn't set!" << std::endl;
	}

	cv::resize(colorMat, colorMat, cv::Size(renderWidth / SSAA, renderHeight / SSAA), 0, 0);

	return colorMat;

}

}
