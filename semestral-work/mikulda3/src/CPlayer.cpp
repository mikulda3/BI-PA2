#include "CPlayer.hpp"

/**
 * constructor to initialize player
 */
CPlayer::CPlayer(WINDOW * win, int y, int x, int lvl, int pts)
    : x_loc(x), y_loc(y), choice(0), last_choice(0), score(0), frightened(0),
      level(lvl), points(pts), last_position(0),
      direction(1), character('C'), curwin(win) {
    getmaxyx(curwin, y_max, x_max);
    keypad(curwin, true);
}

/**
 * simple method to get player's location in order to check collision for example
 */
void CPlayer::Get_Location(int & y, int & x) const {
    y = y_loc;
    x = x_loc;
}

/**
 * when the level ends or player dies, move him to starting position
 */
void CPlayer::To_Start(){
    mvwaddch(curwin, y_loc, x_loc, ' ');
    last_choice = 0;
    choice = 0;
    last_position = 0;
    y_loc = 23;
    x_loc = 13;
}

/**
 * methods to move player to desired position and add score if possible
 */
void CPlayer::Move_Up(vector<string> & map, int level){
    direction = 3;
    mvwaddch(curwin, y_loc, x_loc, ' ');
    if(map.at(y_loc-1).at(x_loc) == '*'){
        map.at(y_loc-1).at(x_loc) = ' ';
        score += 100*level;
    } else if(map.at(y_loc-1).at(x_loc) == '.'){
        map.at(y_loc-1).at(x_loc) = ' ';
        score += 10;
        points--;
    } else if(map.at(y_loc-1).at(x_loc) == 'O'){
        map.at(y_loc-1).at(x_loc) = ' ';
        frightened = 1;
        score += 50;
    }
    if(map.at(y_loc-1).at(x_loc) != '#' && map.at(y_loc-1).at(x_loc) != '-')
        y_loc--;
    else if(choice != last_choice){
        choice = last_choice;
        Get_Position(map, level);
    }
}

void CPlayer::Move_Down(vector<string> & map, int level){
    direction = 4;
    mvwaddch(curwin, y_loc, x_loc, ' ');
    if(map.at(y_loc+1).at(x_loc) == '*'){
        map.at(y_loc+1).at(x_loc) = ' ';
        score += 100*level;
    } else if(map.at(y_loc+1).at(x_loc) == '.'){
        map.at(y_loc+1).at(x_loc) = ' ';
        score += 10;
        points--;
    } else if(map.at(y_loc+1).at(x_loc) == 'O'){
        map.at(y_loc+1).at(x_loc) = ' ';
        frightened = 1;
        score += 50;
    }
    if(map.at(y_loc+1).at(x_loc) != '#' && map.at(y_loc+1).at(x_loc) != '-')
        y_loc++;
    // continue in direction
    else if(choice != last_choice){
        choice = last_choice;
        Get_Position(map, level);
    }

}

void CPlayer::Move_Left(vector<string> & map, int level){  
    direction = 1;
    mvwaddch(curwin, y_loc, x_loc, ' ');
    if(map.at(y_loc).at(x_loc-1) == '*'){
        map.at(y_loc).at(x_loc-1) = ' ';
        score += 100*level;
    } else if(map.at(y_loc).at(x_loc-1) == '.'){
        map.at(y_loc).at(x_loc-1) = ' ';
        score += 10;
        points--;
    } else if(map.at(y_loc).at(x_loc-1) == 'O'){
        map.at(y_loc).at(x_loc-1) = ' ';
        frightened = 1;
        score += 50;
    }
    if(map.at(y_loc).at(x_loc-1) != '#' && map.at(y_loc).at(x_loc-1) != '-')
        x_loc--;
    else if(choice != last_choice){
        choice = last_choice;
        Get_Position(map, level);
    }
}

void CPlayer::Move_Right(vector<string> & map, int level){
    direction = 2;
    mvwaddch(curwin, y_loc, x_loc, ' ');
    if(map.at(y_loc).at(x_loc-1) == '*'){
        map.at(y_loc).at(x_loc-1) = ' ';
        score += 100*level;
    } else if(map.at(y_loc).at(x_loc+1) == '.'){
        map.at(y_loc).at(x_loc+1) = ' ';
        points--;
        score += 10;
    } else if(map.at(y_loc).at(x_loc+1) == 'O'){
        map.at(y_loc).at(x_loc+1) = ' ';
        frightened = 1;
        score += 50;
    }
    if(map.at(y_loc).at(x_loc+1) != '#' && map.at(y_loc).at(x_loc+1) != '-')
        x_loc++;
    else if(choice != last_choice){
        choice = last_choice;
        Get_Position(map, level);
    }
}

/**
 * method to either continue with previous direction or chose a new one
 */
int CPlayer::Get_Position(vector<string> & map, int level){
    switch(choice){
        case KEY_UP:
            Move_Up(map, level);
            break;
        case KEY_DOWN:
            Move_Down(map, level);
            break;
        case KEY_LEFT:
            Move_Left(map, level);
            break;
        case KEY_RIGHT:
            Move_Right(map, level);
            break;
        default:
            break;
    }
    return 0;
}

/**
 * move player to next position according to pressed arrow key
 */
int CPlayer::Get_Move(int ch, vector<string> & map, int level){
    choice = ch;
    Get_Position(map, level);
    return choice;
}

/**
 * display current position
 */
void CPlayer::Display(){
    last_choice = choice;
    wattron(curwin, COLOR_PAIR(1));
    // use teleport from left to right...
    if(x_loc == 0){
        x_loc = x_max-2;
        mvwaddch(curwin, y_loc, x_loc, character);
    // ...from right to left...
    } else if(x_loc == x_max){
        x_loc = 1;
        mvwaddch(curwin, y_loc, x_loc, character);
    // ...or just move
    } else 
        mvwaddch(curwin, y_loc, x_loc, character);
    wattroff(curwin, COLOR_PAIR(1));
}

/**
 * method to decide if whether the player can move or not or if he wants to
 * quit the game
 */
int CPlayer::Move_Player(CPlayer * player, int & out_score,
            vector<string> & map, int & out_frightened, int level, int ch){
    // level ends
    if(points == 0)
        return 3;

    // get info only when the mode ends
    if(!out_frightened)
        out_frightened = frightened;
    frightened = 0;

    // if player does not change direction, continue in previous direction or
    // player pressed ivalid key
    if(ch == ERR || (ch != KEY_UP && ch != KEY_DOWN && 
                     ch != KEY_LEFT && ch != KEY_RIGHT)){
        player->Get_Position(map, level);
        player->Display();
        out_score += score;
        score = 0;
        return 2;
    // player wants to change direction
    } else if(ch == KEY_UP || ch == KEY_DOWN || 
                ch == KEY_LEFT || ch == KEY_RIGHT){
        // or quit game     
        player->Get_Move(ch, map, level);
        player->Display();
        out_score += score;
        score = 0;
        last_position = ch;
        return 2;
    }
    return 0;
}