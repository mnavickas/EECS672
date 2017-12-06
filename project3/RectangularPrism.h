// RectangularPrism.h

#ifndef RectangularPrism_H
#define RectangularPrism_H

#include "ObjTypes/MyView.h"
#include "PhongMaterial.h"

class RectangularPrism : public MyView
{
public:
    RectangularPrism
    (
        ShaderIF* sIF,
        const PhongMaterial& mat,
        float scaleX,
        float scaleY,
        float scaleZ,
        float dx,
        float dy,
        float dz
    );

    ~RectangularPrism();

    // xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
    void getMCBoundingBox(double* xyzLimits) const;
    void render();
    virtual void rotate( const cryph::AffPoint& point, const cryph::AffVector& vector, float degrees ){};
private:
    GLuint vao[1];
    GLuint vbo[1];
    GLuint ebo[3];

    float xmin, xmax, ymin, ymax, zmin, zmax;

    static GLuint indexList[3][4];

    void defineRectangularPrism();
    void renderRectangularPrism();
};

#endif
