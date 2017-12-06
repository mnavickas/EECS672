// Waterway.h

#ifndef Waterway_H
#define Waterway_H

#include <GL/gl.h>

#include "ObjTypes/MyContainer.h"
#include "ObjTypes/MyView.h"

class Waterway : public MyContainer
{
public:
    Waterway
    (
        ShaderIF* shaderIF,
        float dx = 0,
        float dy = 0,
        float dz = 0
    );
private:
    class Pool : public MyView
    {
    public:
        Pool
        (
            ShaderIF* sIF
        );

        // xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
        void getMCBoundingBox(double* xyzLimits) const;
        bool handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY);
        void render();
        virtual void rotate( const cryph::AffPoint& point, const cryph::AffVector& vector, float degrees ) {}

    private:
        GLuint vao[1];
        GLuint vbo[2];

        void definePool();
        void renderPool();
    };
};

#endif
