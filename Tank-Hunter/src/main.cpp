//#include "ext.hpp"
//#include <GLFW/glfw3.h>
//#include "glm.hpp"
//#include <iostream>
//#include <cmath>
#include "glew.h"
#include "MainMenu.hpp"
#include "Tank-Hunter.hpp"


int main(int argc, char** argv)
{	
	// uruchomienie Menu-G��wnego (konsolowego)
	start_menu();

	//inicjalizacja glfw -> biblioteki od okien.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// tworzenie okna za pomoca glfw
	GLFWwindow* window = glfwCreateWindow(800, 800, " Tank-Hunter ", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//ladowanie OpenGL za pomoca glew
	//przekazujemy OpenGl'owi ja jakim rozmiarze okna b�dziemy pracowali
	glewInit();
	glViewport(0, 0, 800, 800); 


	/* Unikni�cie b��du gry i nieoczekiwanego zamkni�cia aplkiacji.
	Je�eli u�ytkownik wybra� gr� za pomoc� GamePada - zachodzi sprawdzenie czy takowy jest pod��czony. 
	Je�li nie jest on widoczny - gra przechodzi na sterowanie za pomoc� myszki i klawiatury.*/
	if (gamepad) {
		int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
		if (0 == present) {
			gamepad = false;
		}
	}

	init(window);

	// uruchomienie glownej petli programu gry
	renderLoop(window);
	shutdown(window);
	glfwTerminate();
	return 0;
}
