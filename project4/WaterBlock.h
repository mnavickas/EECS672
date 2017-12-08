// WaterBlock.h

#ifndef WaterBlock_H
#define WaterBlock_H

#include "ObjTypes/MyView.h"
#include "PhongMaterial.h"

class WaterBlock : public MyView
{
public:
    WaterBlock
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

    ~WaterBlock();

    // xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
    void getMCBoundingBox(double* xyzLimits) const;
    void render();
    virtual void rotate( const cryph::AffPoint& point, const cryph::AffVector& vector, float degrees ){};

    static float texOffset;
private:
    GLuint vao[1];
    GLuint vbo[2];
    GLuint ebo[3];

    float xmin, xmax, ymin, ymax, zmin, zmax;

    static GLuint indexList[3][4];

    void defineWaterBlock();
    void renderWaterBlock();
};

#endif
