// Door.h

#ifndef Door_H
#define Door_H

#include "ObjTypes/MyView.h"

class Door : public MyView
{
public:
    Door
    (
        ShaderIF* sIF
    );

    ~Door();


        // xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
    void getMCBoundingBox(double* xyzLimits) const;
    void render();
    virtual void rotate( const cryph::AffPoint& point, const cryph::AffVector& vector, float degrees ){};

    bool handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY);
private:
    GLuint vao[1];
    GLuint vbo[2];
    GLuint ebo[3];

    float xmin, xmax, ymin, ymax, zmin, zmax;

    static GLuint indexList[3][4];

    void defineDoor();
    void renderDoor();
};

#endif
