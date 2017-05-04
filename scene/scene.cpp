//
// Created by 陈齐斌 on 04/05/2017.
//

#include "scene.h"
#include "components.h"

namespace raytracer
{

Scene::Scene() : numPrimitives(0), primitives(nullptr)
{}

Scene::~Scene()
{
	if (primitives != nullptr)
	{
		for (int i= 0; i < numPrimitives; i++)
			if (primitives[i] != nullptr)
				delete primitives[i];
		delete[] primitives;
	}

}

void Scene::InitScene()
{
	numPrimitives = 5;
	primitives = new Primitive*[numPrimitives];

	// ground plane
	primitives[0] = new Plane(cv::Vec3f(0, -1, 0), 5, "plane");
	primitives[0]->GetMaterial()->SetReflection(0);
	primitives[0]->GetMaterial()->SetDiffusion(1);
	primitives[0]->GetMaterial()->SetColor(cv::Scalar(0.4,0.78,0.27));
	// big sphere
	primitives[1] = new Sphere( cv::Vec3f(1, -0.8f, 3), 2.5f , "big sphere");
	primitives[1]->GetMaterial()->SetReflection(0.6f);
	primitives[1]->GetMaterial()->SetDiffusion(0.4f);
	primitives[1]->GetMaterial()->SetColor(cv::Scalar(0.7, 0.15, 0.8));
	// small sphere
	primitives[2] = new Sphere( cv::Vec3f(-5.5f, -0.5f, 7), 2 , "small sphere");
	primitives[2]->GetMaterial()->SetReflection(0.9f);
	primitives[2]->GetMaterial()->SetDiffusion(0.1f);
	primitives[2]->GetMaterial()->SetColor(cv::Scalar( 0.5, 0.15, 0.08 ));

	// light source 1
	primitives[3] = new Sphere( cv::Vec3f(4, -4, 3), 0.1f , "light1", true);
	primitives[3]->GetMaterial()->SetColor(cv::Scalar( 1, 1, 1));
	// light source 2
	primitives[4] = new Sphere( cv::Vec3f(-1, -4, -2), 0.1f , "light2", true);
	primitives[4]->GetMaterial()->SetColor(cv::Scalar( 1, 1, 1));



}

}   //  namespace raytracer