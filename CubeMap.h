#pragma once

#include <string>
#include <limits>
#include <sstream>
#include <stdexcept>

//#include <glm/glm.hpp>
//#include "math/Vector3f.h"
#include "SceneObject.h"

#include <IL/il.h>
#include <IL/ilu.h>

class CubeMap : public SceneObject {
public:
	CubeMap(std::string posx, std::string negx, 
			std::string posy, std::string negy,
			std::string posz, std::string negz)
	:	posx()
	,	negx()
	,	posy()
	,	negy()
	,	posz()
	,	negz()
	{

		ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

		// Load our images
		loadImage(posx, this->posx);
		loadImage(negx, this->negx);
		loadImage(posy, this->posy);
		loadImage(negy, this->negy);
		loadImage(posz, this->posz);
		loadImage(negz, this->negz);
	}
	
	virtual ~CubeMap(){ std::cout << "Cubemap destr" << std::endl; }
	/**
	  * Ray-trace function that returns what texel you hit in the
	  * cube map, since any ray will hit some point in the cube map
	  */
	Vector3f rayTrace(Ray &ray, const float& t, RayTracerState& state) {

		////std::cout << "Ray tracing cubemap\n";
		Vector3f out_color(0.0f);

		// Since z always has the value of -1.0f, we need to normalize the vector in order to get the proper values. 
		// ( if we didn't normalize it, x would never be larger than z ) 
		Vector3f dir =/* Math::normalize*/ ( ray.getDirection() );

		float x = fabs(dir.x);
		float y = fabs(dir.y);
		float z = fabs(dir.z);

		//std::cout << "\tcollisio with cube map at : " << dir << std::endl;
/*	
		// Find the largest absolute value 
		double largest = 0.0;
		//Math::max( x, y );largest = Math::max( largest, z );

		if ( x > y ) 
			largest = x;
		
		if ( z > largest )
			largest = z;
*/
		//std::cout << "largest : " << largest << std::endl;
		//std::cout << "xyz : " << x << " , " << y << " , " << z << std::endl;
		
		// Compare largest to the diffeent values. 
		// This aproach is easy to read ( comparte to if ( x > y && x > z ) )
		// And at the same time we avoid those black lines separating the faces of the cubemap 
		// we get when using if ( x > y && x > z ) to check wich side tha ray hits
		
		// Check if we are hitting one of the sides
		//if  ( x == largest )
		if ( x > y && x > z )
		{
			if ( dir.x > 0.0f ) {
				// Right side
				//std::cout << "right\n";
				double s = 0.5f + 0.5f * dir.z/dir.x;
				double t = 0.5f - 0.5f * dir.y/dir.x;
		
				out_color = readTexture(negx , s, t); 
			} else  {
				// Left side
				//std::cout << "left\n";
				double s = 0.5f + 0.5f * dir.z/dir.x;
				double t = 0.5f *  dir.y / dir.x + 0.5f;
			
				out_color = readTexture( posx, s, t); 
			}

		} 
		
		// Check if we are hitting one top or bottom
		//if  ( y == largest )
		else if ( y > x && y > z )
		{
			if ( dir.y > 0.0f ) {
				// Top
				//std::cout << "top\n";
				//float s = 0.5f - 0.5f * dir.x/dir.y;
				//float t = 0.5f + 0.5f * dir.z/dir.y;
			
				float s = 0.5f - 0.5f * dir.x/dir.y;
				float t = 0.5f + 0.5f * dir.z/dir.y;
/*
				std::cout <<  "hit y \n";
				std::cout << "dir : " << dir << std::endl;
				std::cout << "s , t : " << s << " , " << t << std::endl;
				std::cin.ignore();
				*/
				out_color = readTexture( posy, s, t); 
			} else  {
				// Bottom
				//std::cout << "bottom\n";
				float s = 0.5f + 0.5f * dir.x/dir.y;
				float t = 0.5f *  dir.z / dir.y + 0.5f;
				
				out_color = readTexture( negy, s, t); 
			}
		} 
		// Check if we are hitting one top or bottom
		//if  ( z == largest )
		else if ( z > x && z > y )
		{
			if ( dir.z > 0.0f ) {
				// Back
				//std::cout << "back\n";
				//std::cout << "dir : " << dir << std::endl;

				float s = 0.5f - 0.5f * dir.x/dir.z;
				float t = 0.5f - 0.5f * dir.y/dir.z;

				//std::cout << "s : " << s << std::endl;
				//std::cout << "t : " << t << std::endl;

				//std::cin.ignore();
			
				out_color = readTexture(posz , s, t); 
			} else  {
				// Front 
				//std::cout << "front\n";
				float s = 0.5f - 0.5f * dir.x / dir.z;
				float t = 0.5f *		dir.y / dir.z + 0.5f;
			
				out_color = readTexture(negz , s, t); 
			}
	
		}  
		return out_color;
	}
	
	/**
	  * A ray will always hit the cube map by definition, but the point of intersection
	  * is as far away as possible
	  */
	float intersect(const Ray& r) {
		return std::numeric_limits<float>::max();
	}

private:
	struct texture {
		texture()
			: data{ 0.0f }
			, width(1600)
			, height(1200)
		{

		}
		std::vector<float> data;
		unsigned int width;
		unsigned int height;
	};

	/**
	  * Returns the texel at texture coordinate [s, t] in texture tex
	  * Will perform bilinear interpolation to find better approximation of pixel value
	  *	and avoid staricasing effect
	  */
	static Vector3f readTexture(texture& tex, float s, float t) {	

		//std::cout << "read text " << s << " , " << t << std::endl;
		if ( s < 0.0 || t < 0.0) 
		{
			//std::cout << "skipping\n";

			//std::cin.ignore();
			return Vector3f(0.0f);
		}

		float xf = std::min(s*tex.width, tex.width-1.0f);
		float yf = std::min(t*tex.height, tex.height-1.0f);
		Vector3f out_color;

		// Calc floor, ceil and reminder for x
		float floor_x = static_cast<float> ( static_cast<int> ( xf >= 0.0f ? xf + 0.5f : xf - 0.5f ) );//std::floor(xf); 
		float ceil_x = static_cast<float> ( static_cast<int> ( xf <= 0.0f ? xf + 0.5f : xf - 0.5f ) );//std::floor(xf); 
		//float floor_x = std::floor(xf); 
		//float ceil_x = std::ceil(xf); 
		float rem_x = xf - floor_x; 

		// Calc floor, ceil and reminder for y
		float floor_y = static_cast<float> ( static_cast<int> ( yf >= 0.0f ? yf + 0.5f : yf - 0.5f ) );//std::floor(xf); 
		float ceil_y = static_cast<float> ( static_cast<int> ( yf <= 0.0f ? yf + 0.5f : yf - 0.5f ) );//std::floor(xf); 
		//float floor_y = std::floor(yf); 
		//float ceil_y = std::ceil(yf); 
		float rem_y = yf - floor_y; 

		//unsigned int xm = static_cast<unsigned int>(xf);
		//unsigned int ym = static_cast<unsigned int>(yf);

		// Calc the the two values for floor y
		unsigned int f_f = (floor_y*tex.width + floor_x) * 3;
		unsigned int f_c = (floor_y*tex.width + ceil_x) *3 ;
		
		// Calc the the two values for ceil y
		//unsigned int c_f = (ceil_y*tex.width + floor_x) *3;
		//unsigned int c_c = (ceil_y*tex.width + ceil_x) *3  ;
		//
		unsigned int c_f = (ceil_y*tex.width + floor_x) *3;
		unsigned int c_c = (ceil_y*tex.width + ceil_x) *3  ;
		
		for (int k=0; k<3; ++k) {
			// Weight the two values for floor y using remainder of x
			float cmin = (  rem_x *  tex.data.at( f_c + k ) ) + 
				( ( 1.0f - rem_x ) * tex.data.at( f_f + k ) );

			// Weight the two values for ceil y using remainder of x
			float cmax = (   rem_x * tex.data.at( c_f + k ) ) + 
				( ( 1.0f - rem_x ) * tex.data.at (c_c + k ) );

			// Weigh the two above values using remainder of y 
			out_color[k] = ( rem_y * cmax ) + 
				( ( 1.0f - rem_y ) * cmin );
		}

		return out_color;
	}

	/**
	  * Loads an image into memory from finotepale
	  */
	static void loadImage(std::string filename, texture& tex) {

		std::cout << "loading : " << filename << std::endl;
		ILuint ImageName;

		ilGenImages(1, &ImageName); // Grab a new image name.
		ilBindImage(ImageName); 
		
		if (!ilLoadImage(filename.c_str())) {
			std::cout << "failed to load : " << filename << std::endl;
			ILenum e;
			std::stringstream error;
			while ((e = ilGetError()) != IL_NO_ERROR) {
				error << e << ": " << iluErrorString(e) << std::endl;
			}
			ilDeleteImages(1, &ImageName); // Delete the image name. 
			throw std::runtime_error(error.str());
		}

		

		tex.width = ilGetInteger(IL_IMAGE_WIDTH); // getting image width
		tex.height = ilGetInteger(IL_IMAGE_HEIGHT); // and height

		std::cout << "size : " << tex.width << " , " << tex.height << std::endl;
		tex.data.resize(tex.width*tex.height*3);
		
		ilCopyPixels(0, 0, 0, tex.width, tex.height, 1, IL_RGB, IL_FLOAT, tex.data.data());
		ilDeleteImages(1, &ImageName); // Delete the image name. 
	}

	texture posx, negx, posy, negy, posz, negz;
};
