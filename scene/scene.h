//
// Created by 陈齐斌 on 04/05/2017.
//

#ifndef RAYTRACING_SCENE_H
#define RAYTRACING_SCENE_H

#include <opencv2/opencv.hpp>

namespace raytracer
{

class Primitive;

class Scene
{
#define BACKGROUND_COLOR cv::Scalar(10, 10, 10)
	Primitive **primitives;
	int numPrimitives;

public:
	Scene();

	virtual ~Scene();

	//  Getter
	int GetNumPrimitives() const { return numPrimitives; }

	Primitive *GetPrimitives(int index) const
	{
		if (index < numPrimitives)
			return primitives[index];
		return nullptr;
	}

	void InitScene();
};

}

#endif //RAYTRACING_SCENE_H
