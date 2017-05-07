//
// Created by 陈齐斌 on 04/05/2017.
//

#include "scene.h"
#include "components.h"

namespace raytracer
{

Primitive::Primitive(const char *id, bool isLight) : isLight(isLight)
{
	char *str = new char[strlen(id) + 1];
	strcpy(str, id);
	Primitive::id = str;
}

Primitive::~Primitive()
{
	delete id;
}

void Primitive::SetId(const char *id)
{
	delete id;
	char *str = new char[strlen(id) + 1];
	strcpy(str, id);
	Primitive::id = str;
}

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
	numPrimitives = 12;
	primitives = new Primitive*[numPrimitives];

	// ground plane
	primitives[0] = new Plane(cv::Vec3f(0, -1, 0), 6, "floor");
	primitives[0]->GetMaterial()->SetColor(cv::Scalar(0.81,0.90,0.96));

	// ceiling plane
	primitives[1] = new Plane(cv::Vec3f(0, 1, 0), 6, "ceiling");
	primitives[1]->GetMaterial()->SetColor(cv::Scalar(0.81,0.90,0.96));

	// left plane
	primitives[2] = new Plane(cv::Vec3f(1, 0, 0), 6, "left");
	primitives[2]->GetMaterial()->SetColor(cv::Scalar(0.24,0.078,0.86));

	// right plane
	primitives[3] = new Plane(cv::Vec3f(-1, 0, 0), 6, "right");
	primitives[3]->GetMaterial()->SetColor(cv::Scalar(0.34,0.54,0.18));

	// front plane
	primitives[4] = new Plane(cv::Vec3f(0, 0, -1), 12, "front");
	primitives[4]->GetMaterial()->SetColor(cv::Scalar(0.81,0.90,0.96));

	// back plane
	primitives[5] = new Plane(cv::Vec3f(0, 0, 1), 8, "back");
	primitives[5]->GetMaterial()->SetColor(cv::Scalar(0.81,0.90,0.96));

	for (int i = 0; i < 6; i++)
	{
		primitives[i]->GetMaterial()->SetDiffusion(0.3);
//		primitives[i]->GetMaterial()->SetReflection(0.1);
	}

//	big sphere
	primitives[6] = new Sphere( cv::Vec3f(1, 1.8f, 3), 2.5f , "big sphere");
	primitives[6]->GetMaterial()->SetReflection(0.1f);
	primitives[6]->GetMaterial()->SetDiffusion(0.4f);
	primitives[6]->GetMaterial()->SetN(2);
	primitives[6]->GetMaterial()->SetRoughness(0.3);
	primitives[6]->GetMaterial()->SetColor(cv::Scalar(0.3, 0.1, 0.7));

//	small sphere
	primitives[7] = new Sphere( cv::Vec3f(-3.5f, 1.5f, 5), 2 , "small sphere");
	primitives[7]->GetMaterial()->SetReflection(0.1f);
	primitives[7]->GetMaterial()->SetDiffusion(0.8f);
	primitives[7]->GetMaterial()->SetN(1.5);
	primitives[7]->GetMaterial()->SetRoughness(0.3);
	primitives[7]->GetMaterial()->SetColor(cv::Scalar( 0.5, 0.15, 0.08 ));

//	Light sources
	primitives[8] = new Sphere( cv::Vec3f(2, -3.8f, 5), 0.5f , "light1", true);

	primitives[9] = new Sphere( cv::Vec3f(2, -3.8f, 1), 0.5f , "light2", true);

	primitives[10] = new Sphere( cv::Vec3f(-2, -3.8f, 5), 0.5f , "light3", true);

	primitives[11] = new Sphere( cv::Vec3f(-2, -3.8f, 1), 0.5f , "light4", true);

	for (int i = 8; i < 12; i++)
		primitives[i]->GetMaterial()->SetColor(LIGHT_COLOR);
}

}   //  namespace raytracer