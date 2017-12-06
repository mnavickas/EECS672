// TriBezierModel.h - a concrete 3D subclass of ModelView

#ifndef TRIBEZIERMODEL_H
#define TRIBEZIERMODEL_H

#include <string>

#include "ProjPoint.h"
#include "ModelView.h"

#include "ShaderIF.h"

class TriBezierModel : public ModelView
{
public:
	TriBezierModel(bool useTessShadersIn, bool useGeometryShaderIn);
	virtual ~TriBezierModel();

	std::string getShaderString() const;
	void getMCBoundingBox(double* xyzLimits) const; // {xmin, xmax, ymin, ymax, zmin, zmax}
	bool handleCommand(unsigned char key, double ldsX, double ldsY);
	void printKeyboardKeyList(bool firstCall) const;
	void render();

	static void getBackgroundColor(double& bkR, double& bkG, double& bkB, double& bkA);

private:
	GLuint vaoCtlPoints[1];
	GLuint ctlPointBuffer[2]; // 0: color; 1: coordinates
	GLuint ebo[3]; // Element buffer for bottom, middle, top control polygon row
	GLenum polyMode_CtlPolygon, polyMode_TessSurface;
	cryph::AffVector normalTo_43_Row, normalTo_32_Row, normalTo_21_Row;
	float pointSize;
	int pointGenerationMethod; // for Geometry Shader
	bool drawControlPolygon, drawControlPoints, drawTessSurface;
	bool useTessShaders, useGeometryShader;

	// Each instance may have a different shader program.
	ShaderIF::ShaderSpec* shaders;
	ShaderIF* shaderIF;

	void cycleMode(std::string what, GLenum& mode);
	void defineInitialGeometry();

	static void floatBufferData(cryph::ProjPoint* pts, int nPoints);
	static float* floatBuffer;
	static int floatBufferSize;
	static TriBezierModel* firstModel;

	static void scaleBackground(double f);
	static double bkgRed, bkgGreen, bkgBlue, bkgAlpha;
};

#endif
