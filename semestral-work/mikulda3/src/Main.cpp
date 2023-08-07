#include "CApplication.hpp"

/**
 * Basic main function that firstly checks if the command line is able
 * to use colors, if so, game shall start, if not, game cannot be played
 */
int main() {
    CApplication app;
    app.Check_Colors();
    app.Show_Menu();
    return 0;
}