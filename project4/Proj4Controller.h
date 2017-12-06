#include "GLFWController.h"
#include "ModelView.h"
#include "ShaderIF.h"


class Proj4Controller : public GLFWController
{
public:
    Proj4Controller(const std::string& windowTitle, int rcFlags = 0);
    void recordShaderIF(ShaderIF* shader);

protected:
    void handleDisplay();

private:
    void drawAllObjects();
    ShaderIF* theShader;
};
