// Tree.c++

#include "Tree.h"

#include "Cylinder.h"
#include "RectangularPrism.h"
#include "Materials.h"

Tree::Tree(ShaderIF* sIF, float dx, float dy, float dz)
    : MyContainer(sIF,PHONG_NONE)
{

    Cylinder* trunk = new Cylinder(sIF, PHONG_LOG1,30,4,dx,dy+25,dz,0,0,0);

    addComponent(trunk);


    addComponent(new RectangularPrism(sIF,PHONG_LEAVES,50,10,10,dx,dy+40,dz));
    addComponent(new RectangularPrism(sIF,PHONG_LEAVES,40,25,25,dx,dy+40,dz));

    addComponent(new RectangularPrism(sIF,PHONG_LEAVES,10,10,50,dx,dy+40,dz));
    addComponent(new RectangularPrism(sIF,PHONG_LEAVES,25,25,40,dx,dy+40,dz));

    addComponent(new RectangularPrism(sIF,PHONG_LEAVES,25,35,25,dx,dy+40,dz));
}
