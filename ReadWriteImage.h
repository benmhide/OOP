#pragma once

//Include directives
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "Image.h"
#include "ImageZoom.h"

//Image read write class
class ReadWriteImage
{
public:
	//Read image ppm files
	Image readPPM(const char* filename);

	//Overloaded write function that will read imgaes in parts
	Image readPPM(const char *filename, unsigned int imgPart, const std::streampos pos);

	//Write image ppm files
	void writePPM(const Image &writeImg, const char* file);
};