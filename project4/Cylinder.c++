// Cylinder.c++
#include "Cylinder.h"

#include "Matrix3x3.h"
#include "Materials.h"

GLuint Cylinder::indices[2][20] ={
    {1,3,5,7,9,11,13,15,17,19,21,23,25,27,29,31,33,35,37,3},
    {0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,2}
};

Cylinder::Cylinder(ShaderIF* sIF, const PhongMaterial& mat, double h, double r, double x, double y, double z, double thetaX, double thetaY,double thetaZ)
    : MyView(sIF, mat)
    , mScaleHeight(h)
    , mScaleRadius(r)
    , shift(x,y,z)
    , mRotateX(thetaX)
    , mRotateY(thetaY)
    , mRotateZ(thetaZ)
{
    int maxMod = mScaleRadius;
    if( mScaleHeight > mScaleRadius) maxMod = mScaleHeight;


    minX = shift.x-maxMod;
    maxX = shift.x+maxMod;
    minY = shift.y-maxMod;
    maxY = shift.y+maxMod;
    minZ = shift.z-maxMod;
    maxZ = shift.z+maxMod;

    topNormal = cryph::AffVector::yu;
    defineCylinder();
}

Cylinder::~Cylinder()
{
    glDeleteBuffers(2, vbo);
    glDeleteVertexArrays(1, vao);
}


const int N_POINTS_AROUND_SLICE = 18; // number points around a cross-section

void Cylinder::defineCylinder()
{
    typedef float vec3[3];
    int nPoints = 2 * (N_POINTS_AROUND_SLICE + 1) + 2;
    vec3* coords = new vec3[nPoints];
    vec3* normals = new vec3[nPoints];
    double theta = 0.0;
    double dTheta = 2.0*M_PI/N_POINTS_AROUND_SLICE;


    const double r = 1 * mScaleRadius;
    const double yTop = 1 * mScaleHeight;
    const double yBot = -1 * mScaleHeight;

    topNormal = cryph::Matrix3x3::zRotationDegrees(mRotateZ) * cryph::Matrix3x3::yRotationDegrees(mRotateY) * cryph::Matrix3x3::xRotationDegrees(mRotateX) * topNormal;

    // EXERCISE: Use cryph to generalize this to define a cylinder in
    //           an arbitrary position and orientation.
    for (int i=1 ; i<=N_POINTS_AROUND_SLICE+1; i++, theta += dTheta)
    {
        const int j = 2 * i;
        #define TOP j+1
        #define BOTTOM j

        cryph::AffPoint p1, p2;
        cryph::AffVector v1, v2;
        double dx = cos(theta);
        double dz = sin(theta);

        //Scale
        p1.assign(r*dx, yBot,r*dz);
        //rotate around origin
        p1=  cryph::Matrix3x3::zRotationDegrees(mRotateZ) * cryph::Matrix3x3::yRotationDegrees(mRotateY) * cryph::Matrix3x3::xRotationDegrees(mRotateX) * p1;

        p2.assign(r*dx, yTop, r*dz);
        p2=  cryph::Matrix3x3::zRotationDegrees(mRotateZ) * cryph::Matrix3x3::yRotationDegrees(mRotateY) * cryph::Matrix3x3::xRotationDegrees(mRotateX) * p2;
        v1.assign(dx, 0.0, dz);
        v1=  cryph::Matrix3x3::zRotationDegrees(mRotateZ) * cryph::Matrix3x3::yRotationDegrees(mRotateY) * cryph::Matrix3x3::xRotationDegrees(mRotateX) * v1;
        v2.assign(dx, 0.0, dz);
        v2=  cryph::Matrix3x3::zRotationDegrees(mRotateZ) * cryph::Matrix3x3::yRotationDegrees(mRotateY) * cryph::Matrix3x3::xRotationDegrees(mRotateX) * v2;


        //translate
        p1+=shift;
        p2+=shift;


        //copy it into arrays
        normals[BOTTOM][0] = v1.dx;
        normals[BOTTOM][1] = v1.dy;
        normals[BOTTOM][2] = v1.dz;
        normals[TOP][0] = v2.dx;
        normals[TOP][1] = v2.dy;
        normals[TOP][2] = v2.dz;

        coords[BOTTOM][0] = p1.x;
        coords[BOTTOM][1] = p1.y;
        coords[BOTTOM][2] = p1.z;
        coords[TOP][0] = p2.x;
        coords[TOP][1] = p2.y;
        coords[TOP][2] = p2.z;
    }

    // center point of top and bottom
    coords[0][0] = (coords[2][0] + coords[6][0])/2;
    coords[0][1] = (coords[2][1] + coords[6][1])/2;
    coords[0][2] = (coords[2][2] + coords[6][2])/2;

    coords[1][0] = (coords[3][0] + coords[7][0])/2;
    coords[1][1] = (coords[3][1] + coords[7][1])/2;
    coords[1][2] = (coords[3][2] + coords[7][2])/2;

    // ************************************************************************
    // EXERCISE: Create/fill VAOs and VBOs here.
    //           Also use glVertexAttribPointer and glEnableVertexAttribArray
    // ************************************************************************
    glGenVertexArrays(1,vao);
    glGenBuffers(2,vbo);

    glBindVertexArray(vao[0]);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, (nPoints)*sizeof(vec3), coords, GL_STATIC_DRAW);
    glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, nPoints*sizeof(vec3), normals, GL_STATIC_DRAW);
    glVertexAttribPointer(shaderIF->pvaLoc("mcNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));

}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Cylinder::getMCBoundingBox(double* xyzLimits) const
{
    xyzLimits[0] = minX;
    xyzLimits[1] = maxX;
    xyzLimits[2] = minY;
    xyzLimits[3] = maxY;
    xyzLimits[4] = minZ;
    xyzLimits[5] = maxZ;
}

void Cylinder::render()
{
    initRender(VIEW | MATERIAL | LIGHTING );


    glBindVertexArray(vao[0]);
    glDrawArrays(GL_TRIANGLE_STRIP, 2, 2*(N_POINTS_AROUND_SLICE+1));

    //handle cylinder caps
    glDisableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));

    glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), topNormal.dx, topNormal.dy, topNormal.dz);
    glDrawElements(GL_TRIANGLE_FAN, 20, GL_UNSIGNED_INT, &indices[0]);

    glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), -topNormal.dx, -topNormal.dy, -topNormal.dz);
    glDrawElements(GL_TRIANGLE_FAN, 20, GL_UNSIGNED_INT, &indices[1]);

    glEnableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));


    finishRender();
}
