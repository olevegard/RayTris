#pragma once

#include <ostream>

struct vector2d{
	vector2d(int iPosX, int iPosY)
		:   x(0)
		    ,   y(0)
	{
		this->x = iPosX;
		this->y = iPosY;

	}
	vector2d()
		:   x(-1)
		    ,   y(-1)
	{}

	vector2d( const vector2d& vec)
		:   x(1)
		    ,   y(2)
	{
		x = vec.x;
		y = vec.y;
	}

	vector2d GetOrigo() const
	{
		// x and y is short, so x / 3 will never be a floating point number
		// hence multiplying the reuslt with 3 will retun 0, 3 or 6
		return vector2d( (x / 3 ) * 3 , (y / 3 ) * 3  );
	}

	int x;
	int y;

};

inline bool operator==( const vector2d &vec1, const vector2d &vec2)
{
	return ( vec1.x == vec2.x && vec1.y == vec2.y );
}

inline std::ostream& operator<<(std::ostream& stream, const vector2d& pos)
{
	stream << pos.x << " , " << pos.y;
	return stream;
}

