#include "Proj4Controller.h"
#include "ModelView.h"
#include <stdlib.h>

Proj4Controller::Proj4Controller(const std::string& windowTitle, int rcFlags)
    : GLFWController( windowTitle, rcFlags )
{

}


void Proj4Controller::handleDisplay()
{
	if (theWindow == nullptr)
		return;
	glfwMakeContextCurrent(theWindow);
	int width, height;
	glfwGetFramebufferSize(theWindow, &width, &height);
	glViewport(0, 0, width, height);

	// clear the frame buffer
	glClear(glClearFlags);

    //begin Translucency implementation
    {
    glUseProgram(theShader->getShaderPgmID());

    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUniform1i(theShader->ppuLoc("sceneHasTranslucentObjects"), 1);

    // draw opaque objects
    glDisable(GL_BLEND);
    glUniform1i(theShader->ppuLoc("drawingOpaqueObjects"), 1);
    drawAllObjects();

    // draw translucent objects
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glUniform1i(theShader->ppuLoc("drawingOpaqueObjects"), 0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    drawAllObjects();
    }
    //end Translucency implementation

	glfwSwapBuffers(theWindow);

	checkForErrors(std::cout, "Proj4Controller::handleDisplay");
}

void Proj4Controller::drawAllObjects()
{
    for (std::vector<ModelView*>::iterator it=models.begin() ; it<models.end() ; it++)
    {
        (*it)->render();
    }
}

void Proj4Controller::recordShaderIF(ShaderIF* shader)
{
    theShader = shader;
}
