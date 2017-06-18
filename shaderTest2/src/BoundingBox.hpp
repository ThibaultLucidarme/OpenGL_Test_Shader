#ifndef __BB__
#define __BB__


#include "Mesh.hpp"

class BoundingBox
{
protected:

	BoundingBox( Mesh* );
	virtual float MaximumDimension ()
	{
		float max = _vertices[0].x
		for (it = _vertices.begin(); it != _vertices.end(); ++it)
		{
			max = (it->x > max)? it->x : max;
			max = (it->y > max)? it->y : max;
			max = (it->z > max)? it->z : max;
		}

		return max;
	}

	virtual bool isColliding ( BoundingBox* );

};

class EllipsoidalBB: public BoundingBox
{

public:
	RectangularBB( Mesh* m): BoundingBox(m)
	{}
};

class RectangularBB: public BoundingBox
{

public:
	RectangularBB( Mesh* m): BoundingBox(m)
	{}
};

class AABB: public RectangularBB
{

public:
	AABB( Mesh* m): RectangularBB(m)
	{
		//find max vertices in all 3 directions
	}
};

#endif