// TriBezierModel.c++ - a concrete 3D subclass of ModelView

#include "Inline.h"

#include "Controller.h"
#include "TriBezierModel.h"
#include "ShaderIF.h"

// following used when send cryph data to GPU
float* TriBezierModel::floatBuffer = NULL;
int TriBezierModel::floatBufferSize = 0;
// END

static const int NUM_CONTROL_POINTS_PER_PATCH = 16;
static const int NUM_PATCHES = 1;
static const int NUM_CONTROL_POINTS = NUM_CONTROL_POINTS_PER_PATCH * NUM_PATCHES;

static ShaderIF::ShaderSpec vertexShader   = {"shaders/TriBezier.vsh",  GL_VERTEX_SHADER};
static ShaderIF::ShaderSpec tessCtlShader  = {"shaders/TriBezier.tcsh", GL_TESS_CONTROL_SHADER};
static ShaderIF::ShaderSpec tessEvalShader = {"shaders/TriBezier.tesh", GL_TESS_EVALUATION_SHADER};
static ShaderIF::ShaderSpec geomShader     = {"shaders/TriBezier.gsh",  GL_GEOMETRY_SHADER};
static ShaderIF::ShaderSpec fragShader     = {"shaders/TriBezier.fsh",  GL_FRAGMENT_SHADER};

// Index lists for drawing the triangular bezier control polygon
static unsigned int indicesBottomRow[] = { 0, 4, 1, 5, 2, 6, 3 },
                    indicesMiddleRow[] = { 4, 7, 5, 8, 6 },
                    indicesTopRow[]    = { 7, 9, 8 };
static const int numInBottomRow = 7, numInMiddleRow = 7, numInTopRow = 7;

TriBezierModel* TriBezierModel::firstModel = NULL;

double TriBezierModel::bkgRed = 0.05;
double TriBezierModel::bkgGreen = 0.05;
double TriBezierModel::bkgBlue = 0.05;
double TriBezierModel::bkgAlpha = 1.0;

TriBezierModel::TriBezierModel(bool useTessShadersIn, bool useGeometryShaderIn) :
	polyMode_CtlPolygon(GL_LINE), polyMode_TessSurface(GL_FILL), pointSize(1.0),
	pointGenerationMethod(1), // for Geometry Shader only
	drawControlPolygon(true), drawControlPoints(true),
	drawTessSurface(true),
	useTessShaders(useTessShadersIn), useGeometryShader(useGeometryShaderIn)
{
	if (firstModel == NULL)
		firstModel = this;

	// The first time an instance of this class is generated, create the common shader program:
	int nShaders = 2;
	if (useTessShaders)
		nShaders += 2;
	if (useGeometryShader)
		nShaders++;
	shaders = new ShaderIF::ShaderSpec[nShaders];
	shaders[0] = vertexShader;
	if (nShaders == 3)
		shaders[1] = geomShader;
	else if (nShaders > 2)
	{
		shaders[1] = tessCtlShader;
		shaders[2] = tessEvalShader;
		if (nShaders == 5)
			shaders[3] = geomShader;
	}
	shaders[nShaders-1] = fragShader;
	shaderIF = new ShaderIF(shaders, nShaders);

	// Now do instance-specific initialization:
	defineInitialGeometry();
}

TriBezierModel::~TriBezierModel()
{
	glDeleteBuffers(2, ctlPointBuffer);
	glDeleteBuffers(3, ebo);
	glDeleteVertexArrays(1, vaoCtlPoints);
	delete shaderIF;
	delete [] shaders;
}

void TriBezierModel::cycleMode(std::string what, GLenum& mode)
{
	if (mode == GL_POINT)
	{
		if (this == firstModel) std::cout << what << ": GL_LINE\n";
		mode = GL_LINE;
	}
	else if (mode == GL_LINE)
	{
		if (this == firstModel) std::cout << what << ": GL_FILL\n";
		mode = GL_FILL;
	}
	else // GL_FILL
	{
		if (this == firstModel) std::cout << what << ": GL_POINT\n";
		mode = GL_POINT;
	}
}

void TriBezierModel::defineInitialGeometry()
{
	cryph::ProjPoint diffuseRef[NUM_CONTROL_POINTS] = // really reflectivities...
	{
		// first row of 4:
		cryph::ProjPoint(1.0, 0.0, 0.0, 1.0), cryph::ProjPoint(1.0, 0.0, 0.0, 1.0),
		cryph::ProjPoint(1.0, 0.0, 0.0, 1.0), cryph::ProjPoint(1.0, 0.0, 0.0, 1.0),
		// second row of 3:
		cryph::ProjPoint(1.0, 1.0, 0.0, 1.0), cryph::ProjPoint(1.0, 1.0, 0.0, 1.0),
		cryph::ProjPoint(1.0, 1.0, 0.0, 1.0),
		// third row of 2:
		cryph::ProjPoint(0.0, 1.0, 0.0, 1.0), cryph::ProjPoint(0.0, 1.0, 0.0, 1.0),
		// last row of 1:
		cryph::ProjPoint(0.0, 1.0, 0.0, 1.0)
	};

	cryph::ProjPoint mcPosition[NUM_CONTROL_POINTS] = // actual control point position
	{
		// first row of 4:
		cryph::ProjPoint(-0.75, -0.75, 0.25, 1.0), cryph::ProjPoint(-0.25, -0.75, 0.25, 1.0),
		cryph::ProjPoint( 0.25, -0.75, 0.25, 1.0), cryph::ProjPoint( 0.75, -0.75, 0.25, 1.0),
		// second row of 3:
		cryph::ProjPoint(-0.75, -0.25, 0.15, 1.0), cryph::ProjPoint(-0.25, -0.25, 0.15, 1.0),
		cryph::ProjPoint( 0.25, -0.25, 0.15, 1.0), cryph::ProjPoint( 0.75, -0.75, 0.15, 1.0),
		// third row of 2:
		cryph::ProjPoint(-0.75, 0.25, -0.45, 1.0), cryph::ProjPoint(-0.25,  0.25, -0.45, 1.0),
		cryph::ProjPoint( 0.25, 0.25, -0.45, 1.0), cryph::ProjPoint( 0.75,  0.25, -0.45, 1.0),
		// last row of 1:
		cryph::ProjPoint(-0.75, 0.75, -0.60, 1.0), cryph::ProjPoint(-0.25, 0.75, -0.60, 1.0),
		cryph::ProjPoint( 0.25, 0.75, -0.60, 1.0), cryph::ProjPoint( 0.75, 0.75, -0.60, 1.0)
	};

	normalTo_43_Row = (mcPosition[1].aCoords() - mcPosition[0].aCoords()).cross
	                  (mcPosition[4].aCoords() - mcPosition[0].aCoords());

	normalTo_32_Row = (mcPosition[5].aCoords() - mcPosition[4].aCoords()).cross
	                  (mcPosition[7].aCoords() - mcPosition[4].aCoords());

	normalTo_21_Row = (mcPosition[8].aCoords() - mcPosition[7].aCoords()).cross
	                  (mcPosition[9].aCoords() - mcPosition[7].aCoords());

	glGenVertexArrays(1, vaoCtlPoints);
	glBindVertexArray(vaoCtlPoints[0]);

	glGenBuffers(2, ctlPointBuffer);
	glGenBuffers(3, ebo);

	glBindBuffer(GL_ARRAY_BUFFER, ctlPointBuffer[0]);
	floatBufferData(diffuseRef, NUM_CONTROL_POINTS);
	glVertexAttribPointer(shaderIF->pvaLoc("diffuseRef"), 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderIF->pvaLoc("diffuseRef"));

	glBindBuffer(GL_ARRAY_BUFFER, ctlPointBuffer[1]);
	floatBufferData(mcPosition, NUM_CONTROL_POINTS);
	glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));
	glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numInBottomRow*sizeof(unsigned int),
		indicesBottomRow, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numInMiddleRow*sizeof(unsigned int),
		indicesMiddleRow, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numInTopRow*sizeof(unsigned int),
		indicesTopRow, GL_STATIC_DRAW);

	glDisableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));
}

void TriBezierModel::floatBufferData(cryph::ProjPoint* pts, int nPoints)
{
	int requiredSize = nPoints * 4;
	if (TriBezierModel::floatBufferSize < requiredSize)
	{
		if (TriBezierModel::floatBuffer != NULL)
			delete [] floatBuffer;
		TriBezierModel::floatBuffer = new float[requiredSize];
		TriBezierModel::floatBufferSize = requiredSize;
	}
	int offset = 0;
	for (int i=0 ; i<nPoints ; i++, offset+=4)
		pts[i].pCoords(TriBezierModel::floatBuffer, offset);
	int sizeInBytes = requiredSize * sizeof(float);
	glBufferData(GL_ARRAY_BUFFER, sizeInBytes, TriBezierModel::floatBuffer, GL_STATIC_DRAW);
}

void TriBezierModel::getBackgroundColor(double& bkR, double& bkG, double& bkB, double& bkA)
{
	bkR = bkgRed;
	bkG = bkgGreen;
	bkB = bkgBlue;
	bkA = bkgAlpha;
}

std::string TriBezierModel::getShaderString() const
{
	std::string lab = "v";
	if (useTessShaders)
		lab += "t";
	if (useGeometryShader)
		lab += "g";
	return lab + "f";
}

void TriBezierModel::getMCBoundingBox(double* xyzLimits) const // {xmin, xmax, ymin, ymax, zmin, zmax}
{
	xyzLimits[0] = -1.0; xyzLimits[1] = 1.0;
	xyzLimits[2] = -1.0; xyzLimits[3] = 1.0;
	xyzLimits[4] = -1.0; xyzLimits[5] = 1.0;
}

bool TriBezierModel::handleCommand(unsigned char key, double ldsX, double ldsY)
{
	bool retValue = true;
	if (key == 'b')
		scaleBackground(1.0/1.1);
	else if (key == 'B')
		scaleBackground(1.1);
	else if (key == 'g')
		pointGenerationMethod = (pointGenerationMethod + 1) % 4;
	else if (key == 'm')
		cycleMode("tessellated surface", polyMode_TessSurface);
	else if (key == 'M')
		cycleMode("control polygon", polyMode_CtlPolygon);
	else if (key == 's')
	{
		std::cout << "For " << getShaderString() << ", pointSize = " << pointSize << "; enter new value: ";
		std::cin >> pointSize;
	}
	else if (key == '1')
		drawControlPolygon = !drawControlPolygon;
	else if (key == '2')
		drawControlPoints = !drawControlPoints;
	else if (key == '3')
		drawTessSurface = !drawTessSurface;
	else
		retValue = ModelView::handleCommand(key, ldsX, ldsY);
	Controller::getCurrentController()->redraw();
	return retValue;
}

void TriBezierModel::printKeyboardKeyList(bool firstCall) const
{
	if (!firstCall)
		return;

	std::cout << "TriBezierModel:\n";
	std::cout << "\tb: darken background\n";
	std::cout << "\tB: brighten background\n";
	std::cout << "\tg: cycle through geometry shader modes (vgf or vtgf)\n";
	std::cout << "\tm: cycle through polygon modes for tessSurface (vtf)\n";
	std::cout << "\t   (Overrides ModelView's 'p@0', et al.)\n";
	std::cout << "\tM: cycle through polygon modes for ctlPolygon (vf)\n";
	std::cout << "\t   (Overrides ModelView's 'p@0', et al.)\n";
	std::cout << "\ts: set point size\n";
	std::cout << "\t1: toggle drawing control polygon (vf or vgf)\n";
	std::cout << "\t2: toggle drawing control points (vf)\n";
	std::cout << "\t3: toggle drawing point cloud (vf)\n";
	std::cout << "\t4: toggle drawing tessellated surface (vtf or vtgf)\n";
}

void TriBezierModel::render()
{
	// save the current GLSL program in use
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);

	// Establish ours
	glUseProgram(shaderIF->getShaderPgmID());

	// define the mapping from MC to -1..+1 space:
	cryph::Matrix4x4 mc_ec, ec_lds;
	ModelView::getMatrices(mc_ec, ec_lds);
	float m[16];
	glUniformMatrix4fv(shaderIF->ppuLoc("mc_ec"), 1, false, mc_ec.extractColMajor(m));
	glUniformMatrix4fv(shaderIF->ppuLoc("ec_lds"), 1, false, ec_lds.extractColMajor(m));

	glPointSize(pointSize);
	if (useTessShaders)
	{
		if (drawTessSurface)
		{
			GLint vp[4];
			glGetIntegerv(GL_VIEWPORT, vp);
			glUniform2i(shaderIF->ppuLoc("vpWidthHeight"), vp[2], vp[3]);
			if (shaderIF->ppuExists("pointGenerationMethod") >= 0)
				glUniform1i(shaderIF->ppuLoc("pointGenerationMethod"), pointGenerationMethod);
			glBindVertexArray(vaoCtlPoints[0]);
			glPatchParameteri(GL_PATCH_VERTICES, NUM_CONTROL_POINTS_PER_PATCH);
			glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, 1.0, 0.0); // will be replaced by tess shader
			glPolygonMode(GL_FRONT_AND_BACK, polyMode_TessSurface);
			glDrawArrays(GL_PATCHES, 0, NUM_CONTROL_POINTS);
		}
	}
	else
	{
		float normalBuf[3];
		if (drawControlPolygon)
		{
			glBindVertexArray(vaoCtlPoints[0]);
			glPolygonMode(GL_FRONT_AND_BACK, polyMode_CtlPolygon);
			if (shaderIF->ppuExists("pointGenerationMethod") >= 0)
				glUniform1i(shaderIF->ppuLoc("pointGenerationMethod"), pointGenerationMethod);
			glVertexAttrib3fv(shaderIF->pvaLoc("mcNormal"), normalTo_43_Row.vComponents(normalBuf));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
			glDrawElements(GL_TRIANGLE_STRIP, numInBottomRow, GL_UNSIGNED_INT, NULL);
			glVertexAttrib3fv(shaderIF->pvaLoc("mcNormal"), normalTo_32_Row.vComponents(normalBuf));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
			glDrawElements(GL_TRIANGLE_STRIP, numInMiddleRow, GL_UNSIGNED_INT, NULL);
			glVertexAttrib3fv(shaderIF->pvaLoc("mcNormal"), normalTo_21_Row.vComponents(normalBuf));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[2]);
			glDrawElements(GL_TRIANGLE_STRIP, numInTopRow, GL_UNSIGNED_INT, NULL);
		}

		if (!useGeometryShader)
		{
			// can only draw point clouds if there is no geometry shader. The
			// geometry shader is expecting only triangles.
			if (drawControlPoints)
			{
				glBindVertexArray(vaoCtlPoints[0]);

				glVertexAttrib3fv(shaderIF->pvaLoc("mcNormal"), normalTo_43_Row.vComponents(normalBuf));
				glDrawArrays(GL_POINTS, 0, 4);

				cryph::AffVector avg = normalTo_43_Row + normalTo_32_Row;
				glVertexAttrib3fv(shaderIF->pvaLoc("mcNormal"), avg.vComponents(normalBuf));
				glDrawArrays(GL_POINTS, 4, 3);

				avg = normalTo_32_Row + normalTo_21_Row;
				glVertexAttrib3fv(shaderIF->pvaLoc("mcNormal"), avg.vComponents(normalBuf));
				glDrawArrays(GL_POINTS, 7, 2);

				glVertexAttrib3fv(shaderIF->pvaLoc("mcNormal"), normalTo_21_Row.vComponents(normalBuf));
				glDrawArrays(GL_POINTS, 9, 1);
			}
		}
	}

	// restore the previous program
	if (pgm > 0)
		glUseProgram(pgm);
	glFlush(); // Seems to be important to do this here since we are about to change GLSL programs.
}

void TriBezierModel::scaleBackground(double f)
{
	double r = bkgRed * f;
	double g = bkgGreen * f;
	double b = bkgBlue * f;
	if ((r <= 1.0) && (g <= 1.0) && (b <= 1.0))
	{
		bkgRed = r;
		bkgGreen = g;
		bkgBlue = b;
		glClearColor(bkgRed, bkgBlue, bkgGreen, bkgAlpha);
	}
}
