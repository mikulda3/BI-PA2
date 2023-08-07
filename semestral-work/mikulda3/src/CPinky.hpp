#pragma once 

#include "CGhost.hpp"

/**
 * Pinky is another ghost. His special ability while chasing is to target 
 * four tiles in front of pacman.
 */

class CPinky : public CGhost {
    public:
		~CPinky() = default;
        CPinky(WINDOW * win, int y, int x){
            x_loc = x, y_loc = y;
            curwin = win;
            old_position = ' ';
            getmaxyx(curwin, y_max, x_max);
        }
        void Chase(CPlayer * player, vector<string> & map) override;
        void Scatter(vector<string> & map) override;
};