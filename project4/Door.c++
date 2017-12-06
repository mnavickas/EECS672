// Door.c++

#include "Door.h"
#include "Materials.h"

// index lists for the three faces that can't be drawn with glDrawArrays
GLuint Door::indexList[3][4] = {
    { 6, 7, 0, 1 }, // xmin face
    { 6, 0, 4, 2 }, // ymin face
    { 1, 7, 3, 5 }  // ymax face
};

Door::Door(ShaderIF* sIF)
    : MyView(sIF, PHONG_LOG2)
{
    xmin = -15;
    xmax = -14.9;

    ymin = -4;
    ymax = 31;

    zmin = -20;
    zmax = 0;

    defineDoor();
    setTextureImage("tex/door.png",0);
}

Door::~Door()
{
    glDeleteBuffers(3, ebo);
    glDeleteBuffers(2, vbo);
    glDeleteVertexArrays(1, vao);
}

void Door::defineDoor()
{
    vec3 vtx[] = { // The 8 unique vertices (Note the order)
    {xmin ,ymin, zmax}, {xmin, ymax, zmax},
    {xmax, ymin, zmax}, {xmax, ymax, zmax},
    {xmax, ymin, zmin}, {xmax, ymax, zmin},
    {xmin, ymin, zmin}, {xmin, ymax, zmin}
    };

    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);

    glGenBuffers(2, vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 8*sizeof(vec3), vtx, GL_STATIC_DRAW);
    glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));

    glGenBuffers(3, ebo);
    for (int i=0 ; i<3 ; i++)
    {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4*sizeof(GLuint), indexList[i], GL_STATIC_DRAW);
    }


    vec2 texCoords[]=
    {
        {0,0},{0,1},
        {0,0},{0,1},

        {.95,0},{.95,1},
        {.95,0},{.95,1}
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, 8*sizeof(vec2), texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(shaderIF->pvaLoc("texCoords"), 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(shaderIF->pvaLoc("texCoords"));


    glDisableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Door::getMCBoundingBox(double* xyzLimits) const
{
    xyzLimits[0] = xmin;
    xyzLimits[1] = xmax;
    xyzLimits[2] = ymin;
    xyzLimits[3] = ymax;
    xyzLimits[4] = zmin;
    xyzLimits[5] = zmax;
}

void Door::renderDoor()
{
    glBindVertexArray(vao[0]);

    // The three faces that can be drawn with glDrawArrays
    glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, 0.0, 1.0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 1.0, 0.0, 0.0);
    glDrawArrays(GL_TRIANGLE_STRIP, 2, 4);
    glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, 0.0, -1.0);
    glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

    // The three faces that are drawn with glDrawElements
    glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), -1.0, 0.0, 0.0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
    glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, -1.0, 0.0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
    glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, 1.0, 0.0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[2]);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
}

void Door::render()
{
    initRender(VIEW | MATERIAL | LIGHTING | TEXTURE);

    renderDoor();

    finishRender();
}

bool Door::handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY)
{
    // Door does not look for events; just hand off to inherited handleCommand.
    return this->ModelView::handleCommand(anASCIIChar, ldsX, ldsY);
}
