#pragma once

#include "Ray.h"
#include "RayTracerState.h"
#include <algorithm>
/**
  * Abstract class that defines what it means to be an effect for a scene object
  */
class SceneObjectEffect {
public:
	virtual ~SceneObjectEffect(){}
	/**
	  * This function "shades" an intersection point between a scene object
	  * and a ray. It can also fire new rays etc.
	  */
	virtual Vector3f rayTrace(Ray &ray, const float& t, const Vector3f& normal, RayTracerState& state) = 0;
private:
};

/**
  * The color effect simply colors any point with the same color
  */
class ColorEffect : public SceneObjectEffect {
public:
	ColorEffect(Vector3f color_)
		: color( color_ )
	{
		this->color = color;
	}

	Vector3f rayTrace(Ray &ray, const float& t, const Vector3f& normal, RayTracerState& state) {
		
		return color;
	}

private:
	Vector3f color;
};
/** 
*/
class Reflect : public SceneObjectEffect {
public:
	Reflect() {
		
	}

	virtual ~Reflect(){}
		Vector3f rayTrace(Ray &ray, const float& t, const Vector3f& normal, RayTracerState& state) {
		Ray ray_reflect = ray.spawn(
			(t * 1.0000001 ), 
			Math::reflect(ray.getDirection(), normal ), 
			0.9f
		); 
	
		return state.rayTrace(ray_reflect) * ray_reflect.getEnergy(); 
	}

private:
	
};
/**
  * The phong effect simply uses phong shading to color the intersection point
  */
class PhongEffect : public SceneObjectEffect {
public:
	PhongEffect(
			Vector3f light_pos_ = Vector3f(0.0),
			Vector3f light_diff_= Vector3f(0.5),
			Vector3f light_spec_ = Vector3f(0.5)
		)
		:	light_pos( light_pos_)
		,	light_diff( light_diff_ )
		,	light_spec( light_spec_)
	{
		this->light_pos = light_pos;
		this->light_diff = light_diff;
		this->light_spec = light_spec;
	}

	virtual ~PhongEffect(){}

	Vector3f rayTrace(Ray &ray, const float& t, const Vector3f& normal, RayTracerState& state) {
		Vector3f out_color ( 0.2f, 0.5f, 0.3f);

		Vector3f p = ray.getOrigin() + ( ray.getDirection() * t ); 

		Vector3f f_v = Math::normalize(-ray.getDirection());   
		Vector3f f_l = Math::normalize(light_pos - p); 

		Vector3f l = Math::normalize(f_l); 
		Vector3f h = Math::normalize ( f_v  + l ); 

		float spec = pow( 
				std::max(
					0.0f,
					Math::dot(normal, h) 
				)
				, 128.0f);

		float diff = std::max(0.0f, Math::dot(normal, l)) ;
	
		return (out_color * diff  + (spec / 0.5f ));

		//return out_color;
	}
private:
	Vector3f light_pos; //Light position
	Vector3f light_diff; //Light diffuse component
	Vector3f light_spec; //Light specular component
};

class FresnelEffect : public SceneObjectEffect {
public:
	FresnelEffect()
	:	out_color(0.0f)
	,	eta_air( 1.000293 )
	,	eta_carbondioxide( 1.00045 )
	,	eta_some_test( 1.15315 )
	,	eta_water( 1.3330 )
	,	eta_ethanol( 1.361 )
	,	eta_pyrex( 1.470 )
	,	eta_diamond( 2.419 )
	,	eta0( eta_air )
	//,	eta1( eta_some_test )
	,	eta1( eta_diamond )
	//,	eta1( eta_carbondioxide )
	//,	eta1( 1.25 )
	//,	eta0( eta_water )
	//,	eta1( eta_air )
	//,	eta0( eta_pyrex )
	//,	eta1( eta_air )
	,	eta01( eta0 / eta1 )
	,	R01 ( 0.0f )
	,	reflectiveness( 0.9f )
	{
		R01 =  ( (( eta0 - eta1 ) /  ( eta0 + eta1 )) *  ( (eta0 - eta1) /  (eta0 + eta1)) );
		eta01 = eta0 / eta1;

		out_color = Vector3f(0.0f); 
	}

	// Calc Rf
	float Schlick2( Vector3f normal, Vector3f incident )
	{
		float R0 = ( eta0 - eta1 ) / ( eta0 + eta1 );
		R0 *= R0;

		float cosX = -Math::dot( incident, normal );

		if ( eta0 > eta1 )
		{
			float eta = eta0 / eta1;
			float sinT2 = eta * eta * ( 1.0 - cosX * cosX );
			
			if ( sinT2 > 1.0 )
				return 1.0f; // Total Inner Reflection

			cosX = sqrt( 1.0f - sinT2 );

		}

		float x = 1.0f - cosX;
		return R0 + ( 1.0f - R0 ) * x * x * x * x * x;

	}

	virtual ~FresnelEffect(){}
	Vector3f rayTrace(Ray &ray, const float& t, const Vector3f& normal, RayTracerState& state) 
	{
		
		Vector3f normal_n = Math::normalize ( normal );

		Vector3f dir_n = Math::normalize ( ray.getDirection()  );

		// Calc dot to determine if we are entering or entering the material. 
		float dot =  Math::dot( dir_n,  normal_n) ;

		// Entering materiall..
		if (  dot  < 0.0f )
		{
			// Air to water... ( eta0 to eta1 )
			float Rf = R01 + (1.0 - R01) * pow( ( 1.0 - fabs( dot ) ), 5.0);
			
			//eta01 = eta1 / eta0;
			// Relfect colour ( multiplied with energy since the rays loose energy per jump, which also effects the colour ) 
			Ray ray_reflect = ray.spawn(( t * 1.00000000001 ), Math::reflect( dir_n, normal_n ), reflectiveness); 
			Vector3f reflect = state.rayTrace(ray_reflect) * ray_reflect.getEnergy(); // Reflect color
			
			//Ray ray_refract = ray.spawn(( t * 1.0000000001 ), Math::refract( normal_n, dir_n, eta01 ),reflectiveness);
			//Ray ray_refract = ray.spawn( (t * 1.00000000001 ), Math::refract_print( dir_n, normal_n, eta01), reflectiveness);
			//Vector3f refract = state.rayTrace(ray_refract); // Refract color
	
			//Vector3f refractDir = Math::refract( normal_n, dir_n, eta01 ) ;
			//Vector3f refractDir = Math::refract( dir_n,normal_n,  ( 1.0f -  eta01 ) ) ; // Previous
			Vector3f refractDir = Math::refract( dir_n,normal_n,  eta01 ) ;
			Ray ray_refract = ray.spawn(( t * 1.0000000001 ), refractDir, reflectiveness);
			Vector3f refract = state.rayTrace(ray_refract); // Refract color

			// Return mix of refract and reflect colour weighetd by the fresnel term
			//out_color = refract;// Does not work 100%
			//out_color = reflect;// Works 100%
			if ( refract.x != 0.0f || refract.y == 0.0f || refract.z == 0 )
				out_color = Math::mix( refract, reflect, Rf); // Refracts most
			else
				out_color =  reflect;// Refracts most
			//out_color = Math::mix( reflect, refract, Rf); // Reflects most
			//std::cout << "entering Rf : " << Rf << std::endl;
			
			if (  Rf >= 1.0f && Rf <= 0.0f )
			{
				std::cout << "entering Rf : " << Rf << std::endl;
				std::cin.ignore();
			}

		 }  else { // exiting  

			 //std::cout << "exiting...\n";
			// Water to air
			R01 =  ( (( eta1 - eta0 ) /  ( eta1 + eta0 )) *  ( (eta1 - eta0) /  (eta1 + eta0)) );
			eta01 = eta1 / eta0;

			// Calc fresnel term  using Schlick's approximation
			float Rf = R01 + (1.0 - R01) * pow( ( 1.0 - fabs ( dot ) ), 5.0);

			// Spawn reflect ray with mirrored normal ( to make the angle correct ) 
			//Ray ray_refract = ray.spawn( (t * 1.00000000001 ), Math::refract( dir_n, -normal_n, eta01), reflectiveness);
			//Vector3f refract = state.rayTrace(ray_refract); // Refract color

			Vector3f dirRefract = Math::refract( dir_n, -normal_n,  ( /* 1 - */eta01  ) );
			Ray ray_refract = ray.spawn((t * 1.00000000001 ), dirRefract , reflectiveness);
			Vector3f refract = state.rayTrace(ray_refract); // Refract color

			// Relfect colour ( working )
			Ray ray_reflect = ray.spawn((t  * 1.0000000001), Math::reflect(dir_n, -normal_n), reflectiveness); 
			Vector3f reflect = state.rayTrace(ray_reflect) * ray_reflect.getEnergy(); // Reflect color

			if ( refract.x != 0.0f && refract.y != 0.0f && refract.z != 0 )
				out_color = Math::mix( refract, reflect, Rf); // Refracts most
			else
				out_color =  reflect;// Total inner reflection 

			//out_color = Math::mix( refract, reflect, Rf); // Reflection 
			//out_color = Math::mix( reflect, refract, Rf);  // Refraction 
			if ( Rf >= 1.0f && Rf <= 0.0f)
			{
				std::cout << "exitig Rf : " << Rf << std::endl;
				std::cin.ignore();
			}
			
		 }

		return out_color; 

	}
private:
	Vector3f out_color;// (0.0f);
	// Our different matrials
	//I declare them here and assign them in constructor 
		// so I don't have to do it every single time rayTrace() is executed

	float eta_air;// = 1.000293;
	float eta_carbondioxide;// = 1.00045;
	float eta_some_test;// = 1.15315;
	float eta_water;// = 1.3330;
	float eta_ethanol;// = 1.361;
	float eta_pyrex;// = 1.470;
	float eta_diamond;// = 2.419;

	float eta0;// = eta_diamond;//eta_diamond
	float eta1;// = eta_air;//eta_air

	float eta01;
	float R01;

	float reflectiveness; // = 0.9f;
};
