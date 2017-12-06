// triBezier.c++: An OpenGL program that manipulates triangular Bezier
//                patches with various shaders

#include <iostream>
#include <string.h>

#include "GLFWController.h"
#include "TriBezierModel.h"

void set3DViewingInformation(double xyz[6])
{
	// Simplest case: Just tell the ModelView we want to see it all:
	ModelView::setMCRegionOfInterest(xyz);

	// COMMON HEURISTIC FOR ESTABLISHING THE MC ==> EC TRANSFORMATION:
	// Create the line of sight through the center of the scene:
	// 1) Make the center of attention be the center of the bounding box.
	// 2) Move the eye away along some direction - here (0,0,1) - so that the
	//    distance between the eye and the center is (2*max scene dimension).
	// 3) Set the "up" direction vector to orient the 3D view
	// NOTE: ModelView::getMatrices - used during display callbacks -
	//       implicitly assumes the line of sight passes through what we want
	//       to have in the center of the display window.

	// 1:
	double xmid = 0.5 * (xyz[0] + xyz[1]);
	double ymid = 0.5 * (xyz[2] + xyz[3]);
	double zmid = 0.5 * (xyz[4] + xyz[5]);
	cryph::AffPoint center(xmid, ymid, zmid);

	// 2:
	double maxDelta = xyz[1] - xyz[0];
	double delta = xyz[3] - xyz[2];
	if (delta > maxDelta)
		maxDelta = delta;
	delta = xyz[5] - xyz[4];
	if (delta > maxDelta)
		maxDelta = delta;
	double distEyeCenter = 2.0 * maxDelta;
	cryph::AffPoint eye(xmid, ymid, zmid + distEyeCenter);

	// 3:
	cryph::AffVector up = cryph::AffVector::yu;

	// Notify the ModelView of our MC->EC viewing requests:
	ModelView::setEyeCenterUp(eye, center, up);

	// COMMON HEURISTIC FOR ESTABLISHING THE PROJECTION TRANSFORMATION:
	// Place the projection plane roughly at the front of scene and
	// set eye coordinate zmin/zmax clipping planes relative to it.
	// IMPORTANT NOTE: For perspective projections, the following must hold:
	// 1) zpp < 0
	// 2) zmin < zmax < 0
	// For non-perspective projections, it is only necessary that zmin < zmax.
	double zpp = -(distEyeCenter - 0.5*maxDelta);
	ModelView::setProjectionPlaneZ(zpp);
	double zmin = zpp - maxDelta;
	double zmax = zpp + 0.5*maxDelta;
	ModelView::setECZminZmax(zmin, zmax);
}

int main(int argc, char* argv[])
{
	std::string title = "Triangular Bezier";
	Controller* c = new GLFWController(title, MVC_USE_DEPTH_BIT);
	for (int i=1 ; i<argc ; i++)
	{
		if (strcmp(argv[i], "vf") == 0)
			c->addModel( new TriBezierModel(false, false) );
		else if (strcmp(argv[i], "vtf") == 0)
			c->addModel( new TriBezierModel(true, false) );
		else if (strcmp(argv[i], "vtgf") == 0)
			c->addModel( new TriBezierModel(true, true) );
		else if (strcmp(argv[i], "vgf") == 0)
			c->addModel( new TriBezierModel(false, true) );
		else
		{
			std::cerr << "\nUnrecognized command line parameter: " << argv[i] << '\n';
			continue;
		}
		if (i == 1)
			title += std::string(": ");
		else
			title += std::string(", ");
		title += std::string(argv[i]);
	}

	if (c->getNumModels() == 0)
		std::cerr << "\nUsage: " << argv[0] << " { vf | vtf | vtgf | vgf }\n";
	else
	{
		c->setWindowTitle(title);
		c->reportVersions(std::cout);
		double r, g, b, a;
		TriBezierModel::getBackgroundColor(r, g, b, a);
		glClearColor(r, g, b, a);

		// initialize 3D Viewing information:
		// Get the overall scene bounding box in Model Coordinates:
		double xyz[6];
		c->getOverallMCBoundingBox(xyz);
		set3DViewingInformation(xyz);

		c->run();
	}
	delete c;

	return 0;
}
