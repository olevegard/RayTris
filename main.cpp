#include <iostream>
#include <string>

#ifdef _WIN32
#include <Windows.h>
#endif
 
#include "SceneObjectEffect.h"
#include "RayTracer.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Cube.h"
#include "CubeMap.h"
//#include "Timer.h"

/**
 * Simple program that starts our raytracer
 */
int main(int argc, char *argv[]) {
	try {
		RayTracer* rt;
		//Timer t;
		//rt = new RayTracer( 400, 300);
		rt = new RayTracer( 800, 600);
		//rt = new RayTracer( 3200, 2400);
		//rt = new RayTracer( 1600, 1200);

		// Our different effects...
		std::shared_ptr<SceneObjectEffect> color(new ColorEffect( Vector3f (0.0, 1.0, 0.0)));
		std::shared_ptr<SceneObjectEffect> phong(new PhongEffect( Vector3f (0.0, 0.0, 10.0)));
		std::shared_ptr<SceneObjectEffect> reflect(new Reflect( ));
		std::shared_ptr<SceneObjectEffect> fresnel(new FresnelEffect( ));

		// Our different objects
		std::shared_ptr<SceneObject> sphereFresnel1(new Cube(Vector3f(  0.0f,  0.0f,  3.0f), 3.0f, reflect));
		std::shared_ptr<SceneObject> sphereReflect1(new Sphere(Vector3f( -7.0f,  0.0f,  3.0f), 1.0f, reflect));
		std::shared_ptr<SceneObject> sphereReflect2(new Sphere( Vector3f(-4.0f, -2.0f, -3.0f), 1.0f, reflect));
		std::shared_ptr<SceneObject> cubeMap( new CubeMap( 
			"cubemaps/SaintLazarusChurch3/posx.jpg", "cubemaps/SaintLazarusChurch3/negx.jpg", 
			"cubemaps/SaintLazarusChurch3/posy.jpg", "cubemaps/SaintLazarusChurch3/negy.jpg",
			"cubemaps/SaintLazarusChurch3/posz.jpg", "cubemaps/SaintLazarusChurch3/negz.jpg"  ) );

		//std::shared_ptr<SceneObject> s4(new Triangle( Vector3f(2.0f, -3.0f, 2.0f), 3.0f, fresnel));
		//
		//rt->addSceneObject(sphereReflect1);
		rt->addSceneObject(sphereReflect2);
		rt->addSceneObject(sphereFresnel1);
		//rt->addSceneObject(s4);
		rt->addSceneObject(cubeMap);

		//t.restart();
		std::cout << "Rendering...\n";
		rt->render();

		//double elapsed = t.elapsed();
		//std::cout << "Computed in " << elapsed << " seconds" <<  std::endl;
		//std::cout << "Done, saving...\n";
		rt->save("test", "bmp"); //We want to write out bmp's to get proper bit-maps (jpeg encoding is lossy)
		//system("Pause"); 
		//std::cout << "DONE!\n";
		delete rt;
	} catch (std::exception &e) {
		std::string err = e.what();
		std::cout << err.c_str() << std::endl;
		std::cin.ignore();
		return -1;
	}
	return 0;
}
