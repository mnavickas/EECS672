// Campfire.h

#ifndef Campfire_H
#define Campfire_H

#include <GL/gl.h>

#include "ObjTypes/MyContainer.h"

class Campfire : public MyContainer
{
public:
    Campfire
    (
        ShaderIF* shaderIF
    );

    static const float x,y,z;

};

#endif
