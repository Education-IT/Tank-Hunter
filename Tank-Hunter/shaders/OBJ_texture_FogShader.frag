#version 430 core

float f = 30.0;
float n = 5;

uniform vec3 color;

uniform sampler2D colorTexture;

//Spotlight
uniform vec3 cameraDir;
uniform float cutOff;
uniform float outerCutOff;
uniform int on_off;
uniform int fog;
//


//FOG
uniform vec3 cameraPos;
//

in vec3 vecNormal;
in vec3 vertexPos;
in vec2 verTexC;


out vec4 outColor;


float getFogFactor(float depthZ)
{
    if (depthZ >= f) {return 1;}
    if (depthZ <= n) {return 0;}

    return (1 - (f - depthZ) / (f - n));
}

void main()
{	
	//https://vicrucann.github.io/tutorials/osg-shader-fog/
	vec3 result;
	vec3 color;
	
	
	//AMBIENT (Oswietlenie otoczenia - symuluje odbicia promieni œwiat³a od innych obiektów + dalekiego swiat³a)
	float ambientStrength = 0.04;
	//vec3 lightColor = vec3(1.0 , 1.0 ,1.0); //NIGHT
	vec3 lightColor = vec3(1.0f , 0.725f , 0.513f); //SUNSET
	vec3 ambient = ambientStrength * lightColor;

	
	
	//OSWIETLENIE ROZPROSZONE -> Diffuse
	vec3 normal = normalize(vecNormal);

	
	//vec3 lightDir = normalize(vec3( 0.7f , 0.7f , -0.8f ));  //NIGHT

	vec3 lightDir = normalize(vec3( 0.6f , 0.5f , -0.3f ));  //SUNSET
	float diff = max(dot(normal,lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	
	
	//OSWIETLENIE ZWIERCIADLANE -> Specular
	float specularStrength = 0.5;
	//float specularStrength = 1; 
	vec3 V = normalize(cameraPos-vertexPos);
	vec3 R = reflect(-lightDir,normal);
	float spec = pow(max(dot(V,R),0.0),1000);
	vec3 specular = specularStrength * spec * lightColor;


	//LATARECZKA ----------------------------------------------------------
	if (on_off >= 1){
	vec3 FL_Dir = normalize(cameraPos - vertexPos);
	float FL_DIFF = max(dot(normal,FL_Dir), 0.0);
	vec3 FL_ReflectDir = reflect(-FL_Dir,normal);
	float FL_spec = pow(max(dot(FL_Dir, FL_ReflectDir), 0.0), 64);
	float distance = length(cameraPos - vertexPos);
	float attenuation = 1.0 / (1.0f + 0.09f * distance + 0.032f * (distance * distance));  
	float theta = dot(FL_Dir, normalize(-cameraDir)); 
	float epsilon = cutOff - outerCutOff;
	float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);

	vec3 FLambient = vec3(0.0f);
    vec3 FLdiffuse = vec3(1.0f) * FL_DIFF;
    vec3 FLspecular = vec3(1.0f) * FL_spec;

	FLambient *= attenuation * intensity;
    FLdiffuse *= attenuation * intensity;
    FLspecular *= attenuation * intensity;

	 result = FLambient + FLdiffuse + FLspecular;
	}else{
	 result = vec3(0.f , 0.f , 0.f);
	}

	
	//LATARECZKA ----------------------------------------------------------

	//WYNIK (Oswietlenie trojskladnikowe)
	vec3 textureColor = texture2D(colorTexture, -verTexC).xyz;
	
	
	//FOG
	if(fog == 1){
	vec3 fog_colour = vec3( 0.2 , 0.2 , 0.2 );
	vec4 Ve = vec4(vertexPos,1);
	vec4 cPos = vec4(cameraPos,1.0);
    float depthZ = distance( cPos, Ve);
    float alpha = getFogFactor(depthZ);    
	color = mix(textureColor*(ambient+specular+diffuse+result) , fog_colour, alpha);
	}
	else{
	color = (textureColor*(ambient+specular+diffuse+result));
	}


	outColor = vec4(color,1);
}





