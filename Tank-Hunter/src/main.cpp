#include "glew.h" // OpenGL Extenson Wrangler - load OpenGL - choose version.
#include "MainMenu.hpp"
#include "Tank-Hunter.hpp"

int main(int argc, char** argv)
{	
	// Launching the console Main-Menu
	// start_menu();

	// glfw initialization and configuration - Graphics Library Framework Wrangler
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Creating a window with glfw
	GLFWwindow* window = glfwCreateWindow(800, 800, " Tank-Hunter ", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Loading OpenGL with glew
	// We tell OpenGl what window size we will be working with
	glewInit();
	glViewport(0, 0, 800, 800); 


	/* Avoid game error and application closing unexpectedly.
	If the user selected the game using a GamePad - there is a check whether it is connected.
	If it is not visible - the game switches to mouse and keyboard control.*/
	if (gamepad) {
		int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
		if (0 == present) {
			gamepad = false;
		}
	}

	
	init(window);

	// Game - Main loop 
	renderLoop(window);

	shutdown(window);
	glfwTerminate(); // Correctly closes applications, removes all resources
	return 0;
}
