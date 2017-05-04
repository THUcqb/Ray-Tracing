#include <iostream>
#include <opencv2/opencv.hpp>
#include "raytracer.h"
#include "scene/scene.h"

using namespace cv;

int main(int argc, char** argv)
{
	raytracer::Engine *engine =  new raytracer::Engine(new raytracer::Scene);

	cv::imshow("image", *(engine->Render()));

	waitKey(0);

	return 0;
}
