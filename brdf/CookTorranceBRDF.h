//
// Created by 陈齐斌 on 10/05/2017.
//

#ifndef RAYTRACING_COOKTORRANCEBRDF_H
#define RAYTRACING_COOKTORRANCEBRDF_H

#include "BRDF.h"

namespace raytracer
{

class CookTorranceBRDF : public BRDF
{
	float diffusion, specular, refractiveIndex, roughness;

	float D(float alpha, float roughness);

	float G(cv::Vec3f L, cv::Vec3f N, cv::Vec3f H, cv::Vec3f V);

	float rho(float theta, float phi);

public:
	enum Type
	{
		DIFFUSE,
		MIRROR,
		METAL
	};

	CookTorranceBRDF(float diffusion, float specular, float refractiveIndex, float roughness)
			: diffusion(diffusion), specular(specular), refractiveIndex(refractiveIndex),
			  roughness(roughness)
	{}

	static CookTorranceBRDF *CreateCookTorranceBRDF(Type type)
	{
		switch (type)
		{
			case DIFFUSE:
				return new CookTorranceBRDF(0.2, 0, 1.2, 1);
			case MIRROR:
				return new CookTorranceBRDF(0.0, 0.9, 1.5, 0.05);
			case METAL:
				return new CookTorranceBRDF(0.2, 0.5, 2, 0.2);
		}
	}

	float F(cv::Vec3f surfaceNormal, cv::Vec3f omega_i, cv::Vec3f omega_r) override;

	bool isDiffuse() override
	{
		return diffusion > 0 && specular == 0;
	}
};

}

#endif //RAYTRACING_COOKTORRANCEBRDF_H
