// Wall.c++

#include "Wall.h"

#include <math.h>
#include "Materials.h"

GLuint Wall::indexList[2][4] = {
    { 0, 1, 2, 5 }, // left portion
    {3,2,5,4}
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
    setTextureImage("tex/log_cabin.jpg",0);
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
    vec3 vtx[6] =
    {
        {-15,50+yMod,-50},
        {-15,0+yMod,-50},

        {70,50+yMod,-50},
        {70,50+yMod,30},

        {70,0+yMod,30},

        {70,0+yMod,-50},   //8

    };


    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);

    glGenBuffers(2, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 6*sizeof(vec3), vtx, GL_STATIC_DRAW);
    glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));

    vec2 texCoords[]=
    {
        {0,1},

        {0,0},
        {1,1},

        {0,1},

        {0,0},

        {1,0},

    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, 6*sizeof(vec2), texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(shaderIF->pvaLoc("texCoords"), 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(shaderIF->pvaLoc("texCoords"));

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
    glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, indexList[1]);


    glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, 0.0, 1.0);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, indexList[0]);
}

void Wall::render()
{
    initRender(VIEW | MATERIAL | LIGHTING | TEXTURE);


    renderWall();

    finishRender();
}
