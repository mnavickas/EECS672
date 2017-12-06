// Wall.h

#ifndef Wall_H
#define Wall_H

#include "ObjTypes/MyView.h"

class Wall : public MyView
{
public:
    Wall
    (
        ShaderIF* sIF,
        float dx,
        float dy,
        float dz
    );

    ~Wall();

    // xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
    void getMCBoundingBox(double* xyzLimits) const;
    bool handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY);
    void render();
    virtual void rotate( const cryph::AffPoint& point, const cryph::AffVector& vector, float degrees ) {}
private:
    GLuint vao[1];
    GLuint vbo[1];

    float xmin, xmax, ymin, ymax, zmin, zmax;
    float dx, dy, dz;
    static GLuint indexList[4][7];

    void defineWall();
    void renderWall();
};

#endif
