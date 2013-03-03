#pragma once

#include <memory>
#include <string>
#include <vector>

#include "CubeMap.h"
#include "FrameBuffer.h"
#include "SceneObject.h"
#include "RayTracerState.h"
#include "Ray.h"

/**
  * The RayTracer class is the main entry point for raytracing
  * our scene, saving results to file, etc.
  */
class RayTracer {
public:
	RayTracer(unsigned int width, unsigned int height);

	/**
	  * Adds an object to the scene
	  */
	void addSceneObject(std::shared_ptr<SceneObject>& o);

	/**
	  * Renders the current scene
	  */
	void render();

	/**
	  * Saves the currently rendered frame as an image file
	  */
	void save(std::string basename, std::string extension);

private:
	std::shared_ptr<FrameBuffer> fb;
	std::shared_ptr<RayTracerState> state;

	/**
	  * Defines the virtual screen we project our rays through
	  */
	struct {
		float left;
		float right;
		float top;
		float bottom;
	} screen;
};

