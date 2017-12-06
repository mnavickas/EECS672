// Cylinder.h

#ifndef MyView_H
#define MyView_H

#include "../SceneElement.h"

#include <GL/gl.h>

#include "ShaderIF.h"
#include "AffPoint.h"
#include "AffVector.h"

//extend some basic functionallity across modelviews
class MyView : public SceneElement
{

protected:
	typedef float vec3[3];

public:

	MyView
	(
		ShaderIF* sIF,
		const PhongMaterial& material
	)
	: SceneElement(sIF, material)
	{}

	virtual void rotate( const cryph::AffPoint& point, const cryph::AffVector& vector, float degrees ) = 0;
};

#endif
