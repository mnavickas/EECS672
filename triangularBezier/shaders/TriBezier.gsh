#version 410 core

layout ( triangles ) in;
// layout ( points, max_vertices = 1024 ) out;
layout ( points, max_vertices = 93 ) out;

in PVA
{
	vec4 diffuseRef;
	vec3 unitNormal;
} pva_in[];

out PVA
{
	vec4 diffuseRef;
	vec3 unitNormal;
} pva_out;

uniform int pointGenerationMethod = 2;

void outputPoint(float b0, float b1, float b2)
{
	gl_Position = b0*gl_in[0].gl_Position + b1*gl_in[1].gl_Position + b2*gl_in[2].gl_Position;
	pva_out.diffuseRef= b0*pva_in[0].diffuseRef+ b1*pva_in[1].diffuseRef+ b2*pva_in[2].diffuseRef;
	pva_out.unitNormal = b0*pva_in[0].unitNormal + b1*pva_in[1].unitNormal + b2*pva_in[2].unitNormal;
	EmitVertex();
}

//const int RAND_TABLE_SIZE = 100;
float rand0To1Table[] = float[] (
	0.170828, 0.749902, 0.0963717, 0.870465, 0.577304, 0.785799, 0.692194,
	0.368766, 0.873904, 0.745095, 0.446046, 0.353728, 0.73252, 0.260222,
	0.394294, 0.77679, 0.845035, 0.575788, 0.715539, 0.0830042, 0.455825,
	0.109947, 0.545228, 0.390687, 0.568585, 0.959066, 0.867719, 0.16319,
	0.275509, 0.260361, 0.924095, 0.435923, 0.789461, 0.127617, 0.0822057,
	0.940642, 0.0255749, 0.154211, 0.382182, 0.154737, 0.529333, 0.876849,
	0.430611, 0.263906, 0.313594, 0.770092, 0.107391, 0.771042, 0.705196,
	0.21864, 0.761794, 0.411713, 0.648827, 0.929956, 0.502419, 0.687441,
	0.436091, 0.608301, 0.576559, 0.632622, 0.463426, 0.632244, 0.138295,
	0.960761, 0.144375, 0.446683, 0.324584, 0.952584, 0.358184, 0.398208,
	0.101282, 0.955086, 0.984617, 0.57597, 0.865914, 0.149876, 0.90915,
	0.651253, 0.0638609, 0.954999, 0.966263, 0.785543, 0.805165, 0.571254,
	0.282586, 0.962551, 0.579477, 0.436859, 0.375436, 0.923408, 0.0286994,
	0.76886, 0.723107, 0.590969, 0.425889, 0.642143, 0.746763, 0.0690186,
	0.0532949, 0.774314
);

int randIndex = 0;

float next0To1Random()
{
	float r = rand0To1Table[randIndex++];
//	if (randIndex == RAND_TABLE_SIZE)
	if (randIndex == rand0To1Table.length())
		randIndex = 0;
	return r;
}

void random()
{
	const int nSamples = 90;
	for (int i=0 ; i<nSamples/3 ; i++)
	{
		float b0 = next0To1Random();
		float b1 = next0To1Random() * (1.0 - b0);
		float b2 = 1.0 - b0 - b1;
		outputPoint(b0, b1, b2);

		// to get symmetric distribution, cycle through the roles for b0, et al.

		b1 = next0To1Random();
		b2 = next0To1Random() * (1.0 - b1);
		b0 = 1.0 - b1 - b2;
		outputPoint(b0, b1, b2);

		b2 = next0To1Random();
		b0 = next0To1Random() * (1.0 - b2);
		b1 = 1.0 - b2 - b0;
		outputPoint(b0, b1, b2);
	}
}

void slanted()
{
	float b0 = 0.1;
	float db = 0.3/9.0; //30.0;
	while (b0 < 0.4)
	{
		float b1 = 0.1;
		while (b1 < 0.4)
		{
			float b2 = 1.0 - b0 - b1;
			outputPoint(b0, b1, b2);
			b1 += db;
		}
		b0 += db;
	}
}

void symmetricRandom()
{
	const int nSamples = 30;
	for (int i=0 ; i<nSamples ; i++)
	{
		float b = next0To1Random();
		float h = 0.5 * (1.0 - b);

		outputPoint(b, h, h);
		outputPoint(h, b, h);
		outputPoint(h, h, b);
	}
}

void yShaped()
{
	const int nSamples = 4;
	float db = 0.6 / nSamples;
	float b = 0.35; // must be > 1/3
	for (int i=0 ; i<nSamples ; i++)
	{
		float h = 0.5 * (1.0 - b);

		outputPoint(b, h, h);
		outputPoint(h, b, h);
		outputPoint(h, h, b);

		b += db;
	}
}

void main()
{
	if (pointGenerationMethod == 1)
		symmetricRandom();
	else if (pointGenerationMethod == 2)
		slanted();
	else if (pointGenerationMethod == 3)
		yShaped();
	else
		random();
}
