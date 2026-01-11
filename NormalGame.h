#pragma once

#include "Game.h"

class NormalGame : public Game
{
public:
    void handleInput() override;
    NormalGame();

};