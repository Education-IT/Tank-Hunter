
#include <GLFW/glfw3.h> // Graphics Library Framework Wrangler/window - Umozliwia Tworzenie okien i obs�ug� wej�cia
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
#include "skybox.h"
#include "scene.h"


float currentTime;
float aspectRatio = 1.f; 




//macierz wodoku/kamery przeksztalca wszytskie wspolrzedne swiata we wspolrzedne widoku, ktore sa umieszczone wzgledem pozycji i kierunku kamery
/*
Aby zdefiniowac kamere potrzebujemy jej pozycji w przestrzeni swiata, kierunku w ktorym patrzy, wektora skierowanego w prawo i wektora skierowanego w gore
POZYZCJA KAMERY -> CameraPos
KIERUNEK PATZRENIA KAMERY -> CameraDir
PRAWA O� -> cameraRight (wyznacza praw� strone - tam gdzie x zaczynaj� by� dodatnie
G�RNA O� -> CameraUp (wyznacza g�r� - tam gdzie y zaczynaj� by� dodatnie)

Normalizacja wektora polega na wzi�ciu wektora o dowolnej d�ugo�ci, zachowa� kierunek w kt�ry jest zwr�cony i zmieni� jego d�ugo�� na 1
glm::cross jest to iloczyn wektorowy - Jego wynikiem jest wektor prostopadly do obu wektorow
*/

glm::mat4 createCameraMatrix()
{
	glm::vec3 cameraRight = glm::normalize(glm::cross(cameraDir, glm::vec3(0.f, 1.f, 0.f)));
	glm::vec3 cameraUp = glm::normalize(glm::cross(cameraRight, cameraDir));
	glm::mat4 cameraRotrationMatrix = glm::mat4({
		cameraRight.x,cameraRight.y,cameraRight.z,0,
		cameraUp.x,cameraUp.y,cameraUp.z ,0,
		-cameraDir.x,-cameraDir.y,-cameraDir.z,0, // wektor cameraDir jest odwr�cony - bo chcemy aby �wiat porusza� si� zawsze w przeciwnym kierunku -> z�udzenie poruszania sie kamery -> tak na prawd� ca�y �wiat si� zmienia pozycje a my stoimi w miejscu!
		0.,0.,0.,1.,
		});
	cameraRotrationMatrix = glm::transpose(cameraRotrationMatrix);
	glm::mat4 cameraMatrix = cameraRotrationMatrix * glm::translate(-cameraPos); //matrix lookAt

	return cameraMatrix;
}

glm::mat4 createPerspectiveMatrix()
{
	glm::mat4 perspectiveMatrix;
	float n = 0.01f;
	float f = 100.0f;
	float formula = 2 * f * n / (n - f);
	
	//macierz projekcji/rzutowania /wizjer kamery - bryla widzenia / rzut perspektywiczny
	//bry�a jest obcieta przez 2 plaszczyzny near and far - powstaje ostroslup sciety -> FRUSTRUM
	// polo�enie plaszczyzn obcinania = near i far
	//aspect ratio - proporcja widoku
	perspectiveMatrix = glm::mat4({
		aspectRatio,0.,0.,0.,
		0.,aspectRatio,0.,0.,
		0.,0.,(f + n) / (n - f), formula,
		0.,0.,-1.,0.,
		});

	return glm::transpose(perspectiveMatrix);
}

glm::mat4 createEquipmentMatrix()
{
	glm::vec3 Equipment_Right = glm::normalize(glm::cross(Equipment_DIR, glm::vec3(0.f, 1.0f, 0.f)));
	glm::vec3 Equipment_Up = glm::normalize(glm::cross(Equipment_Right, Equipment_DIR));
	
	glm::mat4 Equipment_Rotation_Matrix = glm::mat4({
		Equipment_Right.x,Equipment_Right.y,Equipment_Right.z,0,
		Equipment_Up.x,Equipment_Up.y,Equipment_Up.z ,0,
		Equipment_DIR.x,Equipment_DIR.y,Equipment_DIR.z,0,
		0.,0.,0.,1.,
		});
	
	return Equipment_Rotation_Matrix;
}

void drawSKYBOX() {
	glUseProgram(skybox);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_context);

	glm::mat4 transformation = createPerspectiveMatrix() * createCameraMatrix();
	glm::mat4 VIEW = glm::translate(cameraPos);
	glUniformMatrix4fv(glGetUniformLocation(skybox, "transformation"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(skybox, "VIEW"), 1, GL_FALSE, (float*)&VIEW);
	Core::DrawContext(cube);
	glUseProgram(0);
}

void drawObjectColor(GLuint ShaderID ,Core::RenderContext& context, glm::mat4 modelMatrix, glm::vec3 color) {
	
		
	
	glUseProgram(ShaderID); // Activating a specific shader

	if (flashlight && onOff_FL) {
		glUniform1i(glGetUniformLocation(ShaderID, "on_off"), 1);
	}
	else {
		glUniform1i(glGetUniformLocation(ShaderID, "on_off"), 0);
	}
	
	if (fog) {
		glUniform1i(glGetUniformLocation(ShaderID, "fog"), 1);
	}
	else {
		glUniform1i(glGetUniformLocation(ShaderID, "fog"), 0);
	}
	
	

	glm::mat4 viewProjectionMatrix = createPerspectiveMatrix() * createCameraMatrix();
	glm::mat4 transformation = viewProjectionMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(ShaderID, "transformation"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(ShaderID, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);
	glUniform3f(glGetUniformLocation(ShaderID, "color"), color.x, color.y, color.z);
	glUniform1f(glGetUniformLocation(ShaderID, "cutOff"), glm::cos(glm::radians(12.5f)));
	glUniform1f(glGetUniformLocation(ShaderID, "outerCutOff"), glm::cos(glm::radians(15.0f)));
	glUniform3f(glGetUniformLocation(ShaderID, "cameraDir"), cameraDir.x, cameraDir.y, cameraDir.z);
	glUniform3f(glGetUniformLocation(ShaderID, "cameraPos"), cameraPos.x, cameraPos.y, cameraPos.z);

	Core::DrawContext(context);

	// Disabling a given shader (if zero - then disabling the currently used one)
	glUseProgram(0);
}

void drawObjectTexture(GLuint ShaderID , Core::RenderContext& context, glm::mat4 modelMatrix, GLuint textureID) {
	
	
		Core::SetActiveTexture(textureID, "colorTexture", ShaderID, 0);
		drawObjectColor(ShaderID, context, modelMatrix, glm::vec3(0, 0, 0));

}

void drawEquipment() {
	glm::mat4 Equipment_Matrix = createEquipmentMatrix();
	if (rpg7) {

		if (hold) {
			if (reload) {
				drawObjectTexture(programTex, RPG7_Context,
					glm::translate(Equipment_POS) * (Equipment_Matrix * glm::translate(glm::vec3(movementX, -0.07f + movementY, 0.05f + movementZ))) * glm::eulerAngleXY(deltaTime / 1.2f, 1.44f) * glm::scale(glm::vec3(.0005f)),
					texture::RPG7
				);
				RPG_reload_animation(currentTime);
			}
			else {
				//ANIMACJA STANDAROWEGO PORUSZANIA SI� Z RPG7_Context
				if (rpg7_hide || rpg7_takeOut) {
					drawObjectTexture(programTex, RPG7_Context,
						glm::translate(Equipment_POS) * (Equipment_Matrix * glm::translate(glm::vec3(movementX, -0.07f + movementY, 0.05f + movementZ))) * glm::eulerAngleXY(deltaTime * 3 / 4, 1.44f - deltaTime / 2) * glm::scale(glm::vec3(.0005f)),
						texture::RPG7
					);
					if (rpg7_hide) {
						RPG_takeOut_animation(currentTime);
					}
					else if (rpg7_takeOut) {
						RPG_hide_animation(currentTime);
					}
				}
				else {
					drawObjectTexture(programTex, RPG7_Context,
						glm::translate(Equipment_POS) * (Equipment_Matrix * glm::translate(glm::vec3(movementX, -0.07f + movementY, 0.05f + movementZ))) * glm::eulerAngleY(1.44f) * glm::scale(glm::vec3(.0005f)), texture::RPG7
					);
				}
			}
		}
		else {
			//ANIMACJA CELOWANIA Z RPG7_Context
			drawObjectTexture(programTex, RPG7_Context,
				glm::translate(Equipment_POS) * (Equipment_Matrix * glm::translate(glm::vec3(0.0f - deltaTime / 9, -0.07f - deltaTime / 600, 0.05f - deltaTime / 45))) * glm::eulerAngleY(1.44f) * glm::scale(glm::vec3(.0005f)), //glm::pi<float>()   /**glm::translate(glm::vec3(-0.10f,-0.0715f,0.03f))*/
				texture::RPG7
			);

			if (!aiming || returnToHold) {
				RPG_aim_animation(currentTime);
			}
		}
	}
	else if (flashlight) {
		drawObjectTexture(programTex, FlashlightContext,
			glm::translate(Equipment_POS) * (Equipment_Matrix * glm::translate(glm::vec3(movementX + 0.12f - deltaTime / 17.143, -0.135f + movementY - deltaTime / 14.1776, 0.05f + movementZ + 0.12f))) * glm::eulerAngleY(1.44f - deltaTime * 144 / 120) * glm::scale(glm::vec3(.08f)),
			texture::FLASHLIGHT
		);
		if (flashlight_hide) {
			Flashlight_hide_animation(currentTime);
		}
		else if (flashlight_takeOut) {
			Flashlight_takeOut_animation(currentTime);
		}
	}
}

void drawScene(){
	
	drawObjectTexture(programTex, Attack_helicopter, glm::scale(glm::vec3(0.06f)), texture::FLASHLIGHT);

	drawObjectTexture(programTex, T34, glm::eulerAngleY(currentTime / 3) * glm::translate(glm::vec3(4.f, 0, 0)) * glm::scale(glm::vec3(0.03f)) * glm::eulerAngleY(currentTime * 2), texture::TANK);

	drawObjectTexture(programTex, T72,
		glm::eulerAngleY(currentTime / 3) * glm::translate(glm::vec3(4.f, 0, 0)) * glm::eulerAngleY(currentTime) * glm::translate(glm::vec3(1.f, 0, 0)) * glm::scale(glm::vec3(0.005f)), texture::TANK);

}


void renderScene(GLFWwindow* window)
{

	
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clearing the color and z-depth buffers
	
	currentTime = glfwGetTime(); // Stores the elapsed time since the window was started
	
	glDepthMask(GL_FALSE);
	drawSKYBOX(); //ODKOMENTUJ TO I InitSkybox (w funkcji init) �eby mie� skybox 
	glDepthMask(GL_TRUE);

	drawEquipment();
	drawScene();

	glfwSwapBuffers(window);  	// Changes the color buffer (double buffering)
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
	InitSkybox(); //ODKOMENTUJ TO I DrawSkybox (w render scene) �eby mie� skybox 
	 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glEnable(GL_DEPTH_TEST); // z-buffor test ON

	// If we play with mouseand keyboard - the following functions will be unlocked
	if (!gamepad) {
		glfwSetMouseButtonCallback(window, mouse_button_callback); // Enable keyboard buttons
		glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE); // Enable mouse buttons
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Turns off the cursor visibilityand the ability to exit the window
		glfwSetCursorPosCallback(window, mouse_callback); // detects cursor position change
	}
	
	
		programTex = shaderLoader.CreateProgram("shaders/OBJ_texture_FogShader.vert", "shaders/OBJ_texture_FogShader.frag");
		program = shaderLoader.CreateProgram("shaders/OBJ_color_FogShader.vert", "shaders/OBJ_color_FogShader.frag");

	
	skybox = shaderLoader.CreateProgram("shaders/Skybox_shader.vert", "shaders/Skybox_shader.frag");

	
	loadModelToContext("./models/Equipment/Rpg7.obj", RPG7_Context);
	loadModelToContext("./models/Equipment/Flashlight.obj", FlashlightContext);
	loadModelToContext("./models/cube.obj", cube);
	loadModelToContext("./models/Vehicles/T72.obj", T72);
	loadModelToContext("./models/Vehicles/Jet.obj", T34);
	loadModelToContext("./models/Vehicles/Helicopter.obj", Attack_helicopter);

	texture::RPG7 = Core::LoadTexture("./textures/forest_camuflage.png");
	texture::FLASHLIGHT = Core::LoadTexture("./textures/metal.jpg");
	texture::TANK = Core::LoadTexture("./textures/tank.jpg");
	texture::HELICOPTER = Core::LoadTexture("./textures/helicopter.jpg");
	texture::RUST = Core::LoadTexture("./textures/rust.jpg");
	
	
}

void shutdown(GLFWwindow* window)
{
	shaderLoader.DeleteProgram(program);
}

// funkcja jest glowna petla
void renderLoop(GLFWwindow* window) {
	while (!glfwWindowShouldClose(window))
	{
		getFrameTime();
		processInput(window);
		renderScene(window);
		glfwPollEvents(); // Detects events 
	}
}
