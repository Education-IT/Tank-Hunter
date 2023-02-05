#version 430 core

float f = 30.0;
float n = 5;

uniform vec3 color;
uniform sampler2D colorTexture;
uniform vec3 cameraPos;

//Spotlight
uniform vec3 cameraDir;
uniform float cutOff;
uniform float outerCutOff;

// if's
uniform int on_off;
uniform int fog;
uniform int timeOfDay;
//

in vec3 vecNormal;
in vec3 vertexPos;
in vec2 verTexC;
out vec4 outColor;

float getFogFactor(float depthZ) //https://vicrucann.github.io/tutorials/osg-shader-fog/
{
    if (depthZ >= f) {return 1;}
    if (depthZ <= n) {return 0;}

    return (1 - (f - depthZ) / (f - n));
}

void main()
{	
	vec3 result;
	vec3 color;
	vec3 lightColor;
	vec3 lightDir;

	//Diffuse lighting // The more part of an object is directed towards the light source, the brighter it becomes./////////////////////////////////////////////////////////
	vec3 normal = normalize(vecNormal);
	if(timeOfDay == 1){
		lightColor = vec3(1.0 , 1.0 ,1.0); //NIGHT
		lightDir = normalize(vec3( 0.7f , 0.7f , -0.8f ));  //NIGHT
	}else{
		lightColor = vec3(1.0f , 0.62f , 0.46f); //SUNSET
		lightDir = normalize(vec3( 0.6f , 0.5f , -0.3f ));  //SUNSET
	}
	float diff = max(dot(normal,lightDir), 0.0); //iloczyn skalarny (obliczenie konta miêdzy wektorem normalnym a promieniem swiatla) mniejszy jest k¹t miêdzy dwoma wektorami jednostkowymi, tym bardziej iloczyn skalarny jest nachylony w kierunku wartoœci 1. Gdy k¹t miêdzy dwoma wektorami wynosi 90 stopni, iloczyn skalarny przyjmuje wartoœæ 0 - im wiêkszy k¹t tym mniejszy wp³yw œwiat³a na kolor fragmentu. Jeœli k¹t miêdzy obydwoma wektorami jest wiêkszy ni¿ 90 stopni, wynik iloczynu skalarnego faktycznie stanie siê ujemny i otrzymamy ujemny komponent rozproszony. Z tego powodu u¿ywamy funkcji max
	vec3 diffuse = diff * lightColor;

	//AMBIENT -> constant ambient lighting ///////////////////////////////////////////////////////////////
	float ambientStrength = 0.04;
	vec3 ambient = ambientStrength * lightColor;

	
	//Specular lighting //////simulates a bright spot of light that appears on shiny objects//////////////////////
	float specularStrength = 0.5;
	
	vec3 vViev = normalize(cameraPos-vertexPos); //wektor kierunku widoku
	vec3 vRefl = reflect(-lightDir,normal); //wektor odbicia wzd³óŸ osi normlnej -> Funkcja reflect oczekuje, ¿e pierwszy wektor wskazuje od Ÿród³a œwiat³a w kierunku po³o¿enia fragmentu,
	float spec = pow(max(dot(vViev,vRefl),0.0),1000);
	vec3 specular = specularStrength * spec * lightColor;


	//Spotlight -> FLASHLIGHT /////////////////////////////////////////////////////////////////////////
	if (on_off == 1){
		vec3 LightDir_FL = normalize(cameraPos - vertexPos);
		float Diff_FL = max(dot(normal,LightDir_FL), 0.0);

		vec3 FL_ReflectDir = reflect(-LightDir_FL,normal);
		float FL_spec = pow(max(dot(LightDir_FL, FL_ReflectDir), 0.0), 1000);
		


		float distance = length(cameraPos - vertexPos);
		float attenuation = 1.0 / (1.0f + 0.22f * distance + 0.02f * (distance * distance));  
		float theta = dot(LightDir_FL, normalize(-cameraDir)); 
		float epsilon = cutOff - outerCutOff;
		float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);

		vec3 FLambient = vec3(0.0f);
		vec3 FLdiffuse = vec3(1.0f) * Diff_FL; // FlashLight light color * (...)
		vec3 FLspecular = vec3(1.0f) * FL_spec;

		FLambient *= attenuation * intensity; //tlumienie * intensywnosc (g³adkie przejscie promienia swiatla latarki)
		FLdiffuse *= attenuation * intensity;
		FLspecular *= attenuation * intensity;

		result = FLambient + FLdiffuse + FLspecular;
	}
	else{
		result = vec3(0.f , 0.f , 0.f);
	}

	

	//WYNIK (Oswietlenie trojskladnikowe) Phonga /////////////////////////////////////////////////////////////////////////
	vec3 textureColor = texture2D(colorTexture, -verTexC).xyz;
	
	//FOG 
	if(fog == 1){
		vec3 fog_colour = vec3( 0.2 , 0.2 , 0.2 );
		vec4 VerPos = vec4(vertexPos,1);
		vec4 camPos = vec4(cameraPos,1.0);
		float depthZ = distance( camPos, VerPos); //distance between the camera and the vertex
		float alpha = getFogFactor(depthZ);    

		color = mix( textureColor * ( ambient + specular + diffuse + result ) , fog_colour, alpha ); //Mixing the color of the object's texture with the color of the fog. (alpha factor)
	}
	else{
		color = textureColor * ( ambient + specular + diffuse + result );
	}

	//Each type of light adds its contribution to the resulting output color
	outColor = vec4(color,1);
}





