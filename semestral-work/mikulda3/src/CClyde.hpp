#pragma once 

#include "CGhost.hpp"

/**
 * The last ghost is Clyde. He has a bit different behaviour than the others.
 * When in chase mode, he targets pacman's tile until he is 8 or less tiles
 * away, then he targets his corner tile.
 */
class CClyde : public CGhost {
    public:
		~CClyde() = default;
        CClyde(WINDOW * win, int y, int x){
            x_loc = x, y_loc = y;
            curwin = win;
            old_position = ' ';
            getmaxyx(curwin, y_max, x_max);
        }
        void Chase(CPlayer * player, vector<string> & map) override;
        void Scatter(vector<string> & map) override;
};