// Table.h

#ifndef Table_H
#define Table_H

#include "ObjTypes/MyContainer.h"

class Table : public MyContainer
{
public:
    Table
    (
        ShaderIF* shaderIF,
        float dx = 0,
        float dy = 0,
        float dz = 0,
        float height = 1,
        float width = 1,
        float length = 1
    );
};

#endif
