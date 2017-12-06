// Cylinder.h

#ifndef MyContainer_H
#define MyContainer_H

#include "../SceneElement.h"
#include "MyView.h"
#include <GL/gl.h>
#include "ShaderIF.h"

#include <vector>
#include <string.h>
#include "AffVector.h"
#include "AffPoint.h"


//extend some basic functionallity across modelview collections
class MyContainer : public SceneElement
{

protected:
	typedef float vec3[3];
    std::vector<MyView*> componentVector;
    double overallMCBoundingBox[6];
	ShaderIF* shaderIF;

    //shamelessly taken from controller.c++
    void addComponent(MyView* m)
    {
        componentVector.push_back(m);

        if (m == nullptr)
        {
            return;
        }
        if (overallMCBoundingBox[0] <= overallMCBoundingBox[1])
        {
            // bounding box already initialized; just update it:
            double xyz[6];
            m->getMCBoundingBox(xyz);
            if (xyz[0] > xyz[1])
                // This model does not want to be included in BBs
                return;
            for (int i=0 ; i<5 ; i+=2)
            {
                if (xyz[i] < overallMCBoundingBox[i])
                    overallMCBoundingBox[i] = xyz[i];
                if (xyz[i+1] > overallMCBoundingBox[i+1])
                    overallMCBoundingBox[i+1] = xyz[i+1];
            }
        }
        else
        {
            m->getMCBoundingBox(overallMCBoundingBox);
        }
    }

public:
    void render()
    {
        for( auto it = componentVector.begin(); it != componentVector.end(); ++it)
        {
            (*it)->render();
        }
    }

	void getMCBoundingBox(double* xyzLimits) const
    {
        memcpy(xyzLimits, overallMCBoundingBox,6*sizeof(double));
    }
    MyContainer
		(
			ShaderIF* sIF,
			const PhongMaterial& material
		)
		: SceneElement(sIF, material)
    {
        overallMCBoundingBox[0] = overallMCBoundingBox[2] = overallMCBoundingBox[4] = 1.0;
        overallMCBoundingBox[1] = overallMCBoundingBox[3] = overallMCBoundingBox[5] = -1.0;
    }


    ~MyContainer()
    {
        for( auto it = componentVector.begin(); it != componentVector.end(); ++it) {
            delete *it;
        }
    }

	void rotate( const cryph::AffPoint& point, const cryph::AffVector& vector, float degrees )
	{
		for( auto it = componentVector.begin(); it != componentVector.end(); ++it) {
			(*it)->rotate(point,vector,degrees);
		}
	}
};

#endif
