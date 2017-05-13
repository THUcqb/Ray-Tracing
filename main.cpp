#include <iostream>
#include <opencv2/opencv.hpp>
#include "engine/engine.h"
#include "scene/scene.h"

int main(int argc, char** argv)
{
	raytracer::Scene *scene = new raytracer::Scene;
	raytracer::Engine *engine = new raytracer::Engine(scene);

	cv::namedWindow("Display Window", cv::WINDOW_AUTOSIZE);

	cv::Mat image = engine->Render();
	cv::imshow("Display Window", image);

	delete engine;
	delete scene;

	cv::waitKey(0);

	return 0;
}
