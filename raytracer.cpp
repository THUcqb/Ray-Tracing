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
/*
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

Radiance Engine::LocalIllumination(Ray ray)
{
	float dist = INFINITY;
	Primitive *hit = Hit(ray, dist);

	//	If hit something
	if (hit != nullptr)
	{
		if (hit->IsLuminaire())
		{
			return hit->GetColor();
		}
		Radiance color = DEFAULT_RADIANCE;

		cv::Vec3f point = ray.GetOrigin() + dist * ray.GetDirection();

//		Normal vector, view vector
		cv::Vec3f N = hit->GetNormal(point), V = -ray.GetDirection();

		for (int i = 0; i < scene->GetNumPrimitives(); i++)
		{
			Primitive *light = scene->GetPrimitives(i);
			Radiance I = hit->GetMaterial()->GetColor().mul(light->GetMaterial()->GetColor());

			if (light->IsLuminaire())
			{
				cv::Vec3f L = cv::normalize(point - ((Sphere *)light)->GetCenter());
//				Shadow
//				if (Hit(Ray(point, -L), dist = INFINITY) != light) continue;

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
	return DEFAULT_RADIANCE;
}
*/
/*
cv::Vec3f Engine::RandDir()
{
	float &&theta = (float) (2 * M_PI * rand() / RAND_MAX), &&phi = (float) (M_PI * rand() / RAND_MAX);
	float &&sin_phi = sinf(phi);

	return cv::Vec3f(sin_phi * cosf(theta), sin_phi * sinf(theta), cosf(phi));
}
*/

Radiance Engine::RayTrace(Ray ray, int depth)
{
	if (depth > 0)
		return DEFAULT_RADIANCE;

	float dist = INFINITY;
	Primitive *surface = Hit(ray, dist);

	if (surface != nullptr)
	{
		cv::Vec3f surfacePoint = ray.GetOrigin() + dist * ray.GetDirection();
		cv::Vec3f surfaceNormal = surface->GetNormal(surfacePoint);
//			cv::Vec3f view = -ray.GetDirection();

//		Radiance / Color
		Radiance radiance = DEFAULT_RADIANCE;

		Primitive *luminaire;
		for (int i = 0; i < scene->GetNumPrimitives(); i++)
		{
			if ((luminaire = scene->GetPrimitives(i))->IsLuminaire() && luminaire->GetType() == Primitive::SPHERE && luminaire != surface)
			{
				#define MONTE_CARLO_TEST 100
				for (int test = 0; test < MONTE_CARLO_TEST; ++test)
				{
					cv::Vec3f luminairePoint = ((Sphere*)luminaire)->GetRandomPoint();
					cv::Vec3f luminaireNormal = luminaire->GetNormal(luminairePoint);
					cv::Vec3f omega = cv::normalize(surfacePoint - luminairePoint);

					dist = INFINITY;
//					if the points x and x' can 'see' each other
					if (surfaceNormal.dot(omega) < 0 && luminaireNormal.dot(omega) > 0 && Hit(Ray(surfacePoint, -omega), dist) == luminaire)
					{
						radiance += surface->GetReflectance(surfacePoint, omega) / M_PI
									* luminaire->GetMaterial()->GetColor()
									* surfaceNormal.dot(-omega) // cos<n, ⍵>
									* luminaireNormal.dot(omega)// cos<n', ⍵>
									/ ((Sphere*)luminaire)->Getpdf(luminairePoint, surfacePoint)
									/ cv::norm(luminairePoint - surfacePoint, cv::NORM_L2SQR);

					}

//					radiance +=
				}
			}
		}

//			Normal vector, view vector

//			cv::Vec3f VR = 2 * N.dot(V) * N - V;
//			Radiance reflection = RayTrace(Ray(point, VR), depth + 1);

//			float diff = RAND_MAX * hit->GetMaterial()->GetDiffusion(), refl = diff + RAND_MAX * hit->GetMaterial()->GetReflection();
/*
		for (int test = 0; test < MONTE_CARLO_TEST; test++)
		{
			cv::Vec3f randDir = RandDir();
			float &&dot = N.dot(randDir);
			if (dot < 0)
			{
				randDir = randDir - 2 * dot * N;
				dot = -dot;
			}
			radiance += RayTrace(Ray(point, randDir), depth + 1) * dot;
		}
*/
		radiance /= MONTE_CARLO_TEST;

		if (surface->IsLuminaire())
			radiance += surface->GetMaterial()->GetColor() / 200;
		else
			radiance = radiance.mul(surface->GetMaterial()->GetColor()) / 255;
//		std::cout << radiance << std::endl;

		return radiance;
	}

	return DEFAULT_RADIANCE;
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

			Radiance &&radiance = RayTrace(ray) + AMBIENT_RADIANCE;
			colorMat.row(idx / renderWidth).col(idx % renderWidth) = radiance;
			std::cout << idx << std::endl;

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
