#pragma once

#include "RayTracerState.h"
#include "SceneObject.h"
//#include "SceneObjectEffect.h"


/**
  * The sphere is a scene object that is easy to work with. We have
  * simple analytical formulations for both the intersection with a ray and
  * for computation of the normal at any point
  */
class Sphere : public SceneObject {
public:
	Sphere( Vector3f center_, float radius_, std::shared_ptr<SceneObjectEffect> effect_) 
	:	effect(effect_)
	, 	center( center_ )	
	,	radius ( radius_ )
	,	radiusSQ ( radius_ * radius_ )
	{
	}

	virtual ~Sphere(){}
	/**
	  * Computes the ray-sphere intersection
	  */
	float intersect(const Ray& r) {
		//const float z_offset = 10e-4f;
		const Vector3f &d = r.getDirection();
		const Vector3f &p0 = r.getOrigin();
		//const Vector3f &origToCenter = p0 - radius;
		const Vector3f &origToCenter = p0 - center;
		
		float a = Math::dot(d, d);
		float b = 2.0f * Math::dot(d, origToCenter);
		float c = Math::dot(origToCenter, origToCenter) - radiusSQ;
		
	
		// Calculate collision  (b ^ 2 - 4ac  )
		float collision = (b*b-4.0f*a*c);
		
		// Check  collision to see if we collide 
		if ( collision >= 0.0f) {

			// Find root
			float sqrt_collision = sqrt( collision );

			// Find the twoo roots ( -b +/- sqrt_collision ) / 2 * a
			float s_1 = ( -b - sqrt_collision )  / ( 2 * a );
			float s_2 = ( -b + sqrt_collision )  / ( 2 * a );

			// Find highest and smallest root
			float s_max = std::max( s_1, s_2 );
			
			// Is min root > 0 if not, we are not coliding. 
			if ( s_max > 0.0f )
			{
				float s_min = std::min( s_1, s_2 );

				// Is max > 0? If so, this is the correct answer.
				if ( s_min > 0.0f ) 
					return s_min;
				else 
					// At this point we knoe that max is > 0 and min > 0 so we return the only positive root, max
					return s_max; 
			}
			else
			{
				// Not colliding
				return -1; 
			}
		}
		else {
			// Not colliding
			return -1;
		}
	}
	
	/**
	  * Computes normal for an intersection point on a sphere
	  */
	const Vector3f computeNormal(const Ray& ray, const float& t) 
	{
		// Conpute normal
		Vector3f normal = (ray.getOrigin() +  ( ray.getDirection() * t) ) - radius;
		
		// Divide by radius ( nornalize ) 
		normal /= radius; 

		return normal;
	}

	Vector3f rayTrace(Ray &ray, const float& t, RayTracerState& state)
	{

		// Get normal and ray trace with the assigned effect 
		Vector3f normal = ( computeNormal(ray, t));
		
		return effect->rayTrace(ray, t, normal, state);
	}

protected:

	std::shared_ptr<SceneObjectEffect> effect;
	Vector3f center; //< center of sphere
	float radius;   //< sphere radius
	float radiusSQ; 
};
