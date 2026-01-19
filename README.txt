EXCERCISE 3 - README - READJUSTED WITH AI:

---------------------------------
Students: ETAY DE BEER 319041612
          SHLOMI LEVI  332091636
---------------------------------

=== Game Description ===
This is a console-based Text Adventure World where two players cooperatively navigate through interconnected rooms. Players must solve puzzles, collect items (keys, torches, bombs), and interact with elements like springs and doors to progress. The game supports loading maps from text files and includes a custom physics and lighting system.

=== Game Goal ===
The players must work together to navigate through the rooms and reach the "Last Room" defined in the game files. The game ends successfully only when both players are present in the final room.

=== Instructions & Controls ===
The game is played with two players on the same keyboard. Input is case-insensitive.

| Action          | Player 1 | Player 2 |
|-----------------|----------|----------|
| UP              | W        | I        |
| LEFT            | A        | J        |
| DOWN            | X        | M        |
| RIGHT           | D        | L        |
| STAY            | S        | K        |
| DISPOSE ITEM    | E        | O        |

* ESC: Pause Game (Press ESC again to resume, or 'H' to exit to Main Menu).

=== Game Modes ===
The game can be run in different modes using command-line arguments:

1. Normal Mode
   Command: ./game
   Description: Standard gameplay. The user plays the game manually using the keyboard.

2. Save Mode
   Command: ./game -save
   Description: Identical to Normal Mode, but records all player actions and key game events (like moving, riddle answers, etc.) to files.
   - Saves actions to: "adv-world.steps.txt"
   - Saves events to: "adv-world.result.txt"

3. Load Mode
   Command: ./game <steps script> 
   (e.g., ./game adv-world.steps.txt)
   Description: Replays a recorded game session. The game reads actions from the provided steps file instead of the keyboard. This allows you to watch a previously played game automatically.

4. Silent Mode
   Command: ./game <steps script> -silent
   (e.g., ./game adv-world.steps.txt -silent)
   Description: Runs the simulation as fast as possible without rendering any graphics to the console. This is used for automated testing.
   - It compares the events occurring in the simulation against the expected events in "adv-world.result.txt".
   - Prints "Test passed" or "Test not passed" based on the comparison.

=== File Formats ===
The game uses specific text formats for recording and verifying sessions.

1. Steps File (*.steps.txt)
   Records every action taken by each player at every game cycle. This file is used to replay the game in Load Mode.
   Format Example:
   CYCLE: 15 PLAYER: 1 ACTION: MOVE_RIGHT
   CYCLE: 200 PLAYER: 1 ACTION: ANSWER_RIDDLE ANSWER: 4

2. Results File (*.result.txt)
   Records significant game milestones such as changing rooms, solving riddles, or losing lives. This file is used in Silent Mode to verify that the replay matches the original session.
   Format Example:
   SCREEN_CHANGE CYCLE: 0 ROOM: 0
   RIDDLE CYCLE: 600 ROOM: 2 QUESTION: "What walks on four legs?" ANSWER: 2 CORRECT: YES

=== Map Creation Instructions ===
To add new levels to the game, create a text file named "adv-worldXX.screen" (where XX is a number, e.g., 01, 02). Files are loaded in lexicographical order.

1. THE VISUAL MAP
   - The file must begin with the visual representation of the room.
   - The map must be exactly 80 characters wide and 25 rows high.
   - You must include the character 'L' (Legend Anchor) in the top-left area. This marks where the HUD (Score/Lives) will be drawn.
   - Use the following characters for map elements:
     W = Wall
     # = Spring
     @ = Bomb
     K = Key
     ? = Riddle
     1-9 = Door IDs
     $ / & = Players (Optional, strictly controlled by SPAWN metadata)
     (Space) = Empty floor

2. THE METADATA
   - Immediately following the 25th row of the map, you must include the metadata section.
   - This section dictates the logic of the room (entrances, exits, doors, lighting).
   - The headers and format must be exactly as follows:

---METADATA---
SPAWN X Y
SPAWN_PREV X Y
NEXT_ROOM <next_room_id>
PREV_ROOM <prev_room_id>
DOOR <id> <keys_needed> <switches_needed>
DARK_ZONE <tl_x> <tl_y> <br_x> <br_y>

=== Metadata Dictionary ===
* SPAWN X Y: 
  The coordinates (Column, Row) where players appear when entering from the Previous Room (or game start).
  
* SPAWN_PREV X Y: 
  The coordinates where players appear when returning to this room from the Next Room (backtracking).

* NEXT_ROOM: 
  The ID number of the next room file. Use -1 if this is the Final Room.

* PREV_ROOM: 
  The ID number of the previous room file. Use -1 if this is the First Room.

* DOOR ID KEYS SWITCHES:
  - ID: The digit (1-9) corresponding to the door character on the map.
  - KEYS: The number of keys required to open this door.
  - SWITCHES: The number of switches that must be active to open this door.

* DARK_ZONE TL_X TL_Y BR_X BR_Y:
  Defines a rectangular area that is visually hidden unless a player has a Torch.
  - TL_X / TL_Y: Top-Left X and Y coordinates.
  - BR_X / BR_Y: Bottom-Right X and Y coordinates.
  (You may include multiple DARK_ZONE lines for complex lighting).

=== Additional Files ===
* riddles.txt: 
  Must be present in the working directory. Contains the riddles and answers corresponding to '?' characters on the map.

* adv-world.steps.txt (or similar steps file):
  - Mandatory for Load Mode and Silent Mode: Used to drive the simulation.
  - Generated in Save Mode: The game creates this file to record actions.
  - Not needed for Normal Mode.

* adv-world.result.txt (or similar results file):
  - Mandatory for Silent Mode: Used to verify that the replay matches expected outcomes.
  - Generated in Save Mode: The game creates this file to record key events.
  - Not needed for Normal Mode or Load Mode (unless verifying).

** Note 1: There's a folder called "Different Steps And Results" containing 3 different recordings of steps and results files for the game to use.
           You can use them to test the game in Load/Silent Mode.
           To do so, choose a gameplay, and place its steps and results files in the same directory as the game executable.
