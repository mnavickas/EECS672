#include "GLFWController.h"
#include "ModelView.h"
#include "ShaderIF.h"

#ifndef MY_CONTROLLER
#define MY_CONTROLLER

class Proj4Controller : public GLFWController
{
public:
    Proj4Controller(const std::string& windowTitle, int rcFlags = 0);
    void recordShaderIF(ShaderIF* shader);
    void run();
    bool useStandardRendering;
    virtual void handleAsciiChar(unsigned char theChar, int x, int y);

protected:
    void handleDisplay();


private:
    void drawAllObjects();
    ShaderIF* theShader;
};

#endif
