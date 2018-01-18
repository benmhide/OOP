//Include directives
#include "ReadWriteImage.h"

//******************************************************************
//**************** Read ppm files into the code ********************
//*They need to be in 'binary' (P6) with no comments in the header *
//**The first line is the 'P' number - P6 indicates it is a binary,*
//***** then the image dimensions and finally the colour range. ****
//******************************************************************
//This header is then followed by the pixel colour data
//eg:	P6
//		3264 2448
//		255
//******************************************************************
//*********Open a .ppm file in notepad++ to see this header*********
//******************************************************************

//Readfunction that will read image in and returns an image object
Image ReadWriteImage::readPPM(const char *filename)
{	
	//Console output
	//std::cout << "Reading image ..." << std::endl;

	//Creates an input file stream
	std::ifstream inputFileStream;

	//Opens the file to read
	inputFileStream.open(filename, std::ios::binary);

	//Create an image object
	Image readImage;

	//Try to open the file and read the contents
	try
	{
		//If the input file can't be opened to be read
		if (inputFileStream.fail())
			throw("Can't open the input file - is it named correctly/is it in the right directory?");

		//String to hold the image header text data
		std::string header;

		//Assign integers to hole the values for the image width, height and colour range
		unsigned int width, height, colourRange;

		//Get the header of the file
		inputFileStream >> header;

		//Compare the header string to see if it contains 'P6' i.e binary format
		if (strcmp(header.c_str(), "P6") != 0)
			throw("Can't read the input file - is it in binary format (Has P6 in the header)?");

		//Get the width, height and colour range of the image file
		inputFileStream >> width >> height >> colourRange;

		//This will throw an exception if bad_alloc 
		readImage.pixels = new Image::Rgb[width * height];

		//Assign the image properties
		readImage.setImageProperties(width, height, colourRange, (sizeof(char) * 8) * 3);

		//Skip empty lines if necessary until we get to the binary data 
		inputFileStream.ignore(256, '\n');

		//Read each pixel one by one and 
		unsigned char pixel[3];

		//Loop through all the pixels
		for (unsigned int i = 0; i < width * height; ++i)
		{
			//Convert bytes to floats 
			inputFileStream.read(reinterpret_cast<char*>(pixel), 3);

			//Assign the pixels of the image
			readImage.pixels[i].setRed(pixel[0] / 255.0f);
			readImage.pixels[i].setGreen(pixel[1] / 255.0f);
			readImage.pixels[i].setBlue(pixel[2] / 255.0f);
		}

		//Close the input file
		inputFileStream.close();
	}

	//Catch any errors
	catch (const char *err)
	{
		//Report the error
		fprintf(stderr, "%s\n", err);

		//Close the input file
		inputFileStream.close();
	}

	//Console output
	//std::cout << "Image read" << std::endl;

	//Return the image
	return readImage;
}

//Overloaded read function that will read image in from a given std::streampos and returns an image object
Image ReadWriteImage::readPPM(const char *filename, unsigned int imgPart, const std::streampos pos)
{
	//Console output
	//std::cout << "Reading image ..." << std::endl;

	//Creates an input file stream
	std::fstream inputFileStream;

	//Opens the file to read
	inputFileStream.open(filename, std::ios::binary | std::ios::in);

	//Create an image object
	Image readImage;

	//Try to open the file and read the contents
	try
	{
		//If the input file can't be opened to be read
		if (inputFileStream.fail())
			throw("Can't open the input file - is it named correctly/is it in the right directory?");

		//String to hold the image header text data
		std::string header;

		//Assign integers to hole the values for the image width, height and colour range
		unsigned int width, height, colourRange;

		//Get the header of the file
		inputFileStream >> header;

		//Compare the header string to see if it contains 'P6' i.e binary format
		if (strcmp(header.c_str(), "P6") != 0)
			throw("Can't read the input file - is it in binary format (Has P6 in the header)?");

		//Get the width, height and colour range of the image file
		inputFileStream >> width >> height >> colourRange;

		//Allocate a Rgb array (size dependant on imgPart variable passed)
		readImage.pixels = new Image::Rgb[width * (height / imgPart)];

		//Assign the image properties
		readImage.setImageProperties(width, (height / imgPart), colourRange, (sizeof(char) * 8) * 3);

		//Skip empty lines if necessary until we get to the binary data 
		inputFileStream.ignore(256, '\n');

		//Read each pixel one by one and 
		unsigned char pixel[3];

		//Seek to the position of the input stream
		inputFileStream.seekg(pos, std::ios::beg);

		//Loop through all the pixels
		for (unsigned int i = 0; i < width * (height / imgPart); ++i)
		{
			//Convert bytes to floats 
			inputFileStream.read(reinterpret_cast<char*>(pixel), 3);

			//Assign the pixels of the image
			readImage.pixels[i].setRed(pixel[0] / 255.0f);
			readImage.pixels[i].setGreen(pixel[1] / 255.0f);
			readImage.pixels[i].setBlue(pixel[2] / 255.0f);
		}

		//Close the input file
		inputFileStream.close();
	}

	//Catch any errors
	catch (const char *err)
	{
		//Report the error
		fprintf(stderr, "%s\n", err);

		//Close the input file
		inputFileStream.close();
	}

	//Console output
	//std::cout << "Image read" << std::endl;

	//Return the image
	return readImage;
}





//******************************************************************
//****************Write data out to a ppm file**********************
//************Constructs the header for the image file**************
//******************************************************************

//Write function which writexs an image object to file
void ReadWriteImage::writePPM(const Image &writeImg, const char *filename)
{
	//Console output
	//cout << "File name: " << filename << endl;
	std::cout << "Writing image ..." << std::endl;

	//If the image has no width or height values - cant write the image to file
	if (writeImg.getWidth() == 0 || writeImg.getHeight() == 0) { fprintf(stderr, "Can't save an empty image\n"); return; }

	//Create a new output file stream
	std::ofstream outputFileStream;

	//Try to open the file and write the image data to it
	try
	{
		//Open the file stream to write to. Need to spec. binary mode for Windows users 
		outputFileStream.open(filename, std::ios::binary);

		//If the file can't be opened to write to
		if (outputFileStream.fail())
			throw("Can't open output file");

		//Add the header to the image file
		outputFileStream << "P6\n" << writeImg.getWidth() << " " << writeImg.getHeight() << "\n255\n";

		//Create 'bytes' to hold the red, blue and green values 
		unsigned char red, green, blue;

		//Loop over each pixel in the image
		for (unsigned int i = 0; i < writeImg.getWidth() * writeImg.getHeight(); ++i)
		{
			//Clamp and convert to byte format
			red = static_cast<unsigned char>(std::min(1.0f, writeImg.pixels[i].getRed()) * 255);
			green = static_cast<unsigned char>(std::min(1.0f, writeImg.pixels[i].getGreen()) * 255);
			blue = static_cast<unsigned char>(std::min(1.0f, writeImg.pixels[i].getBlue()) * 255);

			//Write the pixel colour values to the file
			outputFileStream << red << green << blue;
		}

		//Close the output file stream
		outputFileStream.close();

		//Confirm image written to file
		std::cout << "Image written" << std::endl;
	}

	//Catch any errors
	catch (const char *err)
	{
		//Report the error
		fprintf(stderr, "%s\n", err);

		//Close the output file stream
		outputFileStream.close();
	}
}