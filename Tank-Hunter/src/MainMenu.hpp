#pragma once
#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <MMsystem.h>
#include "ext.hpp"
#include <GLFW/glfw3.h>

    // Colors:
    const int green = 10;
    const int yellow = 14;
    const int white = 15;
    const int blue = 9;
    const int grey = 8;
    int Set[] = { blue, white, white, white };

    // Main menu variables:
    int counter = 1;
    char key;
    bool on_of = true;
    bool startPressed = false;
    bool authorsPressed = false;
    bool settingsPressed = false;
    int counterPlus;
    int counterMinus;

    // Game Settings:
    bool gamepad = false;
    bool sound = true;
    bool night = false;
    bool fog = true;

    // Change console's color
    void color(int color) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }

    void printTank_Ascii_Art() {
        color(white);
        std::cout << "\n\n";
        std::cout << R"(    [ O ])" << '\n';
        std::cout << R"(      \ \      p)" << '\n';
        std::cout << R"(       \ \  \o/)" << '\n';
        std::cout << R"(        \ \--'---_)";   color(blue);  std::cout << R"(                 ______            __      __  __            __)" << '\n'; color(white);
        std::cout << R"(        /\ \   / ~~\_)";   color(blue);  std::cout << R"(             /_  __/___ _____  / /__   / / / /_  ______  / /____  _____)" << '\n'; color(white);
        std::cout << R"(  ./---/__|=/_/------//~~~\)"; color(blue);  std::cout << R"(        / / / __ `/ __ \/ //_/  / /_/ / / / / __ \/ __/ _ \/ ___/)" << '\n'; color(white);
        std::cout << R"( /___________________/O   O \)"; color(blue); std::cout << R"(     / / / /_/ / / / / ,<    / __  / /_/ / / / / /_/  __/ /)" << '\n'; color(white);
        std::cout << R"( (===(\_________(===(Oo o o O))"; color(blue); std::cout << R"(   /_/  \__,_/_/ /_/_/|_|  /_/ /_/\__,_/_/ /_/\__/\___/_/)" << '\n'; color(white);
        std::cout << R"(  \~~~\____/     \---\Oo__o--)" << '\n';
        std::cout << R"(    ~~~~~~~       ~~~~~~~~~~)" << '\n';

    }

    void printMainMenu() {
        printTank_Ascii_Art();
        std::cout << "\n\n"; color(blue);
        std::cout << "                    ##"; color(white); std::cout << "================| MAIN MENU |================"; color(blue); std::cout << "##" << '\n'; color(white);
        std::cout << "                    ||                                             ||" << '\n';
        std::cout << "                    ||                                             ||" << '\n';
        std::cout << "                    ||                                             ||" << '\n';
        std::cout << "                    ||                                             ||" << '\n';
        std::cout << "                    ||                                             ||" << '\n';
        std::cout << "                    ||                                             ||" << '\n';
        std::cout << "                    ||                                             ||" << '\n'; color(blue);
        std::cout << "                    ##"; color(white); std::cout << "============================================="; color(blue); std::cout << "##" << '\n'; color(white);

    }

    // Change console's coordinates:
    void gotoxy(int x, int y) {
        COORD c;
        c.X = x;
        c.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    }

    // if bool setting -> return: string on/off
    std::string f_on_off(bool on_off) {
        if (on_off == true) {
            return "ON";
        }
        else {
            return "OFF";
        }
    }

    void MainMenuLogic() {

        if (!startPressed && !authorsPressed && !settingsPressed) {
            // MAIN MENU
            gotoxy(37, 15);
            color(Set[0]);
            std::cout << "1. Start game! ";

            gotoxy(37, 17);
            color(Set[1]);
            std::cout << "2.  Settings";

            gotoxy(37, 19);
            color(Set[2]);
            std::cout << "3.  Authors";

            gotoxy(37, 21);
            color(Set[3]);
            std::cout << "4. Exit game";
        }
        else if (startPressed) {
            // START OPTIONS:
            gotoxy(36, 16);
            color(Set[0]);
            std::cout << "1. Mouse & keyboard ";

            gotoxy(36, 18);
            color(Set[1]);
            std::cout << "2.   GamePad";

            gotoxy(36, 20);
            color(Set[2]);
            std::cout << "->   Return";

        }
        else if (!startPressed && authorsPressed) {
            // CREDITS:
            gotoxy(22, 15);
            color(white);
            std::cout << "  Final project:";
            gotoxy(22, 16);
            std::cout << "    Computer Graphics - AMU - 5th semester:";

            gotoxy(24, 18);
            color(white);
            std::cout << "1. Krystian Osinski - s444820";

            gotoxy(24, 19);
            color(white);
            std::cout << "2. Szymon Ptaszynski - s462035";

            gotoxy(24, 21);
            color(Set[2]);
            std::cout << "-> Return";

        }
        else if (!startPressed && !authorsPressed && settingsPressed) {
            // Game settings:
            gotoxy(38, 15);
            color(Set[0]);
            std::cout << "1. Sound: " << f_on_off(sound);

            gotoxy(38, 17);
            color(Set[1]);
            std::cout << "2. Fog: " << f_on_off(fog);

            gotoxy(38, 19);
            color(Set[2]);
            std::cout << "3. Night: " << f_on_off(night);

            gotoxy(38, 21);
            color(Set[3]);
            std::cout << "->  Return";

        }

        key = _getch(); // get_key_number
        system("cls");


        if (!startPressed || settingsPressed) { // 4 opitons 
            counterPlus = 4;
            counterMinus = 3;
        }
        else { // 3 options
            counterPlus = 3;
            counterMinus = 2;
        }

        // key Up
        if (key == 72 && counter >= 2 && counter <= counterPlus && !authorsPressed) {
            counter--;

        }

        // Key Down
        if (key == 80 && counter >= 1 && counter <= counterMinus && !authorsPressed) {
            counter++;

        }

        // Key Enter or Space
        if (key == '\r' || key == 32) {

            // Option No. 1 in each view =====================================================================================
            if (counter == 1 && startPressed == false && settingsPressed == false) {
                startPressed = true; // 
            }
            else if (counter == 1 && startPressed) {

                // Launching the game with appropriate controls
                gamepad = false;
                on_of = false;
            }
            else if (counter == 1 && startPressed == false && settingsPressed) {
                // on/off Sound
                sound = !sound;
                if (sound) {
                    PlaySound(TEXT("Sounds/COD2.wav"), NULL, SND_LOOP | SND_ASYNC | SND_NOSTOP);
                }
                else {
                    PlaySound(NULL, 0, 0);
                }
            }

            // Option No. 2 in each view =====================================================================================
            if (counter == 2 && startPressed == true) {

                // Launching the game with appropriate controls
                gamepad = true;
                on_of = false;

            }
            else if (counter == 2 && startPressed == false && settingsPressed == false) {
                settingsPressed = true; // enable settings view
            }
            else if (counter == 2 && settingsPressed) {
                // on/off FOG
                fog = !fog;

            }

            // Option No. 3 in each view =====================================================================================
            if (counter == 3) {
                if (authorsPressed == false && startPressed == false && settingsPressed == false) {
                    authorsPressed = true; // Enabling crredits view

                }
                else if (authorsPressed == false && startPressed && settingsPressed == false) {
                    startPressed = false; // return to main menu from start options view 
                    counter = 1;
                }
                else if (authorsPressed && startPressed == false && settingsPressed == false) {
                    authorsPressed = false; // return to main menu from credits view
                    counter = 3;
                }
                else if (authorsPressed == false && startPressed == false && settingsPressed) {
                    // night on/off
                    night = !night;
                }
            }


            // Option No. 4 in each view =====================================================================================
            if (counter == 4 && settingsPressed) {
                settingsPressed = false; // return to main menu from settings view
                counter = 2;
            }
            else if (counter == 4) {
                color(white);
                exit(0); // exit game
            }
        }

        // Change color to white
        Set[0] = white;
        Set[1] = white;
        Set[2] = white;
        Set[3] = white;

        // Changing colour to blue - where you are currently in the MENU.
        if (counter == 1) Set[0] = blue;

        if (counter == 2) Set[1] = blue;

        if (counter == 3) Set[2] = blue;

        if (counter == 4) Set[3] = blue;

    }

    void gameInfo() {

        std::string mission = "\"Tank Hunt\"";
        std::string date = "day 4 - 3:29:00 AM";
        std::string place = "Poznan Poland";
        std::string name = "pvt. Krystian Osinski";
        std::string tab_info[] = { mission,date,place,name };

        std::string obj = "Objective:";
        std::string task = "  Find and destroy enemy tanks.";
        std::string score = "  0/3";
        std::string tab_objective[] = { obj,task,score };

        int len;
        int lenpom;
        int time1 = 0;
        std::string xyz = "10#";

        PlaySound(NULL, 0, 0);
        system("cls");
        std::cout << "\n\n\n\n";
        printTank_Ascii_Art();
        Sleep(2500);   system("cls"); Sleep(800);

        gotoxy(36, 10);
        color(green);

        for (int txt = 0; txt < 4; txt++) {
            if (sound) {
                PlaySound(TEXT("Sounds/data-typing2.wav"), NULL, SND_ASYNC);
            }
            len = tab_info[txt].length();
            lenpom = 0;
            for (int i = 0; i < tab_info[txt].length(); i++) {
                gotoxy(36 + i, 10 + txt);
                for (int j = 0; j < xyz.length(); j++) {
                    gotoxy(36 + i, 10 + txt);
                    if (lenpom < len * 3) {
                        std::cout << xyz[j];
                        lenpom++;
                        Sleep(12);
                    }
                    else {
                        break;
                    }
                }
                gotoxy(36 + i, 10 + txt);
                std::cout << tab_info[txt][i];
                Sleep(70);

                if (txt > 1 && i % 7 == 0) {
                    gotoxy(36 + 14, 11);
                    std::cout << time1;
                    time1++;
                }

            }
        }

        PlaySound(NULL, 0, 0);
        Sleep(100);
        if (sound) {
            PlaySound(TEXT("Sounds/WW2.wav"), NULL, SND_ASYNC | SND_LOOP);
        }

        Sleep(900);
        for (int i = 0; i < 4; i++) {

            gotoxy(36 + 14, 11);
            std::cout << time1;
            time1++;
            Sleep(1000);
        }

        for (int txt = 0; txt < 4; txt++) {

            len = tab_info[txt].length();
            lenpom = 0;
            for (int i = 0; i < tab_info[txt].length(); i++) {

                gotoxy(36 + i, 10 + txt);
                std::cout << " ";
                Sleep(40);

                if (txt < 2 && i % 13 == 0) {
                    gotoxy(36 + 14, 11);
                    std::cout << time1;
                    time1++;
                }

            }
        }

        gotoxy(0, 0);
        for (int txt = 0; txt < 3; txt++) {

            len = tab_objective[txt].length();
            lenpom = 0;
            for (int i = 0; i < tab_objective[txt].length(); i++) {

                gotoxy(0 + i, 0 + txt);
                std::cout << tab_objective[txt][i];

                Sleep(40);

            }
        }
        Sleep(1500);
        HWND console = GetConsoleWindow();
        RECT ConsoleRect;
        GetWindowRect(console, &ConsoleRect);
        MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 1450, 800, TRUE);

        if (gamepad) {
            color(white);
            std::cout << "\n\n----------------------------------------------------------------------------------------------------";
            std::cout << "\n\n  GAME SETTINGS: \n\n";
            color(8);
            std::cout << R"(                      _=====_                               _=====_)" << '\n'; color(green);
            std::cout << R"(  (L1) AIM =======>)"; color(grey); std::cout << R"(  / _____ \                             / _____ \)"; color(green); std::cout << R"(<========= (R1) ACTION (SHOOT or TURN ON/OFF FLASHLIGHT))" << '\n';; color(grey);
            std::cout << R"(                   +.-'_____'-.---------------------------.-'_____'-.+)" << '\n';
            std::cout << R"(                  /   |     |  '.     G A M E P A D     .'  |  _  |   \)" << '\n';
            std::cout << R"(                 / ___| /|\ |___ \                     / ___| /_\)"; color(green); std::cout << R"(<|====\======= FLASHLIGH)" << '\n';; color(grey);
            std::cout << R"(                / |      |      | ;  __           _   ; | _         _ | ;)" << '\n';
            std::cout << R"(                | | <---   ---> | | |__|         |_:> | ||_| )"; color(green); std::cout << R"(<=====)"; color(8); std::cout << R"((_)|)"; color(green); std::cout << R"(========== ROCKET LAUNCHER RPG-7)" << '\n';; color(grey);
            std::cout << R"(                | |___   |   ___| ;SELECT       START ; |___       ___| ;)" << '\n';
            std::cout << R"(                |\    | \|/ |    /  _     ___      _   \    | (X) |)"; color(green); std::cout << R"(<===/|====== JUMP)" << '\n'; color(grey);
            std::cout << R"(                | \   |_____|  .','" "', |___|  ,'" "', '.  |_____|  .' |)" << '\n';
            std::cout << R"(                |  '-.______.-' /       \ANALOG/       \  '-._____.-'   |)" << '\n';
            std::cout << R"(                |               |       |------|       |                |)" << '\n';
            std::cout << R"(                |              /\       /      \       /\               |)" << '\n';
            std::cout << R"(                |             /  '.___.'        '.___.'  \              |)" << '\n';
            std::cout << R"(                |            /)"; color(green); std::cout << R"(     ^^              ^^)";     color(grey); std::cout << R"(     \             |)" << '\n';
            std::cout << R"(                 \          /)"; color(green); std::cout << R"(      ||              ||)";      color(grey); std::cout << R"(      \           /)" << '\n';
            std::cout << R"(                  \________/)"; color(green); std::cout << R"(       ||              ||)";      color(grey); std::cout << R"(       \_________/)" << '\n'; color(green);
            std::cout << R"(                                   ||              ||)" << '\n';
            std::cout << R"(                                  MOVE         MOVE CAMERA)" << '\n';
        }
        else {
            color(white);
            std::cout << "\n\n----------------------------------------------------------------------------------------------------";
            std::cout << "\n\n  GAME SETTINGS: \n\n";

            color(grey);
            std::cout << R"(     _______         _______)" << '\n';
            std::cout << R"(    |\     /|       |\     /|                                  )" << '\n';
            std::cout << R"(    | +---+ |       | +---+ |                                  )";                           color(green);           std::cout << R"(               AIM  )" << '\n'; color(grey);
            std::cout << R"(    | |   | |       | |   | |                                  )";                            color(green);           std::cout << R"(               ||)" << '\n'; color(grey);
            std::cout << R"(    | |W  | |       | |F  | | )"; color(green); std::cout << R"(<======= (F)-FLASHLIGHT        )"; color(grey);         std::cout << R"(     , d88b )"; color(green); std::cout << R"(     || )" << '\n'; color(grey);
            std::cout << R"(    | +---+ |       | +---+ |                                  )";                                    std::cout << R"( ,8P'    `8,   )"; color(green); std::cout << R"(|| )" << '\n'; color(grey);
            std::cout << R"(    |/_____\|       |/_____\|                                  )";                                    std::cout << R"( 8'       _.8._)"; color(green);  std::cout << R"(||)"; color(grey);  std::cout << R"(.)" << '\n';
            std::cout << R"(        |                                                      )";                                                        std::cout << R"(8       .'  |. )"; color(green);  std::cout << R"(||)"; color(grey);  std::cout << R"(| )" << '\n';
            std::cout << R"( _______|_______ _______ _______                               )";                                 std::cout << R"(      |    [_] )"; color(green); std::cout << R"(VV)"; color(grey); std::cout << R"(| )" << '\n';
            std::cout << R"(|\     /|\     /|\     /|\     /| )"; color(green); std::cout << R"(ACTION(SHOOT | turn on / off) =====|==>)"; color(grey);                               std::cout << R"(  |     |)" << '\n';
            std::cout << R"(| +---+ | +---+ | +---+ | +---+ |                              )";                                std::cout << R"(      |-----'-----|)" << '\n';
            std::cout << R"(| |   | | |   | | |   | | |   | |                              )";                                std::cout << R"(      |           |)" << '\n';
            std::cout << R"(| |A  | | |S  | | |D  | | |R  | | )"; color(green); std::cout << R"(<======= (R)-ROCEKT LAUNCHER RPG-7)"; color(grey); std::cout << R"( |           |)" << '\n';
            std::cout << R"(| +---+ | +---+ | +---+ | +---+ |                              )";                                std::cout << R"(      |;         .|)" << '\n';
            std::cout << R"(|/_____\|/_____\|/_____\|/_____\|   ________________ )";                                std::cout << R"(                ;\         /;)" << '\n'; color(green);
            std::cout << R"(      \   /    /                   )"; color(grey); std::cout << R"(|                | )"; color(grey);                                                 std::cout << R"(                \\       //)" << '\n'; color(green);
            std::cout << R"(       \ /___ /                    )"; color(grey); std::cout << R"(| SPACE --> )"; color(green);  std::cout << R"(JUMP)"; color(grey); std::cout << R"( |          )";                                                std::cout << R"(        \'._ _.'/ )" << '\n'; color(green);
            std::cout << R"(        |                          )"; color(grey); std::cout << R"(|________________| )"; color(grey);                                                       std::cout << R"(                  '-...-')" << '\n'; color(green);
            std::cout << R"(   (W, A, S, D)                                                    MOUSE - CAMERA MOVE)" << '\n';
            std::cout << R"(       MOVE)" << '\n';
        }


        color(white);
        Sleep(3000);


    }

    void start_menu() {

        
            if (sound) {
                PlaySound(TEXT("Sounds/COD2.wav"), NULL, SND_LOOP | SND_ASYNC | SND_NOSTOP);
            }

            HWND console = GetConsoleWindow();
            RECT ConsoleRect;
            GetWindowRect(console, &ConsoleRect);
            MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 1085, 600, TRUE);

            while (on_of) {

                printMainMenu();
                MainMenuLogic();
            }
            
            gameInfo(); // <- ODKOMENTUJ PRZED ODDANIEM PROJEKTU!
            Sleep(4000);
            // TODO!!! --> DODAJ LOGIKĘ WYGRYWANIA!
            //gameWin();
            on_of = true;
            startPressed = false;
            counter = 1;

        }


