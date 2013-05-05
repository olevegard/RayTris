#pragma once

#include "SceneObject.h"
#include "CubeObjects.h"
#include "RayTracerState.h"

class Cube : public SceneObject {

public:
	Cube( const Vector3f &center, float radius, const std::shared_ptr<SceneObjectEffect> &effect_)
	:	normal( 0.0f, 0.0f, 1.0f )
	,	min ( center.x - radius, center.y - radius, center.z - radius )
	,	max ( center.x + radius, center.y + radius, center.z + radius )
	,	effect(effect_)
	{
		effect = effect_;

		std::cout << "Radius : " << radius << std::endl;
		std::cout << "Center : " << center << std::endl;
		std::cout << "Min : " << min << std::endl;
		std::cout << "Max : " << max << std::endl;
	}

	virtual ~Cube(){}

	float intersect(const Ray& ray, int &face )
	{
		float tmin = 0.0f;
		float tmax = 0.0f;
		float tymin = 0.0f;
		float tymax = 0.0f;
		float tzmin = 0.0f;
		float tzmax = 0.0f;

		Vector3f dir = ray.getDirection();
		Vector3f orig = ray.getOrigin();

		if ( dir.x == -0.0f || dir.y == -0.0f || dir.z == -0.0f )
		{
			std::cout << "OH NO! : " << dir << std::endl;
			std::cin.ignore();
		}

		if ( dir.x >= 0.0f )
		{
			tmin =  ( min.x - orig.x ) / dir.x;
			tmax =  ( max.x - orig.x ) / dir.x;
		} else
		{
			tmin =  ( max.x - orig.x ) / dir.x;
			tmax =  ( min.x - orig.x ) / dir.x;
		}

		if ( dir.y >= 0.0f )
		{
			tymin =  ( min.y - orig.y ) / dir.y;
			tymax =  ( max.y - orig.y ) / dir.y;
		} else
		{
			tymin =  ( max.y - orig.y ) / dir.y;
			tymax =  ( min.y - orig.y ) / dir.y;
		}

		if ( ( tmin > tymax ) || ( tymin > tmax ) )
			return 0.0f;

		if ( tymin > tmin )
			tmin = tymin;

		if ( tymax < tmax )
			tmax = tymax;

		if ( ray.getDirection().z >= 0.0f )
		{
			tzmin =  ( min.z - orig.z ) / dir.z;
			tzmax =  ( max.z - orig.z ) / dir.z;
		} else
		{
			tzmin =  ( max.z - orig.z ) / dir.z;
			tzmax =  ( min.z - orig.z ) / dir.z;
		}

		// Delimits hits i z dir
		if ( ( tmin > tzmax ) || ( tzmin > tmax ) ) 
			return 0.0f;

		if ( tzmin > tmin )
			tmin = tzmin;

		if ( tzmax < tmax )
			tmax = tzmax;


		//if ( tmin != tymin && tmin != tzmin )  std::cout << "X intersection ?\n";
		//std::cout << "Returning : " << tmin << std::endl;
		//std::cout << "tmin : " << tmin << std::endl;
		if ( tmin > 20.0f  && false)
		{
			std::cout << "tmin  : " << tmin << std::endl;
			std::cout << "tmax  : " << tmax << std::endl;
			std::cout << "dir   : " << dir << std::endl;
			std::cout << "orig  : " << orig << std::endl;
			std::cout << "point : " << orig + ( dir * tmin ) << std::endl;
			std::cin.ignore();
			return 0.0f;
		}
		return tmin;
	}
	

	bool floatCompare( float value1, float value2, float epsilon = 0.0001f )
	{
		return fabs( value1 - value2 ) <= epsilon;
	}
	/**
	  * Computes normal for an intersection point on a sphere
	  */
	const Vector3f computeNormal(const Ray& ray, const float& t)
	{

		Vector3f pos = ray.getOrigin()+ (ray.getDirection() * t );
		
		// Collision X ( left / right )
		 if ( floatCompare( pos.x, min.x ) )
		{
			// Left side ( red )
			return Vector3f ( -1.0f, 0.0f, 0.0f );
		}
		else if ( floatCompare( pos.x, max.x ) )
		{
			// Right side ( light red )
			return Vector3f ( 1.0f, 0.0f, 0.0f );
		}

		// Collision Y ( top / bottom )
		else if ( floatCompare( pos.y,  min.y ))
		{
			// Bottom ( green )
			return Vector3f ( 0.0f, -1.0f, 0.0f );
		}
		else if ( floatCompare( pos.y, max.y ) )
		{
			// Top ( light green )
			return Vector3f ( 0.0f, 1.0f, 0.0f );
		}
		
		// Collision Z ( near / far )
		else if ( floatCompare( pos.z, min.z ) )
		{
			// Back ( blue )
			return Vector3f ( 0.0f, 0.0f, -1.0f );
		}
		else if ( floatCompare( pos.z, max.z ) )
		{
			// Fron ( light blue )
			return Vector3f ( 0.0f, 0.0f, 1.0f );
		}


		// No collision, something is wrong....
		std::cout << "No collision! : "
			<< "\n\tpos  : " << pos
			<< std::endl;
		std::cin.ignore();
		return Vector3f(0.0f, 0.0f, 0.0f );

		/*
		   if ( ray.getDirection().x >= 0.0f )
		   {
		   txmin =  ( min.x - orig.x ) / dir.x;
		   } else
		   {
		   txmin =  ( max.x - orig.x ) / dir.x;
		   }

		   if ( ray.getDirection().y >= 0.0f )
		   {
		   tymin =  ( min.y - orig.y ) / dir.y;
		   } else
		   {
		   tymin =  ( max.y - orig.y ) / dir.y;
		   }


		   if ( ray.getDirection().z >= 0.0f )
		   {
		   tzmin =  ( min.z - orig.z ) / dir.z;
		   } else
		   {
		   tzmin =  ( max.z - orig.z ) / dir.z;
		   }

		   if ( txmin >= tymin && txmin >= tzmin )
		//
		return Vector3f( 1.0f, 1.0f, 0.0f );
		else if ( tymin >= tzmin )
		// Top / bottom
		return Vector3f( 0.0f, 1.0f, 0.0f );
		else
		// Front/back
		return Vector3f( 0.0f, 0.0f, 1.0f );
		*/
		/*
		 * tmin =  ( min.x - orig.x ) / dir.x;

		 if ( tmin == t )
		 return Vector3f( 0.0f, 0.0f, 1.0f );

		 tmin =  ( min.y - orig.y ) / dir.y;

		 if ( tmin == t )
		 return Vector3f( 0.0f, 1.0f, 0.0f );

		 tmin =  ( min.z - orig.z ) / dir.z;

		 if ( tmin == t )
		 return Vector3f( 1.0f, 0.0f, 1.0f );

		 return Vector3f( 0.0f, 0.0f, 0.0f );
		 */
	}

	Vector3f rayTrace(Ray &ray, const float& t, RayTracerState& state, int face )
	{
		//if ( face < 0 || face > 5 )
		//return Vector3f( 0.0f );
		// Get normal and ray trace with the assigned effect 
		//Vector3f normal = cube[face].normal;
		normal = computeNormal( ray, t );
		return effect->rayTrace(ray, t, normal, state);
	}
private:
	Vector3f normal;
	Vector3f min;
	Vector3f max;

	std::shared_ptr<SceneObjectEffect> effect;
};
