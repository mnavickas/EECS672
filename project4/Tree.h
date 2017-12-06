// Tree.h

#ifndef Tree_H
#define Tree_H

#include <GL/gl.h>

#include "ObjTypes/MyContainer.h"

class Tree : public MyContainer
{
public:
    Tree
    (
        ShaderIF* shaderIF,
        float dx = 0,
        float dy = 0,
        float dz = 0
    );
};

#endif
