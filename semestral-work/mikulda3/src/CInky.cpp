#include "CInky.hpp"

/**
 * Inky's method to chase is more complex. It is to target corner tile that
 * is closest to pacman.
 */
void CInky::Chase(CPlayer * player, vector<string> & map){
    if(current_mode != 3)
        this->Turn();
    current_mode = 3;
    if(this->Get_Eaten())
        this->Eaten(map);
    else {
        set<Path> corners;
        // coordinates of corners of the map
        int y_left_top = 1, x_left_top = 1;
        int y_left_bottom = 29, x_left_bottom = 1;
        int y_right_top = 1, x_right_top = 26;
        int y_right_bottom = 29, x_right_bottom = 26;

        int y_player, x_player;
        player->Get_Location(y_player, x_player);

        Path tmp;
        // get the distance of each corner
        tmp.distance = sqrt(pow(y_player-y_left_top, 2) + pow(x_player-x_left_top, 2));
        tmp.direction = "left_top";
        corners.insert(tmp);
        tmp.distance = sqrt(pow(y_player-y_left_bottom, 2) + pow(x_player-x_left_bottom, 2));
        tmp.direction = "left_bottom";
        corners.insert(tmp);
        tmp.distance = sqrt(pow(y_player-y_right_top, 2) + pow(x_player-x_right_top, 2));
        tmp.direction = "right_top";
        corners.insert(tmp);
        tmp.distance = sqrt(pow(y_player-y_right_bottom, 2) + pow(x_player-x_right_bottom, 2));
        tmp.direction = "right_bottom";
        corners.insert(tmp);

        auto it = *corners.begin();

        // make move
        if(it.direction == "left_top")
            Move(y_left_top, x_left_top, map);
        else if(it.direction == "left_bottom")
            Move(y_left_bottom, x_left_bottom, map);
        else if(it.direction == "right_top")
            Move(y_right_top, x_right_top, map);
        else if(it.direction == "right_bottom")
            Move(y_right_bottom, x_right_top, map);

        this->Display(7); 
        old_position = map.at(y_loc).at(x_loc);   
    }
}

/**
 * method to get Inky into his corner, that is bottom left corner
 */
void CInky::Scatter(vector<string> & map){
    if(current_mode != 2 && current_mode != 0)
        this->Turn();
    current_mode = 2;
    // coord of bottom left corner, which is Inky's target in scatter mode
    double y_corner = 29, x_corner = 1;
    Move(y_corner, x_corner, map);

    this->Display(7);   

    old_position = map.at(y_loc).at(x_loc);  
}