// WindowedWall.h

#ifndef WindowedWall_H
#define WindowedWall_H

#include "ObjTypes/MyView.h"

class WindowedWall : public MyView
{
public:
    WindowedWall
    (
        ShaderIF* sIF,
        float dx,
        float dy,
        float dz
    );

    ~WindowedWall();

    // xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
    void getMCBoundingBox(double* xyzLimits) const;
    bool handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY);
    void render();
    virtual void rotate( const cryph::AffPoint& point, const cryph::AffVector& vector, float degrees ) {}
private:
    GLuint vao[1];
    GLuint vbo[2];

    float xmin, xmax, ymin, ymax, zmin, zmax;
    float dx, dy, dz;
    static GLuint indexList[5][4];

    void defineWindowedWall();
    void renderWindowedWall();
};

#endif
