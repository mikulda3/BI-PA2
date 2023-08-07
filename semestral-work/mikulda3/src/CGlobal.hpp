#pragma once 

#include <string>

using namespace std;

/**
 * Class to save possible paths ghost can go. We overload operator < in
 * order to find path according to priority of paths, which is up, left,
 * down and right.
 */
class Path {
    public:
        friend bool operator < (const Path & a, const Path & b);
        double distance;
        string direction;
};