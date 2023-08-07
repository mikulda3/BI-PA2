#include "CClyde.hpp"

/**
 * clyde's method to chase is to target pacman's tile if clyde is 8 or more 
 * tiles away, once the distance is smaller than 8, he goes to scatter mode
 */
void CClyde::Chase(CPlayer * player, vector<string> & map){
    if(current_mode != 3)
        this->Turn();
    current_mode = 3;
    if(this->Get_Eaten())
        this->Eaten(map);
    else {
        int y_player, x_player;
        player->Get_Location(y_player, x_player);

        int distance = sqrt(pow(y_loc-y_player, 2) + pow(x_loc-x_player, 2));

        if(distance >= 8)
            Move(y_player, x_player, map);
        else
            Move(29, 26, map);

        this->Display(8);   
        old_position = map.at(y_loc).at(x_loc); 
    }
}

/**
 * method to get Clyde into his corner, that is bottom right
 */
void CClyde::Scatter(vector<string> & map){
    if(current_mode != 2 && current_mode != 0)
        this->Turn();
    current_mode = 2;
    // coord of bottom right corner, which is Clyde's target in scatter mode
    double y_corner = 29, x_corner = 26;
    Move(y_corner, x_corner, map);

    this->Display(8);   

    old_position = map.at(y_loc).at(x_loc);  
}