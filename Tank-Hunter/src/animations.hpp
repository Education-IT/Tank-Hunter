#include "ext.hpp"

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


void getAnimationTime(float& StartTime, float delta2) {
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
			deltaTime = deltaTime2 - (currentTime - returnStartTime);
		}
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

void RPG_takeOut_animation(float currentTime) {
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

void RPG_hide_animation(float currentTime) {
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


void Flashlight_takeOut_animation(float currentTime) {
	flashlight_takingOut = true;

	getAnimationTime(animStartTime, 1.2);

	deltaTime = deltaTime2 - (currentTime - animStartTime);
	if (deltaTime <= 0) {

		animStartTime = 0;
		flashlight_takeOut = false;
		flashlight_takingOut = false;


	}
}

void Flashlight_hide_animation(float currentTime) {
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
