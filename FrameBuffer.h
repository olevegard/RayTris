#pragma once

#include <vector>

//#include "math/Vector3f.h"
/**
  * Our framebuffer class is essentially just a wrapper for a pointer to
  * memory where we store our output pixels
  */
class FrameBuffer {
public:
	FrameBuffer(unsigned int width_, unsigned int height_)
	: data{ 0.0f} 
	, data_test{ 0.0f }
	, width( width_ ) 
	, height( height_ )
	{
		this->width = width;
		this->height = height;
		data.resize(width*height*3);
	}

	inline unsigned int getWidth() { return width; }
	inline unsigned int getHeight() {return height; }
	inline const std::vector<float>& getData() { return data; }

	/**
	  * Sets the pixel at (i, j) to the color (r, g, b).
	  */
	inline void setPixel(unsigned int i, unsigned int j, Vector3f color)
	{

		unsigned int index = 3*(i+j*width);

		data.at(index) = color.x;
		data.at(index+1) = color.y;
		data.at(index+2) = color.z;
/*
		data_test[ index ] = color.x;
		data_test[ index + 1 ] = color.x;
		data_test[ index + 2 ] = color.x;
*/

	}

private:
	std::vector<float> data;
	float data_test[1600 * 1200 * 3];
	//float data_test[400 * 300 * 3];
	unsigned int width, height;
};
