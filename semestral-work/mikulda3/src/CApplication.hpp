#pragma once

#include <ncurses.h>
#include <fstream>
#include <string>
#include <vector>

#include "CGame.hpp"

using namespace std;

/**
 * This class maintains the main menu and other settings like choice of map
 * or difficulty.
 */
class CApplication {
    public:
        // constructor
        CApplication();
        void Check_Colors();
        void Init_Colors();
        void Update_Menu(WINDOW * menu, string options[], int options_pos[]);
        void Start_Game();
        void Settings(WINDOW * menu);
        void Quit(WINDOW * menu);
        void Show_Menu();
        void Select_Map(WINDOW * menu);
        void Select_Difficulty(WINDOW * menu);
        void Show_Instructions(WINDOW * menu);
    private:
        int map, difficulty;
        int choice;
        int selected;
        int y_max, x_max;
        int first_time;
};