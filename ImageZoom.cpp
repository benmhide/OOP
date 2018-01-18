//Include directives
#include "ImageZoom.h"

//******************************************************************
//***********ImageZoom class constructors and destructor************
//******************************************************************

//Constructor to create an empty image 
ImageZoom::ImageZoom() : Image::Image(), zoomFactor(1)
{}

//Constructor to create an image with a user specified resolution (defualt colour of black)
ImageZoom::ImageZoom(const unsigned int &_width, const unsigned int &_height) : Image::Image(_width, _height), zoomFactor(1)
{}

//ImageZoom destructor
ImageZoom::~ImageZoom()
{}





//******************************************************************
//********************ImageZoom class functions*********************
//******************************************************************

//Nearest Neighbor resample
bool ImageZoom::nearestNeighbourResample(const float scaleFactor)
{
	//Console output
	std::cout << "Scaling image (nearest neighbor) by scale factor of: " << scaleFactor << " ... " << std::endl;

	//Create a temp image
	Image tempImg;

	//Try to scale the image
	try
	{
		//If the image is of 0 width or length, report error
		if (getWidth() == 0 || getHeight() == 0) { fprintf(stderr, "Cant resize empty image"); return false; }

		//If the scale factor is less than 0, report error
		else if (scaleFactor <= 0) { fprintf(stderr, "Cant resize by negative scale factor or scale factor of 0"); return false;}

		//Set the temp image width and height
		unsigned int newHeight = (int)(getHeight() * scaleFactor);
		unsigned int newWidth = (int)(getWidth() * scaleFactor);

		//Int to hold the index of the new pixel (tempImg) and to hold the index of the pixel from the image to copy from
		unsigned int newPixel = 0;
		unsigned int pixelToMatch = 0;

		//Creates a a rgb pixel array to the new width and height
		tempImg.pixels = new Rgb[newWidth * newHeight];
		tempImg.setImageProperties(newWidth, newHeight, getColourRange(), getBPP());

		//Loop through the new height (all the pixels using both loops see below)
		for (unsigned int i = 0; i < newHeight; i++)
		{
			//Loop the new width (all the pixels using both loops see above)
			for (unsigned int j = 0; j < newWidth; j++)
			{
				//Calculate the index of the new pixel (tempImg)
				newPixel = (i * (newWidth)) + j;

				//Calculate the pixel from the image to copy to the new image
				pixelToMatch = (((int)(i / scaleFactor) * getWidth())) + ((int)(j / scaleFactor));

				//Assign the red, green, and blue values of the temp image
				tempImg.pixels[newPixel].setRgb(pixels[pixelToMatch].getRed(), pixels[pixelToMatch].getGreen(), pixels[pixelToMatch].getBlue());
			}
		}

		//Assign the zoom factor
		setZoomFactor(scaleFactor);

		//Assign the image
		*this = tempImg;

		//Report success
		std::cout << "Image scaled" << std::endl;
		return true;
	}

	//Catch any errors
	catch (const char *err)	{ fprintf(stderr, "%s\n", err);	return false; }
}

//Bilinear resample
bool ImageZoom::bilinearResample(const float scaleFactor)
{
	//Console output
	std::cout << "Scaling image (bilinear) by scale factor of: " << scaleFactor << " ... " << std::endl;

	//Create a temp image
	ImageZoom tempImg;

	//Try to scale the image
	try
	{
		//If the image is of 0 width or length, report error
		if (getWidth() == 0 || getHeight() == 0) { fprintf(stderr, "Cant resize empty image"); return false; }

		//If the scale factor is less than 0, report error
		else if (scaleFactor <= 0) { fprintf(stderr, "Cant resize by negative scale factor or scale factor of 0"); return false; }

		//Set the temp image width and height
		unsigned int newHeight = (int)(getHeight() * scaleFactor);
		unsigned int newWidth = (int)(getWidth() * scaleFactor);

		//Creates a a rgb pixel array to the new width and height
		tempImg.pixels = new Rgb[newWidth * newHeight];
		tempImg.setImageProperties(newWidth, newHeight, getColourRange(), getBPP());

		//Assign values to hold the x, y and index variables
		unsigned int x, y, index;

		//The scaling factor (ratio) of the x and y axis
		float xRatio = ((float)(getWidth() - 1)) / newWidth;
		float yRatio = ((float)(getHeight() - 1)) / newHeight;

		//Floats to hold the red, green, blue x-difference and y-difference
		float xDiff, yDiff;

		//Integer to hold the offset index
		unsigned int offset = 0;

		//Loop the new height (all the pixels using both loops see below)
		for (unsigned int i = 0; i < newHeight; i++)
		{
			//Loop the new width (all the pixels using both loops see above)
			for (unsigned int j = 0; j < newWidth; j++)
			{
				//Calculate the x and y values
				x = (int)(xRatio * j);
				y = (int)(yRatio * i);

				//Calculates the x-difference and y-difference
				xDiff = (xRatio * j) - x;
				yDiff = (yRatio * i) - y;

				//Assign the index (of the image pixels array)
				index = (y * getWidth() + x);

				//Calculate the red element and assign to the temp image
				tempImg.pixels[offset].setRed
				(
					pixels[index].getRed()*(1 - xDiff)*(1 - yDiff) +
					pixels[index + 1].getRed()*(xDiff)*(1 - yDiff) +
					pixels[index + getWidth()].getRed()*(yDiff)*(1 - xDiff)	+ 
					pixels[index + getWidth() + 1].getRed()*(xDiff*yDiff)
				);
				
				//Calculate the green element and assign to the temp image
				tempImg.pixels[offset].setGreen
				(
					pixels[index].getGreen()*(1 - xDiff)*(1 - yDiff) +
					pixels[index + 1].getGreen()*(xDiff)*(1 - yDiff) +
					pixels[index + getWidth()].getGreen()*(yDiff)*(1 - xDiff) +
					pixels[index + getWidth() + 1].getGreen()*(xDiff*yDiff)
				);

				//Calculate the blue element and assign to the temp image
				tempImg.pixels[offset].setBlue
				(
					pixels[index].getBlue()*(1 - xDiff)*(1 - yDiff) +
					pixels[index + 1].getBlue()*(xDiff)*(1 - yDiff) +
					pixels[index + getWidth()].getBlue()*(yDiff)*(1 - xDiff) +
					pixels[index + getWidth() + 1].getBlue()*(xDiff*yDiff)
				);

				//Increase the index offset
				offset++;
			}
		}

		//Assign the zoom factor
		setZoomFactor(scaleFactor);

		//Assign the image
		*this = tempImg;

		//Report success
		std::cout << "Image scaled" << std::endl;
		return true;
	}

	//Catch any errors
	catch (const char *err)	{ fprintf(stderr, "%s\n", err);	return false; }
}

//User defined region of interest function (overloaded)
bool ImageZoom::regionOfInterest(unsigned int xPos, unsigned int yPos, unsigned int widthROI, unsigned int heightROI)
{
	//Console output
	std::cout << "Selecting a region of interest ... " << std::endl;

	//Create a temp image
	Image imgROI;

	//Try to scale the image
	try
	{
		//If the image is of 0 width or length, report error
		if (getWidth() == 0 || getHeight() == 0) { fprintf(stderr, "Cant select a region of interest from an empty image"); return false; }

		//If the region of intereset selection is outside the bounds of the image, report error
		else if (xPos + widthROI > getWidth() || yPos + heightROI > getHeight()) { fprintf(stderr, "Region of interest falls outside the bounds of the image dimensions"); return false; }

		//Int to hold the index of new pixel and to hold the index of the pixel offset from the image to copy from
		unsigned int index = 0;
		unsigned int offset = 0;

		//Creates a a rgb pixel array to the new width and height
		imgROI.pixels = new Rgb[widthROI * heightROI];
		imgROI.setImageProperties(widthROI, widthROI, getColourRange(), getBPP());

		//Loop the height of the ROI (all the pixels using both loops see below)
		for (unsigned int i = 0; i < heightROI; i++)
		{
			//Loop the width of the ROI (all the pixels using both loops see above)
			for (unsigned int j = 0; j < widthROI; j++)
			{
				//Calculate the offset from the pixel index to copy from
				offset = (xPos + (i * (getWidth())) + j) + (yPos * getHeight());

				//Assign the pixels values to the ROI image
				imgROI.pixels[index].setRgb(pixels[offset].getRed(), pixels[offset].getGreen(), pixels[offset].getBlue());

				//Increase the index
				index++;
			}
		}

		//Assign the image
		*this = imgROI;

		//Report success
		std::cout << "Image Region of Interest selected" << std::endl;
		return true;
	}

	//Catch any errors
	catch (const char *err) { fprintf(stderr, "%s\n", err);	return false; }
}





//Image info function
std::string ImageZoom::imageInfo()
{
	//Get the image info and return it
	std::string info =
		"image info:\nWidth: "
		+ std::to_string(getWidth())

		+ "\nHeight: "
		+ std::to_string(getHeight())

		+ "\nColour Range: RGB values from: 0 - "
		+ std::to_string(getColourRange())

		+ "\nBit Per Pixel: "
		+ std::to_string(getBPP())

		+ "\nScale Factor: "
		+ std::to_string((int)zoomFactor);

	return info;
}

//Mutator functions for the image zoom factor
void ImageZoom::setZoomFactor(const float zoom)
{	zoomFactor = zoom; }

//Accessor functions for the image zoom factor
const float ImageZoom::getZoomFactor() const
{ return zoomFactor; }





//******************************************************************
//*************ImageZoom class overloaded operators*****************
//******************************************************************

//Overloaded assignment operator
ImageZoom& ImageZoom::operator=(const ImageZoom &img)
{
	//Converts a pointer to the base Image class to a ImageZoom pointer
	*this = static_cast<const Image&>(img);

	//If the pixels array exsist delete it
	if (pixels)
	{
		delete[] pixels;
		pixels = NULL;
	}

	//Assign the width, height, colour range and bits per pixel
	setWidth(img.getWidth());
	setHeight(img.getHeight());
	setColourRange(img.getColourRange());
	setBPP(img.getBPP());

	//Allocate the pixels of assigning image
	pixels = new Rgb[img.getWidth() * img.getHeight()];

	//Copy over the data from the img to the pixels of the assigned image
	for (unsigned int i = 0; i < img.getWidth() * img.getHeight(); ++i)
		pixels[i] = img.pixels[i];

	//return image
	return *this;
}