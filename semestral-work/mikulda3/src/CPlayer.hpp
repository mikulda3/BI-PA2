#pragma once

#include <string>
#include <ncurses.h>
#include <fstream>
#include <vector>

using namespace std;

/**
 * This is class that maintains player. It decides whether or not player 
 * can move, which directions are possible and at last but not least
 * if he picked any casual or bonus coin etc. 
 */
class CPlayer {
    public:
        CPlayer(WINDOW * win, int y, int x, int level, int pts);
        int Move_Player(CPlayer * player, int & out_score, 
            vector<string> & map, int & out_frightened, int level, int ch);
        void Move_Up(vector<string> & map, int level);
        void Move_Down(vector<string> & map, int level);
        void Move_Left(vector<string> & map, int level);
        void Move_Right(vector<string> & map, int level);
        int Get_Position(vector<string> & map, int level);
        int Get_Move(int ch, vector<string> & map, int level);
        void Display();
        void To_Start();
        int Get_Direction() const { return direction; }
        void Get_Location(int & y, int & x) const;
    private:
        int x_loc, y_loc, x_max, y_max;
        int choice, last_choice;
        int score;
        int frightened;
        int level;
        int points;
        int last_position;
        int direction;
        char character;
        WINDOW * curwin;
};