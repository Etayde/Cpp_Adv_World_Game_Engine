#pragma once

//////////////////////////////////////       INCLUDES & FORWARDS       //////////////////////////////////////////

#include "StaticObjects.h"
#include "Room.h"
#include "Screen.h"
#include <vector>

class Player;

////////////////////////////////////////        SpringCell         //////////////////////////////////////////
struct SpringCell
{
    Point pos;
    Direction projectionDirection;
    bool collapsed;  // Set to true when player steps on this cell
    bool startPoint;
    bool anchor;

    SpringCell() : pos(Point(-1, -1)), projectionDirection(Direction::STAY),
                   collapsed(false), startPoint(false), anchor(false) {}
};

//////////////////////////////////////////          Spring            //////////////////////////////////////////

// Spring object - to be reimplemented
class Spring : public StaticObject
{
private:
    std::vector<SpringCell> cells;
    Point* anchorPosition;
    SpringCell* startingCell;
    int compressionState;
    Direction compressionDir;
    bool compressed;

    // Compression tracking
    int compressedCellCount;      // How many cells are collapsed (0 to cells.size())
    bool isPlayerCompressing;     // Is a player currently compressing this spring?
    Point playerLastPosition;     // Track player's last position in spring

public:
    // Launch calculation result
    struct LaunchData {
        bool shouldLaunch;
        int velocityX;
        int velocityY;
        int frames;
    };
    // Default constructor
    Spring() : StaticObject(), cells(),
               anchorPosition(nullptr), startingCell(nullptr),
               compressionState(0), compressionDir(Direction::STAY),
               compressed(false),
               compressedCellCount(0), isPlayerCompressing(false),
               playerLastPosition(Point(-1, -1))
    {
        sprite = '#';
        type = ObjectType::SPRING;
    }

    // Constructor with position
    Spring(const Point& pos) : StaticObject(pos, '#', ObjectType::SPRING),
                                cells(),
                                anchorPosition(nullptr),
                                startingCell(nullptr),
                                compressionState(0),
                                compressionDir(Direction::STAY),
                                compressed(false),
                                compressedCellCount(0),
                                isPlayerCompressing(false),
                                playerLastPosition(Point(-1, -1)) {}

    // Destructor
    ~Spring();

    // Initialize spring with cells, anchor, and projection direction
    void initialize(const std::vector<Point>& springCells,
                    const Point& anchor,
                    Direction projectionDir);

    // Multi-cell query
    bool containsCell(int x, int y) const;
    SpringCell* getCellAt(int x, int y);

    // Compression management
    bool tryCompress(int playerX, int playerY, Direction playerMoveDir);
    void resetCompression();
    bool isFullyCompressed() const;
    int getCompressionLevel() const { return compressedCellCount; }

    // Launch calculation
    LaunchData calculateLaunch() const;

    // Visual update
    void updateVisuals(Room* room);

    // GameObject inherited interface - must be implemented
    GameObject* clone() const override;
    const char* getName() const override { return "Spring"; }
    bool isBlocking() const override { return false; }
    bool onExplosion() override { return true; }

};
