//#include "glew.h"
#include <GLFW/glfw3.h> // Graphics Library Framework Wrangler/window - Umozliwia Tworzenie okien i obs³ugê wejœcia
#include "glm.hpp" //openGl Mathematics - biblioteka matemtyczna - macierze/wektory
#include "ext.hpp"
#include <iostream>
#include <cmath>

#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include "SOIL/SOIL.h"

#include "gameInput.hpp"

Core::RenderContext sphereContext;
Core::RenderContext shipContext;
Core::RenderContext RPG7_Context;
Core::RenderContext FlashlightContext;

// GLuint = opengl UnsignedINT!!!
GLuint program; // Shader ID
Core::Shader_Loader shaderLoader; 

float aspectRatio = 1.f; 

glm::mat4 createCameraMatrix()
{
	glm::vec3 cameraSide = glm::normalize(glm::cross(cameraDir, glm::vec3(0.f, 1.f, 0.f)));
	glm::vec3 cameraUp = glm::normalize(glm::cross(cameraSide, cameraDir));
	glm::mat4 cameraRotrationMatrix = glm::mat4({
		cameraSide.x,cameraSide.y,cameraSide.z,0,
		cameraUp.x,cameraUp.y,cameraUp.z ,0,
		-cameraDir.x,-cameraDir.y,-cameraDir.z,0,
		0.,0.,0.,1.,
		});
	cameraRotrationMatrix = glm::transpose(cameraRotrationMatrix);
	glm::mat4 cameraMatrix = cameraRotrationMatrix * glm::translate(-cameraPos);

	return cameraMatrix;
}

glm::mat4 createPerspectiveMatrix()
{

	/* 
	float n = 0.05;
	float f = 20; 
	double fov = 100;
	double s = 1 / tan(fov*0.5 * M_PI/180);
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// ! Macierz translation jest definiowana wierszowo dla poprawy czytelnosci. OpenGL i GLM domyslnie stosuje macierze kolumnowe, dlatego musimy ja transponowac !
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	glm::mat4 perspectiveMatrix = glm::mat4({
		aspectRatio2*s,0.,0.,0.,  // zmniejszenie wartoÅ›ci w m11 i m22 wiÄ™kszy kÄ…t widzenia danej osi.-> to te parametry odpowiadajÄ… za Å›ciskanie i rozszerzanie przestrzeni w tych osiach.
		0.,aspectRatio*s,0.,0.,
		0.,0.,(n+f)/(n-f),(2*n*f)/(n-f),
		0.,0.,-1.,0.});
	*/

	glm::mat4 perspectiveMatrix;
	float n = 0.01;
	float f = 20;
	float formula = 2 * f * n / (n - f);
	
	perspectiveMatrix = glm::mat4({
		1,0.,0.,0.,
		0.,aspectRatio,0.,0.,
		0.,0.,(f + n) / (n - f), formula,
		0.,0.,-1.,0.,
		});

	return glm::transpose(perspectiveMatrix);
}

glm::mat4 createEquipmentMatrix()
{
	glm::vec3 Equipment_Side = glm::normalize(glm::cross(Equipment_DIR, glm::vec3(0.f, 1.0f, 0.f)));
	glm::vec3 Equipment_Up = glm::normalize(glm::cross(Equipment_Side, Equipment_DIR));
	glm::mat4 Equipment_Rotation_Matrix = glm::mat4({
		Equipment_Side.x,Equipment_Side.y,Equipment_Side.z,0,
		Equipment_Up.x,Equipment_Up.y,Equipment_Up.z ,0,
		Equipment_DIR.x,Equipment_DIR.y,Equipment_DIR.z,0,
		0.,0.,0.,1.,
		});
	return Equipment_Rotation_Matrix;
}


void drawObjectColor(Core::RenderContext& context, glm::mat4 modelMatrix, glm::vec3 color) {

	glUseProgram(program); // Activating a specific shader
	glm::mat4 viewProjectionMatrix = createPerspectiveMatrix() * createCameraMatrix();
	glm::mat4 transformation = viewProjectionMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "transformation"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);
	glUniform3f(glGetUniformLocation(program, "color"), color.x, color.y, color.z);
	glUniform3f(glGetUniformLocation(program, "lightPos"), 0, 0, 0);
	Core::DrawContext(context);

}


void renderScene(GLFWwindow* window)
{
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glm::mat4 transformation; Czy to potrzebne? Na razie nie usuwaæ!
	glm::mat4 Equipment_Matrix = createEquipmentMatrix();

	// Stores the elapsed time since the window was started
	float currentTime = glfwGetTime();
	
	glDepthMask(GL_FALSE);
	
	glDepthMask(GL_TRUE);


	drawObjectColor(sphereContext, glm::scale(glm::vec3(1.6f)), glm::vec3(1.f, 1.f, 1.f));
	 
	drawObjectColor(sphereContext, glm::eulerAngleY(currentTime / 3) * glm::translate(glm::vec3(4.f, 0, 0)) * glm::scale(glm::vec3(0.3f)) * glm::eulerAngleY(currentTime * 2), glm::vec3(1.f, 0, 0)); 

	drawObjectColor(sphereContext,
		glm::eulerAngleY(currentTime / 3) * glm::translate(glm::vec3(4.f, 0, 0)) * glm::eulerAngleY(currentTime) * glm::translate(glm::vec3(1.f, 0, 0)) * glm::scale(glm::vec3(0.1f)), glm::vec3(0, 0, 1.f));

	
	if (rpg7) {

		if (hold) {
			if (reload) {
				//ANIMACJA PRZE£ADOWANIA RPG7_Context
				drawObjectColor(RPG7_Context,
					glm::translate(Equipment_POS) * (Equipment_Matrix * glm::translate(glm::vec3(movementX, -0.07f + movementY, 0.05f + movementZ))) * glm::eulerAngleXY(deltaTime / 1.2f, 1.44f) * glm::scale(glm::vec3(.0005f)),
					glm::vec3(0.6f, 0.3f, 1.0f)
				);
				RPG_reload_animation(currentTime);

			}
			else {
				//ANIMACJA STANDAROWEGO PORUSZANIA SIÊ Z RPG7_Context
				if (rpg7_hide || rpg7_takeOut) {
					drawObjectColor(RPG7_Context,
						glm::translate(Equipment_POS) * (Equipment_Matrix * glm::translate(glm::vec3(movementX, -0.07f + movementY, 0.05f + movementZ))) * glm::eulerAngleXY(deltaTime*3/4, 1.44f - deltaTime/2) * glm::scale(glm::vec3(.0005f)),
						glm::vec3(0.6f, 0.3f, 1.0f)
					);
					if (rpg7_hide) {
						RPG_takeOut_animation(currentTime);
					}
					else if (rpg7_takeOut){
						RPG_hide_animation(currentTime);
					}
				}
				else {
					drawObjectColor(RPG7_Context,
						glm::translate(Equipment_POS) * (Equipment_Matrix * glm::translate(glm::vec3(movementX, -0.07f + movementY, 0.05f + movementZ))) * glm::eulerAngleY(1.44f) * glm::scale(glm::vec3(.0005f)),glm::vec3(0.6f, 0.3f, 1.0f)
					);
				}	
			}
		}
		else {
			//ANIMACJA CELOWANIA Z RPG7_Context
			drawObjectColor(RPG7_Context,
				glm::translate(Equipment_POS) * (Equipment_Matrix * glm::translate(glm::vec3(0.0f - deltaTime / 9, -0.07f - deltaTime / 600, 0.05f - deltaTime / 45))) * glm::eulerAngleY(1.44f) * glm::scale(glm::vec3(.0005f)), //glm::pi<float>()   /**glm::translate(glm::vec3(-0.10f,-0.0715f,0.03f))*/
				glm::vec3(0.6f, 0.3f, 1.0f)
			);

			if (!aiming || returnToHold) {
				RPG_aim_animation(currentTime);
			}
		}
	}
	else if (flashlight) {
		drawObjectColor(FlashlightContext,
			glm::translate(Equipment_POS) * (Equipment_Matrix * glm::translate(glm::vec3(movementX + 0.12f - deltaTime/17.143, -0.135f + movementY - deltaTime/14.1776, 0.05f + movementZ + 0.12f))) * glm::eulerAngleY(1.44f - deltaTime*144/120) * glm::scale(glm::vec3(.08f)),
			glm::vec3(0.2f, 0.1f, 1.0f)
		);
		if (flashlight_hide) {
			Flashlight_hide_animation(currentTime);
		}
		else if(flashlight_takeOut) {
			Flashlight_takeOut_animation(currentTime);
		}
	}

	// Disabling a given shader (if zero - then disabling the currently used one)
	glUseProgram(0);
	glfwSwapBuffers(window); 
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	aspectRatio = width / float(height);
	glViewport(0, 0, width, height);
}
void loadModelToContext(std::string path, Core::RenderContext& context)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	context.initFromAssimpMesh(scene->mMeshes[0]);
}


void init(GLFWwindow* window)
{
	// If we play with mouseand keyboard - the following functions will be unlocked
	if (!gamepad) {
		glfwSetMouseButtonCallback(window, mouse_button_callback); // Enable keyboard buttons
		glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE); // Enable mouse buttons
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Turns off the cursor visibilityand the ability to exit the window
	}
	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);
	program = shaderLoader.CreateProgram("shaders/shader_5_1.vert", "shaders/shader_5_1.frag");
	
	loadModelToContext("./models/sphere.obj", sphereContext);
	loadModelToContext("./models/spaceship.obj", shipContext);
	loadModelToContext("./models/Equipment/Rpg7.obj", RPG7_Context);
	loadModelToContext("./models/Equipment/Flashlight.obj", FlashlightContext);
}

void shutdown(GLFWwindow* window)
{
	shaderLoader.DeleteProgram(program);
}

// funkcja jest glowna petla
void renderLoop(GLFWwindow* window) {
	while (!glfwWindowShouldClose(window))
	{
		// If we play with mouseand keyboard - the following function will be unlocked
		if (!gamepad) {
			glfwSetCursorPosCallback(window, mouse_callback);
		}

		processInput(window);

		renderScene(window);
		glfwPollEvents();
	}
}
