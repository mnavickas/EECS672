#version 410 core

// phong.fsh - a fragment shader that implements a Phong Lighting model.


in PVA
{
	vec3 ecPosition;
	vec3 ecUnitNormal;
} pvaIn;

out vec4 fragmentColor;

const int MAX_NUM_LIGHTS = 8;

// material
uniform vec3 kd = vec3(0.8, 0.0, 0.0); //default
uniform vec3 ka = vec3(0.5, 0.5, 0.5); //default
uniform vec3 ks = vec3(0.5, 0.5, 0.5);
uniform float specular = 1.0;
uniform float alpha = 1.0;


uniform int numLights = 1;
uniform vec3 strengthLights[MAX_NUM_LIGHTS];
uniform vec3 ambientStrength;
uniform vec4 p_ecLightSourcePos[MAX_NUM_LIGHTS];

in vec3 mcPositionOut;

const int ORTHOGONAL = 0;
const int OBLIQUE = 1;
const int PERSPECTIVE = 2;

uniform int projection_type = PERSPECTIVE;
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

	if(PERSPECTIVE == projection_type)
	{
		vHat = -normalize(pvaIn.ecPosition);
	}
	else if( ORTHOGONAL == projection_type)
	{
		vHat = normalize( vec3(0,0,1) );
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

		if( dot(liHat, normal) > 0)
		{

			diffusePart += (attenuation * kd * strengthLights[i] * dot(liHat,normal));
		}


		vec3 rHat = normalize(reflect(-liHat,normal));

		if( dot(rHat, vHat) > 0)
		{
			specularPart+= (attenuation * ks * strengthLights[i] * pow(dot(rHat,vHat),specular));
		}

	}

	float totalR = ambientPart.r + diffusePart.r + specularPart.r;
	float totalG = ambientPart.g + diffusePart.g + specularPart.g;
	float totalB = ambientPart.b + diffusePart.b + specularPart.b;

	return vec4( clamp(totalR,0.0,1.0), clamp(totalG,0.0,1.0), clamp(totalB,0.0,1.0), 1.0);
}

void main ()
{
	fragmentColor = evaluateLightingModel();
}
