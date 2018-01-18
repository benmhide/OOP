//Include directives
#include "ImageBlending.h"

//******************************************************************
//*****************Image Blending class functions*******************
//******************************************************************

//Mean blend image files function
bool ImageBlending::meanBlending(Image *meanImg, const std::string &image, const std::vector<Image> &stackerImages)
{
	//Console output
	std::cout << "Mean blending images, please wait ..." << std::endl;

	//Try to open the files and mean blend the images
	try
	{
		//If the vector of filenames is empty report failure
		if (stackerImages.size() <= 0) { fprintf(stderr, "No images to stack!"); return false; }

		//Assign value to hole the size of the images vector
		float imgSize = (float)stackerImages.size();

		//Assign the first file to the mean image
		*meanImg = readWritePPM.readPPM(image.c_str());

		//Loop through the files
		for (unsigned int fileNumber = 1; fileNumber < imgSize; fileNumber++)
		{
			//Loop through the pixels of each file and sum up all the Rgb values from all the images
			for (unsigned int i = 0; i < (meanImg->getWidth() * meanImg->getHeight()); i++)
				meanImg->pixels[i].addRgb(stackerImages[fileNumber].pixels[i].getRed(), stackerImages[fileNumber].pixels[i].getGreen(), stackerImages[fileNumber].pixels[i].getBlue());
		}

		//Loop through all the pixels of the mean image and divide thier values by the number of files
		for (unsigned int i = 0; i < (meanImg->getWidth() * meanImg->getHeight()); i++)
			meanImg->pixels[i].divideRgb(imgSize);

		//Console output
		std::cout << "Images mean blended!" << std::endl;
		return true;
	}

	//Catch any errors
	catch (const char *err)	{ fprintf(stderr, "%s\n", err);	return false;}
}

//Median blend image files function
bool ImageBlending::medianBlending(Image *medianImg, const std::string &image, const std::vector<Image> &stackerImages)
{
	//Console output
	std::cout << "Median blending images, please wait ..." << std::endl;

	//Try to open the files and mean blend the images
	try
	{
		//If the vector of filenames is empty report failure
		if (stackerImages.size() <= 0) { fprintf(stderr, "No images to stack!"); return false; }

		//Assign value to hole the size of the images vector
		float imgSize = (float)stackerImages.size();

		//Assign the first file to the mean image
		*medianImg = readWritePPM.readPPM(image.c_str());

		//Pointer to arrays to hold the red, green and blue pixel values for each pixel in each image at a given location
		float *reds = new float[(int)imgSize];
		float *greens = new float[(int)imgSize];
		float *blues = new float[(int)imgSize];

		//Loop through each pixel in the images
		for (unsigned int i = 0; i < medianImg->getWidth() * medianImg->getHeight(); i++)
		{
			//Loop through each file and assign the images pixel values (r/g/b) to the arrays to hold the median values (reds/greens/blues)
			for (unsigned int k = 0; k < imgSize; k++)
			{
				//If the image to be processed is the first image use the medain image values
				if (k == 0)
				{
					reds[k] = medianImg->pixels[i].getRed();
					greens[k] = medianImg->pixels[i].getGreen();
					blues[k] = medianImg->pixels[i].getBlue();
				}

				//Else use the temp image values 
				else
				{
					reds[k] = stackerImages[k].pixels[i].getRed();
					greens[k] = stackerImages[k].pixels[i].getGreen();
					blues[k] = stackerImages[k].pixels[i].getBlue();
				}
			}

			//Find the median element of the arrays holding the red, green and blue values
			std::nth_element(reds, (reds + (stackerImages.size() / 2)), (reds + stackerImages.size()));
			std::nth_element(greens, (greens + (stackerImages.size() / 2)), (greens + stackerImages.size()));
			std::nth_element(blues, (blues + (stackerImages.size() / 2)), (blues + stackerImages.size()));

			//Assign the median value of each channel of each pixel to the medianImg 
			medianImg->pixels[i].setRgb(reds[stackerImages.size() / 2], greens[stackerImages.size() / 2], blues[stackerImages.size() / 2]);
		}

		delete[] reds; delete[] greens;	delete[] blues;
		reds = NULL; greens = NULL; blues = NULL;

		//Console output
		std::cout << "Images mean blended!" << std::endl;
		return true;
	}

	//Catch any errors
	catch (const char *err) { fprintf(stderr, "%s\n", err);	return false; }
}

//Sigma clipped mean blend image files function
bool ImageBlending::sigmaClippedMeanBlending(Image *scMeanImg, const float &sigma, const std::string &image, const std::vector<Image> &stackerImages)
{
	//Console output
	std::cout << "Sigma clipped mean blending images, please wait ..." << std::endl;

	//Try to open the files and sigma clip mean blend the images
	try
	{
		//If the vector of filenames is empty report failure
		if (stackerImages.size() <= 0) { fprintf(stderr, "No images to stack!"); return false; }

		//Assign value to hole the size of the images vector
		float imgSize = (float)stackerImages.size();

		///Step 1: Calculate mean
		//Create a image object (to be assigned the blended images)
		Image *sMeanImage;

		//Itterate the sigma clipping proccess 3 times
		for (unsigned int itterations = 0; itterations < 3; itterations++)
		{
			//On the first itteration mean blend the exsisting image set
			if (itterations == 0)
			{
				//Create the mean blending image
				sMeanImage = new Image();

				//Mean blending was unsuccessfull
				if (!meanBlending(sMeanImage, image, stackerImages)) { fprintf(stderr, "Unable to mean blend the images!"); return false; }

				//Set the sigma clipped image properties
				scMeanImg->setImageProperties(sMeanImage->getWidth(), sMeanImage->getHeight(), sMeanImage->getColourRange(), sMeanImage->getBPP());
			}

			//On ever other itteration use the last sigma clipped image as the mean image
			else
			{
				//Assign the sigma clipped mean blended image
				sMeanImage = new Image(*scMeanImg);
			}

			///Step 2: Calculate deviation from mean
			//Allocate a image to hold the mean deviations
			Image *meanDev = new Image(sMeanImage->getWidth(), sMeanImage->getHeight());

			//Loop through the files
			for (unsigned int fileNumber = 0; fileNumber < imgSize; fileNumber++)
			{

				///Step 3: Square each deviation
				//Loop through the pixels of each file calculate the deviation from the mean, square the deviations and sum up all the deviations
				for (unsigned int i = 0; i < (sMeanImage->getWidth() * sMeanImage->getHeight()); i++)
					meanDev->pixels[i].addRgb
					(
						pow((stackerImages[fileNumber].pixels[i].getRed() - sMeanImage->pixels[i].getRed()), 2),
						pow((stackerImages[fileNumber].pixels[i].getGreen() - sMeanImage->pixels[i].getGreen()), 2),
						pow((stackerImages[fileNumber].pixels[i].getBlue() - sMeanImage->pixels[i].getBlue()), 2)
					);
			}

			///Step 4: Find mean of all deviations
			//Loop through all the pixels of the mean deviations image and divide thier values by the number of files
			for (unsigned int i = 0; i < (sMeanImage->getWidth() * sMeanImage->getHeight()); i++)
				meanDev->pixels[i].divideRgb(imgSize);

			///Step 5: Calculate Standard Deviation by taking the square root 
			//Image to hold the square root of the standard deviations
			Image *sqrtStdDev = new Image(sMeanImage->getWidth(), sMeanImage->getHeight());

			//Loop through all the pixels of the mean deviations image and calculate the standard Deviation by taking the square root
			for (unsigned int i = 0; i < (sMeanImage->getWidth() * sMeanImage->getHeight()); i++)
				sqrtStdDev->pixels[i].setRgb(sqrt(meanDev->pixels[i].getRed()), sqrt(meanDev->pixels[i].getGreen()), sqrt(meanDev->pixels[i].getBlue()));

			//Deallocate memory
			delete meanDev;
			meanDev = NULL;

			///Step 6: S(igma Factor) is the user definable Sigma factor. Let S(igma Factor) = 1.5 in this case
			//Initialise the sigma factor value and the lower and upper bounds
			float sigmaValue = sigma;

			///Step 7: Calculate the upper and lower bounds: Mean – S * Standard Deviation -- Min = mean –(1.5 * std dev) -- Max = mean + (1.5 * std dev)
			//Assign images to hold the upper and lower bounds to be clipped
			Image *upperBound = new Image(sMeanImage->getWidth(), sMeanImage->getHeight());
			Image *lowerBound = new Image(sMeanImage->getWidth(), sMeanImage->getHeight());

			//Loop through each pixel and calculate the lower and upper bounds for each pixel
			for (unsigned int i = 0; i < (sMeanImage->getWidth() * sMeanImage->getHeight()); i++)
			{
				//Calculate the values outside of mean (lower bound), mean - sigmaValue * Standard Deviation
				lowerBound->pixels[i].setRgb
				(
					sMeanImage->pixels[i].getRed() - (sigmaValue * sqrtStdDev->pixels[i].getRed()),
					sMeanImage->pixels[i].getGreen() - (sigmaValue * sqrtStdDev->pixels[i].getGreen()),
					sMeanImage->pixels[i].getBlue() - (sigmaValue * sqrtStdDev->pixels[i].getBlue())
				);

				//Calculate the values outside of mean (upper bound), mean + sigmaValue * Standard Deviation
				upperBound->pixels[i].setRgb
				(
					sMeanImage->pixels[i].getRed() + (sigmaValue * sqrtStdDev->pixels[i].getRed()),
					sMeanImage->pixels[i].getGreen() + (sigmaValue * sqrtStdDev->pixels[i].getGreen()),
					sMeanImage->pixels[i].getBlue() + (sigmaValue * sqrtStdDev->pixels[i].getBlue())
				);
			}

			//If the itteration is not the first one delete the exsisting pixel array of the sigma clipped mean image from the last itteration
			if (itterations > 0)
			{
				delete[] scMeanImg->pixels;
				scMeanImg->pixels = NULL;
			}

			//Deallocate memory
			delete sMeanImage; delete sqrtStdDev;
			sMeanImage = NULL; sqrtStdDev = NULL;

			//Image allocated to hold the sigma clipped image
			scMeanImg->pixels = new Image::Rgb[upperBound->getWidth() * upperBound->getHeight()]();

			//Allocate memory to hold the values of the dividers for each channel of the pixels
			unsigned short *rDiv = new unsigned short[scMeanImg->getWidth() * scMeanImg->getHeight()]();
			unsigned short *gDiv = new unsigned short[scMeanImg->getWidth() * scMeanImg->getHeight()]();
			unsigned short *bDiv = new unsigned short[scMeanImg->getWidth() * scMeanImg->getHeight()]();

			///Step 8: Reject values outside of min, max
			//Loop through the files
			for (unsigned int fileNumber = 0; fileNumber < imgSize; fileNumber++)
			{
				//Loop through the pixels of each file
				for (unsigned int i = 0; i < (scMeanImg->getWidth() * scMeanImg->getHeight()); i++)
				{
					//If the red values fall outside the the lower and upper bounds
					if (stackerImages[fileNumber].pixels[i].getRed() <= lowerBound->pixels[i].getRed() || stackerImages[fileNumber].pixels[i].getRed() >= upperBound->pixels[i].getRed())
					{
						//Set the pixel value of the reds to 0 (maybe NULL) and increase the count for the denominator values of the reds
						stackerImages[fileNumber].pixels[i].setRed(NULL);
						rDiv[i]++;
					}

					//If the green values fall outside the the lower and upper bounds
					if (stackerImages[fileNumber].pixels[i].getGreen() <= lowerBound->pixels[i].getGreen() || stackerImages[fileNumber].pixels[i].getGreen() >= upperBound->pixels[i].getGreen())
					{
						//Set the pixel value of the greens to 0 (maybe NULL) and increase the count for the denominator values of the greens
						stackerImages[fileNumber].pixels[i].setGreen(NULL);
						gDiv[i]++;
					}

					//If the blue values fall outside the the lower and upper bounds
					if (stackerImages[fileNumber].pixels[i].getBlue() <= lowerBound->pixels[i].getBlue() || stackerImages[fileNumber].pixels[i].getBlue() >= upperBound->pixels[i].getBlue())
					{
						//Set the pixel value of the blues to 0 (maybe NULL) and increase the count for the denominator values of the blues
						stackerImages[fileNumber].pixels[i].setBlue(NULL);
						bDiv[i]++;
					}

					//Loop through the pixels of each file and sum up all the Rgb values from all the images
					scMeanImg->pixels[i].addRgb(stackerImages[fileNumber].pixels[i].getRed(), stackerImages[fileNumber].pixels[i].getGreen(), stackerImages[fileNumber].pixels[i].getBlue());
				}
			}

			///Step 9: Average the remaining values
			//Loop through all the pixels of the sigma clipped image and divide thier values by the number of files minus the number of clipped values
			for (unsigned int i = 0; i < (scMeanImg->getWidth() * scMeanImg->getHeight()); i++)
				scMeanImg->pixels[i].setRgb(scMeanImg->pixels[i].getRed() / (imgSize - rDiv[i]), scMeanImg->pixels[i].getGreen() / (imgSize - gDiv[i]), scMeanImg->pixels[i].getBlue() / (imgSize - bDiv[i]));

			//Deallocate memory
			delete[] rDiv; delete[] gDiv; delete[] bDiv; delete upperBound;	delete lowerBound;
			rDiv = NULL; gDiv = NULL; bDiv = NULL; upperBound = NULL; lowerBound = NULL;
		}

		//Console output
		std::cout << "Images sigma clipped mean blended!" << std::endl;

		//Return success
		return true;
	}

	//Catch any errors
	catch (const char *err)
	{
		//Report the error
		fprintf(stderr, "%s\n", err);
		return false;
	}
}