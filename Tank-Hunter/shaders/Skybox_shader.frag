#version 430 core

uniform samplerCube skybox;

in vec3 texCoord;

out vec4 out_color;

void main()
{
	out_color = texture(skybox,texCoord);
	//�eby widzie� ja�niej -> pomn� przez np.: *3 :D
}



