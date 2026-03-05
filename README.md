# Two Player Cooperative Adventure

## 🎮 Game Description
This is a console-based Text Adventure World where two players cooperatively navigate through interconnected rooms. Players must solve puzzles, collect items (keys, torches, bombs), and interact with elements like springs and doors to progress. The game supports loading maps from text files and includes a custom physics and lighting system.

### Team Member
- **Etay De Beer** (319041612)

## 🎯 Game Goal
The players must work together to navigate through the rooms and reach the "Last Room" defined in the game files. The game ends successfully only when both players are present in the final room.

## ⌨️ Instructions & Controls
The game is played with two players on the same keyboard. Input is **case-insensitive**.

| Action          | Player 1 | Player 2 |
|-----------------|----------|----------|
| **UP**          | `W`      | `I`      |
| **LEFT**        | `A`      | `J`      |
| **DOWN**        | `X`      | `M`      |
| **RIGHT**       | `D`      | `L`      |
| **STAY**        | `S`      | `K`      |
| **DISPOSE ITEM**| `E`      | `O`      |

> **Note:** Press `ESC` to pause the game. Press it again to resume, or `H` to exit to the Main Menu.

## 🚀 Game Modes
The game can be run in different modes using command-line arguments:

### 1. Normal Mode
Standard gameplay. The user plays the game manually using the keyboard.
```bash
./game
```

### 2. Save Mode
Identical to Normal Mode, but records all player actions and key game events (like moving, riddle answers, etc.) to files.
- Saves actions to: `adv-world.steps.txt`
- Saves events to: `adv-world.result.txt`
```bash
./game -save
```

### 3. Load Mode
Replays a recorded game session. The game reads actions from the provided steps file instead of the keyboard. This allows you to watch a previously played game automatically.
```bash
./game adv-world.steps.txt
```

### 4. Silent Mode
Runs the simulation as fast as possible without rendering any graphics to the console. This is used for automated testing.
- It compares the events occurring in the simulation against the expected events in `adv-world.result.txt`.
- Prints "Test passed" or "Test not passed" based on the comparison.
```bash
./game adv-world.steps.txt -silent
```

## 📁 File Formats
The game uses specific text formats for recording and verifying sessions.

- **Steps File (`*.steps.txt`)**: Records every action taken by each player at every game cycle. This file is used to replay the game in Load Mode.
  - *Example*: `CYCLE: 15 PLAYER: 1 ACTION: MOVE_RIGHT`
- **Results File (`*.result.txt`)**: Records significant game milestones such as changing rooms, solving riddles, or losing lives. This file is used in Silent Mode to verify that the replay matches the original session.
  - *Example*: `SCREEN_CHANGE CYCLE: 0 ROOM: 0`

## 🛠️ Map Creation Instructions
To add new levels to the game, create a text file named `adv-worldXX.screen` (where XX is a number, e.g., 01, 02). Files are loaded in lexicographical order.

### The Visual Map
- Field layout: Exactly `80` characters wide and `25` rows high.
- Include the character `L` (Legend Anchor) in the top-left area. This marks where the HUD (Score/Lives) will be drawn.

**Map Elements:**
- `W` = Wall
- `#` = Spring
- `@` = Bomb
- `K` = Key
- `?` = Riddle
- `1` - `9` = Door IDs
- `$` / `&` = Players (Optional, strictly controlled by SPAWN metadata)
- `(Space)` = Empty floor

### The Metadata
Immediately following the 25th row of the map, include the metadata section exactly as below to dictate the logic:

```text
---METADATA---
SPAWN X Y
SPAWN_PREV X Y
NEXT_ROOM <next_room_id>
PREV_ROOM <prev_room_id>
DOOR <id> <keys_needed> <switches_needed>
DARK_ZONE <tl_x> <tl_y> <br_x> <br_y>
```

#### Metadata Dictionary
- `SPAWN X Y`: Coordinates (Column, Row) where players appear when entering from the Previous Room.
- `SPAWN_PREV X Y`: Coordinates where players appear when returning to this room from the Next Room.
- `NEXT_ROOM`: The ID number of the next room file. Use `-1` if this is the Final Room.
- `PREV_ROOM`: The ID number of the previous room file. Use `-1` if this is the First Room.
- `DOOR ID KEYS SWITCHES`:
  - ID: The digit (1-9) corresponding to the door character.
  - KEYS: The number of keys required.
  - SWITCHES: The number of active switches required.
- `DARK_ZONE TL_X TL_Y BR_X BR_Y`: Defines a rectangular area hidden unless a player has a Torch. (Multiple lines permitted).

## 🗂️ Additional Files
- `riddles.txt`: Must be present in the working directory. Contains the riddles and answers corresponding to `?` characters on the map.
- `adv-world.steps.txt` / `adv-world.result.txt`: Used for automated testing and replaying features.

> **Note:** There's a folder called "Different Steps And Results" containing 3 different gameplay recordings. You can use them to test the game in Load/Silent Mode by placing their steps and results files in the same directory as the game executable.
