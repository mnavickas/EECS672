#version 410 core

uniform mat4 mc_ec, ec_lds;

layout (location = 0) in vec4 mcPosition;
in vec4 diffuseRef;
in vec3 mcNormal;

out PVA
{
	vec4 diffuseRef;
	vec3 unitNormal;
} pva_out;

void main()
{
	pva_out.diffuseRef= diffuseRef;

	vec4 p_ecPosition = mc_ec * mcPosition;
	mat3 normalMatrix = transpose( inverse( mat3x3(mc_ec) ) );
	pva_out.unitNormal = normalize( normalMatrix*mcNormal );
	gl_Position = ec_lds * p_ecPosition;
}
