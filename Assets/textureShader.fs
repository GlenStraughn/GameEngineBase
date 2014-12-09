
#version 410 core

layout (location = 0) smooth in vec4 vPosition;
layout (location = 1) smooth in vec4 vNormal;
layout (location = 2) smooth in vec2 vTexCo;
layout (location = 3) smooth in vec4 vColor;

layout (location = 0) out vec4 fColor;

uniform vec4 uLightDirection = normalize(vec4(1.0, 1.0, 1.5, 0.0));
uniform vec4 uLightColor = vec4(1, 1, 1, 1.0);

uniform vec4 uViewDirection = normalize(vec4(0.0, 1.0, 1.0, 0.0));
uniform vec4 uHeadLightColor = vec4(0.2, 0.1, 0.1, 1.0);
uniform vec4 uRimLightColor = vec4(0.7, 0.7, 1.0, 3.0);

uniform vec4 uAmbientLight = vec4(1.0, 1.0, 1.0, 1.0);

uniform vec4 uDiffuseColor = vec4(1.0, 1.0, 1.0, 1.0);
uniform vec4 uGlossColor = vec4(0.8, 0.25, 0.0, 3.0);
uniform vec4 uSpecularColor = vec4(0.2, 0.2, 0.2, 0.9);

uniform sampler2D uDiffuseTex;

#define NO_LIGHT 0
#define AMBIENT_LIGHT 1
#define DIRECTIONAL_LIGHT 2
#define POINT_LIGHT 3
#define SPOT_LIGHT 4


#define MAX_LIGHTS 10
struct Light
{
    // type is in attenuation.w
    vec4 position, direction, color, attenuation, coneAngles;
};

layout(std140) uniform Lights
{
    Light L[MAX_LIGHTS];
};

void main()
{
	vec4 N = normalize(vNormal + texture(uDiffuseTex, vTexCo)*0.05);
    //vec4 N = normalize(vNormal);
	vec4 diffuseColor = uDiffuseColor;
	if (dot(vColor.rgb, vColor.rgb) > 0.0) diffuseColor *= vColor;
	diffuseColor *= texture(uDiffuseTex, vTexCo);
	vec4 specColor = 1-texture(uDiffuseTex, vTexCo);
	//specColor *= specColor;

	fColor = vec4(0,0,0,1);
	
	// NORMAL (debug)
	//fColor += 0.5 * (vec4(1,1,1,1) + N.xyzw);

	// AMBIENT TERM
	//fColor += diffuseColor * uAmbientLight;
	
	for (int i=0; i<MAX_LIGHTS; i++)
    {
        float type = L[i].attenuation.w;
        if (type == NO_LIGHT) {
            break; // unknown light type
        }
        else if (type == AMBIENT_LIGHT) {
            fColor += diffuseColor * L[i].color;
        }
        else if (type == DIRECTIONAL_LIGHT) {
            float NdotL = max(0.0, dot(N, L[i].direction));
            fColor += diffuseColor * NdotL * L[i].color;
        }
        else if (type == POINT_LIGHT) {
            vec4 Li = vec4((L[i].position - vPosition).xyz, 0.0);
            float d = sqrt(dot(Li,Li));
            Li *= (1/d);
            vec4 attDist = vec4(d*d, d, 1, 0);
            float attenVal = min(1.0, 1.0/dot(attDist, L[i].attenuation));
            float NdotL = max(0.0, dot(N, Li));
            fColor += (NdotL * attenVal) * diffuseColor * L[i].color;
        }
        else if (type == SPOT_LIGHT) {
            vec4 Li = vec4((L[i].position - vPosition).xyz, 0.0);
            float d = sqrt(dot(Li,Li));
            Li *= (1/d);
            vec4 attDist = vec4(d*d, d, 1, 0);
            float attenVal = min(1.0, 1.0/dot(attDist, L[i].attenuation));
            float NdotL = max(0.0, dot(N, Li));
            
            float cosA = L[i].coneAngles[0];
            float cosB = L[i].coneAngles[1];
            float liDotDir = -(dot(Li, L[i].direction));
            if (liDotDir <= cosB) attenVal = 0.0;
            else if (liDotDir < cosA) attenVal *= (liDotDir - cosB) / (cosA - cosB);
            fColor += (NdotL * attenVal) * diffuseColor * L[i].color;
        }
    }

	// SET W TERM
	fColor.w = 1.0;
}