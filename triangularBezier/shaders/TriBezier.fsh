#version 410 core

in PVA
{
	vec4 diffuseRef;
	vec3 unitNormal;
} pva_in;

out vec4 fragColor;

int numLights = 2;
float lightStrength[2] = float[] ( 0.85, 0.95 );
vec3 lightSourceDir[2] = vec3[] ( vec3(0.5, 0.5, 0.5),
	vec3(-0.5, 0.5, 0.0) );

void main()
{
	float lightAtten = 0.0;
	// pva_in.unitNormal was created with unit length (either in the
	// vertex shader or tessellation shader), but it has since been
	// interpolated across the interior of a triangle, hence it
	// won't in general have unit length here, and hence:
	vec3 nHat = normalize(pva_in.unitNormal);
	for (int i=0 ; i<numLights ; i++)
	{
		vec3 liHat = normalize(lightSourceDir[i]);
		lightAtten += lightStrength[i]*abs(dot(liHat, nHat));
	}
	fragColor = vec4(lightAtten*pva_in.diffuseRef.rgb, 1.0);
}
