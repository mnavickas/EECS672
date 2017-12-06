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
	typedef float vec2[2];

public:

	MyView
	(
		ShaderIF* sIF,
		const PhongMaterial& material
	)
	: SceneElement(sIF, material)
	{}

	virtual void rotate( const cryph::AffPoint& point, const cryph::AffVector& vector, float degrees ) = 0;

protected:

	enum
	{
		TEXTURE_BIT = 1,
		VIEW_BIT,
		MATERIAL_BIT,
		LIGHTING_BIT,

		TEXTURE = 	1 << TEXTURE_BIT,
		LIGHTING = 	1 << LIGHTING_BIT,
		VIEW = 		1 << VIEW_BIT,
		MATERIAL =	1 << MATERIAL_BIT

	};

	GLint pgm;
	void initRender(uint fields)
	{
	    glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	    glUseProgram(shaderIF->getShaderPgmID());

		if( fields & VIEW ) 	establishView();
		if( fields & MATERIAL ) establishMaterial();
		if( fields & LIGHTING ) establishLightingEnvironment();
		if( fields & TEXTURE )	establishTexture();

		cryph::Matrix4x4 mc_ec, ec_lds;
	    getMatrices(mc_ec, ec_lds);
	    float mat[16];
	    glUniformMatrix4fv(shaderIF->ppuLoc("mc_ec"), 1, false, mc_ec.extractColMajor(mat));
	    glUniformMatrix4fv(shaderIF->ppuLoc("ec_lds"), 1, false, ec_lds.extractColMajor(mat));
	}

	void finishRender()
	{
	    glUseProgram(pgm);
	}
};

#endif
