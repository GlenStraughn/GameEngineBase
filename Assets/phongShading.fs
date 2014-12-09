
#version 410 core

//---------------------------------------------------------------------------
// Definitions
//---------------------------------------------------------------------------

// LIGHT TYPES
//
#define MAX_LIGHTS 10
#define NO_LIGHT 0
#define AMBIENT_LIGHT 1
#define DIRECTIONAL_LIGHT 2
#define POINT_LIGHT 3
#define SPOT_LIGHT 4
#define HEAD_LIGHT 5
#define RIM_LIGHT 6

//---------------------------------------------------------------------------
// Shader inputs and outputs
//---------------------------------------------------------------------------

// INPUTS FROM VERTEX SHADER STAGE
//
layout (location = 0) smooth in vec4 vPosition;
layout (location = 1) smooth in vec4 vNormal;
layout (location = 2) smooth in vec2 vTexCo;
layout (location = 3) smooth in vec4 vColor;

// OUTPUT FRAGMENT COLOR
//
out vec4 fColor;

//---------------------------------------------------------------------------
// Transform matrices from object to world space
//---------------------------------------------------------------------------

// TRANSFORM MATRICES
//
uniform mat4 uObjectPerpsectM;      // object-world-view-perspect transform
uniform mat4 uObjectWorldM;         // object-world transform
uniform mat4 uObjectWorldInverseM;  // inverse object-world transform (for normals)

//---------------------------------------------------------------------------
// Global Lights
//---------------------------------------------------------------------------

struct Light
{
	// type is in attenuation.w
	vec4 position, direction, color, attenuation, coneAngles;
};

layout(std140) uniform Lights
{
	Light L[MAX_LIGHTS];
};

//---------------------------------------------------------------------------
// Camera properties
//---------------------------------------------------------------------------

uniform vec4 uViewPosition = vec4(0,0,10,0); // position of camera
uniform vec4 uViewDirection = vec4(0,0,1,0); // direction to the camera

//---------------------------------------------------------------------------
// Surface properties
//---------------------------------------------------------------------------

// Texture scale (transform texture coordinates)
uniform float uTextureScale = 1.0;

// Emissive color
uniform vec4 uEmissiveColor = vec4(0,0,0,0);

// Diffuse color (we also use the vertex color for this)
uniform vec4 uDiffuseColor = vec4(1.0, 1.0, 1.0, 1.0);
uniform sampler2D uDiffuseTex; 

// Specular color
uniform vec4 uSpecularColor = vec4(1,1,1,10);

//---------------------------------------------------------------------------
// Main shader code
//
// C = Ke + Ia*Kd + SUM Ci
//
// Ci = Kd * (Ii * FATTi) * max(0, dot(Li,N)
//	  + Ks * (Ii * FATTi) * max(0, dot(Li,R)^rho
//---------------------------------------------------------------------------

void main()
{
	//fColor = uSpecularColor;
	//return;

	// normal, reflection, and view vectors
	vec4 N = normalize(vNormal);
	//vec4 V = vec4(normalize(uViewPosition.xyz-vPosition.xyz), 0);
	vec4 V = uViewDirection;
	vec4 R = reflect(-V, N);

	// emissive color
	vec4 Ke = uEmissiveColor;

	// diffuse color (uniformDiffuseColor * vertexColor * textureColor)
	vec4 Kd = uDiffuseColor;
	Kd *= vColor;
	vec4 tex = texture(uDiffuseTex, vTexCo*uTextureScale);
    
    //fColor = tex;
    //return;
    
	if (dot(tex.xyz, tex.xyz) != 0) Kd *= tex;

	// Specular color
	vec4 Ks = vec4(uSpecularColor.rgb, 0);
	float rho = uSpecularColor.a; // exponent

	//--------------------------------------------------------------

	// temporary variables
	float NdotL, RdotL, Fatt;

	// START WITH BLACK
	fColor = vec4(0,0,0,1);
	
	//LOOP OVER LIGHTS
	for (int i=0; i<MAX_LIGHTS; i++)
	{
		float type = L[i].attenuation.w;
		vec4 Ii = L[i].color;

		if (type == NO_LIGHT) {
			break; // unknown light type, no more lights
		}
		else if (type == AMBIENT_LIGHT) {
			fColor += Kd * Ii;
			continue;
		}
		else if (type == DIRECTIONAL_LIGHT) {
			NdotL = max(0.0, dot(N, L[i].direction));
			RdotL = max(0.0, dot(R, L[i].direction));
			Fatt = 1.0;
		}
		else if (type == POINT_LIGHT) {
			vec4 Li = vec4((L[i].position - vPosition).xyz, 0.0);
			float d = sqrt(dot(Li,Li));
			Li *= (1/d);
			//
			NdotL = max(0.0, dot(N, Li));
			RdotL = max(0.0, dot(R, Li));
			vec4 attDist = vec4(d*d, d, 1, 0);
			Fatt = min(1.0, 1.0/dot(attDist, L[i].attenuation));
		}
		else if (type == SPOT_LIGHT) {
			vec4 Li = vec4((L[i].position - vPosition).xyz, 0.0);
			float d = sqrt(dot(Li,Li));
			Li *= (1/d);
			//
			NdotL = max(0.0, dot(N, Li));
			RdotL = max(0.0, dot(R, Li));
			vec4 attDist = vec4(d*d, d, 1, 0);
			Fatt = min(1.0, 1.0/dot(attDist, L[i].attenuation));
			//
			float cosA = L[i].coneAngles[0];
			float cosB = L[i].coneAngles[1];
			float liDotDir = -(dot(Li, L[i].direction));
			if (liDotDir <= cosB) Fatt = 0.0;
			else if (liDotDir < cosA) Fatt *= (liDotDir - cosB) / (cosA - cosB);
		}

		float RdotL_rho = pow(RdotL, rho);

		// Toon shader
		//NdotL = smoothstep(0.48,0.54,NdotL);
		//RdotL_rho = smoothstep(0.4, 0.6, RdotL_rho);

		// Diffuse contribution
		fColor += Kd * Ii * (Fatt * NdotL);

		// Specular contribution
		fColor += Ks * Ii * (Fatt * RdotL_rho);
	}

	// SET W VALUE TO 1
	fColor.w = 1.0;
}
