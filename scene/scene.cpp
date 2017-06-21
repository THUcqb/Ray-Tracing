//
// Created by 陈齐斌 on 04/05/2017.
//

#include "scene.h"
#include "components.h"

namespace raytracer
{

Primitive::Primitive(const char *id, const Radiance &color, BRDF *brdf, bool isLuminaire, float reflection) : isLuminaire(isLuminaire), reflection(reflection)
{
	char *str = new char[strlen(id) + 1];
	strcpy(str, id);
	Primitive::color = color;
	Primitive::brdf = brdf;
	Primitive::id = str;
}

Primitive::~Primitive()
{
	delete id;
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
#define DIFFUSE_SURFACE CookTorranceBRDF::CreateCookTorranceBRDF(CookTorranceBRDF::DIFFUSE)
#define METAL_SURFACE CookTorranceBRDF::CreateCookTorranceBRDF(CookTorranceBRDF::METAL)
#define MIRROR_SURFACE CookTorranceBRDF::CreateCookTorranceBRDF(CookTorranceBRDF::MIRROR)

	numPrimitives = 25;
	primitives = new Primitive*[numPrimitives];

	// ground plane
	primitives[0] = new Triangle("floor1", Radiance(206, 230, 245),
	                             cv::Vec3f(6, 6, -8), cv::Vec3f(6, 6, 12), cv::Vec3f(-6, 6, -8));
	primitives[1] = new Triangle("floor2", Radiance(206, 230, 245),
	                             cv::Vec3f(-6, 6, -8), cv::Vec3f(6, 6, 12), cv::Vec3f(-6, 6, 12));

	// ceiling plane
	primitives[2] = new Triangle("ceiling1", Radiance(206, 230, 245),
	                             cv::Vec3f(-6, -4, -8), cv::Vec3f(6, -4, 12), cv::Vec3f(6, -4, -8));
	primitives[3] = new Triangle("ceiling2", Radiance(206, 230, 245),
	                             cv::Vec3f(-6, -4, 12), cv::Vec3f(6, -4, 12), cv::Vec3f(-6, -4, -8));

	// left plane
	primitives[4] = new Triangle("left1", Radiance(61, 20, 219),
	                             cv::Vec3f(-6, -4, -8), cv::Vec3f(-6, 6, -8), cv::Vec3f(-6, -4, 12));
	primitives[5] = new Triangle("left2", Radiance(61, 20, 219),
	                             cv::Vec3f(-6, -4, 12), cv::Vec3f(-6, 6, -8), cv::Vec3f(-6, 6, 12));

	// right plane
	primitives[6] = new Triangle("right1", Radiance(87, 138, 46),
	                             cv::Vec3f(6, -4, 12), cv::Vec3f(6, 6, -8), cv::Vec3f(6, -4, -8));
	primitives[7] = new Triangle("right2", Radiance(87, 138, 46),
	                             cv::Vec3f(6, 6, 12), cv::Vec3f(6, 6, -8), cv::Vec3f(6, -4, 12));

	// front plane
	primitives[8] = new Triangle("front1", Radiance(205, 90, 106),
	                             cv::Vec3f(6, -4, 12), cv::Vec3f(-6, -4, 12), cv::Vec3f(-6, 6, 12));
	primitives[9] = new Triangle("front2", Radiance(205, 90, 106),
	                             cv::Vec3f(6, -4, 12), cv::Vec3f(-6, 6, 12), cv::Vec3f(6, 6, 12));

	// back plane
	primitives[10] = new Triangle("back1", Radiance(206, 230, 245),
	                              cv::Vec3f(-6, -4, -8), cv::Vec3f(6, -4, -8), cv::Vec3f(-6, 6, -8));
	primitives[11] = new Triangle("back2", Radiance(206, 230, 245),
	                              cv::Vec3f(-6, 6, -8), cv::Vec3f(6, -4, -8), cv::Vec3f(6, 6, -8));

//	big sphere
	primitives[12] = new Sphere("big sphere", Radiance(120, 120, 120),
	                            cv::Vec3f(3, 3.5f, 5), 2.5f, DIFFUSE_SURFACE);

//	small sphere
	primitives[13] = new Sphere("small sphere", Radiance(180, 180, 180),
	                            cv::Vec3f(-3.5f, 4, 7), 2, MIRROR_SURFACE, false, 0.9);

//	tiny sphere
	primitives[14] = new Sphere("tiny sphere", Radiance(135, 199, 234),
	                            cv::Vec3f(-0.5, 5, 4), 1, METAL_SURFACE);

//	Light sources
	primitives[15] = new Triangle("light1", LIGHT_RADIANCE,
	                              cv::Vec3f(1, -3.8, 5), cv::Vec3f(1, -3.8, 3), cv::Vec3f(-1, -3.8, 3),
	                              DIFFUSE_SURFACE, true);

	primitives[16] = new Triangle("light2", LIGHT_RADIANCE,
	                              cv::Vec3f(1, -3.8, 5), cv::Vec3f(-1, -3.8, 3), cv::Vec3f(-1, -3.8, 5),
	                              DIFFUSE_SURFACE, true);

	primitives[17] = new Triangle("light3", LIGHT_RADIANCE,
	                              cv::Vec3f(1, -4, 3), cv::Vec3f(-1, -4, 3), cv::Vec3f(-1, -3.8, 3),
	                              DIFFUSE_SURFACE, true);
	primitives[18] = new Triangle("light4", LIGHT_RADIANCE,
	                              cv::Vec3f(1, -3.8, 3), cv::Vec3f(1, -4, 3), cv::Vec3f(-1, -3.8, 3),
	                              DIFFUSE_SURFACE, true);

	primitives[19] = new Triangle("light5", LIGHT_RADIANCE,
	                              cv::Vec3f(1, -4, 5), cv::Vec3f(1, -3.8, 5), cv::Vec3f(-1, -4, 5),
	                              DIFFUSE_SURFACE, true);
	primitives[20] = new Triangle("light6", LIGHT_RADIANCE,
	                              cv::Vec3f(1, -3.8, 5), cv::Vec3f(-1, -3.8, 5), cv::Vec3f(-1, -4, 5),
	                              DIFFUSE_SURFACE, true);

	primitives[21] = new Triangle("light7", LIGHT_RADIANCE,
	                              cv::Vec3f(-1, -4, 3), cv::Vec3f(-1, -4, 5), cv::Vec3f(-1, -3.8, 5),
	                              DIFFUSE_SURFACE, true);
	primitives[22] = new Triangle("light8", LIGHT_RADIANCE,
	                              cv::Vec3f(-1, -3.8, 3), cv::Vec3f(-1, -4, 3), cv::Vec3f(-1, -3.8, 5),
	                              DIFFUSE_SURFACE, true);
	primitives[23] = new Triangle("light7", LIGHT_RADIANCE,
	                              cv::Vec3f(1, -4, 3), cv::Vec3f(1, -3.8, 5), cv::Vec3f(1, -4, 5),
	                              DIFFUSE_SURFACE, true);
	primitives[24] = new Triangle("light8", LIGHT_RADIANCE,
	                              cv::Vec3f(1, -3.8, 3), cv::Vec3f(1, -3.8, 5), cv::Vec3f(1, -4, 3),
	                              DIFFUSE_SURFACE, true);

}

}   //  namespace raytracer