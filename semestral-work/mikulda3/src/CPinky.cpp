#include "CPinky.hpp"

/**
 * Pinky's method to chase is to target the tile that is 4 tiles in front
 * of pacman
 */
void CPinky::Chase(CPlayer * player, vector<string> & map){
    if(current_mode != 3)
        this->Turn();
    current_mode = 3;
    if(this->Get_Eaten())
        this->Eaten(map);
    else {
        int y_player, x_player;
        player->Get_Location(y_player, x_player);
        int direction = player->Get_Direction();

        switch(direction){
            case 1: // left
                Move(y_player, x_player-4, map);
                break;
            case 2: // right
                Move(y_player, x_player+4, map);
                break;
            case 3: // up
                Move(y_player-4, x_player, map);
                break;
            case 4: // down
                Move(y_player+4, x_player, map);
                break;
            default:
                break;
        }

        this->Display(6);   
        old_position = map.at(y_loc).at(x_loc); 
    }
}

/**
 * method to get Pinky into his corner, that is top left corner
 */
void CPinky::Scatter(vector<string> & map){
    if(current_mode != 2 && current_mode != 0)
        this->Turn();
    current_mode = 2;
    // coord of top left corner, which is Pinky's target in scatter mode
    double y_corner = 1, x_corner = 1;
    Move(y_corner, x_corner, map);

    this->Display(6);   

    old_position = map.at(y_loc).at(x_loc);  
}