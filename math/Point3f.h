#pragma once

#include <ostream>

struct Point3f{
	Point3f(int iPosX, int iPosY, int iPosZ)
		:   x(0)
		,   y(0)
		,   z(0)
	{
		this->x = iPosX;
		this->y = iPosY;
		this->z = iPosZ;

	}
	Point3f()
		:   x(-1)
		,   y(-1)
		,   z(0)
	{}

	Point3f( const Point3f& vec)
		:   x(0)
		,   y(0)
		,   z(0)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}

	float x;
	float y;
	float z;

};

inline bool operator==( const Point3f &p1, const Point3f &p2)
{
	return ( p1.x == p2.x && p1.y == p2.y && p1.z == p2.z );
}

inline std::ostream& operator<<(std::ostream& stream, const Point3f& pos)
{
	stream << pos.x << " , " << pos.y << " , " << pos.z;
	return stream;
}

inline Point3f operator+( const Point3f &point1, const Point3f &point2 )
{
	Point3f p( point1.x + point2.x, point1.y + point2.y, point1.z + point2.z );
}

inline Point3f operator-( const Point3f &point1, const Point3f &point2 )
{
	Point3f p( point1.x - point2.x, point1.y - point2.y, point1.z - point2.z );
}
