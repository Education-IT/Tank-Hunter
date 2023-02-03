#version 430 core

float AMBIENT = 0.1;

uniform vec3 color;
uniform vec3 lightPos;
uniform sampler2D colorTexture;

//
uniform vec3 cameraDir;
//

in vec3 vecNormal;
in vec3 worldPos;
in vec2 verTexC;
in vec4 vertexPositionOUT;


out vec4 outColor;

float getFogFactor(float dd)
{
     float FogMax = 30.0;
     float FogMin = 5;

    if (dd>=FogMax) {
	return 1;
	}
    if (dd<=FogMin) {
	return 0;
	}

    return (1 - (FogMax - dd) / (FogMax - FogMin));
}


void main()
{	
	
	




vec4 fog_colour = vec4(0.1, 0.1, 0.1,1.0);
 vec4 V = vertexPositionOUT;
 vec4 DIR = vec4(cameraDir,1.0);
    float d = distance( DIR, V);
    float alpha = getFogFactor(d);    
	

	vec3 textureColor = texture2D(colorTexture, -verTexC).xyz;
	vec3 lightDir = normalize(lightPos-worldPos);
	vec3 normal = normalize(vecNormal);
	float diffuse=max(0,dot(normal,lightDir));

	vec4 outputColor = mix(vec4(textureColor*min(1,AMBIENT+diffuse),1.0), fog_colour,alpha);
	outColor = outputColor;
	//outColor = vec4(textureColor*min(1,AMBIENT+diffuse), 1.0);
	
	
}





