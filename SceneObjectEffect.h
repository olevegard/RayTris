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
	//,	eta0( eta_water )
	//,	eta1( eta_air )
	,	eta0( eta_pyrex )
	,	eta1( eta_air )
	,	reflectiveness( 0.9f )
	{
		/*
		reflectiveness = 0.9f;

		 eta_air = 1.000293;
		 eta_carbondioxide = 1.00045;
		 eta_some_test = 1.15315;
		 eta_water= 1.3330;
		 eta_ethanol = 1.361;
		 eta_pyrex = 1.470;
		 eta_diamond = 2.419;

		 // Materials currently used 
		 eta0 = eta_diamond;
		 eta1 = eta_air;
		*/
		out_color = Vector3f(0.0f); 
		
	}
	
	virtual ~FresnelEffect(){}
	Vector3f rayTrace(Ray &ray, const float& t, const Vector3f& normal, RayTracerState& state) {

		Vector3f normal_n = Math::normalize ( normal );
		Vector3f dir_n = Math::normalize ( ray.getDirection()  );
		// Calc dot to determine if we are entering or entering the material. 
		//float dot =  Math::dot( Math::normalize  ( ray.getDirection() ), normal);
		//float dot =  Math::dot( Math::normalize  ( ray.getDirection() ), ( normal));
		//float dot =  Math::dot( Math::normalize  ( ray.getDirection() ), ( normal));

		float dot =  Math::dot( dir_n,  normal_n) ;

		// Entering materiall..
		if ( dot < 0.0f )
		{
			//Vector3f dirNormalized = Math::normalize( ray.getDirection);
			// Calc refraction eta so we get right refraction angle
			const float eta = eta0 / eta1; 
			
			// Calc fresnel term  using Schlick's approximation
			float R0 =  ( (( eta0 - eta1 ) /  ( eta0 + eta1 )) *  ( (eta0 - eta1) /  (eta0 + eta1)) );
			//float R0 =  (( eta0 - eta1 ) /  ( eta0 + eta1 ));
		//	R0 *= R0;
			R0 *= -1.0f;
			float Rf = R0 + (1.0 - R0) * pow( ( 1.0 + dot ), 5.0);
			//float Rf = R0 + (1.0 - R0) * pow( ( 1.0 - dot ), 5.0);
			/*
			// Refract colour ( not muliplied with energy since it's not a reflection ) 
			Ray ray_refract = ray.spawn(( t ), Math::refract( dir_n, normal_n, eta ),reflectiveness);
			Vector3f refract = state.rayTrace(ray_refract); // Refract color
			
			// Relfect colour ( multiplied with energy since the rays loose energy per jump, which also effects the colour ) 
			Ray ray_reflect = ray.spawn(( t ), Math::reflect( dir_n, normal_n), reflectiveness); 
			Vector3f reflect = state.rayTrace(ray_reflect) * ray_reflect.getEnergy(); // Reflect color
*/
			// Refract colour ( not muliplied with energy since it's not a reflection ) 
			Ray ray_refract = ray.spawn(( t ), Math::refract( dir_n, normal, eta ),reflectiveness);
			Vector3f refract = state.rayTrace(ray_refract); // Refract color
			
			// Relfect colour ( multiplied with energy since the rays loose energy per jump, which also effects the colour ) 
			Ray ray_reflect = ray.spawn(( t ), Math::reflect(ray.getDirection(), normal ), reflectiveness); 
			Vector3f reflect = state.rayTrace(ray_reflect) * ray_reflect.getEnergy(); // Reflect color

			// Return mix of refract and reflect colour weighetd by the fresnel term
			//out_color = Math::mix(  refract  ,  reflect,  Rf); 
			//std::cout << "Rf___ : " << Rf << std::endl;
			if ( fabs( Rf ) < 1.0f )
			{
				out_color = Math::mix(  refract  ,  reflect,  0.0f); 
				//out_color = Math::mix(  refract  ,  reflect,  0.0f); 
				//std::cout << "R0 entering : " << R0 << std::endl;
				//std::cout << "dot : " << dot << std::endl;
			}
			else 
			{
				// This should not happen....
				std::cout << "R0 entering : " << R0 << std::endl;
				std::cout << "  eta0 : " << eta0 << std::endl;
				std::cout << "  eta1 : " << eta1 << std::endl;
				std::cout << "  eta : " << eta << std::endl;
				std::cout << "RF entering : " << Rf << std::endl;
				//std::cout << "t : " << t << std::endl;
				std::cout << "dot : " << dot << std::endl;
				std::cout << "normal : " << normal << std::endl;
				std::cout << "dir : " << ray.getDirection() << std::endl;
				//std::cout << "reflectiveness : " << reflectiveness << std::endl;
				std::cin.ignore();
				//out_color = Vector3f( 1.0f, 0.0f, 0.0f);
				out_color = Math::mix(  refract  ,  reflect,  Rf); 
			}
			
		 }  else { // exiting  
			
			// Calc refraction eta so we get right refraction angle
			 // Oposite of the one for when we are entering due to the angle of the normal. 
			 // ( the normal is mirrored of what it should be ) 
			const float eta = eta1/eta0;

			// Calc new dot product ( se above comment)
			//float dot_2 = Math::dot(  -normal, ray.getDirection()); 
			float dot_2 =  Math::dot( Math::normalize  ( ray.getDirection() ), Math::normalize ( normal));

			//NOTE: Rf is too high sometimes, should always be < 1.0f
			// Calc fresnel term  using Schlick's approximation
			// Opoosite, se comment aboce 
			//float R0 =  (( eta0 - eta1 ) /  ( eta0 + eta1 ));
			float R0 =   (( eta1 - eta0 ) /  ( eta1 + eta0 ));// *  (( eta1 - eta0) /  (eta1 + eta0 )) );
			R0 *= R0;
			//float Rf = R0 + (1.0 - R0) * pow( ( 1.0f -  dot ), 5.0f);
			//
			float Rf = R0 + (1.0 - R0) * pow( ( 1.0f -  dot_2 ), 5.0f);
/*
			// Spawn reflect ray with mirrored normal ( to make the angle correct ) 
			Ray ray_refract = ray.spawn((t ), Math::refract( ray.getDirection(),  normal, eta ) , reflectiveness);
			Vector3f refract = state.rayTrace(ray_refract); // Refract color
*/		
			// Spawn reflect ray with mirrored normal ( to make the angle correct ) 
			Ray ray_refract = ray.spawn((t ), Math::refract( dir_n,  -normal_n, eta ) , reflectiveness);
			Vector3f refract = state.rayTrace(ray_refract); // Refract color

			// Relfect colour 
			Ray ray_reflect = ray.spawn((t  ), Math::reflect(ray.getDirection(), normal ), reflectiveness); 
			Vector3f reflect = state.rayTrace(ray_reflect) * ray_reflect.getEnergy(); // Reflect color

			if ( fabs( Rf ) < 1.0f )
			{
				/*
				std::cout << "R0  exiting : " << R0 << std::endl;
				std::cout << "  eta0 : " << eta0 << std::endl;
				std::cout << "  eta1 : " << eta1 << std::endl;
				std::cout << "  eta : " << eta << std::endl;
				*/
				//std::cout << "R0 entering : " << R0 << std::endl;
				out_color = Math::mix(  refract  ,  reflect,  0.0f); 
				//out_color = refract;	
				//out_color = reflect;	
				//out_color.z = 1.0f;
				//std::cin.ignore();
			}
			else 
			{
				// This should not happen....
				std::cout << "eta : " << eta << std::endl;
				std::cout << "R0 : " << R0 << std::endl;
				std::cout << "Rf : " << Rf << std::endl;
				std::cout << "t : " << t << std::endl;
				std::cout << "dot : " << dot_2 << std::endl;
				std::cout << "normal : " << normal << std::endl;
				std::cout << "dir : " << ray.getDirection() << std::endl;
				std::cout << "reflectiveness : " << reflectiveness << std::endl;
				std::cin.ignore();
				out_color = Vector3f( 0.0f, 0.0f, 1.0f);
			}
		}

		return out_color; 
		//out_color = mix(refract, reflect, fresnel);
		
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

		float reflectiveness; // = 0.9f;

};

