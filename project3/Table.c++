// Table.c++
#include "Table.h"

#include "Cylinder.h"
#include "RectangularPrism.h"
#include "Materials.h"



Table::Table(ShaderIF* sIF, float dx, float dy, float dz, float height, float width, float length)
    : MyContainer(sIF, PHONG_NONE)
{
    const int tableWidth = 10*width;
    const int tableLength = 10*length;


    addComponent(new Cylinder(sIF,PHONG_TABLELEG,4*height,1,   -(tableWidth/2 - tableWidth/8)+dx, dy,  (tableLength/2 - tableLength/8)+dz));
    addComponent(new Cylinder(sIF,PHONG_TABLELEG,4*height,1,    (tableWidth/2 - tableWidth/8)+dx, dy,  (tableLength/2 - tableLength/8)+dz));
    addComponent(new Cylinder(sIF,PHONG_TABLELEG,4*height,1,   -(tableWidth/2 - tableWidth/8)+dx, dy, -(tableLength/2 - tableLength/8)+dz));
    addComponent(new Cylinder(sIF,PHONG_TABLELEG,4*height,1,    (tableWidth/2 - tableWidth/8)+dx, dy, -(tableLength/2 - tableLength/8)+dz));

    RectangularPrism* prism = new RectangularPrism(sIF, PHONG_TABLETOP, 10*width,1,10*length,  dx, dy+4*height, dz);
    addComponent( prism );
}
