#pragma once

#include <ostream>

struct point2d{
	point2d(int iPosX, int iPosY)
		:   x(0)
		,   y(0)
		,   z(0)
	{
		this->x = iPosX;
		this->y = iPosY;

	}
	point2d()
		:   x(-1)
		,   y(-1)
		,   z(0)
	{}

	point2d( const vector2d& vec)
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

inline bool operator==( const vector2d &vec1, const vector2d &vec2)
{
	return ( vec1.x == vec2.x && vec1.y == vec2.y && vec1.z == vec2.z );
}

inline std::ostream& operator<<(std::ostream& stream, const vector2d& pos)
{
	stream << pos.x << " , " << pos.y << " , " << pos.z;
	return stream;
}

