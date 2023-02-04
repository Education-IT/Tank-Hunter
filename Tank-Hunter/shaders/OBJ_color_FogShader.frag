#version 430 core

float AMBIENT = 0.1;

uniform vec3 color;
uniform vec3 lightPos;
//FOG
uniform vec3 cameraPos;
//

in vec3 vecNormal;
in vec3 worldPos;

out vec4 outColor;

float getFogFactor(float depth)
{
    float FogMax = 25.0;
    float FogMin = 5;

    if (depth >= FogMax) {
	return 1;
	}

    if (depth <= FogMin) {
	return 0;
	}

    return (1 - (FogMax - depth) / (FogMax - FogMin));
}

void main()
{	
	//https://vicrucann.github.io/tutorials/osg-shader-fog/

	vec4 fog_colour = vec4( 0.2 , 0.2 , 0.2 , 1.0 );
	vec4 V = vec4(worldPos,1);
	vec4 cPos = vec4(cameraPos,1.0);
    float depth = distance( cPos, V);
    float alpha = getFogFactor(depth);   


	vec3 lightDir = normalize(lightPos-worldPos);
	vec3 normal = normalize(vecNormal);
	float diffuse=max(0,dot(normal,lightDir));
	outColor  = mix(vec4(color*min(1,AMBIENT+diffuse),1.0), fog_colour,alpha);
}
