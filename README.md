![Tank-hunter the game](https://raw.githubusercontent.com/Education-IT/Tank-Hunter/main//README-images/banner.png)
<img src="https://raw.githubusercontent.com/Education-IT/Tank-Hunter/main/README-images/rpg.png"  height=300 align="right">
## Final project for the course - ***Computer Graphics*** - **UAM**

> **Completed during the fifth semester of computer science studies.**

Tank-Hunter: It is a 3D scene created using the C++ language and the real-time graphics library: OpenGL. This scene represents a war zone where we can play as a soldier equipped with a flashlight and an RPG7. Movement and looking around have been implemented. The project is intended to resemble first-person shooter (FPS) games in its structure. Free ```.obj / .png / .jpg``` files were used to create the project. YouTube Video: **https://www.youtube.com/watch?v=vlE_sZE7wsc&ab_channel=KrystianOsi%C5%84ski**

![C++](https://img.shields.io/badge/C++-00599C.svg?style=for-the-badge&logo=C++&logoColor=white) ![OpenGL](https://img.shields.io/badge/OpenGL-5586A4.svg?style=for-the-badge&logo=OpenGL&logoColor=white) [![Youtube](https://img.shields.io/badge/YouTube-red?style=for-the-badge&logo=youtube&logoColor=white)](https://youtu.be/vlE_sZE7wsc)

<img src="https://raw.githubusercontent.com/Education-IT/Tank-Hunter/main/README-images/scene.png" alt="scene" height=300 align="right">

## Project includes:

Main menu where several settings affecting the gameplay can be changed:
* Enable/disable fog (gray color).
* Night/sunset mode -> Depending on the choice, different skybox, directional light color, and position are used.

* Flashlight - Spotlight (white light) - can be turned on/off using the left mouse button (LMB). It has animation for taking out, putting away, and walking with it.
  

  
* RPG7 - can be aimed with the right mouse button (RMB). It can be reloaded using the LMB. It has animation for taking out, putting away, and walking with it.

* Implemented movement and looking around - both with the keyboard and mouse, as well as with a gamepad. After choosing the game with a gamepad, there is a function that checks if it has been connected - if not, it switches to keyboard and mouse mode.
  
<img src="https://raw.githubusercontent.com/Education-IT/Tank-Hunter/main/README-images/flashlight.png"  height=300 align="right">

* Simple animations simulating walking, jumping, and running. (No running while aiming / jumping is possible while aiming)

* Protection against going out of the scene area. (Out of the map)

* Tracking the "deltaTime" variable, which holds the time it took to render the last frame. This balances the gameplay. The speed of camera movement will be adjusted accordingly, regardless of the computer's speed, ensuring that every user has the same game experience.

* Additional moving objects - Fighter jets flying above our map.

* Animations are created as a result of invented and implemented functions that act on global variables and time, changing the appropriate translation and rotation matrices.

<img src="https://raw.githubusercontent.com/Education-IT/Tank-Hunter/main/README-images/main-menu.gif" alt="main-menu" align="left">

