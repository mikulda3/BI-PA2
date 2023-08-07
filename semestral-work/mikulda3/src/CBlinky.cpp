#include "CBlinky.hpp"

/**
 * unique method to chase pacman which is to target pacman's current tile
 */
void CBlinky::Chase(CPlayer * player, vector<string> & map){
    if(current_mode != 3)
        this->Turn();
    current_mode = 3;
    if(this->Get_Eaten())
        this->Eaten(map);
    else {
        int y_player, x_player;
        player->Get_Location(y_player, x_player);

        Move(y_player, x_player, map);
        this->Display(5);   

        // save new position's original content so it can replace when leaves
        old_position = map.at(y_loc).at(x_loc); 
    }
}

/**
 * method to get Blinky into his corner that is top right
 */
void CBlinky::Scatter(vector<string> & map){
    if(current_mode != 2 && current_mode != 0)
        this->Turn();
    current_mode = 2;
    // coord of top right corner, which is Blinky's target in scatter mode
    double y_corner = 1, x_corner = 26;
    Move(y_corner, x_corner, map);

    this->Display(5);   

    // save new position's original content so it can replace when leaves
    old_position = map.at(y_loc).at(x_loc);  
}