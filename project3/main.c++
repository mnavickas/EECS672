// main.c++: main program for simple 3D scene consisting
//           of an "M and M" shape along with a cylinder.

#include <iostream>
#include <fstream>

#include "GLFWController.h"

#include "Bed.h"
#include "Table.h"
#include "Floor.h"
#include "Fireplace.h"
#include "Materials.h"
#include "Tree.h"
#include "Waterway.h"
#include "Campfire.h"

void set3DViewingInformation(double xyz[6])
{
    // Tell class ModelView we initially want to see the whole scene:
    ModelView::setMCRegionOfInterest(xyz);

    // Two common computations to help determine a reasonable initial view:
    // (i) Find the maximum of the three MC deltas
    double maxDelta = xyz[1] - xyz[0];
    double delta = xyz[3] - xyz[2];
    if (delta > maxDelta)
        maxDelta = delta;
    delta = xyz[5] - xyz[4];
    if (delta > maxDelta)
        maxDelta = delta;
    // (ii) Determine the center of the created scene:
    double xmid = 0.5 * (xyz[0] + xyz[1]);
    double ymid = 0.5 * (xyz[2] + xyz[3]);
    double zmid = 0.5 * (xyz[4] + xyz[5]);

    // COMMON HEURISTIC FOR ESTABLISHING THE MC ==> EC TRANSFORMATION:
    // Create the line of sight through the center of the scene:
    // 1) Make the center of attention be the center of the bounding box.
    // 2) Move the eye away along some direction - here (0,0,1) - so that the
    //    distance between the eye and the center is (2*max scene dimension).
    // 3) Set the "up" direction vector to orient the 3D view
    //
    // IF YOU DO NOT CHOOSE TO USE THE COMMON HEURISTIC:
    // 1) Simply set eye, center, and up however you choose.
    //
    // NOTE: ModelView::getMatrices - used during display callbacks -
    //       implicitly assumes the line of sight passes through what we want
    //       to have in the center of the display window. Hence, even if you
    //       want to define the line of sight in a specific way, you MAY still
    //       want to use the center of the bounding box as the center of
    //       attention. (One common exception: focusing on a part of the scene
    //       far away from the center of the overall scene.)

    // 1:
    cryph::AffPoint center(xmid, ymid, zmid);

    // 2:
    double distEyeCenter = 1.1 * maxDelta;
    cryph::AffPoint eye(xmid, ymid, zmid+distEyeCenter );


    // 3:
    cryph::AffVector up = cryph::AffVector::yu;

    // Notify the ModelView of our MC->EC viewing requests:
    ModelView::setEyeCenterUp(eye, center, up);

    // COMMON HEURISTIC FOR ESTABLISHING THE PROJECTION TRANSFORMATION:
    // Place the projection plane (ECz = ecZpp) roughly at the front of scene
    // and set eye coordinate ecZmin/ecZmax clipping planes relative to it.
    // IMPORTANT NOTE: For perspective projections, the following must hold:
    // 1) ecZpp < 0
    // 2) ecZmin < ecZmax < 0
    // For non-perspective projections, it is only necessary that ecZmin < ecZmax.

    double ecZpp = -1*(distEyeCenter - 0.5*maxDelta);
    double ecZmin = ecZpp - 2.0*maxDelta;
    double ecZmax = ecZpp + 0.5*maxDelta;

    ModelView::setProjection(PERSPECTIVE);
    ModelView::setECZminZmax(ecZmin, ecZmax);
    ModelView::setProjectionPlaneZ(ecZpp);
}

int main(int argc, char* argv[])
{
    GLFWController c("Cabin cross-section in the woods", MVC_USE_DEPTH_BIT);
    c.reportVersions(std::cout);

    ShaderIF* sIF = new ShaderIF("shaders/basic.vsh", "shaders/phong.fsh");

    //first bed
    c.addModel(new Bed(sIF));

    //second (closer) bed
    c.addModel(new Bed(sIF,0,0,24));

    //bedside
    c.addModel(new Table(sIF,-5,0,-12,1,0.8,0.8));

    //second (closer)
    c.addModel(new Table(sIF,-5,0, 12 ,1,0.8,0.8));

    //desk
    c.addModel(new Table(sIF,30,0,-44.9,  1.3,2.3,1.0));


    //c.addModel(new Floor(sIF));
    c.addModel(new Floor(sIF,PHONG_FLOOR,-15,70,-4,-6,-50,30));

    //grass
    c.addModel(new Floor(sIF,PHONG_GRASS,-100,-15,-4,-6,-50,110));
    c.addModel(new Floor(sIF,PHONG_GRASS,-15,70,-4,-6,30,110));

    c.addModel(new Fireplace(sIF,70));
    c.addModel(new Fireplace(sIF,69.5,0,-30));

    c.addModel(new Tree(sIF,0,0,100));

    c.addModel(new Waterway(sIF,-70,0,0));

    c.addModel( new Campfire(sIF));

    glClearColor(0.0, 0.0, 0.0, 0.0);

    double xyz[6];
    c.getOverallMCBoundingBox(xyz);
    set3DViewingInformation(xyz);

    c.run();


    delete sIF;

    return 0;
}
