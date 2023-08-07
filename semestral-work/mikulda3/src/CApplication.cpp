#include "CApplication.hpp"

/**
 * constructor for our game, by default the difficulty is set to normal
 * and game will be played on basic map
 */
CApplication::CApplication()
    : map(1), difficulty(2), choice(0), selected(0), y_max(0),
        x_max(0), first_time(1){
    initscr();
    noecho();
    cbreak();
    curs_set(FALSE);
}

/**
 * method to check if user's terminal supports colors, if not, he cannot play
 */
void CApplication::Check_Colors(){
    if(!has_colors()){
        printw("Terminal does not support colors, get one that does :)");
        getch();
        endwin();
        exit(1);
    }  
}

/**
 * method to initialize color pairs that will be used during gameplay
 */
void CApplication::Init_Colors(){
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK); // menu, points and player
    init_pair(2, COLOR_BLUE, COLOR_BLACK); // map
    init_pair(3, COLOR_BLACK, COLOR_BLACK); // score background
    init_color(COLOR_GREEN, 0, 800, 0); // ghosts in frightened mode
    init_pair(4, COLOR_YELLOW, COLOR_BLACK); // ghosts in frightened mode
    init_pair(5, COLOR_RED, COLOR_BLACK); // blinky
    init_color(COLOR_MAGENTA, 999, 200, 200); // pinky
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK); // pinky
    init_pair(7, COLOR_CYAN, COLOR_BLACK); // inky
    init_pair(8, COLOR_WHITE, COLOR_BLACK); // clyde 

}

/**
 * method that racts to user's input and updates menu
 */
void CApplication::Update_Menu(WINDOW * menu, string options[], int options_pos[]){
    for(int i = 0; i < 3; i++){
        // highlight current position
        if(i == selected)
            wattron(menu, A_REVERSE);
        mvwprintw(menu, 1+i+(i+1)*2, options_pos[i], options[i].c_str());
        wattroff(menu, A_REVERSE);
    }
    choice = wgetch(menu);
    // move to next position
    switch(choice){
        case KEY_UP:
            selected > 0 ? selected-- : selected = 0;         
            break;
        case KEY_DOWN:
            selected < 2 ? selected ++ : selected = 2;
            break;
        default:
            break;
    }
}

/**
 * when "START GAME" in menu selected, game shall start 
 */
void CApplication::Start_Game(){
    CGame game(map, difficulty);  
    game.Start_Game();
}

/**
 * player can choose from 3 maps - basic, no teleports and 4 teleports
 */
void CApplication::Select_Map(WINDOW * menu){
    mvwprintw(menu, 1, 3, "---- * Select  Map * ----");

    string options[3] = {"BASIC", "NO  TELEPORTS", "IS THIS A MAP ?"};
    int options_pos[3] = {13, 9, 8};

    wrefresh(menu);
    while(1){
        Update_Menu(menu, options, options_pos);
        if(choice == 10)
            break;
    }

    if(selected == 0)
        map = 1;
    else if(selected == 1)
        map = 2;
    else if(selected == 2)
        map = 3;

    mvwprintw(menu, 9, 5, "                      ");
    mvwprintw(menu, 6, 5, "                      ");
    wrefresh(menu);
}

/**
 * player can select between 3 levels of difficulty
 */
void CApplication::Select_Difficulty(WINDOW * menu){
    mvwprintw(menu, 1, 3, "- * Select  Difficulty * -");

    string options[3] = {"EASY CLAP", "NORMAL", "WANNA DIE ?"};
    int options_pos[3] = {11, 12, 10};

    wrefresh(menu);
    while(1){
        Update_Menu(menu, options, options_pos);
        if(choice == 10)
            break;
    }

    if(selected == 0)
        difficulty = 1;
    else if(selected == 1)
        difficulty = 2;
    else if(selected == 2)
        difficulty = 3;

    mvwprintw(menu, 6, 5, "                      ");
    mvwprintw(menu, 9, 5, "                      ");
    wrefresh(menu);
}

/**
 * in this method user can define the game settings
 */
void CApplication::Settings(WINDOW * menu){
    mvwprintw(menu, 1, 3, "----- * Settings * -----");

    string options[3] = {"DIFFICULTY", "MAPS", "BACK TO MAIN MENU"};
    int options_pos[3] = {10, 13, 7};

    wrefresh(menu);
    while(1){
        Update_Menu(menu, options, options_pos);
        // if user pressed enter, break and select that choice
        if(choice == 10)
            break;
    }
    mvwprintw(menu, 3, 5, "                      ");
    mvwprintw(menu, 6, 5, "                      ");
    mvwprintw(menu, 9, 5, "                      ");
    if(selected == 0){
        Select_Difficulty(menu);
        selected = 0;
        Settings(menu);
    } else if(selected == 1){
        selected = 0;
        Select_Map(menu);
        mvwprintw(menu, 6, 5, "                      ");
        selected = 1;
        Settings(menu);
    }
    selected = 1;
    wrefresh(menu);
}

/**
 * simple method that clears the screen and shuts down the application
 */
void CApplication::Quit(WINDOW * menu){
    nocbreak();
    cbreak();
    werase(menu);
    box(menu, 0, 0);
    mvwprintw(menu, 5, 6, "Thanks for playing!");
    mvwprintw(menu, 9, 3, "Press any key  to continue");
    wrefresh(menu);
    getch();
    clear();
}

/**
 * method called only before the first game starts to show player some basics
*/
void CApplication::Show_Instructions(WINDOW * menu){
    mvwprintw(menu, 1, 3, "---- * Instructions * ----");
    mvwprintw(menu, 3, 2, "Your goal is  to survive as");
    mvwprintw(menu, 4, 3, "many levels as possible.");
    mvwprintw(menu, 5, 3, "To complete a level, pick");
    mvwprintw(menu, 6, 2, "all little dots on the map.");   
    mvwprintw(menu, 9, 4, "Press enter to continue");
    wrefresh(menu);
    while(getch() != 10){}
    mvwprintw(menu, 3, 2, "Careful, do not get caught");
    mvwprintw(menu, 4, 2, "by ghosts. You got 3 lives.");
    mvwprintw(menu, 5, 1, "You can move with arrow keys.");
    mvwprintw(menu, 6, 1, "If you wish to  quit the game");
    mvwprintw(menu, 7, 5, "at any time, press x.");
    wrefresh(menu);
    while(getch() != 10){}
    first_time = 0;
}

/**
 * basic method to show main menu, user can start the game, adjust the 
 * settings or quit the game which will end the whole application
 */
void CApplication::Show_Menu(){
    // initialize colors
    Init_Colors();

    // initialize window
    getmaxyx(stdscr, y_max, x_max);
    WINDOW * menu = newwin(12, 31, 20, 10);
    wattron(menu, COLOR_PAIR(1));
    box(menu, 0, 0);
    refresh();
    
    // initialize arrow keys
    keypad(menu, true);

    string options[3] = {"START GAME", "SETTINGS", "QUIT  GAME"};
    int options_pos[3] = {10, 11, 10};

    // the app is running till user selects "QUIT GAME" button
    while(1){
        box(menu, 0, 0);
        refresh();
        mvwprintw(menu, 1, 3, "------ * C++ Man * ------");
        // cycle through menu till user selects something
        while(1){
            Update_Menu(menu, options, options_pos);
            // if user pressed enter, break and select that choice
            if(choice == 10)
                break;
        }
        if(selected == 0){
            if(first_time){
                Show_Instructions(menu);
            }
            werase(menu);
            wrefresh(menu);
            Start_Game();
        } else if(selected == 1){
            mvwprintw(menu, 6, 5, "                      ");
            wrefresh(menu);
            selected = 0;
            Settings(menu);
            mvwprintw(menu, 9, 5, "                      ");
        } else if(selected == 2){
            Quit(menu);
            break;
        }
    }
    endwin();
}