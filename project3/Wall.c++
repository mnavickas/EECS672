// Wall.c++

#include "Wall.h"

#include <math.h>
#include "Materials.h"

GLuint Wall::indexList[4][7] = {
    { 4, 9, 10, 11 }, // left portion
    {7,18,19,20,21,22,23},
    {10,29,30,31,32,33,34 },
    {0,1,2,9}
};

Wall::Wall(ShaderIF* sIF, float dx, float dy, float dz)
    : MyView(sIF, PHONG_LOG2)
{
    this->dx = dx;
    this->dy = dy;
    this->dz = dz;
    xmin = -15;
    xmax = 70;


    ymax = -4;
    ymin = ymax-0.1;

    zmin = -50;
    zmax = 30;

    defineWall();
}

Wall::~Wall()
{
    glDeleteBuffers(1, vbo);
    glDeleteVertexArrays(1, vao);
}

void Wall::defineWall()
{
    const int yMod = -4;
    const int zGapMod = 10;
    vec3 vtx[37] =
    {
        {-15,50+yMod,-50}, //12
        {-15,0+yMod,-50}, //13

        {70,50+yMod,-50},  //1
        {70,50+yMod,30},   //2
        {70,11+yMod,-50},  //3
        {70,11+yMod,30},   //4
        {70,0+yMod,30},    //5
        {70,11+yMod,-2+zGapMod},    //6
        {70,0+yMod,-2+zGapMod},     //7
        {70,0+yMod,-50},   //8
        {70,11+yMod,-18+zGapMod},  //9
        {70,0+yMod,-18+zGapMod}    //10 //11

    };

    const int count = 24;
    const float openRadius = 10;
    const float dTheta = M_PI/count;


    float i = 0.0;
    for(int index = 13; index<count+13; index++, i+=dTheta)
    {
        vtx[index][0] = dx;
        vtx[index][1] = openRadius*sin(i) + dy -3;
        vtx[index][2] = openRadius*cos(i) + dz;
    }

    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);

    glGenBuffers(1, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 37*sizeof(vec3), vtx, GL_STATIC_DRAW);
    glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));

    glDisableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Wall::getMCBoundingBox(double* xyzLimits) const
{
    xyzLimits[0] = 69;
    xyzLimits[1] = 70;
    xyzLimits[2] = 0;
    xyzLimits[3] = 50;
    xyzLimits[4] = -50;
    xyzLimits[5] = 30;
}

bool Wall::handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY)
{
    // Wall does not look for events; just hand off to inherited handleCommand.
    return this->ModelView::handleCommand(anASCIIChar, ldsX, ldsY);
}

void Wall::renderWall()
{
    glBindVertexArray(vao[0]);

    glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), -1.0, 0.0, 0.0);
    glDrawArrays(GL_TRIANGLE_STRIP, 2, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 5, 4);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, indexList[0]);
    glDrawElements(GL_TRIANGLE_FAN, 7, GL_UNSIGNED_INT, indexList[1]);
    glDrawElements(GL_TRIANGLE_FAN, 7, GL_UNSIGNED_INT, indexList[2]);


    glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, 0.0, 1.0);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, indexList[3]);
}

void Wall::render()
{
    GLint pgm;
    glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
    glUseProgram(shaderIF->getShaderPgmID());


    establishView();
    establishMaterial();
    establishLightingEnvironment();

    cryph::Matrix4x4 mc_ec, ec_lds;
    getMatrices(mc_ec, ec_lds);
    float mat[16];
    glUniformMatrix4fv(shaderIF->ppuLoc("mc_ec"), 1, false, mc_ec.extractColMajor(mat));
    glUniformMatrix4fv(shaderIF->ppuLoc("ec_lds"), 1, false, ec_lds.extractColMajor(mat));


    renderWall();

    glUseProgram(pgm);
}
