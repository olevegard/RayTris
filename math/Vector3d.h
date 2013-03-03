#pragma once

#include <ostream>

struct vector3d{
	vector3d(int x, int y, int z)
		:   x(0)
		,   y(0)
		,   z(0)
	{
		this->x = iPosX;
		this->y = iPosY;

	}
	vector3d()
		:   x(-1)
		,   y(-1)
		,   z(0)
	{}

	vector3d( const vector3d& vec)
		:   x(0)
		,   y(0)
		,   z(0)
	{
		x = vec.x;
		y = vec.y;
	}

	int x;
	int y;
	int z;

};

inline bool operator==( const vector3d &vec1, const vector3d &vec2)
{
	return ( vec1.x == vec2.x && vec1.y == vec2.y && vec1.z == vec2.z );
}

inline std::ostream& operator<<(std::ostream& stream, const vector2d& pos)
{
	stream << pos.x << " , " << pos.y << " , " << pos.z;
	return stream;
}

inline vector3d perator+( const point3d &point1, const point3d &point2 )
{
	point3d p( point1.x + point2.x, point1.y + point2.y, point1.z + point2.z );
}

inline vector3d operator-( const point3d &point1, const point3d &point2 )
{
	point3d p( point1.x - point2.x, point1.y - point2.y, point1.z - point2.z );
}
