#pragma once

#include <cmath>
#include <ostream>

struct Vector3f{
	Vector3f(float x, float y, float z)
		:   x(0.0f)
		,   y(0.0f)
		,   z(0.0f)
	{
		this->x = x;
		this->y = y;
		this->z = z;

	}
	Vector3f ( float f )
		: Vector3f(f, f, f)
	{

	}
	Vector3f()
		:   x(0.0f)
		,   y(0.0f)
		,   z(0.0f)
	{}

	Vector3f( const Vector3f& vec)
		:   x(0.0f)
		,   y(0.0f)
		,   z(0.0f)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}

	float getLength() const
	{
		float length = ( x * x ) + ( y * y ) + ( z * z );
		length = sqrt( length );
		return length;
	}

	float& operator[](  int i)
	{

		switch ( i )
		{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			default:
				return x;
		}

	}
	float x;
	float y;
	float z;

};

inline bool operator==( const Vector3f &vec1, const Vector3f &vec2)
{
	return ( vec1.x == vec2.x && vec1.y == vec2.y && vec1.z == vec2.z );
}

inline bool operator!=( const Vector3f &vec1, const Vector3f &vec2)
{
	return !( vec1 == vec2);
}

inline std::ostream& operator<<(std::ostream& stream, const Vector3f& pos)
{
	stream << pos.x << " , " << pos.y << " , " << pos.z;
	return stream;
}

inline Vector3f operator+( const Vector3f &point1, const Vector3f &point2 )
{
	Vector3f p( point1.x + point2.x, point1.y + point2.y, point1.z + point2.z );
	return p;
}

inline Vector3f operator-( const Vector3f &point1, const Vector3f &point2 )
{
	Vector3f v( point1.x - point2.x, point1.y - point2.y, point1.z - point2.z );
	return v;
}

inline Vector3f operator-( const Vector3f &point1)
{
	Vector3f v( -point1.x, -point1.y, -point1.z);
	return v;
}
inline Vector3f operator-=( const Vector3f &point1, const Vector3f &point2 )
{
	return point1 - point2; 
}

inline Vector3f operator*( const Vector3f &point1, const Vector3f &point2 )
{
	Vector3f v( point1.x * point2.x, point1.y * point2.y, point1.z * point2.z );
	return v;
}
inline Vector3f operator*( float c, const Vector3f &vec)
{
	Vector3f v( vec.x * c, vec.y * c, vec.z * c);
	return v;
}

inline Vector3f operator+( float c, const Vector3f &vec)
{
	Vector3f v( vec.x + c, vec.y + c, vec.z + c);
	return v;
}
inline Vector3f operator/=( Vector3f vec, const Vector3f &vec2)
{
	Vector3f v( vec.x /=  vec2.x, vec.y /= vec2.y, vec.z /= vec2.z);
	return v;
}
inline Vector3f operator/=( Vector3f vec, float f )
{
	Vector3f v( vec.x /=  f, vec.y /= f, vec.z /= f);
	//Vector3f v( vec.x /  f, vec.y / f, vec.z / f);
	return v;
}

inline Vector3f operator/( Vector3f vec, float f )
{
	Vector3f v( vec.x /  f, vec.y / f, vec.z / f);
	//Vector3f v( vec.x /  f, vec.y / f, vec.z / f);
	return v;
}

namespace Math
{
	Vector3f normalize( const Vector3f &vec );

	float dot( const Vector3f &vec1, const Vector3f &vec2 );

	Vector3f reflect( const Vector3f &vec, const Vector3f &normal );
	
	Vector3f refract( const Vector3f &vec, const Vector3f &normal, float eta );

	Vector3f refract_test( const Vector3f &vec, const Vector3f &normal, float eta0, float eta1 );

	Vector3f min( const Vector3f &vec1, const Vector3f &vec2);

	Vector3f mix( const Vector3f &vec, const Vector3f &normal, float eta );

	double max( double val1, double val2);

	float min( float val1, float val2);

}

