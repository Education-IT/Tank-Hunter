#include "animations.hpp"

// Mouse control
bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 800.0f / 2.0;

glm::vec3 cameraUp = glm::vec3(0.0f, 0.001f, 0.0f);
glm::vec3 cameraPos = glm::vec3(0.f, 0.f, 4.f);
glm::vec3 cameraDir = glm::normalize(cameraPos - glm::vec3(0.f, 0.f, 0.f));

glm::vec3 Equipment_POS = glm::vec3(0.f, 0.f, 4.f);
glm::vec3 Equipment_DIR = glm::normalize(Equipment_POS - glm::vec3(0.f, 0.f, 0.f));

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
			// TODO ! TURN ON/OFF FLASHLIGHT!
		}
		// TODO ! SHOOTING RPG!
	}
}


void processInput(GLFWwindow* window)
{
	glm::vec3 cameraSide = glm::normalize(glm::cross(cameraDir, glm::vec3(0.f, 1.f, 0.f)));
	bool moveXYZ = false;
	float moveSpeed = 0.0025f;


	int axesCount;
	const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
	int buttonCount;
	const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

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
		}

		if (movement_Gamepad) {
			moveXYZ = true;
		}

		cameraPos += -(cameraDir * moveSpeed * axes[1]);
		cameraPos += glm::normalize(glm::cross(cameraDir, cameraUp)) * moveSpeed * axes[0];
		if ((glm::abs(axes[2]) > 0.1 || glm::abs(axes[3]) > 0.1)) {

			float xpos = static_cast<float>(axes[2]);
			float ypos = static_cast<float>(axes[3]);


			glm::vec3 front;
			front.x = cos(xpos) * cos(-ypos);
			front.y = sin(-ypos);
			front.z = sin(xpos) * cos(-ypos);
			cameraDir = glm::normalize(front);
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
				cameraPos += cameraDir * moveSpeed;
				moveXYZ = true;
			}
			if (Backward){
				cameraPos -= cameraDir * moveSpeed;
				moveXYZ = true;
			}
			if (Left) {
				cameraPos -= glm::normalize(glm::cross(cameraDir, cameraUp)) * moveSpeed;
				moveXYZ = true;
			}
			if (Right) {
				cameraPos += glm::normalize(glm::cross(cameraDir, cameraUp)) * moveSpeed;
				moveXYZ = true;
			}
			//PÓKI CO MO¯NA SWOBODNIE LATAÆ! ¯eby temu zapobiec odkomentuj ten wiersz kodu!
			//cameraPos.y = 0.0f; 
	}


	if (Exit) {
		glfwSetWindowShouldClose(window, true);
	}

	if (Jump) {
		std::cout << "JUMP!"; // <- TODO! JUMPING! 
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

	// Where we look - there follows the equipment
	Equipment_POS = cameraPos;
	Equipment_DIR = cameraDir;


}

// Moving the mouse - moving the camera
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

