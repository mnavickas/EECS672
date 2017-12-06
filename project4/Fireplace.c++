// Fireplace.c++

#include "Fireplace.h"

#include "Cylinder.h"
#include "RectangularPrism.h"
#include "Wall.h"
#include "Materials.h"

static int fcount = 0;
Fireplace::Fireplace(ShaderIF* sIF, float dx, float dy, float dz)
    : MyContainer(sIF,PHONG_NONE)
{
    const int layers = 3;
    const int count = 24*layers;
    const float openRadius = 10;
    const float dTheta = M_PI/(count/layers);

    vec3 cylinderCenters[count];
    float cylinderOrientation[count];

    float i = 0.0;
    for(int index = 0; index<count; i+=dTheta)
    {
        for(int j = 0; j < layers; j++, index++)
        {
            cylinderCenters[index][0] = dx+3*j-5.5;
            cylinderCenters[index][1] = openRadius*sin(i) + dy -3;
            cylinderCenters[index][2] = openRadius*cos(i) + dz;
            cylinderOrientation[index] = 90+(180.0/M_PI*cos(i));
        }
    }

    for(int j = 0;j< count; j++)
    {
        addComponent( new Cylinder(sIF,PHONG_BRICK,2.5,2,    cylinderCenters[j][0],cylinderCenters[j][1],cylinderCenters[j][2], cylinderOrientation[j],0,0 ) );
    }

    //logs
    addComponent( new Cylinder(sIF,PHONG_LOG1,5,1,    dx-4,dy,dz, 90,30,0 ) );
    addComponent( new Cylinder(sIF,PHONG_LOG2,5.5,1.2,    dx-4.2,dy-0.1,dz+1.5, 90,-30,45 ) );

    //floor
    addComponent( new RectangularPrism(sIF,PHONG_BRICK,16,2,27,dx-3,dy-3,dz) );

    addComponent( new RectangularPrism(sIF,PHONG_BRICK,1,20,27,dx+1.5,dy+10-3,dz) );
    addComponent( new RectangularPrism(sIF,PHONG_BRICK,1,10,18,dx,dy,dz) );
    addComponent( new RectangularPrism(sIF,PHONG_BRICK, 10,42,10,    dx+3.8,dy+26,dz) );

}
