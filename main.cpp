//////////////////////////////////////       INCLUDES & FORWARDS       //////////////////////////////////////////

#include "Console.h"
#include "Game.h"
#include "DebugLog.h"
#include <iostream>

int main()
{
    // Initialize debug logging
    DebugLog::init("spring_debug.log");

    // Initialize console for raw input
    init_console();
    hideCursor();
    clrscr();

    // Create and run the game
    Game game;
    game.run();

    // Cleanup
    showCursor();
    clrscr();
    cleanup_console();

    // Close debug log
    DebugLog::close();

    std::cout << "Thanks for playing!" << std::endl;

    return 0;
}
