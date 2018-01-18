//Include directives
#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>
#include "Image.h"
#include "ImageZoom.h"
#include "ReadWriteImage.h"
#include "ImageBlending.h"

//Function prototype which returns the stacker image file names as a vector
std::vector<std::string> stackerFiles(const unsigned int numbOfFiles);

//Function prototype which displays image info (Polymorphism)
void displayImageInfo(Image* img);

//Load all images function prototype
std::vector<Image> readImages(ReadWriteImage* readWriteImage, std::vector<std::string> images);

//Main entry point
int main()
{
	//******************************************************************
	//********************Console Output - Title************************
	//******************************************************************
	std::cout << "******************************************************************************" << std::endl;
	std::cout << "**************************Image Stacker / Image Scaler************************" << std::endl;
	std::cout << "******************************************************************************" << std::endl;

	//Create a program objects
	ReadWriteImage *readWriteImage = new ReadWriteImage();
	ImageBlending *imageBlender = new ImageBlending();

	//Vector to hold the stacker image file names as strings
	std::vector<std::string> stackerFileNames = stackerFiles(13);

	//String to hold the zoom image file name
	std::string zoomImage = "Images/Zoom/zIMG_1.ppm";

	//Creates a timer
	std::clock_t start = 0;
	double duration = 0;

	//Creates a timer
	std::clock_t fullProg = std::clock();
	double fullProgTime = 0;

	//Memory dellocation check
	{

	//Vector of images
	std::vector<Image> stackerImageFiles = readImages(readWriteImage, stackerFileNames);





		//******************************************************************
		//*********************Mean Blending of Images**********************
		//******************************************************************

		std::cout << "******************************************************************************" << std::endl;
		std::cout << "*******************************Mean Blending Images***************************" << std::endl;
		std::cout << "******************************************************************************" << std::endl;

		//Create a image object (to be assigned the blended images)
		Image *meanImage = new Image();

		//Start the timer
		start = std::clock();

		//If the mean blending is successfull write the image to file
		if (imageBlender->meanBlending(meanImage, stackerFileNames[0], stackerImageFiles))
		{
			//Calculate the time taken to complete the operation
			duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

			//Console output the time
			std::cout << "Time taken to complete mean blending operation: " << duration << " seconds\n" << std::endl;


			//Console output the image info
			std::cout << "Mean blended "; displayImageInfo(meanImage); std::cout << std::endl << std::endl;

			//Write the image to file
			readWriteImage->writePPM(*meanImage, "Images/ImageStacker_set1/meanBlended_PPM.ppm");
		}
		else
			//Console output the time
			std::cout << "Failed to blend images" << std::endl << std::endl;


		//Deallocate memory
		delete meanImage;
		meanImage = NULL;




		//******************************************************************
		//********************Median Blending of Images*********************
		//******************************************************************

		std::cout << "******************************************************************************" << std::endl;
		std::cout << "******************************Median Blending Images**************************" << std::endl;
		std::cout << "******************************************************************************" << std::endl;

		//Create a image object (to be assigned the blended images)
		Image *medianImage = new Image();

		//Start the timer
		start = std::clock();

		//If the median blending is successfull write the image to file
		if (imageBlender->medianBlending(medianImage, stackerFileNames[0], stackerImageFiles))
		{
			//Calculate the time taken to complete the operation
			duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

			//Console output the time
			std::cout << "Time taken to complete median blending operation: " << duration << " seconds\n" << std::endl;


			//Console output the image info
			std::cout << "Median blended "; displayImageInfo(medianImage); std::cout << std::endl << std::endl;

			//Write the image to file
			readWriteImage->writePPM(*medianImage, "Images/ImageStacker_set1/medianBlended_PPM.ppm");

		}
		else
			//Console output the time
			std::cout << "Failed to blend images" << std::endl << std::endl;

		//Deallocate memory
		delete medianImage;
		medianImage = NULL;





		//******************************************************************
		//****************Sigma Clipping Blending of Images*****************
		//******************************************************************

		std::cout << "******************************************************************************" << std::endl;
		std::cout << "************************Sigma Clipping Blending of Images*********************" << std::endl;
		std::cout << "******************************************************************************" << std::endl;


		//Create a image object (to be assigned the blended images)
		Image *sigmaClippedMean = new Image();

		//Start the timer
		start = std::clock();

		//If the sigma clipping mean blending is successfull write the image to file
		if (imageBlender->sigmaClippedMeanBlending(sigmaClippedMean, 1.5f, stackerFileNames[0], stackerImageFiles))
		{
			//Calculate the time taken to complete the operation
			duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

			//Console output the time
			std::cout << "Time taken to complete sigma clipped mean blending operation: " << duration << " seconds\n"
				<< "Please note - Sigma Clipping is iterated 3 times on the mean/sigma clipped mean image.\n" << std::endl;

			//Console output the image info
			std::cout << "Sigma Clipped Mean "; displayImageInfo(sigmaClippedMean); std::cout << std::endl << std::endl;

			//Write the image to file
			readWriteImage->writePPM(*sigmaClippedMean, "Images/ImageStacker_set1/sigmaClippedMeanBlended_PPM.ppm");

		}
		else
			//Console output the time
			std::cout << "Failed to blend images" << std::endl << std::endl;

		//Deallocate memory
		delete sigmaClippedMean;
		sigmaClippedMean = NULL;

	}




	//******************************************************************************
	//*************************Image Scaling - Nearest Neighbor*********************
	//******************************************************************************

	std::cout << "******************************************************************************" << std::endl;
	std::cout << "*************************Image Scaling - Nearest Neighbor*********************" << std::endl;
	std::cout << "******************************************************************************" << std::endl;

	//Scale image using nearest neighbor resampling (X2)
	Image *nnZoom1 = new ImageZoom();
	*nnZoom1 = readWriteImage->readPPM(zoomImage.c_str());

	//Start the timer
	start = std::clock();

	//If the nearest neighbor resample is successfull write the image to file
	if (nnZoom1->nearestNeighbourResample(2.0f))
	{
		//Calculate the time taken to complete the operation
		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

		//Console output the time
		std::cout << "Time taken to complete X2 nearest neighbor scaling operation: " << duration << " seconds" << std::endl << std::endl;

		//Console output the image info
		std::cout << "Nearest Neighbor X2 scaled "; displayImageInfo(nnZoom1); std::cout << std::endl << std::endl;

		//Write the image to file
		readWriteImage->writePPM(*nnZoom1, "Images/Zoom/nearestNeighbor_X2_PPM.ppm");
	}
	else
		//Console output the time
		std::cout << "Failed to scale images" << std::endl << std::endl;

	//Deallocate Memory
	delete nnZoom1;
	nnZoom1 = NULL;





	//Scale image using nearest neighbor resampling (X4)
	Image *nnZoom2 = new ImageZoom();
	*nnZoom2 = readWriteImage->readPPM(zoomImage.c_str());

	//Start the timer
	start = std::clock();

	//If the nearest neighbor resample is successfull write the image to file
	if (nnZoom2->nearestNeighbourResample(4.0f))
	{
		//Calculate the time taken to complete the operation
		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

		//Console output the time
		std::cout << "Time taken to complete X4 nearest neighbor scaling operation: " << duration << " seconds" << std::endl << std::endl;

		//Console output the image info
		std::cout << "Nearest Neighbor X4 scaled "; displayImageInfo(nnZoom2); std::cout << std::endl << std::endl;

		//Write the image to file
		readWriteImage->writePPM(*nnZoom2, "Images/Zoom/nearestNeighbor_X4_PPM.ppm");
	}
	else
		//Console output the time
		std::cout << "Failed to scale images" << std::endl << std::endl;

	//Deallocate Memory
	delete nnZoom2;
	nnZoom2 = NULL;





	//******************************************************************
	//*********************Image Scaling - Bilinear*********************
	//******************************************************************

	std::cout << "******************************************************************************" << std::endl;
	std::cout << "**************************Image Scaling - Bilinear****************************" << std::endl;
	std::cout << "******************************************************************************" << std::endl;

	//Scale image using bilinear resampling (X2)
	Image *bZoom1 = new ImageZoom();
	*bZoom1 = readWriteImage->readPPM(zoomImage.c_str());

	//Start the timer
	start = std::clock();

	//If the bilinear resample is successfull write the image to file
	if (bZoom1->bilinearResample(2.0f))
	{
		//Calculate the time taken to complete the operation
		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

		//Console output the time
		std::cout << "Time taken to complete X2 bilinear scaling operation: " << duration << " seconds" << std::endl << std::endl;

		//Console output the image info
		std::cout << "Bilinear X2 scaled "; displayImageInfo(bZoom1); std::cout << std::endl << std::endl;

		//Write the image to file
		readWriteImage->writePPM(*bZoom1, "Images/Zoom/bilinear_X2_PPM.ppm");
	}
	else
		//Console output the time
		std::cout << "Failed to scale images" << std::endl << std::endl;

	//Deallocate Memory
	delete bZoom1;
	bZoom1 = NULL;





	//Scale image using nearest neighbor (X4)
	Image *bZoom2 = new ImageZoom();
	*bZoom2 = readWriteImage->readPPM(zoomImage.c_str());

	//Start the timer
	start = std::clock();

	//If the bilinear resample is successfull write the image to file
	if (bZoom2->bilinearResample(4.0f))
	{
		//Calculate the time taken to complete the operation
		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

		//Console output the time
		std::cout << "Time taken to complete X4 bilinear scaling operation: " << duration << " seconds" << std::endl << std::endl;

		//Console output the image info
		std::cout << "Bilinear X4 scaled "; displayImageInfo(bZoom2); std::cout << std::endl << std::endl;

		//Write the image to file
		readWriteImage->writePPM(*bZoom2, "Images/Zoom/bilinear_X4_PPM.ppm");
	}
	else
		//Console output the time
		std::cout << "Failed to scale images" << std::endl << std::endl;

	//Deallocate Memory
	delete bZoom2;
	bZoom2 = NULL;





	//******************************************************************
	//****************Image Scaling - Region of Interest****************
	//******************************************************************

	std::cout << "******************************************************************************" << std::endl;
	std::cout << "*********************Image Scaling - Region of Interest***********************" << std::endl;
	std::cout << "******************************************************************************" << std::endl;

	//Region of interest image allocation
	Image *nnImageROI = new ImageZoom();
	*nnImageROI = readWriteImage->readPPM(zoomImage.c_str());

	//Start the timer
	start = std::clock();

	//If the region of intereset selection is successfull, scale the image and write the images to file
	if (nnImageROI->regionOfInterest(300, 300, 150, 150))
	{
		//Calculate the time taken to complete the operation
		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

		//Console output the time
		std::cout << "Time taken to complete region of interest selection operation: " << duration << " seconds" << std::endl << std::endl;

		//Console output the image info
		std::cout << "Region of Interest "; displayImageInfo(nnImageROI); std::cout << std::endl << std::endl;

		//Write the image to file
		readWriteImage->writePPM(*nnImageROI, "Images/Zoom/ROI_1.ppm");

		//Start the timer
		start = std::clock();

		//If the nearest neighbor resample is successfull write the image to file
		if (nnImageROI->nearestNeighbourResample(2.0f))
		{
			//Calculate the time taken to complete the operation
			duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

			//Console output the time
			std::cout << "Time taken to complete X2 nearest neighbor scaling operation: " << duration << " seconds" << std::endl << std::endl;

			//Console output the image info
			std::cout << "Nearest Neighbor scaling X2 scaled Region of Interest "; displayImageInfo(nnImageROI); std::cout << std::endl << std::endl;

			//Write the image to file
			readWriteImage->writePPM(*nnImageROI, "Images/Zoom/ROI_nearestNeighbor_X2_PPM.ppm");
		}
		else
			//Console output the time
			std::cout << "Failed to scale images" << std::endl << std::endl;
	}
	else
		//Console output the time
		std::cout << "Failed to select a region of interest " << std::endl << std::endl;

	//Deallocate Memory
	delete nnImageROI;
	nnImageROI = NULL;





	//Region of interest image allocation
	Image *nnImageROI2 = new ImageZoom();
	*nnImageROI2 = readWriteImage->readPPM(zoomImage.c_str());

	//Start the timer
	start = std::clock();

	//If the region of intereset selection is successfull, scale the image and write the images to file
	if (nnImageROI2->regionOfInterest(300, 300, 150, 150))
	{
		//Calculate the time taken to complete the operation
		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

		//Console output the time
		std::cout << "Time taken to complete region of interest selection operation: " << duration << " seconds" << std::endl << std::endl;

		//Console output the image info
		std::cout << "Region of Interest "; displayImageInfo(nnImageROI2); std::cout << std::endl << std::endl;

		//Write the image to file
		readWriteImage->writePPM(*nnImageROI2, "Images/Zoom/ROI_2.ppm");

		//Start the timer
		start = std::clock();

		//If the nearest neighbor resample is successfull write the image to file
		if (nnImageROI2->nearestNeighbourResample(4.0f))
		{
			//Calculate the time taken to complete the operation
			duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

			//Console output the time
			std::cout << "Time taken to complete X4 nearest neighbor scaling operation: " << duration << " seconds" << std::endl << std::endl;

			//Console output the image info
			std::cout << "Nearest Neighbor scaling X4 scaled Region of Interest "; displayImageInfo(nnImageROI2); std::cout << std::endl << std::endl;

			//Write the image to file
			readWriteImage->writePPM(*nnImageROI2, "Images/Zoom/ROI_nearestNeighbor_X4_PPM.ppm");
		}
		else
			//Console output the time
			std::cout << "Failed to scale images" << std::endl << std::endl;
	}
	else
		//Console output the time
		std::cout << "Failed to select a region of interest " << std::endl << std::endl;

	//Deallocate Memory
	delete nnImageROI2;
	nnImageROI2 = NULL;






	//Region of interest image allocation
	Image *bImageROI = new ImageZoom();
	*bImageROI = readWriteImage->readPPM(zoomImage.c_str());

	//Start the timer
	start = std::clock();

	//If the region of intereset selection is successfull, scale the image and write the images to file
	if (bImageROI->regionOfInterest(250, 250, 100, 100))
	{
		//Calculate the time taken to complete the operation
		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

		//Console output the time
		std::cout << "Time taken to complete region of interest selection operation: " << duration << " seconds" << std::endl << std::endl;

		//Console output the image info
		std::cout << "Region of Interest "; displayImageInfo(bImageROI); std::cout << std::endl << std::endl;

		//Write the image to file
		readWriteImage->writePPM(*bImageROI, "Images/Zoom/ROI_3.ppm");

		//Start the timer
		start = std::clock();

		//If the bilinear resample is successfull write the image to file
		if (bImageROI->bilinearResample(2.0f))
		{
			//Calculate the time taken to complete the operation
			duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

			//Console output the time
			std::cout << "Time taken to complete X2 bilinear scaling operation: " << duration << " seconds" << std::endl << std::endl;

			//Console output the image info
			std::cout << "Bilinear X2 scaled Region of Interest "; displayImageInfo(bImageROI); std::cout << std::endl << std::endl;

			//Write the image to file
			readWriteImage->writePPM(*bImageROI, "Images/Zoom/ROI_bilinear_X2_PPM.ppm");
		}
		else
			//Console output the time
			std::cout << "Failed to scale images" << std::endl << std::endl;
	}
	else
		//Console output the time
		std::cout << "Failed to select a region of interest " << std::endl << std::endl;

	//Deallocate Memory
	delete bImageROI;
	bImageROI = NULL;






	//Region of interest image allocation
	Image *bImageROI2 = new ImageZoom();
	*bImageROI2 = readWriteImage->readPPM(zoomImage.c_str());

	//Start the timer
	start = std::clock();

	//If the region of intereset selection is successfull, scale the image and write the images to file
	if (bImageROI2->regionOfInterest(250, 250, 100, 100))
	{
		//Calculate the time taken to complete the operation
		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

		//Console output the time
		std::cout << "Time taken to complete region of interest selection operation: " << duration << " seconds" << std::endl << std::endl;

		//Console output the image info
		std::cout << "Region of Interest "; displayImageInfo(bImageROI2); std::cout << std::endl << std::endl;

		//Write the image to file
		readWriteImage->writePPM(*bImageROI2, "Images/Zoom/ROI_4.ppm");

		//Start the timer
		start = std::clock();

		//If the ilinear resample is successfull write the image to file
		if (bImageROI2->bilinearResample(4.0f))
		{
			//Calculate the time taken to complete the operation
			duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

			//Console output the time
			std::cout << "Time taken to complete X4 bilinear scaling operation: " << duration << " seconds" << std::endl << std::endl;

			//Console output the image info
			std::cout << "Bilinear X4 scaled Region of Interest "; displayImageInfo(bImageROI2); std::cout << std::endl << std::endl;

			//Write the image to file
			readWriteImage->writePPM(*bImageROI2, "Images/Zoom/ROI_bilinear_X4_PPM.ppm");
		}
		else
			//Console output the time
			std::cout << "Failed to scale images" << std::endl << std::endl;
	}
	else
		//Console output the time
		std::cout << "Failed to select a region of interest " << std::endl << std::endl;

	//Deallocate Memory
	delete bImageROI2;
	bImageROI2 = NULL;






	//******************************************************************
	//***********Deallocate any remaining objects from memory***********
	//******************************************************************

	//Deallocate memory
	delete readWriteImage;
	delete imageBlender;
	readWriteImage = NULL;
	imageBlender = NULL;

	//Calculate the time taken to complete the operation
	fullProgTime = (std::clock() - fullProg) / (double)CLOCKS_PER_SEC;

	//Console output the time
	std::cout << "Time taken to complete all operations: " << fullProgTime << " seconds" << std::endl << std::endl;

	


	//Console output
	std::cout << "BLENDED IMAGES are stored in the: Images/ImageStacker_set1/ folder (of the program directory)" << std::endl;
	std::cout << "SCALED IMAGES are stored in the: Images/Zoom/ folder (of the program directory)" << std::endl;
	std::cout << "Please refer to these folders to view the processed images ... " << std::endl;
	std::cout << "press any key to continue ..." << std::endl;

	//Wait for user input
	std::cin.get();

	//Program execution success
	return 0;
}

//Function which returns the 'stacker image file names' as a vector
std::vector<std::string> stackerFiles(const unsigned int numbOfFiles)
{
	//Creates a vector of filenames for the image stacking
	std::vector<std::string> stackerFileNames(numbOfFiles);

	//Assign all of the stacker filenames to a vector
	for (unsigned int i = 0; i < numbOfFiles; i++)
		stackerFileNames[i] = ("Images/ImageStacker_set1/IMG_" + std::to_string(i + 1) + ".ppm").c_str();

	//Return the vector
	return stackerFileNames;
}

//Dsiplay the image info function 
void displayImageInfo(Image* img)
{
	//Console output
	std::cout << img->imageInfo() << std::endl;
}

//Load all images function
std::vector<Image> readImages(ReadWriteImage* readWriteImage, std::vector<std::string> images)
{
	//Console output
	std::cout << "Loading images, please wait ..." << std::endl;

	//Vector of images
	std::vector<Image> imagesVector;

	//Loop through the files
	for (unsigned int fileNumber = 0; fileNumber < images.size(); fileNumber++)
	{
		//Create a temp image
		Image tempImage;

		//Read the image at 'filenumber' and assign it to the temp image
		tempImage = readWriteImage->readPPM(images[fileNumber].c_str());

		//Push the image to the vector
		imagesVector.push_back(tempImage);
	}

	//Return images
	return imagesVector;
}