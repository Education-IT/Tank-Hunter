
Core::RenderContext RPG7_Context;
Core::RenderContext FlashlightContext;
Core::RenderContext destroyedCar;
Core::RenderContext destroyedHelicopter;
Core::RenderContext Helicopter;
Core::RenderContext Jet;
Core::RenderContext T34;
Core::RenderContext T72;
Core::RenderContext grass;
Core::RenderContext Branch;
Core::RenderContext Bush;
Core::RenderContext Conifer;
Core::RenderContext deadTree1;
Core::RenderContext deadTree2;
Core::RenderContext deadTree3;
Core::RenderContext Rocks;
Core::RenderContext Tree;
Core::RenderContext destroyedHouse;
Core::RenderContext House;
Core::RenderContext Well;

// GLuint = opengl UnsignedINT!!!
GLuint program; // Shader ID
GLuint programTex;


Core::Shader_Loader shaderLoader;

namespace texture {


	GLuint RPG7; //
	GLuint FLASHLIGHT;//
	GLuint RUST;//
	GLuint HELICOPTER;
	GLuint T72;
	GLuint T34;
	GLuint GRASS;
	GLuint BRANCH;
	GLuint DEADTREE1;
	GLuint DEADTREE2;
	GLuint ROCKS;
	GLuint TREE;
	GLuint DESTROYEDHOUSE;
	GLuint HOUSE;
	GLuint WELL;
	
}


