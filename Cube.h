#pragma once

#include "RayTracerState.h"
#include "SceneObject.h"

class Cube : public SceneObject {

public:
	Cube( const Vector3f &center_, float radius_, const std::shared_ptr<SceneObjectEffect> &effect_)
	:	pointTL( 0.0f )
	,	pointTR( 0.0f )
	,	pointBL( 0.0f )
	,	pointBR( 0.0f )

	,	normal( 0.0f, 0.0f, 1.0f )

	,	effect(effect_)

	, 	center( center_ )	
	,	radius ( radius_ )
	,	radiusSQ ( radius_ * radius_ )
	{
		center = center_;
		radius = radius_;
		effect = effect_;
	
		// CAlculate the four points in the cube
		//pointTL = Vector3f( center.x + radius, center.y + radius, center.z);
		//pointTR = Vector3f( center.x - radius, center.y + radius, center.z);
		//pointBL = Vector3f( center.x + radius, center.y - radius, center.z);
		//pointBR = Vector3f( center.x - radius, center.y - radius, center.z);
		//
		pointTL = Vector3f( center.x + radius, center.y, center.z - radius);
		pointTR = Vector3f( center.x - radius, center.y, center.z - radius);
		pointBL = Vector3f( center.x + radius, center.y, center.z + radius);
		pointBR = Vector3f( center.x - radius, center.y, center.z + radius);

		// Calculate our normal
		//normal = Math::normalize( glm::cross ( (a-c), (a-b)  )); 
		normal = Vector3f( 0.0f, 1.0f, 0.0f); 


		std::cout << "TL : " << pointTL << std::endl;
		std::cout << "BR : " << pointBR << std::endl;
	
	}

	virtual ~Cube(){}

	float intersect(const Ray& ray)
	{
		/* 
		Ray / plane intersection test
		======================================================

		 Denominator
			den = ( p0 - l0 ) * n 
 
		Numerator
			num = l * n

		Result
			d = den / num

		Legend:
			p0 = point on plane
			l0 = point on ray
			l = vecotr of ray
			n = normal of plane
			d = point of intersection on sphere

		Notes:
			If den == 0 and num != 0, the ray starts outside the plane and is paralell to it, no collision. 
			if den == 0 and num == 0, the ray starts inside the plane and is paralell to it, hits plane everywhere.
			In all other cases the ray will hit the plane, use d to find the intersection point and check if it's a point on the cube.
		*/

		Vector3f dir_n = Math::normalize( ray.getDirection() );
		Vector3f orig_n = ( ray.getOrigin() );

		//float den = Math::dot ( ( pointTL * orig_n ), normal);
		float den = Math::dot ( ( pointTL - orig_n ), normal);

		float num = Math::dot( dir_n,  normal );

		if ( den == 0 && num != 0 ) 
			return -1.0f;
		else if ( den == 0 && num == 0 )
			return 0.0f;
		
		float distance = den / num;

		// Ray hits the infitie plane. Find intersection point and check if it's on this cube.
		Vector3f point = orig_n + ( dir_n * distance );

		if ( point.x < pointTL.x && point.x > pointBR.x)
		{
			if ( point.y < pointTL.y && point.y > pointBR.y)
			{
				float t = Math::dot( 
						( pointTL - ray.getOrigin() ) , 
						( normal / (  Math::dot ( ray.getDirection(), normal ) ) ) 
						); 

				if ( distance == 2.0f )
				{
					//std::cout << "==========================================================\n";
					//std::cout << "intersection at point : " << point << " distance : " << distance << std::endl;
					//std::cout << "Orig : " << ray.getOrigin() << "\nVector : " << ray.getDirection() << std::endl;
				}
				return t;
			}
		}
		
		return -1.0f;

		
		
		/*
		float t = Math::dot( 
				( center - r.getOrigin() ) , 
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
		*/

	}
	
	/**
	  * Computes normal for an intersection point on a sphere
	  */
	const Vector3f computeNormal(const Ray& ray, const float& t) 
	{
		// Conpute normal
		//Vector3f normal( 0.0f, 0.0f, 1.0f );
		
		return normal;
	}

	Vector3f rayTrace(Ray &ray, const float& t, RayTracerState& state)
	{
		// Get normal and ray trace with the assigned effect 
		Vector3f normal = ( computeNormal(ray, t));
		
		return effect->rayTrace(ray, t, normal, state);
	}

protected:

	Vector3f pointTL; // Point in cube
	Vector3f pointTR; // Point in cube
	Vector3f pointBL; // Point in cube
	Vector3f pointBR; // Point in cube

	Vector3f normal;

	std::shared_ptr<SceneObjectEffect> effect;
	Vector3f center; //< center of sphere
	float radius;   //< sphere radius
	float radiusSQ; 
};
