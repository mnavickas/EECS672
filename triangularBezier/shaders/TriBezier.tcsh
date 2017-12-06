#version 410 core

layout ( vertices = 25 ) out; // Max number of vertices per PATCH

uniform ivec2 vpWidthHeight;
uniform float maxTriEdgePixelLength = 50.0;

in PVA
{
	vec4 diffuseRef;
	vec3 unitNormal;
} pva_in[];

out PVA
{
	vec4 diffuseRef;
	vec3 unitNormal;
} pva_out[];

float getLDSCoord(float v)
{
	if (isinf(v))
		return 0.0;
	return v;
}

float getPixelMeasure()
{
	vec2 p = gl_in[0].gl_Position.xy / gl_in[0].gl_Position.w;
	float xmin = getLDSCoord(p.x);
	float xmax = xmin;
	float ymin = getLDSCoord(p.y);
	float ymax = ymin;
	for (int i=1 ; i<gl_in.length() ; i++)
	{
		p = gl_in[i].gl_Position.xy / gl_in[i].gl_Position.w;
		float v = getLDSCoord(p.x);
		if (v < xmin)
			xmin = v;
		else if (v > xmax)
			xmax = v;
		v = getLDSCoord(p.y);
		if (v < ymin)
			ymin = v;
		else if (v > ymax)
			ymax = v;
	}
	float dx = xmax - xmin;
	float dy = ymax - ymin;
	// The coordinates are in 3D LDS, hence the deltas should all
	// be: 0<=d_<=2. Scale to pixels.
	int pixelDim = max(vpWidthHeight[0], vpWidthHeight[1]);
	float f = (dx + dy) / 4.0;
	return f * float(pixelDim);
}

void main()
{
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

	pva_out[gl_InvocationID].diffuseRef= pva_in[gl_InvocationID].diffuseRef;
	pva_out[gl_InvocationID].unitNormal = pva_in[gl_InvocationID].unitNormal;

	// Guess at what reasonable tessellation levels should be. Compute the maximum
	// individual coordinate delta in LDS. Then convert to pixels.

	float pixelMeasure = getPixelMeasure();

	// Our heuristic: set the tessellation level so that triangle edges are no more
	// than "maxTriEdgePixelLength" long.
	float outerTessLevel = max(pixelMeasure / maxTriEdgePixelLength, 1.0);
	float innerTessLevel = max(outerTessLevel-1.0, 1.0);

	gl_TessLevelOuter[0] = gl_TessLevelOuter[1] = gl_TessLevelOuter[2] = outerTessLevel;
	gl_TessLevelInner[0] = innerTessLevel;
}
