#include "CGame.hpp"

/**
 * simple constructor, not much to say here 
 */
CGame::CGame(int m_id, int dif)
    : map_id(m_id), difficulty(dif), points(0), rage(1), score(0),
        level(0), lives(3), status(0)
{}

void CGame::Cannot_Load(){
    WINDOW * menu = newwin(12, 31, 20, 10);
    wattron(menu, COLOR_PAIR(1));
    box(menu, 0, 0);
    mvwprintw(menu, 3, 2, "Cannot load data from files.");
    mvwprintw(menu, 5, 4, "If you are lost, contact");
    mvwprintw(menu, 6, 9, "David Mikulka.");
    mvwprintw(menu, 9, 3, "Press any key  to continue");
    wrefresh(menu);
    getch();
    delwin(menu);
    endwin();
    exit(1);
}

/**
 * method to load settings according to user's choice in main menu
 */
void CGame::Load_Settings(vector<int> & times){
    ifstream difficulty_file;
    if(difficulty == 1)
        difficulty_file.open("./examples/mode_1.txt");
    else if(difficulty == 2)
        difficulty_file.open("./examples/mode_2.txt");
    else if(difficulty == 3)
        difficulty_file.open("./examples/mode_3.txt");
    
    if(!difficulty_file.is_open()){
        Cannot_Load();
    }

    string line;
    while(getline(difficulty_file, line)){
        if(line.find("=") == string::npos)
            Cannot_Load();
        char space = line.find("=");
        string tmp = line.substr(0, space);
        string value = line.substr(space+1);
        try { times.push_back(stoi(value));
        } catch (...) {
            Cannot_Load();
        }
    }
    difficulty_file.close();
}

/**
 * method to load map according to user's choice in main menu
 */
void CGame::Load_Map(){
    ifstream map_file;
    if(map_id == 1)
        map_file.open("./examples/map_1.txt");
    else if(map_id ==2)
        map_file.open("./examples/map_2.txt");
    else if(map_id == 3)
        map_file.open("./examples/map_3.txt");

    if(!map_file.is_open()){
        Cannot_Load();
    }

    string line;
    map.clear();
    while(getline(map_file, line)){
        map.push_back(line);
    }
    map_y = map.size(); 
    if(map_y <= 0)
        Cannot_Load();
    map_x = map.at(0).size() - 1;
    map_file.close();
    if(map_x < 28 || map_y < 31)
        Cannot_Load();
}

/**
 * method to print map at the beginning of each level
 */
void CGame::Print_Map(WINDOW * playwin){
    points = 0;
    for(unsigned i = 0; i < map.size(); i++){
        string tmp = map.at(i).data();
        for(unsigned j = 0; j < tmp.size(); j++){
            if(tmp.at(j) == '#'){
                wattron(playwin, COLOR_PAIR(2));
                mvwprintw(playwin, i, j, &tmp.at(j));
            } else if(tmp.at(j) == '.' || tmp.at(j) == 'O'){
                wattron(playwin, COLOR_PAIR(1));
                mvwprintw(playwin, i, j, &tmp.at(j));
            } else 
                mvwprintw(playwin, i, j, &tmp.at(j));
            if(tmp.at(j) == '.')
                points++;
        }
    }
}

/**
 * method to generate cherry at certain point twice every round or less, 
 * depends on game mode
 */
void CGame::Generate_Cherry(WINDOW * playwin, vector<string> & map_tmp){
    int y_pos = 17;
    int x_pos = 13;
    map_tmp.at(y_pos).at(x_pos) = '*';
    wattron(playwin, COLOR_PAIR(5));
    mvwaddch(playwin, y_pos, x_pos, '*');
    wrefresh(playwin);
}

/**
 * method to check collisions in case player eats ghost in frightened mode, 
 * or player is eaten in not frightened mode 
 */
int CGame::Check_Collision(CPlayer * player, vector<CGhost*> ghosts, int & score){
    int y_player, x_player;
    int y_blinky, x_blinky;
    int y_pinky, x_pinky;
    int y_inky, x_inky;
    int y_clyde, x_clyde;
    player->Get_Location(y_player, x_player);
    ghosts.at(0)->Get_Location(y_blinky, x_blinky);
    ghosts.at(1)->Get_Location(y_pinky, x_pinky);
    ghosts.at(2)->Get_Location(y_inky, x_inky);
    ghosts.at(3)->Get_Location(y_clyde, x_clyde);


    // check if eaten ghost reached ghost house
    if(ghosts.at(0)->Get_Eaten() && y_blinky == 11 && x_blinky == 13)
        ghosts.at(0)->Set_Eaten(0);
    if(ghosts.at(1)->Get_Eaten() && y_pinky == 11 && x_pinky == 13)
        ghosts.at(1)->Set_Eaten(0);
    if(ghosts.at(2)->Get_Eaten() && y_inky == 11 && x_inky == 13)
        ghosts.at(2)->Set_Eaten(0);
    if(ghosts.at(3)->Get_Eaten() && y_clyde == 11 && x_clyde == 13)
        ghosts.at(3)->Set_Eaten(0);

    // if ghost touches pacman or pacman ghost, player loses life
    if(!frightened){ 
        if( (y_player == y_blinky && x_player == x_blinky) ||
            (y_player == y_pinky && x_player == x_pinky) ||
            (y_player == y_inky && x_player == x_inky) ||
            (y_player == y_clyde && x_player == x_clyde) )
            return 1;
    } else { // if game is in frightened mode and pacman eats ghost that has
             // not been eaten yet
        if(y_player == y_blinky && x_player == x_blinky && !ghosts.at(0)->Get_Eaten()){
            ghosts.at(0)->Set_Eaten(1);
            score += rage*200;
            rage *= 2;
        } else if(y_player == y_pinky && x_player == x_pinky && !ghosts.at(1)->Get_Eaten()){
            ghosts.at(1)->Set_Eaten(1);
            score += rage*200;
            rage *= 2;
        } else if(y_player == y_inky && x_player == x_inky && !ghosts.at(2)->Get_Eaten()){
            ghosts.at(2)->Set_Eaten(1);
            score += rage*200;
            rage *= 2;
        } else if(y_player == y_clyde && x_player == x_clyde && !ghosts.at(3)->Get_Eaten()){
            ghosts.at(3)->Set_Eaten(1);
            score += rage*200;
            rage *= 2;
        }
    }
    return 0;
}

/**
 * change the game mode to frightened
 */
void CGame::Frightened(vector<CGhost*> ghosts, vector<string> & map_tmp){
    ghosts.at(0)->Frightened(map_tmp);
    if(time > 10 || difficulty == 3)
        ghosts.at(1)->Frightened(map_tmp);
    if(time > 25 || difficulty == 3)
        ghosts.at(2)->Frightened(map_tmp);
    if(time > 100 || difficulty == 3)
        ghosts.at(3)->Frightened(map_tmp);
    if(time_frightened == 30){
        time_frightened = 0;
        frightened = 0;
        rage = 1;
    }
    time_frightened++;
}

/**
 * change the game mode to scatter
 */
void CGame::Scatter(vector<CGhost*> ghosts, vector<string> & map_tmp){
    ghosts.at(0)->Scatter(map_tmp);
    if(time > 10 || difficulty == 3)
        ghosts.at(1)->Scatter(map_tmp);
    if(time > 25 || difficulty == 3)
        ghosts.at(2)->Scatter(map_tmp);
    if(time > 100 || difficulty == 3)
        ghosts.at(3)->Scatter(map_tmp);
}

/**
 * change the game mode to chase
 */
void CGame::Chase(CPlayer * player, vector<CGhost*> ghosts, vector<string> & map_tmp){
    ghosts.at(0)->Chase(player, map_tmp);
    if(time > 10 || difficulty == 3)
        ghosts.at(1)->Chase(player, map_tmp);
    if(time > 25 || difficulty == 3)
        ghosts.at(2)->Chase(player, map_tmp);
    if(time > 100 || difficulty == 3)
        ghosts.at(3)->Chase(player, map_tmp);
}

/**
 * level ended or player have been killed so we restart positions
 */
void CGame::To_Start(CPlayer * player, vector<CGhost*> ghosts, int y, int x){
    player->To_Start();
    for(unsigned i = 0; i < ghosts.size(); i++)
        ghosts.at(i)->To_Start(y, x);
}

/**
 * method to show score after the game ends
 */
void CGame::Show_Score(int final_score){
    WINDOW * scoreboard = newwin(12, 31, 20, 10);
    wattron(scoreboard, COLOR_PAIR(1));
    box(scoreboard, 0, 0);
    if(status == 1)
        mvwprintw(scoreboard, 3, 10, "GAME ENDED");
    else
        mvwprintw(scoreboard, 3, 11, "YOU DIED");
    mvwprintw(scoreboard, 5, 7, "Your score is %d.", final_score);
    mvwprintw(scoreboard, 9, 4, "Press enter to continue");
    wrefresh(scoreboard);
    // nocbreak();
    // cbreak();
    int choice;
    while((choice = getch()))
        if(choice == 10)
            break;
    delwin(scoreboard);
}

/**
 * method to call player's move and different types of moves for ghost
 */
int CGame::Play_Level(CPlayer * player, vector<CGhost*> ghosts, vector<string> & map_tmp,
                       vector<int> times, int & final_score, WINDOW * scorewin,
                       WINDOW * playwin){
    final_score = score;
    // get input from user
    int ch = wgetch(playwin);
    if(ch == 'x'){
        status = 1;
        return 1;
    }
    // move player
    status = player->Move_Player(player, score, map_tmp, frightened, level, ch);
    // level ends
    if(status == 3)
        return 1;

    // update windows
    wrefresh(playwin);
    mvwprintw(scorewin, 0, 24, "%d", score);
    mvwprintw(scorewin, 0, 0, "   ");
    for(int i = 0; i < lives; i++)
        mvwprintw(scorewin, 0, i, "O");
    wrefresh(scorewin);

    // check collision after player moves
    if(Check_Collision(player, ghosts, score)){
        time = 0;
        lives--;
        if(lives == 0)
            return 1;
        To_Start(player, ghosts, 11, 13);
    }

    // each round consists of repeating scatter and chase mode for
    // certain time, once all phases have ended, the game will be 
    // in chase mode till the end of the round or death of pacman
    if(frightened)
        Frightened(ghosts, map_tmp);
    else if(time <= times.at(0))
        Scatter(ghosts, map_tmp);
    else if(time > times.at(0) && time <= times.at(1)) 
        Chase(player, ghosts, map_tmp);
    else if(time > times.at(1) && time <= times.at(2)) 
        Scatter(ghosts, map_tmp);
    else if(time > times.at(2) && time <= times.at(3)) 
        Chase(player, ghosts, map_tmp);
    else if(time > times.at(3) && time <= times.at(4)) 
        Scatter(ghosts, map_tmp);
    else if(time > times.at(4) && time <= times.at(5)) 
        Chase(player, ghosts, map_tmp);
    else if(time > times.at(6) && time <= times.at(6)) 
        Scatter(ghosts, map_tmp);
    else if(time > times.at(6)) 
        Chase(player, ghosts, map_tmp);

    // check collision after ghosts move
    if(Check_Collision(player, ghosts, score)){
        time = 0;
        lives--;
        if(lives == 0)
            return 1;
        To_Start(player, ghosts, 11, 13);
    }
            
    // random bonus fruit generator
    if(score == times.at(7) || score == times.at(8))
        Generate_Cherry(playwin, map_tmp);

    // time updates only in non-frightened mode
    if(!frightened)
        time++;

    return 0;
}

/**
 * when we reach certain level, some time changes for certain modes must be made
 */
void CGame::Change_Times(vector<int> & times){
    if(level == 2){
        times.at(5) += 5065;
        times.at(6) = times.at(5) + 1;
    } else if(level == 5){
        times.at(0) -= 10; 
        times.at(1) -= 10;
        times.at(2) -= 20;
        times.at(3) -= 20;
    }
}

/**
 * method that starts the game, manages levels, calls all other important 
 * methods and finally shows score
 */
void CGame::Start_Game(){
    // load time settings for game modes
    vector<int> times;
    Load_Settings(times);

    // load map from file and create a window for it
    Load_Map();
    WINDOW * playwin = newwin(map_y, map_x, 10, 10);
    wattron(playwin, COLOR_PAIR(2));
    box(playwin, 32, 32 );
    wrefresh(playwin);

    // create score window
    WINDOW * scorewin = newwin(map_y+1, map_x+3, 9, 11);
    wattron(scorewin, COLOR_PAIR(3));
    box(scorewin, 0, 0 );
    wattron(scorewin, COLOR_PAIR(1));
    mvwprintw(scorewin, 0, 24, "0");
    wrefresh(scorewin);

    // let the game not wait for user input
    nodelay(playwin, TRUE);

    // cycle for levels  
    int final_score = 0;
    while(1){
        // print map
        Print_Map(playwin);
        vector<string> map_tmp = map;

        level++;
        CPlayer * player = new CPlayer(playwin, 23, 13, level, points);
        
        vector<CGhost*> ghosts;
        CBlinky * blinky = new CBlinky(playwin, 11, 13);
        ghosts.push_back(blinky);
        CPinky * pinky = new CPinky(playwin, 11, 13);
        ghosts.push_back(pinky);
        CInky * inky = new CInky(playwin, 11, 13);
        ghosts.push_back(inky);
        CClyde * clyde = new CClyde(playwin, 11, 13);
        ghosts.push_back(clyde);

        if((level == 2 || level == 5) && difficulty != 3)
            Change_Times(times);

        // play the level
        time = 0, time_frightened = 0, frightened = 0;
        mvwprintw(scorewin, 0, 10, "LEVEL %d", level);
        while(1){
            if(Play_Level(player, ghosts, map_tmp, times,
                       final_score, scorewin, playwin))
                break;
            
            // make sure the buffer is empty 
            flushinp();

            // wait certain time between moves, this depends on difficulty
            if(difficulty == 1)
                this_thread::sleep_for(std::chrono::milliseconds(210));
            else if(difficulty == 2)
                this_thread::sleep_for(std::chrono::milliseconds(180));
            else
                this_thread::sleep_for(std::chrono::milliseconds(150));
        }
        delete blinky;
        delete pinky;
        delete inky;
        delete clyde;
        delete player;
        if(lives == 0 || status == 1)
            break;
    }   

    // show final score
    Show_Score(final_score);

    // clear game and score window
    wclear(scorewin);
    wrefresh(scorewin);
    delwin(scorewin);
    wclear(playwin);
    wrefresh(playwin);
    delwin(playwin);
}