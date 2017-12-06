// Campfire.c++

#include "Campfire.h"

#include "Cylinder.h"
#include "RectangularPrism.h"
#include "Materials.h"


const float Campfire::x = -55;
const float Campfire::y = 0;
const float Campfire::z = 100;

Campfire::Campfire(ShaderIF* sIF)
    : MyContainer(sIF,PHONG_NONE)
{

    for(int deg = 0; deg < 360; deg+=60) addComponent(new Cylinder(sIF,PHONG_LOG2,5,1,    Campfire::x-3*sin(deg*M_PI/180.0),Campfire::y,Campfire::z-3*cos(deg*M_PI/180.0), 20,deg,0 ));

    addComponent(new Cylinder(sIF,PHONG_FIRE, 5,1.5,Campfire::x,Campfire::y+2,Campfire::z));
}
