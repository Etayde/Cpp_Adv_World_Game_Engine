#include "NormalGame.h"
#include "Console.h"
#include "Constants.h"

//////////////////////////////////////////     NormalGame Constructor     /////////////////////////////////////////////

NormalGame::NormalGame() : Game() {}

//////////////////////////////////////////        handleInput         /////////////////////////////////////////////

void NormalGame::handleInput()
{

  while (check_kbhit())
  {
    int pressed = get_char_nonblocking();

    if (pressed == -1)
      break;

    // Detect and ignore special key sequences (arrow keys, function keys, etc.)
    // On Windows, special keys send 0 or 224 followed by a key code (apperently
    // :) )
    if (pressed == 0 || pressed == 224)
    {
      if (check_kbhit())
      {
        get_char_nonblocking();
      }
      continue;
    }

    // Debugging shortcuts
    if (pressed == 'v') {
      currentState = GameState::victory;
      return;
    }

    if (pressed == 'g') {
      currentState = GameState::gameOver;
      return;
    }

    for (int i = 0; i < NUM_KEY_BINDINGS; i++)
    {
      if (keyBindings[i].key == pressed)
      {
        if (keyBindings[i].action == Action::ESC)
        {
          currentState = GameState::paused;
          return;
        }
        Player &player = (keyBindings[i].playerID == 1) ? player1 : player2;

        player.performAction(keyBindings[i].action, getCurrentRoom());
        break;
      }
    }
  }
}
