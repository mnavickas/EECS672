// WindowedWall.c++

#include "WindowedWall.h"

#include <math.h>
#include "Materials.h"

GLuint WindowedWall::indexList[5][4] = {
    { 0, 1, 6, 11 }, // left portion
    {7,10,2,5},
    {8,9,6,7},
    {2,12,14,5},
    {13,3,4,15}
};

WindowedWall::WindowedWall(ShaderIF* sIF, float dx, float dy, float dz)
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

    defineWindowedWall();
    setTextureImage("tex/log_cabin.jpg",0);
}

WindowedWall::~WindowedWall()
{
    glDeleteBuffers(1, vbo);
    glDeleteVertexArrays(1, vao);
}

void WindowedWall::defineWindowedWall()
{
    const int yMod = -4;
    const int zGapMod = 10;
    vec3 vtx[] =
    {
        {-15,50+yMod,-50},
        {-15,0+yMod,-50},

        {-15,50+yMod,30},
        {70,50+yMod,30},

        {70,0+yMod,30},

        {-15,0+yMod,30},
        {-15,50+yMod,-20},
        {-15,50+yMod,0},

        {-15,35+yMod,-20},
        {-15,35+yMod,0},
        {-15,0+yMod,0},
        {-15,0+yMod,-20},

        {13.333,50+yMod,30},
        {41.666,50+yMod,30},
        {13.333,0+yMod,30},
        {41.666,0+yMod,30},


    };


    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);

    glGenBuffers(2, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 16*sizeof(vec3), vtx, GL_STATIC_DRAW);
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
        {0.5+.125,1},
        {0.5-.125,1},

        {0.5+.125,35.0/50},
        {0.5-.125,35.0/50},



        {0.5+.125,0},
        {0.5-.125,0},
        {.666,1},
        {.333,1},
        {.666,0},
        {.333,0},

    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, 16*sizeof(vec2), texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(shaderIF->pvaLoc("texCoords"), 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(shaderIF->pvaLoc("texCoords"));

    glDisableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void WindowedWall::getMCBoundingBox(double* xyzLimits) const
{
    xyzLimits[0] = 69;
    xyzLimits[1] = 70;
    xyzLimits[2] = 0;
    xyzLimits[3] = 50;
    xyzLimits[4] = -50;
    xyzLimits[5] = 30;
}

bool WindowedWall::handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY)
{
    // WindowedWall does not look for events; just hand off to inherited handleCommand.
    return this->ModelView::handleCommand(anASCIIChar, ldsX, ldsY);
}

void WindowedWall::renderWindowedWall()
{
    glBindVertexArray(vao[0]);

    glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, 0.0, -1.0);
    glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, indexList[3]);
    glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, indexList[4]);

    glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 1.0, 0.0, 0.0);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, indexList[2]);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, indexList[1]);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, indexList[0]);
}

void WindowedWall::render()
{
    initRender(VIEW | MATERIAL | LIGHTING | TEXTURE);

    renderWindowedWall();

    finishRender();
}
