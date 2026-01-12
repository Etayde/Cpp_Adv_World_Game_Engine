# Steps File Format Specification

## Valid Format

Each recorded action is stored on a single line with the following format:

```
CYCLE: <cycleNumber> | PLAYER: <playerId> | ACTION: <actionDescription>
```

### Format Rules

1. **One action per line** - Each line represents a single player action
2. **Field order is fixed** - Must be CYCLE, then PLAYER, then ACTION
3. **Field delimiters** - Fields are separated by ` | ` (space, pipe, space)
4. **Field format**:
   - `CYCLE:` followed by space, then unsigned long integer
   - `PLAYER:` followed by space, then integer (1 or 2)
   - `ACTION:` followed by space, then action name string

### Valid Action Names

- `MOVE_UP`
- `MOVE_DOWN`
- `MOVE_LEFT`
- `MOVE_RIGHT`
- `STAY`
- `DROP_ITEM`
- `ESC`

### Example Steps File

```
CYCLE: 0 | PLAYER: 1 | ACTION: MOVE_RIGHT
CYCLE: 1 | PLAYER: 1 | ACTION: MOVE_RIGHT
CYCLE: 1 | PLAYER: 2 | ACTION: STAY
CYCLE: 2 | PLAYER: 1 | ACTION: DROP_ITEM
CYCLE: 3 | PLAYER: 2 | ACTION: MOVE_UP
CYCLE: 3 | PLAYER: 1 | ACTION: MOVE_LEFT
CYCLE: 5 | PLAYER: 2 | ACTION: ESC
```

### Notes

- Multiple actions can occur on the same cycle (one line per action)
- Cycle numbers may have gaps (e.g., cycle 2, then cycle 5)
- File is created with `-save` flag: `./game -save`
- Default filename: `adv-world.steps`
