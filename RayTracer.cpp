#include "RayTracer.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <sys/stat.h>

//#include <IL/il.h>
//#include <IL/ilu.h>


RayTracer::RayTracer(unsigned int width, unsigned int height) 
	:	fb( NULL )
	,	state( NULL )
	,	screen()
{
	const Vector3f camera_position(0.0f, 0.0f, 10.0f);

	//Initialize framebuffer and virtual screen
	fb.reset(new FrameBuffer(width, height));
	float aspect = width/static_cast<float>(height);
	screen.top = 1.0f;
	screen.bottom = -1.0f;
	screen.right = aspect;
	screen.left = -aspect;
	
	//Initialize state
	state.reset(new RayTracerState(camera_position));
	
	//Initialize IL and ILU
	ilInit();
	iluInit();
}

void RayTracer::addSceneObject(std::shared_ptr<SceneObject>& o) {
	state->getScene().push_back(o);
}

void RayTracer::render() {
	float xConversion = (screen.right-screen.left) /static_cast<float>(fb->getWidth());
	float yConversion = (screen.top-screen.bottom)/static_cast<float>(fb->getHeight()); 

	timespec startTime;
	timespec stopTime;

	Vector3f camPos = state->getCamPos();

	clock_gettime( CLOCK_REALTIME, &startTime );

	//For every pixel, ray-trace using multiple CPUs
#ifdef _OPENMP
#pragma omp set nested
#pragma omp parallel for
	for (int j=0; j<static_cast<int>(fb->getHeight()); ++j) {
#else
	for (unsigned int j=0; j<fb->getHeight(); ++j) {
#endif
		for (unsigned int i=0; i<fb->getWidth(); ++i) {
			//Vector3f out_color(0.0, 0.0, 0.0);
			//float t = std::numeric_limits<float>::max();
			
			//float x, y, z;
			// Create the ray using the view screen definition 
			//x = i*(screen.right-screen.left) /static_cast<float>(fb->getWidth()) + screen.left;
			//y = j*(screen.top-screen.bottom)/static_cast<float>(fb->getHeight()) + screen.bottom;
			//glm::vec3 direction = glm::vec3(x, y, z);
			//Ray r = Ray(state->getCamPos(), direction);
			//float z = -1.0f;

			// Calculate the four new pixels ( i / j +/- 0.25 ) 
			float xp = ( i + 0.25f ) * xConversion + screen.left;
			float yp = ( j + 0.25f ) * yConversion + screen.bottom;
			
			float xm = ( i - 0.25f ) * xConversion + screen.left;
			float ym = ( j - 0.25f ) * yConversion + screen.bottom;
			//std::cout << "coord : " << j << " , " << i  << std::endl;
			
			// Create our four rays ( on for each corner of the pixel ) 
			Ray r1 = Ray(camPos, Vector3f(xp, yp ,-1.0f));
			Ray r2 = Ray(camPos, Vector3f(xp ,ym, -1.0f));
			Ray r3 = Ray(camPos, Vector3f(xm ,yp, -1.0f));
			Ray r4 = Ray(camPos, Vector3f(xm ,ym, -1.0f));


			//Now do the ray-tracing to shade the pixel
			Vector3f c1 = state->rayTrace(r1);
			Vector3f c2 = state->rayTrace(r2);
			Vector3f c3 = state->rayTrace(r3);
			Vector3f c4 = state->rayTrace(r4);

			// Set our new out colour to be a combination of the colours returned by our 4 rays, each weighting 25 % of the colour ) 
			Vector3f out_color = 0.25f * ( c1 + c2 + c3 + c4 ); 
			fb->setPixel(i, j, out_color);

/*	
			float xp = i * xConversion + screen.left;
			float yp = j * yConversion + screen.bottom;
			//std::cout << "creating ray\n";
			Ray r = Ray( camPos, Vector3f( xp, yp , -1.0f));

			//std::cout << "creating ray done\n";
			Vector6003f clr = state->rayTrace(r);
			//std::cout << "ray tracing done\n";
			fb->setPixel(i, j, clr);
*/		
		}
	
	}
	std::cout << "done\n";

	clock_gettime( CLOCK_REALTIME, &stopTime );

	unsigned long long iTimeStart = startTime.tv_sec * 1000000000 + startTime.tv_nsec;
	unsigned long long iTimeStop = stopTime.tv_sec * 1000000000 + stopTime.tv_nsec;
	unsigned long long iTimeDelta = iTimeStop - iTimeStart;

	std::cout << "====================Time used ====================\n"
		<< "||\tSeconds      : " << static_cast< float > ( iTimeDelta / 1000000000.0f ) << " s\t||\n"
		<< "||\tMilliseconds : " << static_cast< float > ( iTimeDelta / 1000000.0f ) << " ms\t||\n"
		<< "||\tMicroseconds : " << static_cast< float > ( iTimeDelta / 1000.0f ) << " µs\t||\n"
		<< "||\tNanoseconds  : " << static_cast< float > ( iTimeDelta ) << " ns\t\t||\n";

}

void RayTracer::save(std::string basename, std::string extension) {
	ILuint texid;
	struct stat buffer;
	int i;
	std::stringstream filename;

	ilOriginFunc(IL_ORIGIN_UPPER_LEFT);

	//Create image
	ilGenImages(1, &texid);
	ilBindImage(texid);

	//FIXME: Ugly const cast:( DevILs fault, unfortunately
	ilTexImage(fb->getWidth(), fb->getHeight(), 1, 3, IL_RGB, IL_FLOAT, const_cast<float*>(fb->getData().data()));

	std::cout << "data : " <<  fb->getData().data()[0];

	//Find a unique filename...
	for (i=0; i<10000; ++i) {
		filename.str("");
		filename << "images\\" << basename << std::setw(4) << std::setfill('0') << i << "." << extension;
		if (stat(filename.str().c_str(), &buffer) != 0) break;
	}

	if (i == 10000) {
		std::stringstream log;
		log << "Unable to find unique filename for " << basename << "%d." << extension;
		throw std::runtime_error(log.str());
	}

	if (!ilSaveImage(filename.str().c_str())) {
		std::stringstream log;
		log << "Unable to save " << filename.str();
		throw std::runtime_error(log.str());
	}
	else {
		std::cout << "Saved " << filename.str() << std::endl;
	}

	ilDeleteImages(1, &texid);
}
