// Waterway.h

#ifndef Waterway_H
#define Waterway_H

#include <GL/gl.h>

#include "ObjTypes/MyContainer.h"

class Waterway : public MyContainer
{
public:
    Waterway
    (
        ShaderIF* shaderIF,
        float dx = 0,
        float dy = 0,
        float dz = 0
    );
};

#endif
