#include "Point3f.h"
#include "Vector3f.h"

struct Material
{
	float reflection;

	float red;
	float green;
	float blue;
	float alpha;
};
/*
struct Sphere
{
	Point3f pos;
	float size;
	int materialID;
};*/

struct Light
{
	Point3f pos;
	float red;
	float green;
	float blue;
};
/*
struct Ray
{
	Point3f start;
	Vector3f direction;
};*/
