// Floor.c++

#include "Floor.h"
#include "Materials.h"

// index lists for the three faces that can't be drawn with glDrawArrays
GLuint Floor::indexList[3][4] = {
    { 6, 7, 0, 1 }, // xmin face
    { 6, 0, 4, 2 }, // ymin face
    { 1, 7, 3, 5 }  // ymax face
};

Floor::Floor(ShaderIF* sIF,
        const PhongMaterial& mat,
        float minx,
        float maxx,
        float miny,
        float maxy,
        float minz,
        float maxz,
        bool addToBB)
    : MyView(sIF,mat)
{
    xmin = minx;
    xmax = maxx;

    addToBb = addToBB;

    ymax = miny;
    ymin = maxy;

    zmin = minz;
    zmax = maxz;

    defineFloor();
}

Floor::~Floor()
{
    glDeleteBuffers(1, vbo);
    glDeleteVertexArrays(1, vao);
}

void Floor::defineFloor()
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

    glDisableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Floor::getMCBoundingBox(double* xyzLimits) const
{
    if( addToBb)
    {
        xyzLimits[0] = xmin;
        xyzLimits[1] = xmax;
        xyzLimits[2] = ymin;
        xyzLimits[3] = ymax;
        xyzLimits[4] = zmin;
        xyzLimits[5] = zmax;
    }
    else
    {
        xyzLimits[0] = 1;
        xyzLimits[1] = -1;
    }
}

bool Floor::handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY)
{
    // Floor does not look for events; just hand off to inherited handleCommand.
    return this->ModelView::handleCommand(anASCIIChar, ldsX, ldsY);
}

void Floor::renderFloor()
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

void Floor::render()
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
    renderFloor();

    glUseProgram(pgm);
}
