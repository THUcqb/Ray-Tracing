//
// Created by 陈齐斌 on 04/05/2017.
//

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
	cameraOrigin = cv::Vec3f(0, 0, -8);
	canvasCenter = cv::Vec3f(0, 0, -8 + focalLength);
	srand((unsigned int) time(0));
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

float Engine::D(float alpha, float roughness)
{
//	Berkmann Distribution function

	return powf((float) M_E, -powf(tanf(alpha), 2) / powf(roughness, 2)) / (powf(roughness, 2) * cosf(alpha));
}

float Engine::G(cv::Vec3f L, cv::Vec3f N, cv::Vec3f H, cv::Vec3f V)
{
//	Attenuation factor
	return fminf(1, 2 * N.dot(H) / V.dot(H) * fminf(N.dot(V), N.dot(-L)));
}

float Engine::ρ(float θ, float φ)
{
	return 0.5f * (powf(tanf(θ - φ), 2) / powf(tanf(θ + φ), 2)
	               + powf(sinf(θ - φ), 2) / powf(sinf(θ + φ), 2));
}

cv::Scalar Engine::LocalIllumination(Ray ray)
{
	float dist = INFINITY;
	Primitive *hit = Hit(ray, dist);

	//	If hit something
	if (hit != nullptr)
	{
		if (hit->IsLight())
		{
			return hit->GetColor();
		}
		cv::Scalar color = DEFAULT_COLOR;

		cv::Vec3f point = ray.GetOrigin() + dist * ray.GetDirection();

//		Normal vector, view vector
		cv::Vec3f N = hit->GetNormal(point), V = -ray.GetDirection();

		for (int i = 0; i < scene->GetNumPrimitives(); i++)
		{
			Primitive *light = scene->GetPrimitives(i);
			cv::Scalar I = hit->GetMaterial()->GetColor().mul(light->GetMaterial()->GetColor());

			if (light->IsLight())
			{
				cv::Vec3f L = cv::normalize(point - ((Sphere *)light)->GetCenter());
//				Shadow
				if (Hit(Ray(point, -L), dist = INFINITY) != light) continue;

				double dot = -N.ddot(L);
				if (dot > 0)
				{
					cv::Vec3f R = L - 2 * N.dot(L) * N;
					cv::Vec3f H = cv::normalize(V - L);

					float alpha = acosf(N.dot(H));
					float θ = acosf(-L.dot(N)), φ = asinf(sinf(θ) / hit->GetMaterial()->GetN());
//					Diffusion = Ii * (N.dot(-L)) * Kd * Rd;
					color += I * hit->GetMaterial()->GetDiffusion() * -N.ddot(L);

//					Cook - Torrance
//					Specular = Ii * (N.dot(-L)) * Ks * Rs, Rs = D(⍺, roughness) * G * ρ(θ, ƛ) / (M_PI * N.dot(-L) * N.dot(V))
					color += I * hit->GetMaterial()->GetSpecular()
					         * D(alpha, hit->GetMaterial()->GetRoughness())
					         * G(L, N, H, V) * ρ(θ, φ) / (M_PI * N.dot(V));

				}

			}

		}
		return color;
	}
	return DEFAULT_COLOR;
}

cv::Vec3f Engine::RandDir()
{
	float &&theta = (float) (2 * M_PI * rand() / RAND_MAX), &&phi = (float) (M_PI * rand() / RAND_MAX);
	float &&cos_phi = cosf(phi);

	return cv::Vec3f(cos_phi * cosf(theta), cos_phi * sinf(theta), sinf(phi));
}

cv::Scalar Engine::RayTrace(Ray ray, int depth)
{
	if (depth > 2)
	{
		return LocalIllumination(ray);
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

			cv::Vec3f VR = 2 * N.dot(V) * N - V;
			cv::Scalar reflection = RayTrace(VR, depth + 1);

			float diff = RAND_MAX * hit->GetMaterial()->GetDiffusion(), refl = diff + RAND_MAX * hit->GetMaterial()->GetReflection();

#define MONTE_CARLO_TEST 100
			for (int test = 0; test < MONTE_CARLO_TEST; test++)
			{
				int &&bet = rand();
				if (bet < diff)
				{
					cv::Vec3f randDir = RandDir();
					float &&dot = N.dot(randDir);
					if (dot < 0)
					{
						randDir = randDir - 2 * dot * N;
						dot = -dot;
					}
					color += RayTrace(Ray(point, randDir), depth + 1) * dot;
				}
				else
				if (bet < refl)
				{
					color += reflection;
				}
			}

			color /= MONTE_CARLO_TEST;

			color = color.mul(hit->GetMaterial()->GetColor());

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
//		std::cout << color << std::endl;

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
