#version 410 core

// phong.fsh - a fragment shader that implements a Phong Lighting model.


in PVA
{
	vec3 ecPosition;
	vec3 ecUnitNormal;
} pvaIn;


out vec4 fragmentColor;

// Phong material properties

uniform vec3 kd = vec3(0.8, 0.0, 0.0); // default: darkish red
uniform vec3 ka = vec3(0.5, 0.5, 0.5);

vec3 globalAmbient = vec3(0.1, 0.1, 0.1);
vec3 lightStrength = vec3(1, 1, 1);

vec4 p_ecLightSourcePos = vec4( 0,0,1,1);

vec4 evaluateLightingModel()
{
	// THIS IS JUST A PLACEHOLDER FOR A LIGHTING MODEL.
	// It only currently implements simple Lambert shading.

	// NOTES:
	// 1. We assume for now a single directional light source defined in EC (liHat).
	// 2. We assume it will be "full strength" (see liStrength).
	//
	// In project 3, both #1 and #2 will be generalized by introducing uniform
	// arrays ("vec4 p_ecLightSourcePos" and "vec3 ecLightSourceStrength") and
	// using them INSTEAD OF the liHat and liStrength you see here.
	//
	// 3. The use of "abs" here is a temporary hack. As we study the Phong
	//    lighting model more carefully, you will REMOVE "abs" since it will
	//    no longer be appropriate.


	vec3 ambientPart = ka * globalAmbient;

	vec3 liHat;
	if( p_ecLightSourcePos.w == 0 )
	{
		liHat = normalize(p_ecLightSourcePos.xyz);
	}
	else
	{
		liHat = normalize( p_ecLightSourcePos.xyz-pvaIn.ecPosition );
	}

	vec3 diffusePart = lightStrength * kd * dot(normalize(pvaIn.ecUnitNormal),liHat);


	return vec4(ambientPart+diffusePart, 1.0);
}

void main ()
{
	fragmentColor = evaluateLightingModel();
}
