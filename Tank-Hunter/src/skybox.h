
Core::RenderContext cube;
GLuint skybox;
GLuint skybox_context;

void InitSkybox() {
	glGenTextures(1, &skybox_context);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_context);
	
	int w, h;
	
	char* zero;
	char* one;
	char* two;
	char* three;
	char* four;
	char* five;

	
	if (night) {

		zero = "textures/skybox/night/sky_0.png";
		one = "textures/skybox/night/sky_1.png";
		two = "textures/skybox/night/sky_2.png";
		three = "textures/skybox/night/sky_3.png";
		four = "textures/skybox/night/sky_4.png";
		five = "textures/skybox/night/sky_5.png";

	}
	else {

		zero = "textures/skybox/sunset/sky_0.png";
		one = "textures/skybox/sunset/sky_1.png";
		two = "textures/skybox/sunset/sky_2.png";
		three = "textures/skybox/sunset/sky_3.png";
		four = "textures/skybox/sunset/sky_4.png";
		five = "textures/skybox/sunset/sky_5.png";
	}

	
	char* tab[] = {zero,one,two,three,four,five};

	unsigned char* data;
	for (unsigned int i = 0; i < 6; i++)
	{
		data = SOIL_load_image(tab[i], &w, &h, 0, SOIL_LOAD_RGBA);
		if (data) {
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data
			);
		}
		else {
			std::cout << "Cubemap tex failed to load at path: " << tab[i] << std::endl;
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data
			);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

