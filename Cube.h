#pragma once

#include "SceneObject.h"
#include "CubeObjects.h"
#include "RayTracerState.h"

class Cube : public SceneObject {

public:
	Cube( const Vector3f &center, float radius, const std::shared_ptr<SceneObjectEffect> &effect_)
	:	normal( 0.0f, 0.0f, 1.0f )
	,	effect(effect_)
	{
		effect = effect_;

		cube[0].init( FRONT, center, radius);
		cube[1].init( BACK, center, radius);
		cube[2].init( TOP, center, radius);
		cube[3].init( BOTTOM, center, radius);
		cube[4].init( LEFT, center, radius);
		cube[5].init( RIGHT, center, radius);

		std::cout << "Radius : " << radius << std::endl;
		std::cout << "Center : " << center << std::endl;
	}

	virtual ~Cube(){}

	float intersect(const Ray& ray, int &face )
	{
		float f = 99999999999.0f;
		float f_min = f;
		bool found = false;
		
		for ( int i = 0; i < 6; ++i )
		{	
			//if ( i == 4 ) continue;

			if ( cube[i].intersect( ray, f ))
			{
				//std::cout << "intersect : " << f << std::endl;
					
				if ( f < f_min )
				{
					found = true;
					f_min = f;
					face = i;
					normal = cube[i].normal;
				}
			}
		}
		
		if ( found && f_min < 99.0f )
		{
			return f_min;
		}
		else
			return -1.0f;
	}
	
	/**
	  * Computes normal for an intersection point on a sphere
	  */
	const Vector3f computeNormal(const Ray& ray, const float& t)
	{
		/*
		// Calc ray intersect point
		Vector3f point = ray.getOrigin() + ( ray.getDirection() * t );

		for ( int i = 0; i < 6; ++i )
		{
			if ( cube[i].CheckFaceCollision( point ) )
				return cube[i].normal;
		}
		*/
		return normal;
	}

	Vector3f rayTrace(Ray &ray, const float& t, RayTracerState& state, int face )
	{
		if ( face < 0 || face > 5 )
			return Vector3f( 0.0f );
		// Get normal and ray trace with the assigned effect 
		Vector3f normal = cube[face].normal; 
		
		return effect->rayTrace(ray, t, normal, state);
	}
private:
	CubeFace cube[6];
	Vector3f normal;
	std::shared_ptr<SceneObjectEffect> effect;
};
