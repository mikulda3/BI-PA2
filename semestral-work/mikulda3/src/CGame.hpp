#pragma once

#include "CPlayer.hpp"
#include "CGlobal.hpp"
#include "CGhost.hpp"
#include "CBlinky.hpp"
#include "CInky.hpp"
#include "CPinky.hpp"
#include "CClyde.hpp"

#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>

/**
 * Second important class is CGame. This class runs the whole game, loads
 * desired settings and maps, prints maps and makes sure the rules are being
 * executed.
 */

class CGame {
    public:
        CGame(int m_id, int dif);
        void Start_Game();
        void Cannot_Load();
        void Load_Map();
        void Load_Settings(vector<int> & times);
        void Print_Map(WINDOW * playwin);
        void Generate_Cherry(WINDOW * playwin, vector<string> & map_tmp);
        int Check_Collision(CPlayer * player, vector<CGhost*> ghosts, int & score);
        void Frightened(vector<CGhost*> ghosts, vector<string> & map_tmp);
        void Scatter(vector<CGhost*> ghosts, vector<string> & map_tmp);
        void Chase(CPlayer * player, vector<CGhost*> ghosts, vector<string> & map_tmp);
        void To_Start(CPlayer * player, vector<CGhost*> ghosts, int y, int x);
        void Show_Score(int final_score);
        int Play_Level(CPlayer * player, vector<CGhost*> ghosts, vector<string> & map_tmp,
                        vector<int> times, int & final_score, WINDOW * scorewin,
                        WINDOW * playwin);
        void Change_Times(vector<int> & times);
    private:
        int map_id;
        int difficulty;
        int points;
        int rage;
        int map_y, map_x;
        vector<string> map;
        int time, time_frightened, frightened;
        int score;
        int level;
        int lives;
        int status; // status: 1 end game | 3 new level
};