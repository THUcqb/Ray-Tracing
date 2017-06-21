//
// Created by 陈齐斌 on 04/05/2017.
//

#include "engine.h"
#include "../scene/scene.h"
#include "../scene/components.h"

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
	cameraOrigin = cv::Vec3f(0, 1, -8);
	canvasCenter = cv::Vec3f(0, 1, -8 + focalLength);
	srand((unsigned int) time(0));
//	memset(set, 0, sizeof(set));
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
		if (prim->Intersect(ray, t_dist) == raytracer::HIT && t_dist > 1e-5)
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

Radiance Engine::RayTrace(int depth, Ray ray, float dist, Primitive *surface)
{
	bool directLighting = false;
	if (depth >= MAX_DEPTH)
		directLighting = true;

	Radiance radiance = DEFAULT_RADIANCE;

	if (surface != nullptr)
	{
		if (surface->IsLuminaire())
			return surface->GetRadiance();
		else
		{
			cv::Vec3f surfacePoint = ray.GetOrigin() + dist * ray.GetDirection();

//			if (depth > 0 && surface->GetBRDF()->isDiffuse())
//			if (set[(int)(10*(surfacePoint.val[0] + 6))][(int)(10*(surfacePoint.val[1] + 4))][(int)(10*(surfacePoint.val[2] + 8))][depth - 1])
//				return map[(int)(10*(surfacePoint.val[0] + 6))][(int)(10*(surfacePoint.val[1] + 4))][(int)(10*(surfacePoint.val[2] + 8))][depth - 1];

			cv::Vec3f surfaceNormal = surface->GetNormal(surfacePoint);

			Primitive *luminaire;

			for (int i = 0; i < scene->GetNumPrimitives(); i++)
			{
				if ((luminaire = scene->GetPrimitives(i)) != surface
				    && (!directLighting || luminaire->IsLuminaire()))
				{
//					for (int test = 0; test < 1; ++test)
//					{
						cv::Vec3f luminairePoint = luminaire->GetRandomPoint();
						cv::Vec3f luminaireNormal = luminaire->GetNormal(luminairePoint);
						cv::Vec3f omega = cv::normalize(surfacePoint - luminairePoint);

						dist = INFINITY;
//				if the points x and x' can 'see' each other
						if (surfaceNormal.dot(omega) < 0 && luminaireNormal.dot(omega) > 0
						    && Hit(Ray(surfacePoint, -omega), dist) == luminaire)
						{
							radiance += surface->GetBRDF()->F(surfaceNormal, omega, -ray.GetDirection()) / M_PI
									            * surfaceNormal.dot(-omega) // cos<n, ⍵>
									            * luminaireNormal.dot(omega)// cos<n', ⍵>
									            / luminaire->GetPdf(luminairePoint, surfacePoint)
							                    / cv::norm(luminairePoint - surfacePoint, cv::NORM_L2SQR)
//									            * 3 / pow(cv::norm(luminairePoint - surfacePoint, cv::NORM_L2SQR), 1.5)
									            * (luminaire->IsLuminaire() ? luminaire->GetRadiance() : RayTrace(
											depth + 1, Ray(surfacePoint, -omega), dist, luminaire));
						}
//					}
				}
			}

			if (surface->GetReflection() > 0)
			{
				cv::Vec3f V = -ray.GetDirection();
				cv::Vec3f VR = 2 * surfaceNormal.dot(V) * surfaceNormal - V;
				dist = INFINITY;
				luminaire = Hit(Ray(surfacePoint, VR), dist);
				radiance *= 1 - surface->GetReflection();
				Radiance t(0, 0, 0);
				t += surface->GetReflection() * RayTrace(depth, Ray(surfacePoint, VR), dist, luminaire);
				if (cv::norm(t) > 500)
					radiance += t * (500 / cv::norm(t));
				else
					radiance += t;
			}

			radiance = radiance.mul(surface->GetColor()) / 255;

//			if (depth > 0 && surface->GetBRDF()->isDiffuse())
//			{
//				set[(int)(10*(surfacePoint.val[0] + 6))][(int)(10*(surfacePoint.val[1] + 4))][(int)(10*(surfacePoint.val[2] + 8))][depth - 1] = true;
//				map[(int)(10*(surfacePoint.val[0] + 6))][(int)(10*(surfacePoint.val[1] + 4))][(int)(10*(surfacePoint.val[2] + 8))][depth - 1] = radiance;
//			}
		}

	}

	return radiance;
}

int Primitive::WHOLE_TEST = 10000, Primitive::CURRENT_TEST = 1;

cv::Mat Engine::Render()
{
	InitRender();

	cv::Mat colorMat(renderHeight, renderWidth, CV_8UC3, Radiance(0, 0, 0));
	Radiance sum[renderHeight][renderWidth];
	memset(sum, 0, sizeof(sum));
	if (scene != nullptr)
	{
		for (; Primitive::CURRENT_TEST < Primitive::WHOLE_TEST; Primitive::CURRENT_TEST++)
		{
			cv::Mat current(renderHeight, renderWidth, CV_8UC3, Radiance(0, 0, 0));
			for (int idx = 0; idx < renderWidth * renderHeight; ++idx)
			{
				Ray &&ray = IndexToRay(idx);
				float dist = INFINITY;
				Primitive *hit = Hit(ray, dist);
				Radiance &&radiance = RayTrace(0, ray, dist, hit) + AMBIENT_RADIANCE;
//			colorMat.row(idx / renderWidth).col(idx % renderWidth) = (colorMat.row(idx / renderWidth).col(idx % renderWidth) * (Primitive::CURRENT_TEST - 1) + radiance) / Primitive::CURRENT_TEST;
				sum[idx / renderWidth][idx % renderWidth] += radiance;
				current.row(idx / renderWidth).col(idx % renderWidth) = radiance;
				colorMat.row(idx / renderWidth).col(idx % renderWidth) =
						sum[idx / renderWidth][idx % renderWidth] / Primitive::CURRENT_TEST;
				if (idx % renderWidth == 0)
				{
					cv::Mat t;
					cv::resize(colorMat, t, cv::Size(renderWidth / SSAA, renderHeight / SSAA), 0, 0);
					for (int k = 0; k < renderWidth; k++)
						t.row(idx / renderWidth).col(k) = Radiance(255, 255, 255);
					cv::imshow("Current", current);
					cv::imshow("Display Window", t);
					cv::waitKey(1);
				}
			}
		}

	}
	else
	{
		std::cout << "Scene hasn't been set!" << std::endl;
	}

	cv::resize(colorMat, colorMat, cv::Size(renderWidth / SSAA, renderHeight / SSAA), 0, 0);

	return colorMat;

}



}
