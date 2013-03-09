#include "Vector3f.h"
#include <iostream>
namespace Math
{
	Vector3f normalize( const Vector3f &vec )
	{
		float length = vec.getLength();
		if ( length == 1.0f ) 
		{
			//std::cout << "allready normalized!\n";
			return vec;
		} else
		{
			//std::cout << "needs normalization\n";

			Vector3f vecRet( vec.x / length, vec.y / length, vec.z / length );

			/*std::cout << "normaiing : "
					<< "\n\tLenght : " << length
					<< "\n\tVector : " << vec
					<< "\n\tResult : " << vecRet << std::endl; 
					*/
			return vecRet;
		}

	}

	float dot( const Vector3f &vec1, const Vector3f &vec2 )
	{
		//float dot = vec1.x * vec2.x;
		//dot += vec1.y * vec2.y;
		//dot += vec1.z * vec2.z;
		return ( vec1.x * vec2.x) + ( vec1.y * vec2.y ) + ( vec1.z * vec2.z);
	}

	Vector3f reflect( const Vector3f &vec, const Vector3f &normal )
	{
		float dotProduct = 2 * dot( normal, vec );
		Vector3f v = normal * dotProduct;
		v = vec - v;
		return v;

	}
	
	
	Vector3f refract( const Vector3f &vec, const Vector3f &normal, float eta )
	{
		float dotNormalVec = dot( normal, vec );
		float k = 1.0f -  eta * eta  * ( 1.0f -  dotNormalVec * dotNormalVec);

		if ( k < 0.0f )
		{
			return Vector3f(0.0f);
		}
		else
			return ( eta * vec - ( eta * dotNormalVec + sqrt(k) ) * normal );
	}

	Vector3f min( const Vector3f &vec1, const Vector3f &vec2)
	{
		return ( vec1.getLength() > vec2.getLength() ? vec1 : vec2 );
	}



	
	Vector3f mix( const Vector3f &vec, const Vector3f &normal, float eta )
	{
		float eta2 = 1.0f - eta;

		Vector3f v;
		v.x = ( vec.x * eta2 ) + ( normal.x * eta );
		v.y = ( vec.y * eta2 ) + ( normal.y * eta );
		v.z = ( vec.z * eta2 ) + ( normal.z * eta );

		return v;
	}
/*	
	Vector3f mix( const Vector3f &vec, const Vector3f &normal, float eta )
	{
		float eta2 = 1.0f - eta;

		Vector3f v;
		v.x = ( vec.x * eta ) + ( normal.x * eta2 );
		v.y = ( vec.y * eta ) + ( normal.y * eta2 );
		v.z = ( vec.z * eta ) + ( normal.z * eta2 );
		std::cout << "mix" 
			<< "\n\tV1    : " << vec
			<< "\n\tV2    : " << normal
			<< "\n\teta   : " << eta
			<< "\n\t1-eta : " << eta2
			<< "\n\tres : " << v << std::endl;
		return v;
	}

*/
	double max( double val1, double val2)
	{
		return ( val1 > val2 ? val1 : val2 );
	}

	float min( float val1, float val2)
	{
		return ( val1 > val2 ? val1 : val2 );
	}

}
