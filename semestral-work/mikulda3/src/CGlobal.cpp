#include "CGlobal.hpp"

/**
 * overloaded operator to choose according to priority of turns, which is
 * up, left, down, right
 */
bool operator < (const Path & a, const Path & b){
    if(a.distance == b.distance){
        if(a.direction == "up")
            return a.distance < b.distance;
        else if(b.direction == "up")
            return b.distance < a.distance;
        else if(a.direction == "left")
            return a.distance < b.distance;
        else if(b.direction == "left")
            return b.distance < a.distance;
        else if(a.direction == "down")
            return a.distance < b.distance;
        else if(b.direction == "down")
            return b.distance < a.distance;
        else if(a.direction == "right")
            return a.distance < b.distance;
        else
            return b.distance < a.distance;
    }
    return a.distance < b.distance;
}