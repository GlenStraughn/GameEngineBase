
#version 410 core

//---------------------------------------------------------------------------
// Stuff that should not have to change between vertex shaders
//---------------------------------------------------------------------------

// INPUTS FROM VERTEX SHADER STAGE
layout (location = 0) smooth in vec4 vPosition;
layout (location = 0) smooth in vec4 vNormal;
layout (location = 0) smooth in vec2 vTexCo;
layout (location = 0) smooth in vec4 vColor;

// OUTPUT FRAGMENT COLOR
out vec4 fColor;

//---------------------------------------------------------------------------
// Global inputs (uniforms)
//---------------------------------------------------------------------------

// TRANSFORM MATRICES
uniform mat4 uObjectPerpsectM;      // object-world-view-perspect transform
uniform mat4 uObjectWorldM;         // object-world transform
uniform mat4 uObjectWorldInverseM;  // inverse object-world transform

// LIGHT PROPERTIES
uniform vec4 uLightDirection = normalize(vec4(1.0, 1.0, 1.0, 0.0));
uniform vec4 uLightColor = vec4(1.5, 1.5, 1.5, 1.0);
uniform vec4 uAmbientLight = vec4(0.2, 0.2, 0.2, 1.0);
uniform vec4 uLightIntensity = vec4(0.4, 0.4, 0.4, 1.0);

// SURFACE PROPERTIES (diffuse only)
uniform float uTextureScale = 1.0;
uniform vec4 uDiffuseColor = vec4(1.0, 1.0, 1.0, 1.0);
uniform vec4 uSpecularColor = vec4(1.0, 1.0, 1.0, 1.0);
uniform sampler2D uDiffuseTex; // diffuse texture

// CAMERA
uniform vec4 uViewDirection;

//---------------------------------------------------------------------------
// Main shader code
//---------------------------------------------------------------------------

void main()
{
	// get a normalized version of the surface normal
	vec4 N = normalize(vNormal);
    
    vec4 reflectionVector = reflect(uViewDirection, N);
	
	// compute the diffuse color (uDiffuseColor * vertexColor * textureColor)
	vec4 diffuseColor = uDiffuseColor;
	diffuseColor *= vColor;
	diffuseColor *= texture(uDiffuseTex, vTexCo*uTextureScale);
    fColor = vec4(1,0,0,1);

	// START WITH BLACK*/
	fColor = vec4(0,0,0,1);
   
    // FIND LENGTHS
    float lengthN = length(N);
    float lengthView = length(uViewDirection);
    float lengthLight = length(uLightDirection);
    float lengthReflection = length(reflectionVector);
    
    float cosTheta = dot(N, uViewDirection)/(lengthN*lengthView);
    float cosPhi = dot(reflectionVector, uLightDirection)/(lengthReflection*lengthLight);
    
    
	// AMBIENT
    fColor += uAmbientLight * diffuseColor * max(0, cosTheta);

	// DIFFUSE TERM
    fColor += uLightIntensity * diffuseColor;
	float NdotL = max(0.0, dot(uLightDirection, N));
	fColor += diffuseColor * NdotL * uLightColor;
    
    fColor += uSpecularColor * uLightIntensity *(pow( max(0,cosPhi), 5) );

	// SET W VALUE TO 1
   
    fColor.w = 1.0;
}
