//
// Created by 陈齐斌 on 10/05/2017.
//

#include "CookTorranceBRDF.h"

namespace raytracer
{

float CookTorranceBRDF::D(float alpha, float roughness)
{
//	Berkmann Distribution function
	return powf((float) M_E, -powf(tanf(alpha), 2) / powf(roughness, 2)) / (powf(roughness, 2) * cosf(alpha));
}

float CookTorranceBRDF::G(cv::Vec3f L, cv::Vec3f N, cv::Vec3f H, cv::Vec3f V)
{
//	Attenuation factor
	return fminf(1, 2 * N.dot(H) / V.dot(H) * fminf(N.dot(V), N.dot(-L)));
}

float CookTorranceBRDF::rho(float theta, float phi)
{
	return 0.5f * (powf(tanf(theta - phi), 2) / powf(tanf(theta + phi), 2)
	               + powf(sinf(theta - phi), 2) / powf(sinf(theta + phi), 2));
}

float CookTorranceBRDF::F(cv::Vec3f surfaceNormal, cv::Vec3f omega_i, cv::Vec3f omega_r)
{
	cv::Vec3f L = cv::normalize(omega_i);
	cv::Vec3f N = cv::normalize(surfaceNormal);
	cv::Vec3f R = L - 2 * N.dot(L) * N;
	cv::Vec3f V = cv::normalize(omega_r);
	cv::Vec3f H = cv::normalize(V - L);

	float &&dot = -N.dot(L);
	float reflectance = 0;

	if (dot > 0)
	{

		float alpha = acosf(N.dot(H));
		float theta = acosf(-L.dot(N)), phi = asinf(sinf(theta) / refractiveIndex);

		reflectance += diffusion;

//		Cook - Torrance
//      Rs = D(⍺, roughness) * G * ρ(theta, lambda) / (M_PI * N.dot(-L) * N.dot(V))
		reflectance += specular * D(alpha, roughness)
		         * G(L, N, H, V) * rho(theta, phi) / (M_PI * dot * N.dot(V));

	}
	return reflectance;
}

}
