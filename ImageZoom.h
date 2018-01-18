#pragma once

//Include directives
#include <string>
#include "Image.h"

//ImageZoom class - Inherited from Image class
class ImageZoom : public Image
{
public:
	//Defualt constructor
	ImageZoom();

	//Overloaded constructor
	ImageZoom(const unsigned int &_width, const unsigned int &_height);

	//Destructor
	~ImageZoom();





	//Nearest Neighbor resample (base class virtual function which has been redefined in this class)
	virtual bool nearestNeighbourResample(const float scaleFactor);

	//Bilinear resample (base class virtual function which has been redefined in this class)
	virtual bool bilinearResample(const float scaleFactor);

	//User defined region of interest function (overloaded)
	virtual bool regionOfInterest(unsigned int xPos, unsigned int yPos, unsigned int widthROI, unsigned int heightROI);





	//Image info function
	virtual std::string imageInfo();

	//Accessor and mutator functions for the image zoom factor
	void setZoomFactor(const float zoom);
	const float getZoomFactor() const;





	//Using the image overloaded assignment operator
	using Image::operator=;

	//Overloaded assignment operator
	ImageZoom& operator=(const ImageZoom &img);

private:
	//The zoom factor of the image (if it has been scaled)
	float zoomFactor;
};