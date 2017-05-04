#include <iostream>
#include <opencv2/opencv.hpp>
#include "raytracer.h"
#include "scene/scene.h"

using namespace cv;

int main(int argc, char** argv)
{
	raytracer::Scene *scene = new raytracer::Scene;
	raytracer::Engine *engine = new raytracer::Engine(scene);

	cv::namedWindow("Display Window", WINDOW_AUTOSIZE);

	Mat image = engine->Render();
	cv::imshow("Display Window", image);

	delete engine;

	delete scene;

	waitKey(0);

	return 0;
}
