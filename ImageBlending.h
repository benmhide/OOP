#pragma once

//Include directives
#include <vector>
#include "Image.h"
#include "ReadWriteImage.h"

//Image blending class
class ImageBlending
{
public:
	//Mean blend image files function
	bool meanBlending(Image *meanImg, const std::string &image, const std::vector<Image> &stackerImages);

	//Median blend image files function
	bool medianBlending(Image *medianImg, const std::string &image, const std::vector<Image> &stackerImages);

	//Sigma clipped mean blend image files function
	bool sigmaClippedMeanBlending(Image *scMeanImg, const float &sigma, const std::string &image, const std::vector<Image> &stackerImages);

private:
	//Read write ppm object
	ReadWriteImage readWritePPM;
};