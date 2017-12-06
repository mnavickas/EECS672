// SceneElement.c++

#include "SceneElement.h"
#include "ImageReader.h"
#include "Campfire.h"

bool SceneElement::toggleLighting = false;


float SceneElement::lightPos[4*MAX_NUM_LIGHTS] =
	{
		0,1, 1, 0.0,

		27, 40, -10, 1.0,

		Campfire::x+3, Campfire::y+8, Campfire::z+3, 1.0,
		Campfire::x-3, Campfire::y+8, Campfire::z+3, 1.0,
		Campfire::x+3, Campfire::y+8, Campfire::z-3, 1.0,
		Campfire::x-3, Campfire::y+8, Campfire::z-3, 1.0,

		70-4,10,0, 1.0,
		69.5-4,10,-30, 1.0
	};
// Are coordinates in "lightPos" stored in MC or EC?
bool SceneElement::posInModelCoordinates[MAX_NUM_LIGHTS] =
	{ true, true,  true,true,true,true,	true,true };
// The following is the buffer actually sent to GLSL. It will contain a copy of
// the (x,y,z,w) for light sources 	defined in EC; it will contain the coordinates
// after transformation to EC if the position was originally specified in MC.
float posToGLSL[4*MAX_NUM_LIGHTS];

float SceneElement::lightStrength[3*MAX_NUM_LIGHTS] =
	{
		0.4, 0.4, 0.4, //external Z-direction ligting
		0.2, 0.2, 0.2, //ceiling ligting in cabin

		0.5, 0.3, 0.0, //campfires
		0.5, 0.3, 0.0,
		0.5, 0.3, 0.0,
		0.5, 0.3, 0.0,

		0.4, 0.2, 0.0, //fireplace
		0.4, 0.2, 0.0
	};

float SceneElement::globalAmbient[] = { 0.2, 0.2, 0.2 };

SceneElement::SceneElement(ShaderIF* sIF, const PhongMaterial& matlIn) :
	shaderIF(sIF), matl(matlIn), texID(0), colorGenerationMode(-1),
	textureSource(-1)
{
}

SceneElement::~SceneElement()
{
}

void SceneElement::establishLightingEnvironment()
{
	// This should set:
	// "actualNumLights", "ecLightPosition", "lightStrength", "globalAmbient"

	int lights_count = MAX_NUM_LIGHTS;
	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec,ec_lds);

	float lights_in_ec[4*lights_count];
	if( toggleLighting )
	{
		lightStrength[3] = lightStrength[4] = lightStrength[5] = 0.9;
	}
	else
	{
		lightStrength[3] = lightStrength[4] = lightStrength[5] = 0.01;
	}


	for(int i = 0; i < lights_count; i++)
	{
		if( posInModelCoordinates[i] )
		{
			if( lightPos[4*i+3] == 1 )
			{
				cryph::AffPoint point( lightPos[ (4*i) ], lightPos[ (4*i)+1 ],  lightPos[ (4*i)+2 ] );
				point = mc_ec*point;
				lights_in_ec[(4*i)+0] = point.x;
				lights_in_ec[(4*i)+1] = point.y;
				lights_in_ec[(4*i)+2] = point.z;
				lights_in_ec[(4*i)+3] = lightPos[(4*i)+3];
			}
			else
			{
				cryph::AffVector vec( lightPos[ (4*i) ], lightPos[ (4*i)+1 ],  lightPos[ (4*i)+2 ] );
				vec = mc_ec*vec;
				lights_in_ec[(4*i)+0] = vec.dx;
				lights_in_ec[(4*i)+1] = vec.dy;
				lights_in_ec[(4*i)+2] = vec.dz;
				lights_in_ec[(4*i)+3] = lightPos[(4*i)+3];
			}

		}
		else
		{
			lights_in_ec[(4*i)+0] = lightPos[(4*i)+0];
			lights_in_ec[(4*i)+1] = lightPos[(4*i)+1];
			lights_in_ec[(4*i)+2] = lightPos[(4*i)+2];
			lights_in_ec[(4*i)+3] = lightPos[(4*i)+3];
		}
	}

	glUniform1i(shaderIF->ppuLoc("numLights"), lights_count);
	glUniform3fv(shaderIF->ppuLoc("strengthLights"), lights_count, lightStrength);
	glUniform3fv(shaderIF->ppuLoc("ambientStrength"), 1, globalAmbient);
	glUniform4fv(shaderIF->ppuLoc("p_ecLightSourcePos"), lights_count, lights_in_ec);
}

void SceneElement::establishMaterial()
{
	glUniform3fv(shaderIF->ppuLoc("kd"), 1, matl.kd);
	glUniform3fv(shaderIF->ppuLoc("ka"), 1, matl.ka);
	glUniform3fv(shaderIF->ppuLoc("ks"), 1, matl.ks);
	glUniform1f(shaderIF->ppuLoc("alpha"), matl.alpha);
	glUniform1f(shaderIF->ppuLoc("specular"), matl.shininess);
	glUniform1i(shaderIF->ppuLoc("useTexture"), 0);
}


void SceneElement::establishTexture()
{
	// Unless you are texture-mapping onto faces of BasicShape instances,
	// this method should be called from your render method, and it should
	// set texture-related parameters like:
	// "colorGenerationMode", "textureSource", "textureMap"
	// It should also do the appropriate call to glBindTexture.
	// (If you are texture-mapping onto faces of BasicShape instances,
	// you use the "prepareForFace" callback which may or may not be
	// implemented by calling this method.)


	int activeTexture = 0; // We will study this under "advanced texture mapping"
	glActiveTexture(GL_TEXTURE0 + activeTexture);

	// Set the textureMap uniform variable in the fragment shader so
	// that it will use the current "texture unit" we just specified above.
	glUniform1i(shaderIF->ppuLoc("textureMap"), activeTexture);


	// glBindTexture reestablishes the current texture along with its settings.
	glBindTexture(GL_TEXTURE_2D, texID);

	// If any texture parameters need to be set (or if ones set when the texture
	// was defined have changed), we set them here with their current values.
	// For example:
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Depending on how you write your fragment shader, you will probably need to
	// set one or more uniforms here to tell your fragment shader things like
	// (i) whether there is currently a texture map to use, (ii) how you want to
	// use the color from the texture map, (iii) etc.

	glUniform1i(shaderIF->ppuLoc("useTexture"), 1);
	// So set them...
}

void SceneElement::establishView()
{
	// Line of sight, dynamic view controls, 3D-2D projection, & mapping to LDS:
	cryph::Matrix4x4 mc_ec, ec_lds;
	ModelView::getMatrices(mc_ec, ec_lds);
	float m[16];
	glUniformMatrix4fv(shaderIF->ppuLoc("mc_ec"), 1, false, mc_ec.extractColMajor(m));
	glUniformMatrix4fv(shaderIF->ppuLoc("ec_lds"), 1, false, ec_lds.extractColMajor(m));
}

bool SceneElement::handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY)
{
	if (anASCIIChar == 'O')
		ModelView::setProjection(ORTHOGONAL);
	else if (anASCIIChar == 'P')
		ModelView::setProjection(PERSPECTIVE);
	else if (anASCIIChar == 'Q')
		ModelView::setProjection(OBLIQUE);
	else if (anASCIIChar == 'L')
		toggleLighting = true;
	else if( anASCIIChar == 'D')
		toggleLighting = false;
	else
		return ModelView::handleCommand(anASCIIChar, ldsX, ldsY);


	Controller::getCurrentController()->redraw();
	return true;
}

void SceneElement::setColorGenerationMode(int mode, int onFace)
{
	colorGenerationMode = mode;
}

void SceneElement::setTextureImage(const std::string& imgFileName, int onFace)
{
	ImageReader* ir = ImageReader::create(imgFileName.c_str());
	if (ir == nullptr)
	{
		std::cerr << "Could not open '" << imgFileName << "' for texture map.\n";
		return;
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	float white[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, white);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); //GL_CLAMP_TO_BORDER);
	GLint level = 0;
	int pw = ir->getWidth(), ph = ir->getHeight();
	GLint iFormat = ir->getInternalFormat();
	GLenum format = ir->getFormat();
	GLenum type = ir->getType();
	const GLint border = 0; // must be zero (only present for backwards compatibility)
	const void* pixelData = ir->getTexture();
	glTexImage2D(GL_TEXTURE_2D, level, iFormat, pw, ph, border, format, type, pixelData);
	delete ir;
}

void SceneElement::setTextureSource(int source, int onFace)
{
	textureSource = source;
}
