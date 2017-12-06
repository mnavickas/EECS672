// Bed.c++
#include "Bed.h"

#include "Cylinder.h"
#include "RectangularPrism.h"
#include "Materials.h"

Bed::Bed(ShaderIF* sIF, float dx, float dy, float dz)
    : MyContainer(sIF,PHONG_NONE)
{
    //'pillow'
    addComponent(new Cylinder(sIF, PHONG_PILLOW,5.9,2,	-10+dx,6+dy,0+dz,	90,0,0));
    //legs
    addComponent(new Cylinder(sIF,PHONG_CYL,4,1,  -8+dx,0+dy, 4+dz,       0,270,0));
    addComponent(new Cylinder(sIF,PHONG_CYL,4,1,   8+dx,0+dy, 4+dz,       0,270,0));
    addComponent(new Cylinder(sIF,PHONG_CYL,4,1,  -8+dx,0+dy,-4+dz,       0,270,0));
    addComponent(new Cylinder(sIF,PHONG_CYL,4,1,   8+dx,0+dy,-4+dz,       0,270,0));
    //mattress
    addComponent(new RectangularPrism(sIF, PHONG_BED, 24,4,12,  0+dx,4+dy,0+dz));
}
