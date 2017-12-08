#include "Proj4Controller.h"
#include "ModelView.h"
#include <stdlib.h>
#include <chrono>
#include <thread>

Proj4Controller::Proj4Controller(const std::string& windowTitle, int rcFlags)
    : GLFWController( windowTitle, rcFlags )
    , useStandardRendering(true)
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


void Proj4Controller::handleAsciiChar(unsigned char theChar, int x, int y)
{
    Controller::handleAsciiChar(theChar,x,y);

    if(theChar == 65) //'A'
    {
        useStandardRendering = false;
    }
    else if( theChar == 90) //'Z'
    {
        useStandardRendering = true;
    }


}
void Proj4Controller::run()
{

	if (theWindow == nullptr)
		return;

    auto lastExecuteTime = std::chrono::steady_clock::now();
    bool inited = false;

	while (!glfwWindowShouldClose(theWindow))
	{
        if( useStandardRendering )
        {
            glfwWaitEvents();
            handleDisplay();
        }
        else
        {
            glfwPollEvents();
            if( !inited )
            {
                handleDisplay();
                lastExecuteTime = std::chrono::steady_clock::now();
                inited = true;
            }
            else if( std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::steady_clock::now( ) - lastExecuteTime  ) >= std::chrono::milliseconds(1000/30) )
            {
                handleDisplay();
                lastExecuteTime = std::chrono::steady_clock::now();
            }
            else
            {
                //dont endlessly loop using CPU power
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }
	}
	glfwDestroyWindow(theWindow);
	theWindow = nullptr;

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
