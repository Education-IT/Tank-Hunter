#include "glew.h"
#include <GLFW/glfw3.h>
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>

#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "Texture.h"

#include "Box.cpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include "SOIL/SOIL.h"


Core::RenderContext sphereContext;
Core::RenderContext shipContext;
Core::RenderContext RPG7_Context;
Core::RenderContext FlashlightContext;

GLuint program;
Core::Shader_Loader shaderLoader;

////////////
//NAUCZ SIÊ PORUSZANIA MYSZK¥!
glm::vec3 cameraUp = glm::vec3(0.0f, 0.001f, 0.0f);
bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 800.0f / 2.0;
//////////


glm::vec3 cameraPos = glm::vec3(-4.f, 0.f, 0.f);
glm::vec3 cameraDir = glm::vec3(1.f, 0.f, 0.f);

glm::vec3 RPG7_POS = glm::vec3(-4.f, 0.f, 0.f);
glm::vec3 RPG7_DIR = glm::vec3(1.f, 0.f, 0.f);


GLuint VAO, VBO;

float aspectRatio = 1.f; 

//Animations
float movementX = 0;
float movementY = 0;
float movementZ = 0;
bool MINUS = true;
bool PLUS = false;

bool returnToHold = false;
bool reload = false;
bool reloading = false;
float hold = true;
bool aiming = false;
bool animationAming = false;
float deltaTime = 0.f;
float animStartTime = 0;
float returnStartTime = 0;
float deltaTime2 = 0.f;

//Zmiany w ekwipunku
bool rpg7 = false;
bool flashlight = true;
bool flashlight_hide = false;
bool flashlight_takeOut = false;
bool flashlight_takingOut = false;
bool flashlight_hiding = false;
bool rpg7_hide = false;
bool rpg7_takeOut = false;
bool rpg7_hiding = false;
bool flashlight_to_RPG7 = false;
bool RPG7_to_flashlight = false;

void getAnimationTime(float &StartTime,float delta2) {
	if (StartTime <= 0) {
		StartTime = glfwGetTime();
		if (delta2 != 0) {
			deltaTime2 = delta2;
		}
	}
}


void RPG_reload_animation(float currentTime) {
	reloading = true;
	
	getAnimationTime(animStartTime, 0);
	
	deltaTime = currentTime - animStartTime;
	if (deltaTime <= 3.5) {
		if (deltaTime >= 1.75) {

			getAnimationTime(returnStartTime, 1.75);
			deltaTime = deltaTime2 - (currentTime - returnStartTime);}
	}
	else {
		animStartTime = 0;
		returnStartTime = 0;
		reload = false;
		reloading = false;
		
	}
}



void RPG_aim_animation(float currentTime) {
	animationAming = true;
	getAnimationTime(animStartTime, 0);
	deltaTime = currentTime - animStartTime;

	if (aiming) {
		if (returnToHold) {
			getAnimationTime(returnStartTime, 0.9);

			deltaTime = deltaTime2 - (currentTime - returnStartTime);
			if (deltaTime <= 0) {
				animStartTime = 0;
				returnStartTime = 0;
				hold = true;
				aiming = false;
				returnToHold = false;
				animationAming = false;

			}
		}
	}
	else {
		if (deltaTime >= 0.9) {
			aiming = true;
		}
		
	}
}

void RPG_takeOut(float currentTime) {
	rpg7_takeOut = true;

	getAnimationTime(animStartTime, 0);

	deltaTime = currentTime - animStartTime;
	if (deltaTime >= 2.25) {
		
		animStartTime = 0;
		rpg7_hide = false;
		rpg7_takeOut = false;
		rpg7 = false;

	}
}

void RPG_hide(float currentTime) {
	rpg7_hiding = true;

	getAnimationTime(animStartTime, 2.25);

	deltaTime = deltaTime2 - (currentTime - animStartTime);
	if (deltaTime <= 0) {
		
		animStartTime = 0;
		rpg7_takeOut = false;
		rpg7_takeOut = false;
		rpg7_hiding = false;

	}
}


void Flashlight_takeOut(float currentTime) {
	flashlight_takingOut = true;

	getAnimationTime(animStartTime, 1.2);

	deltaTime = deltaTime2 - (currentTime - animStartTime);
	if (deltaTime <= 0) {

		animStartTime = 0;
		flashlight_takeOut = false;
		flashlight_takingOut = false;


	}
}

void Flashlight_hide(float currentTime) {
	flashlight_hiding = true;

	getAnimationTime(animStartTime, 0);

	deltaTime = currentTime - animStartTime;
	if (deltaTime >= 1.2) {

		animStartTime = 0;
		flashlight_hiding = false;
		flashlight = false;
		flashlight_hide = false;

	}
}






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


	perspectiveMatrix = glm::transpose(perspectiveMatrix);

	return perspectiveMatrix;
}



void drawObjectColor(Core::RenderContext& context, glm::mat4 modelMatrix, glm::vec3 color) {

	glUseProgram(program);
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
	glm::mat4 transformation;
	float currentTime = glfwGetTime();
	
	glDepthMask(GL_FALSE);
	
	glDepthMask(GL_TRUE);


	drawObjectColor(sphereContext, glm::scale(glm::vec3(1.6f)), glm::vec3(1.f, 1.f, 1.f));
	 
	drawObjectColor(sphereContext, glm::eulerAngleY(currentTime / 3) * glm::translate(glm::vec3(4.f, 0, 0)) * glm::scale(glm::vec3(0.3f)) * glm::eulerAngleY(currentTime * 2), glm::vec3(1.f, 0, 0)); //0.3f

	drawObjectColor(sphereContext,
		glm::eulerAngleY(currentTime / 3) * glm::translate(glm::vec3(4.f, 0, 0)) * glm::eulerAngleY(currentTime) * glm::translate(glm::vec3(1.f, 0, 0)) * glm::scale(glm::vec3(0.1f)), glm::vec3(0, 0, 1.f));

	glm::vec3 RPG7_Side = glm::normalize(glm::cross(RPG7_DIR, glm::vec3(0.f, 1.0f, 0.f)));
	glm::vec3 RPG7_Up = glm::normalize(glm::cross(RPG7_Side, RPG7_DIR));
	glm::mat4 RPG7_Rotation_Matrix = glm::mat4({
		RPG7_Side.x,RPG7_Side.y,RPG7_Side.z,0,
		RPG7_Up.x,RPG7_Up.y,RPG7_Up.z ,0,
		RPG7_DIR.x,RPG7_DIR.y,RPG7_DIR.z,0,
		0.,0.,0.,1.,
		});
	

	
	
	if (rpg7) {

		if (hold) {
			if (reload) {
				//ANIMACJA PRZE£ADOWANIA RPG7_Context
				drawObjectColor(RPG7_Context,
					glm::translate(RPG7_POS) * (RPG7_Rotation_Matrix * glm::translate(glm::vec3(movementX, -0.07f + movementY, 0.05f + movementZ))) * glm::eulerAngleXY(deltaTime / 1.2f, 1.44f) * glm::scale(glm::vec3(.0005f)),
					glm::vec3(0.6f, 0.3f, 1.0f)
				);
				RPG_reload_animation(currentTime);

			}
			else {
				//ANIMACJA STANDAROWEGO PORUSZANIA SIÊ Z RPG7_Context
					
				if (rpg7_hide || rpg7_takeOut) {
					drawObjectColor(RPG7_Context,
						glm::translate(RPG7_POS) * (RPG7_Rotation_Matrix * glm::translate(glm::vec3(movementX, -0.07f + movementY, 0.05f + movementZ))) * glm::eulerAngleXY(deltaTime*3/4, 1.44f - deltaTime/2) * glm::scale(glm::vec3(.0005f)),
						
						glm::vec3(0.6f, 0.3f, 1.0f)
					);
					if (rpg7_hide) {
						RPG_takeOut(currentTime);
					}
					else if (rpg7_takeOut){
						RPG_hide(currentTime);
					}
				}
				else {
					drawObjectColor(RPG7_Context,
						glm::translate(RPG7_POS) * (RPG7_Rotation_Matrix * glm::translate(glm::vec3(movementX, -0.07f + movementY, 0.05f + movementZ))) * glm::eulerAngleY(1.44f) * glm::scale(glm::vec3(.0005f)),// * glm::translate(glm::vec3(movementX, -0.07f+movementY, 0.05f + movementZ)))
						glm::vec3(0.6f, 0.3f, 1.0f)
					);

				}
				
				
			}
		}
		else {
			//ANIMACJA CELOWANIA Z RPG7_Context
			drawObjectColor(RPG7_Context,
				glm::translate(RPG7_POS) * (RPG7_Rotation_Matrix * glm::translate(glm::vec3(0.0f - deltaTime / 9, -0.07f - deltaTime / 600, 0.05f - deltaTime / 45))) * glm::eulerAngleY(1.44f) * glm::scale(glm::vec3(.0005f)), //glm::pi<float>()   /**glm::translate(glm::vec3(-0.10f,-0.0715f,0.03f))*/
				glm::vec3(0.6f, 0.3f, 1.0f)
			);

			if (!aiming || returnToHold) {
				RPG_aim_animation(currentTime);
			}

		}
	}
	else if (flashlight) {
		drawObjectColor(FlashlightContext,
			//glm::translate(RPG7_POS) * (RPG7_Rotation_Matrix * glm::translate(glm::vec3(movementX+0.12f, -0.135f + movementY, 0.05f + movementZ+0.12f))) * glm::eulerAngleY(1.44f)  * glm::scale(glm::vec3(.08f)),
			glm::translate(RPG7_POS) * (RPG7_Rotation_Matrix * glm::translate(glm::vec3(movementX + 0.12f - deltaTime/17.143, -0.135f + movementY - deltaTime/14.1776, 0.05f + movementZ + 0.12f))) * glm::eulerAngleY(1.44f - deltaTime*144/120) * glm::scale(glm::vec3(.08f)),
			glm::vec3(0.2f, 0.1f, 1.0f)
		);
		if (flashlight_hide) {
			Flashlight_hide(currentTime);
		}
		else if(flashlight_takeOut) {
			Flashlight_takeOut(currentTime);
		}
	}

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

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) //uruchamia siê tylko gdy klikniemy lewy przycisk myszy.
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	//printf("%f,%f\n", xpos, ypos);
	
	
	if (button == GLUT_MIDDLE_BUTTON && action == GLUT_DOWN) {
		
		if (rpg7) {
			
			if (!reload && !rpg7_takeOut) {
				if (aiming) {
					returnToHold = true;
				}
				else {
					if (animationAming == false) {
					hold = !hold;
				}
				}
			}
		}
		
	}

	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {

		if (rpg7) {

			if (hold) {
				if (!reloading && !rpg7_takeOut) {
					reload = !reload;
				}
			}

		}
		else if (flashlight) {
			
		}

	}
  
}


void init(GLFWwindow* window)
{
	glfwSetMouseButtonCallback(window, mouse_button_callback); 
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

void movement_animation() {

	if (hold) {
		if (movementX >= -0.005 && !PLUS && MINUS) {
			movementX = movementX - 0.000025;
			movementY = movementY - 0.000005;
			movementZ = movementZ - 0.0000125;
		}
		if (movementX <= -0.005) {
			PLUS = true;
			MINUS = false;
		}

		if (PLUS && movementX <= 0.005) {
			movementX = movementX + 0.000025;
			movementY = movementY + 0.000005;
			movementZ = movementZ + 0.0000125;
		}
		if (movementX >= 0.005) {
			MINUS = true;
			PLUS = false;
		}

		if (movementX >= 0.005 && PLUS && !MINUS) {
			movementX = movementX + 0.000025;
			movementY = movementY + 0.000005;
			movementZ = movementZ + 0.0000125;
		}

	}

}


//obsluga wejscia
void processInput(GLFWwindow* window)
{
	
	glm::vec3 cameraSide = glm::normalize(glm::cross(cameraDir, glm::vec3(0.f, 1.f, 0.f)));
	bool moveXYZ = false;
	float angleSpeed = 0.0025f;
	float moveSpeed = 0.0025f;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraDir * moveSpeed;
		moveXYZ = true;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraPos -= cameraDir * moveSpeed;
		moveXYZ= true;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= glm::normalize(glm::cross(cameraDir, cameraUp)) * moveSpeed;
		moveXYZ = true;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += glm::normalize(glm::cross(cameraDir, cameraUp)) * moveSpeed;
		moveXYZ = true;
	}
	if ((glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) || RPG7_to_flashlight) {
		if (!rpg7) {
			if (!flashlight && !flashlight_hiding && !flashlight_takeOut) {
				flashlight = true;
				flashlight_takeOut = true;
				RPG7_to_flashlight = false;
				
			}
			else {
				if (!flashlight_hiding && !flashlight_takeOut)
					flashlight_hide = true;
			}
		}
		else if (flashlight == false && !rpg7_hiding && !rpg7_takeOut && !aiming && !reloading && !animationAming) {
			rpg7_hide = true;
			RPG7_to_flashlight = true;
		}
	}
	if ((glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) || flashlight_to_RPG7) {
		if (!flashlight) {
			if (rpg7 && !rpg7_takeOut && !aiming && !reloading && !animationAming && !rpg7_hiding ) {
				rpg7_hide = !rpg7_hide;
			}
			else if (rpg7 == false && !rpg7_hiding && !rpg7_takeOut && !aiming && !reloading && !animationAming)  {
				flashlight_to_RPG7 = false;
				rpg7 = true;
				rpg7_takeOut = !rpg7_takeOut;
			}
		}
		else if(rpg7 == false && !flashlight_hiding && !flashlight_takeOut){
			flashlight_hide = true;
			flashlight_to_RPG7 = true;
			
		}
	}
	
	if (moveXYZ) {
		movement_animation();
	}

	RPG7_POS = cameraPos;
	RPG7_DIR = cameraDir;
	
	
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.5f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraDir = glm::normalize(front);
}

// funkcja jest glowna petla
void renderLoop(GLFWwindow* window) {
	while (!glfwWindowShouldClose(window))
	{
		glfwSetCursorPosCallback(window, mouse_callback);
		
		processInput(window);
		
		renderScene(window);
		glfwPollEvents();
	}
}
//}