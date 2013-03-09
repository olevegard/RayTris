#pragma once

#include <memory>
#include <limits>
#include "Ray.h"

#include "SceneObject.h"

/**
  * The RayTracerState class keeps track of the state of the ray-tracing:
  * the objects in the scene, camera position, etc, and its main responsibility
  * is to RayTrace the whole scene for each ray.
  */
class RayTracerState {
public:
	RayTracerState( Vector3f camera_position_)
		: scene{}
		, camera_position( camera_position_)
	{
		this->camera_position = camera_position_;
	}
	
	inline std::vector<std::shared_ptr< SceneObject > >& getScene() { return scene; } //UNCOMMENT LATER

	inline Vector3f getCamPos() { return camera_position; }

	/**
	  * Performs raycasting on the scene for the ray ray
	  * @param ray The ray to raycast with
	  * @param t The parameter so that t*ray gives the first intersection point
	  * @return -1 if no intersection found, otherwise the object index in the scene
	  */
	inline Vector3f rayTrace(Ray& ray) {
		const float z_offset = 10e-4f;

		float t = -1;
		float t_min = std::numeric_limits<float>::max();
		int k_min=-1;

		if (!ray.isValid()) 
		{ 
			//std::cout << "ray killed\n"; 
			return Vector3f(0.0f);  
		}
		
		//Loop through all the objects, to find the closest intersection, if any
		//This is essentially just ray-casting
		for (unsigned int k=0; k<scene.size(); ++k) 
		{
//			
			t = scene.at(k)->intersect(ray);
			//std::cout << "t : " << t << std::endl;

			if (t > z_offset && t <= t_min) 
			{
				//std::cout << "RaytracerState::rayTrace2...\n";
				k_min = k;
				t_min = t;
			}
		}

		if (k_min >= 0) {
			//glm::vec3 collision_point = glm::vec3( ray.getOrigin() + ray.getDirection() * glm::vec3(t_min) ); 
			//glm::vec3 reflected = glm::vec2( glm::reflect(ray.
			//std::cout << "Spawning new ray\n";
			return scene.at(k_min)->rayTrace(ray, t_min, *this);
		}
		else {
			std::cout << "no intersection\n";
			return Vector3f(0.0 );
		}
	}


private:
	std::vector<std::shared_ptr<SceneObject> > scene;
	Vector3f camera_position;
};
