#include "animations.hpp"

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
bool AIM_C = false;
bool Action_C = false;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) //uruchamia siê tylko gdy klikniemy lewy przycisk myszy.
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	//printf("%f,%f\n", xpos, ypos);


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



void processInput(GLFWwindow* window)
{
	glm::vec3 cameraSide = glm::normalize(glm::cross(cameraDir, glm::vec3(0.f, 1.f, 0.f)));
	bool moveXYZ = false;
	float angleSpeed = 0.0025f;
	float moveSpeed = 0.0025f;


	int axesCount;
	const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
	int buttonCount;
	const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

	if (gamepad) {
		Exit = GLFW_PRESS == buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN];
		Forward = axes[1] != 0 || axes[0] != 0;
		Jump = GLFW_PRESS == buttons[GLFW_GAMEPAD_BUTTON_A];
		Flash = GLFW_PRESS == buttons[GLFW_GAMEPAD_BUTTON_Y];
		Rpg = GLFW_PRESS == buttons[GLFW_GAMEPAD_BUTTON_X];
		AIM = GLFW_PRESS == buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER];
		Action = GLFW_PRESS == buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER];

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

		if (Action) {

			if (rpg7) {

				if (hold) {
					if (!reloading && !rpg7_takeOut) {
						reload = !reload;
					}
				}

			}

		}



		if (Forward) {
			moveXYZ = true;
		}

		cameraPos += -(cameraDir * moveSpeed * axes[1]);
		cameraPos += glm::normalize(glm::cross(cameraDir, cameraUp)) * moveSpeed * axes[0];
		if ((glm::abs(axes[2]) > 0.2 || glm::abs(axes[3]) > 0.2)) {

			float xpos = static_cast<float>(axes[2]);
			float ypos = static_cast<float>(axes[3]);


			glm::vec3 front;
			front.x = cos(xpos) * cos(-ypos);
			front.y = sin(-ypos);
			front.z = sin(xpos) * cos(-ypos);
			cameraDir = glm::normalize(front);
		}

	}
	else {
		Exit = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
		Forward = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
		Backward = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
		Left = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
		Right = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
		Jump = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
		Flash = glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;
		Rpg = glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;
		if (!gamepad) {
			if (Forward) {
				cameraPos += cameraDir * moveSpeed;
				moveXYZ = true;
			}
			if (Backward)
			{
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
		}
	}





	if (Exit) {
		glfwSetWindowShouldClose(window, true);
	}
	if (Jump) {
		std::cout << "SPACJA!";
	}
	if (Flash || RPG7_to_flashlight) {
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
	if (Rpg || flashlight_to_RPG7) {
		if (!flashlight) {
			if (rpg7 && !rpg7_takeOut && !aiming && !reloading && !animationAming && !rpg7_hiding) {
				rpg7_hide = !rpg7_hide;
			}
			else if (rpg7 == false && !rpg7_hiding && !rpg7_takeOut && !aiming && !reloading && !animationAming) {
				flashlight_to_RPG7 = false;
				rpg7 = true;
				rpg7_takeOut = !rpg7_takeOut;
			}
		}
		else if (rpg7 == false && !flashlight_hiding && !flashlight_takeOut) {
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

