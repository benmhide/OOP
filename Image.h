#pragma once

//Include directives
#include <cstdlib> 
#include <cstdio>
#include <string>
#include <iostream>

//Image class
class Image
{
public:
	//Rgb structure, i.e. a pixel 
	struct Rgb
	{
		//Rgb struct constructor 
		Rgb();

		//Rgb struct constructor takes 1 param (colour) i.e. grey scale
		Rgb(float colour);

		//Rgb structure constructor takes 3 params (red/green/blue)
		Rgb(float _red, float _green, float _blue);





		//Mutator functions for red, green and blue Rgb member variables
		void setRed(const float _red);
		void setGreen(const float _green);
		void setBlue(const float _blue);

		//Accessor functions for red, green and blue Rgb member variables
		const float getRed() const;
		const float getGreen() const;
		const float getBlue() const;

		//Mutator functions to add rgb values together, divide rgb values by a divisor value and set all rgb values
		void addRgb(const float _red, const float _green, const float _blue);
		void divideRgb(const float denominator);
		void setRgb(const float _red, const float _green, const float _blue);





		//Overrided friend plus-equals operator for the Rgb struct
		friend float& operator+=(float &friendFloat, const Rgb rgb)
		{
			friendFloat += (rgb.r + rgb.g + rgb.b) / 3.0f;
			return friendFloat;
		}

		//Overrided not equal to operators for the Rgb struct
		bool operator!=(const Rgb &colour) const;

		//Overrided multiply-equals operators for the Rgb struct
		Rgb& operator*=(const Rgb &rgb);

		//Overrided multiply-equals operators for the Rgb struct
		Rgb& operator/=(const float &denominator);

		//Overrided plus-equals operator for the Rgb struct
		Rgb& operator+=(const Rgb &rgb);




	private:
		//Floats to store the red/green/blue rgb values
		float r, g, b;

	};




	//Constructor to create an empty image 
	Image(); 

	//Constructor to create an image with a user specified resolution (defualt colour of black)
	Image(const unsigned int &_width, const unsigned int &_height, const Rgb &colour = statConstBlack);

	//Copy constructor
	Image(const Image &img);
	
	//Move constructor
	Image::Image(Image &&img);
	
	//Image destructor
	~Image();




	//Image info function
	virtual std::string imageInfo();

	//Virtual nearest neighbor resample function
	virtual bool nearestNeighbourResample(const float scaleFactor);

	//Virtual bilinear resample function
	virtual bool bilinearResample(const float scaleFactor);

	//Virtual region of interest function
	virtual bool regionOfInterest(unsigned int xPos, unsigned int yPos, unsigned int widthROI, unsigned int heightROI);




	//Mutator functions for width, height, colour range, and filename member variables
	void setWidth(const unsigned int _width);
	void setHeight(const unsigned int _height);
	void setColourRange(const unsigned int _colourRange);
	void setBPP(const unsigned int _bpp);
	void setFilename(const std::string _filename);
	void setImageProperties(const unsigned int _width, const unsigned int _height, const unsigned int _colourRange, const unsigned int _bpp);

	//Accessor functions for width, height, colour range, and filename member variables
	const unsigned int getWidth() const;
	const unsigned int getHeight() const;
	const unsigned int getColourRange() const;
	const unsigned int getBPP() const;
	const std::string getFilename() const;




	//Overrided index operator
	const Rgb& operator[](const unsigned int &index) const;

	//Overrided index operator
	Rgb& operator[](const unsigned int &index);

	//Overrided assignment operator
	Image& operator=(const Image &img);

	//Move assignment operator
	Image& operator=(Image &&img);

	//Overrided plus operator
	Image& operator+(const Image &img);

	//Overrided plus-equals operator
	Image& operator+=(const Image &img);




	//1D array of pixels
	Rgb *pixels;

private:
	//The width and the height of the image - Image resolution
	unsigned int width, height, colourRange, bpp;

	//Image filename
	std::string filename;

	//Preset colors for black, white, red, green and blue
	static const Rgb statConstBlack, statConstWhite, statConstRed, statConstGreen, statConstBlue;
};
