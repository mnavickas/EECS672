// Cylinder.h

#ifndef CYLINDER_H
#define CYLINDER_H

#include "ObjTypes/MyView.h"

class Cylinder : public MyView
{
public:
    Cylinder
    (
        ShaderIF* sIF,
        const PhongMaterial& mat,
        double h = 1,
        double r = 1,
        double x = 0,
        double y = 0,
        double z = 0,
        double thetaX = 0,
        double thetaY = 0,
        double thetaZ = 0
    );

    ~Cylinder();

    // xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
    void getMCBoundingBox(double* xyzLimits) const;
    void render();
    virtual void rotate( const cryph::AffPoint& point, const cryph::AffVector& vector, float degrees ){}

private:
    double minX,maxX,minY,maxY,minZ,maxZ;
    GLuint vao[1];
    GLuint vbo[2]; // 0: coordinates; 1: normal vectors

    const cryph::AffPoint shift;

    cryph::AffVector topNormal;

    const double mScaleHeight;
    const double mScaleRadius;

    const double mRotateX;
    const double mRotateY;
    const double mRotateZ;
    static GLuint indices[2][20];

    void defineCylinder();
};

#endif
