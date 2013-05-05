#pragma once

#include "math/Vector3f.h"

#include <iostream>
/**
  * The ray class holds the state information of a ray in our ray-tracer:
  * point of origin and direction
  */
class Ray {
public:
	Ray( const Vector3f &origin_, const Vector3f &direction_)
		: 	min_energy( 1.0f )
		,	energy( 0.2f )
		,	origin( origin_ )
		,	direction( direction_ )
	{
		this->origin = origin;
		this->direction = direction;
	
		// Initialize energy to 1.0 
		energy = 0.9f;
		min_energy = 0.02f;
	}

	/**
	  * Returns the origin of the ray
	  */
	inline const Vector3f& getOrigin() const { return origin; }

	/**
	  * Returns the direction of the ray
	  */
	inline const Vector3f& getDirection() const { return direction; }

	/**
	  * Spanws a new ray from this ray originating from getOrigin() + t*getDirection() 
	  * going in the direction of d
	*/
	inline Ray spawn(float t, Vector3f d, float reflectivenes) const 
	{
		// Add a lttile offset to t, this help with reducing the noise in the second refractioon
		float offset = 0.0f;

		// Determine wether offset should be negative or positive
		if ( Math::dot( direction, d ) < 0 )
			offset = -10e-5f;
		else 
			offset = 10e-5f;


		// Create new ray with our offset 
		Ray r( getOrigin() + (t + offset ) * getDirection(), d);

		// Reduce energy of ray by multiplying with reflectiveness
	  	r.energy = this->energy * reflectivenes; 

		return r;
	}

	inline float getEnergy(){
		return energy; 
	}

	/**
	  * Tests whether or not this ray should be raytraced further
	  */
	inline bool isValid() const {
		return (energy >  min_energy);
	}

	/**
	  * Invalidate ray, saying it should not be raytraced further
	  */
	inline void invalidate() {
		// Checks if energay is belowed min_energy ( 0.02 by default )
		this->energy = min_energy;
	}

private:
	friend class RayTracer;

	float min_energy;// = 0.02f;
	float energy;
	Vector3f origin;
	Vector3f direction;
};
