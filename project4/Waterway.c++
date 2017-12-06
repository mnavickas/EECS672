// Waterway.c++

#include "Waterway.h"

#include "Cylinder.h"
#include "WaterBlock.h"
#include "Materials.h"

Waterway::Waterway(ShaderIF* sIF, float dx, float dy, float dz)
    : MyContainer(sIF,PHONG_NONE)
{

    addComponent(new WaterBlock(sIF,PHONG_WATER,12,2,99,dx,dy-4.9,dz));
    addComponent(new WaterBlock(sIF,PHONG_WATER,12,2,80,dx-5,dy-4.8,dz+70));
    addComponent(new WaterBlock(sIF,PHONG_WATER,12,10,3,dx-5,dy-8,dz+110));





    addComponent(new Cylinder(sIF,PHONG_LOG3,35,2,dx+6.5,dy-4,dz+14,90,0,0));
    addComponent(new Cylinder(sIF,PHONG_LOG3,29,2,dx+2,dy-4,dz+80,90,0,0));


    addComponent(new Cylinder(sIF,PHONG_LOG3,26,2,dx-6.5,dy-4,dz+5,90,0,0));
    addComponent(new Cylinder(sIF,PHONG_LOG3,39,2,dx-12,dy-4,dz+70,90,0,0));


    addComponent(new Cylinder(sIF,PHONG_LOG3,3.5,2,dx+4,dy-4,dz+50,0,0,90));


    addComponent(new Cylinder(sIF,PHONG_LOG3,3.5,2,dx-10,dy-4,dz+30,0,0,90));



    addComponent(new Pool(sIF));


}


Waterway::Pool::Pool(ShaderIF* sIF) : MyView(sIF, PHONG_WATER)
{
    definePool();
    setTextureImage("tex/water.jpg",0);
}

void Waterway::Pool::getMCBoundingBox(double* xyzLimits) const
{

}
bool Waterway::Pool::handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY)
{

}
void Waterway::Pool::render()
{
    initRender(VIEW | MATERIAL | LIGHTING | TEXTURE);

    renderPool();

    finishRender();
}

const int nPoints = 24;
void Waterway::Pool::definePool()
{

    const int nPointsAroundCircle = nPoints-2;

    vec3 coords[nPoints];
    vec2 texCoords[nPoints];

    double theta = 0.0;
    double dTheta = 2.0*M_PI/nPointsAroundCircle;
    double r = 25;


    float xShift = -70;
    float zShift = -43;


    coords[0][0]= xShift;
    coords[0][1]= -3.89;
    coords[0][2]= zShift;

    texCoords[0][0]= 0.5;
    texCoords[0][1]= 0.5;

    for (int i=1 ; i<nPoints; i++, theta += dTheta)
    {
        cryph::AffPoint p1, p2;
        float dx = r*cos(theta);
        float dz = r*sin(theta);

        texCoords[i][0]= cos(theta);
        texCoords[i][1]= sin(theta);

        coords[i][0]= dx+xShift;
        coords[i][1]= -3.89;
        coords[i][2]= dz+zShift;
    }


    glGenVertexArrays(1,vao);
    glGenBuffers(2,vbo);

    glBindVertexArray(vao[0]);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, (nPoints)*sizeof(vec3), coords, GL_STATIC_DRAW);
    glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, (nPoints)*sizeof(vec2), texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(shaderIF->pvaLoc("texCoords"), 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(shaderIF->pvaLoc("texCoords"));


//    delete[] coords;
}
void Waterway::Pool::renderPool()
{
    glBindVertexArray(vao[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"),0.0, 1.0, 0.0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, nPoints);
}
