// Floor.h

#ifndef Floor_H
#define Floor_H

#include "ObjTypes/MyView.h"

class Floor : public MyView
{
public:
    Floor
    (
        ShaderIF* sIF,
        const PhongMaterial& mat,
        float minx,
        float maxx,
        float miny,
        float maxy,
        float minz,
        float maxz,
        bool addToBB = true
    );
    ~Floor();

    // xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
    void getMCBoundingBox(double* xyzLimits) const;
    bool handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY);
    void render();
    virtual void rotate( const cryph::AffPoint& point, const cryph::AffVector& vector, float degrees ){};
private:
    GLuint vao[1];
    GLuint vbo[1];
    GLuint ebo[3];
    static GLuint indexList[3][4];
    bool addToBb;

    float xmin, xmax, ymin, ymax, zmin, zmax;

    void defineFloor();
    void renderFloor();
};

#endif
