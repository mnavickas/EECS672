// Waterway.c++

#include "Waterway.h"

#include "Cylinder.h"
#include "RectangularPrism.h"
#include "Materials.h"

Waterway::Waterway(ShaderIF* sIF, float dx, float dy, float dz)
    : MyContainer(sIF,PHONG_NONE)
{

    addComponent(new RectangularPrism(sIF,PHONG_WATER,12,2,99,dx,dy-4.9,dz));
    addComponent(new RectangularPrism(sIF,PHONG_WATER,12,2,80,dx-5,dy-4.9,dz+70));






    addComponent(new Cylinder(sIF,PHONG_LOG3,35,2,dx+6.5,dy-4,dz+14,90,0,0));
    addComponent(new Cylinder(sIF,PHONG_LOG3,29,2,dx+2,dy-4,dz+80,90,0,0));


    addComponent(new Cylinder(sIF,PHONG_LOG3,26,2,dx-6.5,dy-4,dz+5,90,0,0));
    addComponent(new Cylinder(sIF,PHONG_LOG3,39,2,dx-12,dy-4,dz+70,90,0,0));


    addComponent(new Cylinder(sIF,PHONG_LOG3,3.5,2,dx+4,dy-4,dz+50,0,0,90));


    addComponent(new Cylinder(sIF,PHONG_LOG3,3.5,2,dx-10,dy-4,dz+30,0,0,90));



    addComponent(new RectangularPrism(sIF,PHONG_WATER,12,10,3,dx-5,dy-8,dz+110));


    addComponent(new Cylinder(sIF,PHONG_WATER,1,25,dx,dy-4.9,dz-36));






}
