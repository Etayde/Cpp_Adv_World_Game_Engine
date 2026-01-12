#pragma once

#include "Game.h"

class LoadedGame : public Game {

    public:
        LoadedGame();
        void handleInput() override;
};