#include "CGhost.hpp"
#include "CGlobal.hpp"

/**
 * getter which we need in order to recognize collisions
 */
void CGhost::Get_Location(int & y_pos, int & x_pos) const {
    y_pos = y_loc;
    x_pos = x_loc;
}

/**
 * returns ghost back to start position in case player was eaten or level ends
 */
void CGhost::To_Start(int y, int x){
    if(old_position == '*')
        wattron(curwin, COLOR_PAIR(5));
    else 
        wattron(curwin, COLOR_PAIR(1));
    mvwaddch(curwin, y_loc, x_loc, old_position);    
    old_position = ' ';
    eaten = 0;
    x_prev = 14;
    y_prev = 11;
    y_loc = y;
    x_loc = x;
}

/**
 * simple method to display new position of ghost
 */
void CGhost::Display(int color){
    wattron(curwin, COLOR_PAIR(color));
    mvwaddch(curwin, y_loc, x_loc, character);
    wattroff(curwin, COLOR_PAIR(color));
}

/**
 * method to get ghost to desired position
 */
void CGhost::Move(int y_pos, int x_pos, vector<string> & map){
    set<Path> path;
    Path tmp;

    // decide which direction is the best possible way to get to the position
    if(map.at(y_loc).at(x_loc-1) != '#' && map.at(y_loc).at(x_loc-1) != '-'
        && (x_prev != x_loc-1)){
        tmp.direction = "left";
        tmp.distance = sqrt(pow(y_loc-y_pos, 2) + pow(x_loc-1-x_pos, 2));
        path.insert(tmp);
    }
    if(map.at(y_loc).at(x_loc+1) != '#' && map.at(y_loc).at(x_loc+1) != '-' 
        && (x_prev != x_loc+1)){
        tmp.direction = "right";
        tmp.distance = sqrt(pow(y_loc-y_pos, 2) + pow(x_loc+1-x_pos, 2));
        path.insert(tmp);
    }
    if(map.at(y_loc-1).at(x_loc) != '#' && map.at(y_loc-1).at(x_loc) != '-' 
        && (y_prev != y_loc-1) 
        && !(y_loc == 23 && x_loc >= 11 && x_loc <= 16)
        && !(y_loc == 11 && x_loc >= 11 && x_loc <= 16) ){
        tmp.direction = "up";
        tmp.distance = sqrt(pow(y_loc-1-y_pos, 2) + pow(x_loc-x_pos, 2));
        path.insert(tmp);
    }
    if(map.at(y_loc+1).at(x_loc) != '#' && map.at(y_loc+1).at(x_loc) != '-' 
        && (y_prev != y_loc+1)){
        tmp.direction = "down";
        tmp.distance = sqrt(pow(y_loc+1-y_pos, 2) + pow(x_loc-x_pos, 2));
        path.insert(tmp);
    }

    // delete old position of ghost 
    if(old_position == '*')
        wattron(curwin, COLOR_PAIR(5));
    else 
        wattron(curwin, COLOR_PAIR(1));
    mvwaddch(curwin, y_loc, x_loc, old_position);

    // save previous position so he cannot go back to the previous tile
    x_prev = x_loc;
    y_prev = y_loc;

    // decide the right way if there are more than one shortest way
    auto it = *path.begin();

    // make move
    if(it.direction == "left")
        x_loc--;
    else if(it.direction == "right")
        x_loc++;
    else if(it.direction == "up")
        y_loc--;
    else if(it.direction == "down")
        y_loc++;
    
    // use teleport from left to right...
    if(x_loc == 0)
        x_loc = x_max-2;
    // ...from right to left
    else if(x_loc == x_max)
        x_loc = 1;
}

/**
 * in frightened, ghosts move completely randomly
 */
void CGhost::Frightened(vector<string> & map){
    if(current_mode != 1)
        Turn();
    current_mode = 1;
    if(this->Get_Eaten())
        this->Eaten(map);
    else {
        // vector to store possible ways
        vector<int> path;
        
        // decide which tile is available
        // 1 stays for left, 2 for right, 3 for up and 4 for down
        if(map.at(y_loc).at(x_loc-1) != '#' && map.at(y_loc).at(x_loc-1) != '-'
            && (x_prev != x_loc-1)){
            path.push_back(1);
        }
        if(map.at(y_loc).at(x_loc+1) != '#' && map.at(y_loc).at(x_loc+1) != '-' 
            && (x_prev != x_loc+1)){
            path.push_back(2);
        }
        if(map.at(y_loc-1).at(x_loc) != '#' && map.at(y_loc-1).at(x_loc) != '-' 
            && (y_prev != y_loc-1)){
            path.push_back(3);
        }
        if(map.at(y_loc+1).at(x_loc) != '#' && map.at(y_loc+1).at(x_loc) != '-' 
            && (y_prev != y_loc+1)){
            path.push_back(4);
        }

        // roll the dice 
        int random = rand() % path.size();

        // delete old position
        if(old_position == '*')
            wattron(curwin, COLOR_PAIR(5));
        else 
            wattron(curwin, COLOR_PAIR(1));
        mvwaddch(curwin, y_loc, x_loc, old_position);

        // save previous position so he cannot go back to the previous tile
        x_prev = x_loc;
        y_prev = y_loc;

        // make move
        if(path.at(random) == 1){
            x_loc--;
        } else if(path.at(random) == 2){
            x_loc++;
        } else if(path.at(random) == 3){
            y_loc--;
        } else if(path.at(random) == 4){
            y_loc++;
        }

        // use teleport from left to right...
        if(x_loc == 0)
            x_loc = x_max-2;
        // ...from right to left...
        else if(x_loc == x_max)
            x_loc = 1;

        // show new position and save its original content
        this->Display(4);
        old_position = map.at(y_loc).at(x_loc);  
    }
}

/**
 * if ghost is eaten by pacman, he goes back to ghost house
 */
void CGhost::Eaten(vector<string> & map){
    Move(11, 13, map);
    this->Display(2);       
    old_position = map.at(y_loc).at(x_loc); 
}

/**
 * when certain modes change, all ghost do 180° turn, e.g. change direction
 */
void CGhost::Turn(){
    x_prev = x_loc;
    y_prev = y_loc;
}