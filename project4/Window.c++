// Window.c++

#include "Window.h"
#include "Materials.h"

// index lists for the three faces that can't be drawn with glDrawArrays
GLuint Window::indexList[3][4] = {
    { 6, 7, 0, 1 }, // xmin face
    { 6, 0, 4, 2 }, // ymin face
    { 1, 7, 3, 5 }  // ymax face
};

Window::Window(ShaderIF* sIF)
    : MyView(sIF, PHONG_GLASS)
{
    xmin = 13.333;
    xmax = 41.666;

    ymin = -4;
    ymax = 46;

    zmin = 29.5;
    zmax = 30.5;

    defineWindow();
    //setTextureImage("tex/Window.png",0);
}

Window::~Window()
{
    glDeleteBuffers(3, ebo);
    glDeleteBuffers(1, vbo);
    glDeleteVertexArrays(1, vao);
}

void Window::defineWindow()
{
    vec3 vtx[] = { // The 8 unique vertices (Note the order)
    {xmin ,ymin, zmax}, {xmin, ymax, zmax},
    {xmax, ymin, zmax}, {xmax, ymax, zmax},
    {xmax, ymin, zmin}, {xmax, ymax, zmin},
    {xmin, ymin, zmin}, {xmin, ymax, zmin}
    };
    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);

    glGenBuffers(1, vbo);
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
void Window::getMCBoundingBox(double* xyzLimits) const
{
    xyzLimits[0] = xmin;
    xyzLimits[1] = xmax;
    xyzLimits[2] = ymin;
    xyzLimits[3] = ymax;
    xyzLimits[4] = zmin;
    xyzLimits[5] = zmax;
}

void Window::renderWindow()
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

void Window::render()
{
    initRender(VIEW | MATERIAL | LIGHTING );

    glUniform1i(shaderIF->ppuLoc("glass_portion"), 1);
    renderWindow();
    glUniform1i(shaderIF->ppuLoc("glass_portion"), 0);

    finishRender();
}

bool Window::handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY)
{
    // Window does not look for events; just hand off to inherited handleCommand.
    return this->ModelView::handleCommand(anASCIIChar, ldsX, ldsY);
}
