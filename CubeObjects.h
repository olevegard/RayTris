enum FaceNames
{
	FRONT = 0,
	BACK = 1,
	TOP = 2,
	BOTTOM = 3,
	LEFT = 4,
	RIGHT = 5
};

struct CubeFace
{
	CubeFace()
		:	normal(0.0f)
		,	cubeFace(FRONT) 
	{}


	void init( FaceNames face, const Vector3f center, float radius )
	{
		cubeFace = face;
		switch ( face )
		{
			case FRONT:
				std::cout << "========== FRONT =========\n";
				points[0]= Vector3f( center.x + radius, center.y + radius, center.z + radius);
				points[2]= Vector3f( center.x - radius, center.y + radius, center.z + radius);
				points[1]= Vector3f( center.x + radius, center.y - radius, center.z + radius);
				points[3]= Vector3f( center.x - radius, center.y - radius, center.z + radius);
				break;
			case BACK:
				std::cout << "========== BACK =========\n";
				points[0]= Vector3f( center.x - radius, center.y - radius, center.z - radius);
				points[1]= Vector3f( center.x - radius, center.y + radius, center.z - radius);
				points[2]= Vector3f( center.x + radius, center.y - radius, center.z - radius);
				points[3]= Vector3f( center.x + radius, center.y + radius, center.z - radius);
				break;
			case TOP:
				std::cout << "========== TOP =========\n";
				points[0]= Vector3f( center.x - radius, center.y + radius, center.z + radius);
				points[1]= Vector3f( center.x + radius, center.y + radius, center.z + radius);
				points[2]= Vector3f( center.x - radius, center.y + radius, center.z - radius);
				points[3]= Vector3f( center.x + radius, center.y + radius, center.z - radius);
				break;
			case BOTTOM:
				std::cout << "========== BOTTOM =========\n";
				points[0]= Vector3f( center.x - radius, center.y - radius, center.z + radius);
				points[1]= Vector3f( center.x - radius, center.y - radius, center.z - radius);
				points[2]= Vector3f( center.x + radius, center.y - radius, center.z + radius);
				points[3]= Vector3f( center.x + radius, center.y - radius, center.z - radius);
				break;
			case LEFT:
				std::cout << "========== LEFT =========\n";
				points[0]= Vector3f( center.x - radius, center.y + radius, center.z + radius);
				points[1]= Vector3f( center.x - radius, center.y + radius, center.z - radius);
				points[2]= Vector3f( center.x - radius, center.y - radius, center.z - radius);
				points[3]= Vector3f( center.x - radius, center.y - radius, center.z + radius);
				/*
				points[0]= Vector3f( center.x - radius, center.y + radius, center.z + radius);
				points[1]= Vector3f( center.x - radius, center.y - radius, center.z + radius);
				points[2]= Vector3f( center.x - radius, center.y + radius, center.z - radius);
				points[3]= Vector3f( center.x - radius, center.y - radius, center.z - radius);
				*/
				break;
			case RIGHT:
				std::cout << "========== RIGHT =========\n";
				points[0]= Vector3f( center.x + radius, center.y + radius, center.z + radius);
				points[1]= Vector3f( center.x + radius, center.y - radius, center.z + radius);
				points[2]= Vector3f( center.x + radius, center.y + radius, center.z - radius);
				points[3]= Vector3f( center.x + radius, center.y - radius, center.z - radius);
				break;
			default:
				break;

		}
		/*
		for ( int i = 0; i <= 4; ++i )
		{
			points[i].x = points[i].x * 0.3- points[i].z * 0.86602540378f ;
			points[i].z = points[i].x * 0.86602540378f + points[i].z * 0.3;
			points[i].x -= 5.0f;
		}
		*/
	
		   
		Math::cross_points( points, normal  );
		if ( face == LEFT ) 
			normal = -normal;

		// Only normalize of at least one of the scalars are != 0
		if ( normal.x != 0.0f || normal.y != 0.0f || normal.z != 0.0f )
			normal = Math::normalize( normal );

		std::cout << "Point 1: " << points[0] << std::endl;
		std::cout << "Point 2: " << points[1] << std::endl;
		std::cout << "Point 3: " << points[2] << std::endl;
		std::cout << "Point 4: " << points[3] << std::endl;
		std::cout << "Normal : " << normal << std::endl;
		std::cout << "Radius : " << radius << std::endl;
		std::cout << "Center : " << center << std::endl;
	}

	bool intersect( const Ray &ray, float &f )
	{
		f = 0.0f;
		/* 
		Ray / plane intersection test
		======================================================

		Denominator
			den = ( p0 - l0 ) * n 

		Numerator
			num = l * n

		Result
			d = den / num	

		Legend:
			p0 = point on plane
			l0 = point on ray
			l = vecotr of ray
			n = normal of plane
			d = point of intersection on sphere

		Notes:
			If den == 0 and num != 0, the ray starts outside the plane and is paralell to it, no collision. 
			if den == 0 and num == 0, the ray starts inside the plane and is paralell to it, hits plane everywhere.
			In all other cases the ray will hit the plane, use d to find the intersection point and check if it's a point on the cube.
*/

		Vector3f dir_n = Math::normalize( ray.getDirection() );
		//Vector3f dir_n = ( ray.getDirection() );
		Vector3f orig_n = ( ray.getOrigin() );

		float den = Math::dot ( ( points[0]- orig_n ), normal);
		float num = Math::dot( dir_n,  normal );

		if ( den == 0 && num != 0 ) 
		{
			// No collision
			return true;
		}

		else if ( den == 0 && num == 0 )
		{
			// Collision, entire ray is inside plane
			return false;
		}

		float distance = den / num;

		// Ray hits the infitie plane. Find intersection point and check if it's on this cube.
		Vector3f point = orig_n + ( dir_n * distance );

		//std::cout << "distance : " << distance << std::endl;
		if ( CheckFaceCollision( point ) ) 
		{
			calcDistance( orig_n, dir_n, f);
			return true;
		}

		return false;

	}
	void calcDistance( const Vector3f &origin, const Vector3f &dir, float &f)
	{
		f = Math::dot
			( 
			 ( points[0] - origin) , 
			 ( normal / (  Math::dot ( dir, normal ) ) ) 
		);


	}
	bool CheckFaceCollision(  const Vector3f &point )
	{
		if ( cubeFace == FRONT )
		{
			if ( point.x < points[0].x && point.x > points[2].x)
			{
				if ( point.y < points[0].y && point.y > points[3].y)
				{
					return true;
				}
			}
		} else if ( cubeFace == BACK )
		{
			if ( point.x > points[0].x && point.x < points[2].x)
			{
				if ( point.y > points[0].y && point.y < points[1].y)
				{
					return true;
				}
			}
		} else if ( cubeFace == TOP )
		{
			if ( point.x > points[0].x && point.x < points[1].x)
			{
				if ( point.z < points[0].z && point.z > points[2].z)
				{
					return true;
				}
			}
		} else if ( cubeFace == BOTTOM )
		{
			if ( point.x > points[0].x && point.x < points[2].x)
			{
				if ( point.z < points[0].z && point.z > points[1].z)
				{
					//std::cout << "Bottom  x collision\n";
					return true;
				}
			}
		} else if ( cubeFace == LEFT )
		{
			//std::cout << "left" << std::endl;
			if ( point.y < points[0].y && point.y > points[2].y)
			{
				if ( point.z < points[0].z && point.z > points[1].z)
				{
					return true;
				}
			}
		} else if ( cubeFace == RIGHT )
		{
			if ( point.y < points[0].y && point.y > points[1].y)
			{
				if ( point.z < points[0].z && point.z > points[2].z)
				{
					return true;
				}
			}
		}

		// No collisio; ray hits plane, but outside the edges of this cube
		return false;
	}

Vector3f points[4];
Vector3f normal;

FaceNames cubeFace;
};
