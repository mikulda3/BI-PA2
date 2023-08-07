#pragma once 

#include "CGhost.hpp"
#include "CGlobal.hpp"

/**
 * Inky is also one of the ghosts. His special ability while chasing pacman
 * is to target the corner tile that is closest to pacman.
 */

class CInky : public CGhost {
    public:
		~CInky() = default;
        CInky(WINDOW * win, int y, int x){
            x_loc = x, y_loc = y;
            curwin = win;
            old_position = ' ';
            getmaxyx(curwin, y_max, x_max);
        }
        void Chase(CPlayer * player, vector<string> & map) override;
        void Scatter(vector<string> & map) override;
};