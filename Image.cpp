//Include directives
#include "Image.h"

//Rgb member initialisers
const Image::Rgb Image::statConstBlack = Image::Rgb(0);
const Image::Rgb Image::statConstWhite = Image::Rgb(1);
const Image::Rgb Image::statConstRed = Image::Rgb(1, 0, 0);
const Image::Rgb Image::statConstGreen = Image::Rgb(0, 1, 0);
const Image::Rgb Image::statConstBlue = Image::Rgb(0, 0, 1);





//******************************************************************
//*******Image class Rgb struct constructors and destructor*********
//******************************************************************

//Rgb struct constructor r=0, g=0, b=0
Image::Rgb::Rgb() : r(0), g(0), b(0)
{}

//Rgb struct constructor takes 1 param (colour) i.e. grey scale
Image::Rgb::Rgb(float colour) : r(colour), g(colour), b(colour)
{}

//Rgb struct constructor takes 3 params (red/green/blue)
Image::Rgb::Rgb(float _red, float _green, float _blue) : r(_red), g(_green), b(_blue)
{}





//******************************************************************
//***************Image class Rgb struct functions*******************
//******************************************************************

//Mutator functions for red, green and blue Rgb member variables
void Image::Rgb::setRed(const float _red)
{ r = _red; }

void Image::Rgb::setGreen(const float _green)
{ g = _green; }

void Image::Rgb::setBlue(const float _blue)
{ b = _blue; }

//Accessor functions for red, green and blue Rgb member variables
const float Image::Rgb::getRed() const
{ return r; }

const float Image::Rgb::getGreen() const
{ return g; }

const float Image::Rgb::getBlue() const
{ return b; }

//Functions to add rgb values together, divide rgb values by a divisor value and set all rgb values
void Image::Rgb::addRgb(const float _red, const float _green, const float _blue)
{ *this += Rgb(_red, _green, _blue);}

void Image::Rgb::divideRgb(const float denominator)
{ *this /= denominator; }

void Image::Rgb::setRgb(const float _red, const float _green, const float _blue)
{ r = _red; g = _green; b = _blue; }






//******************************************************************
//***********Image class Rgb struct overloaded operators************
//******************************************************************

//Overrided not equal to operators for the Rgb struct
bool Image::Rgb::operator!=(const Rgb &colour) const
{
	return colour.r != r || colour.g != g || colour.b != b;
}

//Overrided multiply-equals operators for the Rgb struct
Image::Rgb& Image::Rgb::operator*=(const Rgb &rgb)
{
	r *= rgb.r, g *= rgb.g, b *= rgb.b;
	return *this;
}

//Overrided divide-equals operators for the Rgb struct
Image::Rgb& Image::Rgb::operator/=(const float &denominator)
{
	r /= denominator, g /= denominator, b /= denominator;
	return *this;
}

//Overrided plus-equals operator for the Rgb struct
Image::Rgb& Image::Rgb::operator+=(const Rgb &rgb)
{
	//std::cout << "RGB += operator used! woop woop!" << std::endl;
	r += rgb.r, g += rgb.g, b += rgb.b;
	return *this;
}






//******************************************************************
//*************Image class constructor and destructors**************
//******************************************************************

//Constructor to create an empty image 
Image::Image() : width(0), height(0), pixels(nullptr)
{}

//Constructor to create an image with a user specified resolution (defualt colour of black)
Image::Image(const unsigned int &_width, const unsigned int &_height, const Rgb &colour) : width(_width), height(_height), pixels(nullptr)
{
	//Allocate a new array 
	pixels = new Rgb[width * height];

	//Loop through the pixels annd assign the colour for each element
	for (unsigned int i = 0; i < width * height; ++i)
		pixels[i] = colour;
}

//Copy constructor
Image::Image(const Image &img) : width(img.width), height(img.height), colourRange(img.colourRange), bpp(img.bpp), pixels(nullptr)
{
	//Assign the pixels array
	pixels = new Rgb[width * height];

	//Copy the pixels from the passed image refeerence and assign to the pixel array
	for (unsigned int i = 0; i < img.width * img.height; ++i)
		pixels[i] = img.pixels[i];
}

//Move constructor
Image::Image(Image &&img) : width(0), height(0), colourRange(0), bpp(0), pixels(nullptr)
{
	//Assign the width, height, colour range and bits per pixel
	width = img.width;
	height = img.height;
	colourRange = img.colourRange;
	bpp = img.bpp;
	pixels = img.pixels;

	//NULL all the img member variables
	img.width = NULL;
	img.height = NULL;
	img.colourRange = NULL;
	img.bpp = NULL;
	img.pixels = nullptr;
}

//Image destructor
Image::~Image()
{
	//If the pixels points to an object
	if (pixels != nullptr)
	{
		//Deallocate the pixels and set to NULL
		delete[] pixels;
		pixels = NULL;
	}
}





//******************************************************************
//**********************Image class functions***********************
//******************************************************************

//Image info function
std::string Image::imageInfo()
{
	//Get the image info and return it
	std::string info =
		"image info:\nWidth: "
		+ std::to_string(width)

		+ "\nHeight: "
		+ std::to_string(height)

		+ "\nColour Range: RGB values from: 0 - "
		+ std::to_string(colourRange)

		+ "\nBit Per Pixel: "
		+ std::to_string(getBPP());

	return info;
}

//Virtual nearest neighbor resample function
bool Image::nearestNeighbourResample(const float scaleFactor) { return false; }

//Virtual bilinear resample function
bool Image::bilinearResample(const float scaleFactor) { return false; }

//Virtual region of interest function
bool Image::regionOfInterest(unsigned int xPos, unsigned int yPos, unsigned int widthROI, unsigned int heightROI) { return false; }






//Mutator functions for width, height, colour range, bpp, and filename member variables
void Image::setWidth(const unsigned int _width)
{ width = _width; }

void Image::setHeight(const unsigned int _height)
{ height = _height; }

void Image::setColourRange(const unsigned int _colourRange)
{ colourRange = _colourRange; }

void Image::setBPP(const unsigned int _bpp)
{ bpp = _bpp; }

void Image::setFilename(std::string _filename)
{ filename = _filename; }

void Image::setImageProperties(const unsigned int _width, const unsigned int _height, const unsigned int _colourRange, const unsigned int _bpp)
{ width = _width;	height = _height; colourRange = _colourRange; bpp = _bpp; }


//Accessor functions for width, height, colour range, bpp, and filename member variables
const unsigned int Image::getWidth() const
{ return width; }

const unsigned int Image::getHeight() const 
{ return height; }

const unsigned int Image::getColourRange() const
{ return colourRange; }

const unsigned int Image::getBPP() const
{ return bpp; }

const std::string Image::getFilename() const
{ return filename; }






//******************************************************************
//***************Image class overloaded operators*******************
//******************************************************************

//Overrided index operator
const Image::Rgb& Image::operator[](const unsigned int &index) const
{
	return pixels[index];
}

//Overrided index operator
Image::Rgb& Image::operator[](const unsigned int &index)
{
	return pixels[index];
}

//Overrided assignment operator
Image& Image::operator=(const Image &img)
{
	//If the pixels points to an object
	if (pixels != nullptr)
	{
		//Deallocate the pixels and set to NULL
		delete[] pixels;
		pixels = NULL;
	}

	//Assign the width, height, colour range and bits per pixel
	width = img.width;
	height = img.height;
	colourRange = img.colourRange;
	bpp = img.bpp;

	//Allocate the pixels of assigning image
	pixels = new Rgb[img.width * img.height];

	//Copy over the data from the img to the pixels of the assigned image
	for (unsigned int i = 0; i < img.width * img.height; ++i)
		pixels[i] = img.pixels[i];

	//return image
	return *this;
}

//Move assignment operator
Image& Image::operator=(Image &&img)
{
	//If this object is not equal to the passed image reference
	if (this != &img)
	{
		//If the pixels points to an object
		if (pixels != nullptr)
		{
			//Deallocate the pixels and set to NULL
			delete[] pixels;
			pixels = NULL;
		}

		//Assign the width, height, colour range and bits per pixel
		width = img.width;
		height = img.height;
		colourRange = img.colourRange;
		bpp = img.bpp;
		pixels = img.pixels;

		//NULL all the img member variables
		img.width = NULL;
		img.height = NULL;
		img.colourRange = NULL;
		img.bpp = NULL;
		img.pixels = nullptr;
	}

	//return image
	return *this;
}

//Overrided plus operator
Image& Image::operator+(const Image &img)
{
	//Allocate a temp image
	Image tempImg(*this);

	//Loop the the image and assign the values to the temp image
	for (unsigned int i = 0; i < width * height; ++i)
		tempImg[i] += img[i];

	//Return image
	return *this;
}

//Overrided plus-equals operator
Image& Image::operator+=(const Image &img)
{
	//Loop throught the images and add the index values
	for (unsigned int i = 0; i < width * height; ++i)
		pixels[i] += img[i];

	//Return image
	return *this;
}