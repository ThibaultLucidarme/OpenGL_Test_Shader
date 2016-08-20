#ifndef __BB__
#define __BB__


#include "Mesh.hpp"

class BoundingBox
{
protected:

	BoundingBox( Mesh* );
	virtual float MaximumDimension ();
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