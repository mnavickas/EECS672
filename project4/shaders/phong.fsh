#version 410 core

// phong.fsh - a fragment shader that implements a Phong Lighting model.


in PVA
{
	vec3 ecPosition;
	vec3 ecUnitNormal;
	vec2 texCoords;
} pvaIn;

out vec4 fragmentColor;

uniform sampler2D textureMap;
uniform int useTexture = 0;

const int MAX_NUM_LIGHTS = 8;

// material
uniform vec3 kd = vec3(0.8, 0.0, 0.0); //default
uniform vec3 ka = vec3(0.5, 0.5, 0.5); //default
uniform vec3 ks = vec3(0.5, 0.5, 0.5);
uniform float specular = 1.0;
uniform float alpha = 1.0;


uniform int glass_portion = 0;

//lighting
uniform int numLights = 1;
uniform vec3 strengthLights[MAX_NUM_LIGHTS];
uniform vec3 ambientStrength;
uniform vec4 p_ecLightSourcePos[MAX_NUM_LIGHTS];

//Translucency

uniform int sceneHasTranslucentObjects = 0;
uniform int drawingOpaqueObjects = 1;

in vec3 mcPositionOut;

uniform mat4 ec_lds =
	mat4(1.0, 0.0, 0.0, 0.0,
	     0.0, 1.0, 0.0, 0.0,
	     0.0, 0.0, -1.0, 0.0,
	     0.0, 0.0, 0.0, 1.0);


float attenuate(vec3 source, vec3 pos, int which_source)
{
	int mod = 0;
	//attenuate the fire a lot more. and the night-light
	if(which_source >= 2)
		mod = 5;

	return 1/ pow( distance(source,pos)/30, 2+mod );
}

vec4 evaluateLightingModel()
{
	vec3 ambientPart = ka * ambientStrength;
	vec3 diffusePart = vec3(0,0,0);
	vec3 specularPart = vec3(0,0,0);


	vec3 vHat;

	if(ec_lds[3][3] != 1.0)
	{
		vHat = -normalize(pvaIn.ecPosition);
	}
	else
	{
		vHat = normalize( vec3((-ec_lds[2][0])/ec_lds[0][0], (-ec_lds[2][1])/ec_lds[1][1], 1.0) );
	}

	vec3 normal = pvaIn.ecUnitNormal;
	if(dot(normal, vHat) < 0)
		normal = -normal;

	for(int i = 0;i <numLights; i++)
	{
		vec3 liHat;
		vec4 light_position = p_ecLightSourcePos[i];
		float attenuation = 1;

		if( i == 1 )
		{
			//clamp indoor light to indoors only.
			if( mcPositionOut.x < -15 || mcPositionOut.x > 70 || mcPositionOut.z < -50 || mcPositionOut.z > 30)
			{
				continue;
			}
		}

		if( light_position.w == 0)
		{
			liHat = normalize(light_position.xyz);
		}
		else
		{
			liHat = normalize( light_position.xyz - pvaIn.ecPosition );
			attenuation = clamp( attenuate(light_position.xyz,pvaIn.ecPosition,i)*50, 0.01, 1.0 );
		}

		if( dot(liHat, normal) > 0 || glass_portion == 1)
		{
			if( glass_portion == 0)
			{
				diffusePart += (attenuation * kd * strengthLights[i] * dot(liHat,normal));
			}
			else
			{
				diffusePart += (attenuation * kd * strengthLights[i] * max( dot(liHat,normal), dot(liHat,-normal) ));
			}


		}


		vec3 rHat = normalize(reflect(-liHat,normal));

		if( dot(rHat, vHat) > 0 && dot(liHat, normal) > 0 )
		{
			specularPart+= (attenuation * ks * strengthLights[i] * pow(dot(rHat,vHat),specular));
		}

	}

	float totalR = ambientPart.r + diffusePart.r + specularPart.r;
	float totalG = ambientPart.g + diffusePart.g + specularPart.g;
	float totalB = ambientPart.b + diffusePart.b + specularPart.b;

	return vec4( clamp(totalR,0.0,1.0), clamp(totalG,0.0,1.0), clamp(totalB,0.0,1.0), alpha);
}

vec4 composeColor(vec4 lmColor, vec4 tColor)
{

	return (tColor+2*lmColor)/3;

}


void main ()
{
	vec4 color = evaluateLightingModel();

	if( useTexture == 1 )
	{
		vec4 textureColor = texture(textureMap,pvaIn.texCoords);
		color = composeColor(color,textureColor);
	}

	if (sceneHasTranslucentObjects == 1)
	{
		if (drawingOpaqueObjects == 1)
		{
			if (color.a < 1.0)
			{
				discard;
			}
			else
			{
				fragmentColor = color;
			}
		}
		else if (color.a < 1)
		{
			fragmentColor = color;
		}
		else
		{
			discard;
		}
	}
	else
	{
		fragmentColor = color;
	}
}
