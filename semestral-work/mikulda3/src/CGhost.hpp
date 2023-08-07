#pragma once

#include <cmath>
#include <set>
#include <ncurses.h>
#include <fstream>
#include <string>
#include <vector>

#include "CPlayer.hpp"

/**
 * Class CGhost is a parent class to CBlinky, CPinky, CInky and CClyde.
 * It has some virtual methods such as Scatter and Chase and some methods,
 * which all ghost share and somehow use.
 */
class CGhost {
    public:
		virtual ~CGhost() = default;
        void Display(int color);
        virtual void Chase(CPlayer * player, vector<string> & map) = 0;
        virtual void Scatter(vector<string> & map) = 0;
        void Move(int y_pos, int x_pos, vector<string> & map);
        void Frightened(vector<string> & map);
        void Eaten(vector<string> & map);
        void Turn();
        void To_Start(int y, int x);
        void Set_Eaten(int i){
            eaten = i;
        }
        int Get_Eaten(){
            return eaten;
        }
        void Get_Location(int & y_pos, int & x_pos) const;
    protected:
        int current_mode = 0;
        char character = 'M';
        WINDOW * curwin;
        int eaten = 0;
        int x_loc, y_loc, x_max, y_max;
        char old_position;
        int y_prev = 11, x_prev = 14;
};