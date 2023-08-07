#pragma once 

#include "CGhost.hpp"

/**
 * Blinky is the first ghost. When he is chasing pacman, he targets the tile
 * on which is pacman currently on.
 */
class CBlinky : public CGhost {
    public:
		~CBlinky() = default;
        CBlinky(WINDOW * win, int y, int x){
            x_loc = x, y_loc = y;
            curwin = win;
            old_position = ' ';
            getmaxyx(curwin, y_max, x_max);
        }
        void Chase(CPlayer * player, vector<string> & map) override;
        void Scatter(vector<string> & map) override;
};