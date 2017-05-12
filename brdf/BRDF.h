//
// Created by 陈齐斌 on 10/05/2017.
//

#ifndef RAYTRACING_BRDF_H
#define RAYTRACING_BRDF_H

#include <opencv2/opencv.hpp>

namespace raytracer
{

class BRDF
{
public:

	virtual float F(cv::Vec3f surfaceNormal, cv::Vec3f omega_i, cv::Vec3f omega_r) = 0;

	virtual bool isDiffuse() = 0;
};

}
#endif //RAYTRACING_BRDF_H
