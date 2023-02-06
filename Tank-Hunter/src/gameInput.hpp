#include "animations.hpp"

// Mouse control
bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 800.0f / 2.0;


glm::vec3 cameraUp = glm::vec3(0.0f, 1.f, 0.0f);
glm::vec3 cameraPos = glm::vec3(0.f, 0.f, 4.f);
glm::vec3 cameraDir = glm::normalize(cameraPos - glm::vec3(0.f, 0.f, 0.f));

glm::vec3 Equipment_POS;
glm::vec3 Equipment_DIR;

// Control settings - depending on the keyboard / gamepad
bool Exit;
bool Forward;
bool Backward;
bool Left;
bool Right;
bool Jump;
bool Flash;
bool Rpg;
bool AIM;
bool Action;

bool jumping = false;


// Mouse button down detection and the corresponding action
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// Aim / zoom out 
	if ((button == GLUT_MIDDLE_BUTTON && action == GLUT_DOWN)) {

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

	// Weapon reload, shot, flashlight on/off
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) {

		if (rpg7) {

			if (hold) {
				if (!reloading && !rpg7_takeOut) {
					reload = !reload;
				}
			}
		}
		else if (flashlight) {
			onOff_FL = !onOff_FL;
		}
		// TODO ! SHOOTING RPG!
	}
}

void Jump_Animation(float currentTime) {
	if (jumpAnimation <= 0) {
		jumpAnimation = glfwGetTime();

	}

	jumpDelta = currentTime - jumpAnimation;
	if (jumpDelta <= 0.5) {
		if (jumpDelta >= 0.25) {
			cameraPos.y -= 0.005f * run;
		}
		else {
			cameraPos.y += 0.005f * run;
		}
	}
	else {
		if (cameraPos.y >= 0.01f) {
			cameraPos.y -= 0.01 * run;
		}
		else {
			jumpAnimation = 0;
			jumpDelta = 0;
			jumping = false;
			cameraPos.y = 0.0f;
		}
		
	}


}


void processInput(GLFWwindow* window)
{
	
	bool moveXYZ = false;
	float moveSpeed = static_cast<float>(2 * deltaFrame);
	run = 1;
	int axesCount;
	const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
	int buttonCount;
	const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

	//Fast run (only when we don't aim)
	if (!aiming && !animationAming) {
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	
/*!!!!!!!!!!!!!!!!!!!!*/run = 5;//ZMIEÑ PRZED ODDANIEM NA MAX 2.5! TO JEST TYLKO PO TO ¯EBY £ATWOIEJ BY£O BUDOWAC MAPE!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//! //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}

	if (gamepad) {
		Exit = GLFW_PRESS == buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN];
		bool movement_Gamepad = axes[1] != 0 || axes[0] != 0;
		Jump = GLFW_PRESS == buttons[GLFW_GAMEPAD_BUTTON_A];
		Flash = GLFW_PRESS == buttons[GLFW_GAMEPAD_BUTTON_Y];
		Rpg = GLFW_PRESS == buttons[GLFW_GAMEPAD_BUTTON_X];
		AIM = GLFW_PRESS == buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER];
		Action = GLFW_PRESS == buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER];

		// Aim / zoom out (GAMEPAD)
		if (AIM) {
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

		// Weapon reload, shot, flashlight on/off (GAMEPAD)
		if (Action) {

			if (rpg7) {

				if (hold) {
					if (!reloading && !rpg7_takeOut) {
						reload = !reload;
					}
				}
			}
			else if (flashlight) {
				onOff_FL = !onOff_FL;
			}
		}

		if (movement_Gamepad) {
			moveXYZ = true;
		}

		cameraPos += -(cameraDir * moveSpeed * axes[1]);
		cameraPos += glm::normalize(glm::cross(cameraDir, cameraUp)) * moveSpeed * axes[0];
		if (axes[2] != GLFW_RELEASE && axes[3] != GLFW_RELEASE) {

			float xxx = static_cast<float>(axes[2]);
			float yyy = static_cast<float>(axes[3]);
			
			glm::vec3 frontx;
			frontx.x = cos(xxx) * cos(-yyy);
			frontx.y = sin(-yyy);
			frontx.z = sin(xxx) * cos(-yyy);
			cameraDir = glm::normalize(frontx);
		}
	
	}
	else { // KEYBOARD AND MOUSE! ---------------------------------------------------------
		Exit = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
		Forward = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
		Backward = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
		Left = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
		Right = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
		Jump = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
		Flash = glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;
		Rpg = glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;
		
			if (Forward) { 
				cameraPos += cameraDir * moveSpeed * run;
				moveXYZ = true;
			}
			if (Backward){
				cameraPos -= cameraDir * moveSpeed * run;
				moveXYZ = true;
			}			//normalizujemy wektor aby nie mieæ zmiennej prêdkoœci!
			if (Left) { //wykonujemy iloczyn wektorowy poniew¹ tworzymy wektor prostopad³y do naszego kierunku i bêdziemy poruszaæ siê po nim w prawo lub w lewo
				cameraPos -= glm::normalize(glm::cross(cameraDir, cameraUp)) * moveSpeed * run;
				moveXYZ = true;
			}
			if (Right) {
				cameraPos += glm::normalize(glm::cross(cameraDir, cameraUp)) * moveSpeed * run;
				moveXYZ = true;
			}
			//PÓKI CO MO¯NA SWOBODNIE LATAÆ! ¯eby temu zapobiec odkomentuj ten wiersz kodu!
			if (!jumping)
			{
				cameraPos.y = 0.0;

			}

			if (cameraPos.y < 0.0){
				cameraPos.y = 0.0f;
			}
			
			
	}


	if (Exit) {
		glfwSetWindowShouldClose(window, true);
	}

	if (Jump) {
		if (!jumping) {
			jumping = true;
		}
	}

	


	if (Flash || RPG7_to_flashlight) {
		if (!rpg7) { // Take out flashlight
			if (!flashlight && !flashlight_hiding && !flashlight_takeOut) {
				flashlight = true;
				flashlight_takeOut = true;
				RPG7_to_flashlight = false;

			}
			else { // Hide flashlight
				if (!flashlight_hiding && !flashlight_takeOut)
					flashlight_hide = true;
			}
		}// Change flashlight to RPG7
		else if (flashlight == false && !rpg7_hiding && !rpg7_takeOut && !aiming && !reloading && !animationAming) {
			rpg7_hide = true;
			RPG7_to_flashlight = true;
		}
	}
	if (Rpg || flashlight_to_RPG7) {
		if (!flashlight) { // Hide RPG7
			if (rpg7 && !rpg7_takeOut && !aiming && !reloading && !animationAming && !rpg7_hiding) {
				rpg7_hide = !rpg7_hide;
			
			}// Take out RPG7
			else if (rpg7 == false && !rpg7_hiding && !rpg7_takeOut && !aiming && !reloading && !animationAming) {
				flashlight_to_RPG7 = false;
				rpg7 = true;
				rpg7_takeOut = !rpg7_takeOut;
			}
		}// Change RPG7 to flashlight
		else if (rpg7 == false && !flashlight_hiding && !flashlight_takeOut) {
			flashlight_hide = true;
			flashlight_to_RPG7 = true;

		}
	}

	// If moving is detected - turn on walking animation
	if (moveXYZ) {
		movement_animation();
	}

	if (cameraPos.x < -20) {
		cameraPos.x = -20;
	}
	if (cameraPos.x > 20) {
		cameraPos.x = 20;
	}
	if (cameraPos.z < -20) {
		cameraPos.z = -20;
	}
	if (cameraPos.z > 20) {
		cameraPos.z = 20;
	}

	// Where we look - there follows the equipment
	Equipment_POS = cameraPos;
	Equipment_DIR = cameraDir;


}

// Moving the mouse - moving the camera
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	/*
	Zmieniamy wektor CameraDir na podstawie rotacji myszy (XY) -> wymaga ona trygonemetri!
	
	Katy Eulera:
	1) Pitch -> jak bardzo poatrzmy w górê lub dó³
	2) Yaw -> jak bardzo patrzymy na boki
	3) Roll -> jak bardzo robimy beczke / turlamy siê
	
	Poziomy ruch myszy wp³ywa na - yaw
	Pionowy ruch mysyz wp³ywa na - pitch

	Ide¹ jest to , ¿eby zapamietaæ pozycje myszy z ostatniej ramki i nastêpnie obliczyæ
	jak bardzo ta pozycja sie zmieni³a

	a) Obliczmy przesuniêcie myszy od czasu ostatniej ramki
	b) Dodajemy wartoœc przesuniêcia do wartoœci pitch i yaw kamery
	c) Dodajemy pewne ograniczenia likwiduj¹ce b³êdy
	d) Obliczamy wektor kierunku , normalizujemy go i przypisujemy go do aktualnej zmiennej CameraDir
	*/
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

	float sensitivity = 0.5f; 
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
