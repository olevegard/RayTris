#pragma once

#include "RayTracerState.h"
#include "SceneObject.h"
#include "SceneObjectEffect.h"

//#include <glm/glm.hpp>

/**
  * The sphere is a scene object that is easy to work with. We have
  * simple analytical formulations for both the intersection with a ray and
  * for computation of the normal at any point
  */
class Triangle : public SceneObject {
public:
	
	Triangle(Vector3f center, float radius, std::shared_ptr<SceneObjectEffect> effect)
		: p( 0.0f )
		,	a(0.0f)
		,	b(0.0f)
		,	c(0.0f)
		,	normal(0.0f)
		,	r(0.0f)
	
	{
		this->p = center;
		this->r = radius;
		this->effect = effect;
	
		// Our three points in the triangle ( create by the center and an offset of the radius ) 
		a = Vector3f( p.x + radius, p.y , p.z); 
		b = Vector3f( p.x - radius, p.y , p.z); 
		c = Vector3f( p.x , p.y + radius * 2, p.z); 

		// Calculate our normal
		//normal = Math::normalize( glm::cross ( (a-c), (a-b)  )); 
		normal = Vector3f( 0.0f, 0.0f, 1.0f); 
		
	}
	
	virtual ~Triangle();
	/**
	  * Computes the ray-sphere intersection
	  */
	float intersect(const Ray& r) {
		float t = Math::dot( 
				( p - r.getOrigin() ) , 
				( normal / (  Math::dot ( r.getDirection(), normal ) ) ) 
			); 

		Vector3f q( r.getOrigin() + r.getDirection() * t ); 
		
		// Code from http://www.blackpawn.com/texts/pointinpoly/default.html
		// ( since the code was in the slide for the lecture I assume it's okay that we use it ) 

		// Compute vectors        
		Vector3f v0 = c - a;
		Vector3f v1 = b - a;
		Vector3f v2 = q - a;

		// Compute dot products
		float dot00 = Math::dot(v0, v0);
		float dot01 = Math::dot(v0, v1);
		float dot02 = Math::dot(v0, v2);
		float dot11 = Math::dot(v1, v1);
		float dot12 = Math::dot(v1, v2);

		// Compute barycentric coordinates
		float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
		float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
		float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

		// Check if point is in triangle
		if ( (u > 0) && (v > 0) && (u + v < 1) )
			return	t; // Point is in triangle, return t
		else 
			return -1; // Point is not in triangle, return -1.0f


	}
	
	/**
	  * Computes normal for an intersection point on a triangle 
	  * ( not used, the normal is the same of the entire triangle and is computed in constructor ) 
	  */
	const Vector3f computeNormal(const Ray& ray, const float& t) {
	
		// No computation needed, normal is computed in constructor and doesn't change 
		return normal;
	}

	Vector3f rayTrace(Ray &ray, const float& t, RayTracerState& state) {
		// Ray trace using the assigned affect
		return effect->rayTrace(ray, t, normal, state);
	}

protected:
	Vector3f p; //< center of sphere
	Vector3f a; // Point in triangle
	Vector3f b; // Point in triangle 
	Vector3f c; // Point in triangle
	Vector3f normal; 
	float r;   //< sphere radius
};

