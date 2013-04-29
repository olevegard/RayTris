#pragma once

#include <vector>
#include <memory>
#include "Ray.h"

class RayTracerState;
class SceneObjectEffect;

/**
  * The abstract SceneObject class defines what a scene object needs to be able to perform:
  * intersection test, and recursive ray-tracing.
  */
class SceneObject {
public:
	/**
	  * Computes the closest point of intersection
	  * @param r The ray to perform intersection test against
	  * @param t If intersection found, this is set so that t*r is the point of intersection
	  * @return true if intersection found, false otherwise
	  */
	virtual float intersect(const Ray& r, int &face) = 0;

	/**
	  * Performs recursive raytracing of the elements in scene
	  * @param scene All elements in the scene
	  * @param r The incoming ray to trace
	  * @param color The color to set
	  * @param depth The recursion depth of this tracing
	  */
	virtual Vector3f rayTrace(Ray &ray, const float& t, RayTracerState& state, int face ) = 0;

protected:
	std::shared_ptr<SceneObjectEffect> effect;
	SceneObject()
	:	effect(NULL)
	{}
	~SceneObject(){}
};
