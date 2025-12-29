#include "Spring.h"
#include "Player.h"
#include "DebugLog.h"
#include "Console.h"
#include <iostream>

//////////////////////////////////////////      Destructor          //////////////////////////////////////////

Spring::~Spring()
{
    if (anchorPosition != nullptr) {
        delete anchorPosition;
        anchorPosition = nullptr;
    }
}

//////////////////////////////////////////      Initialize          //////////////////////////////////////////

void Spring::initialize(const std::vector<Point>& springCells,
                       const Point& anchor,
                       Direction projectionDir)
{
    // Set anchor position
    if (anchorPosition == nullptr) {
        anchorPosition = new Point(anchor);
    } else {
        *anchorPosition = anchor;
    }

    // Set compression direction
    compressionDir = projectionDir;

    // Populate cells vector
    cells.clear();
    for (size_t i = 0; i < springCells.size(); i++) {
        SpringCell cell;
        cell.pos = springCells[i];
        cell.projectionDirection = projectionDir;
        cell.collapsed = false;
        cell.startPoint = (i == 0);  // First cell is the start point
        cell.anchor = false;  // None are anchor (anchor is the wall)
        cells.push_back(cell);
    }

    // Log all cell positions for debugging
    DebugLog::getStream() << "[SPRING_INIT_CELLS] Spring@" << position.x << "," << position.y << " | ";
    for (size_t i = 0; i < cells.size(); i++) {
        DebugLog::getStream() << "Cell[" << i << "]:(" << cells[i].pos.x << "," << cells[i].pos.y << ") ";
    }
    DebugLog::getStream() << std::endl;

    // Set starting cell (first cell in the spring)
    if (!cells.empty()) {
        startingCell = &cells[0];
    }

    // Reset compression state
    compressionState = 0;
    compressed = false;

    // Debug logging
    DebugLog::getStream() << "[SPRING_INIT] Spring@" << position.x << "," << position.y
                          << " | Cells: " << cells.size();
    if (anchorPosition != nullptr) {
        DebugLog::getStream() << " | Anchor: " << anchorPosition->x << "," << anchorPosition->y;
    } else {
        DebugLog::getStream() << " | Anchor: NULL";
    }
    DebugLog::getStream() << " | ComprDir: " << static_cast<int>(compressionDir);
    if (startingCell != nullptr) {
        DebugLog::getStream() << " | StartCell: " << startingCell->pos.x << "," << startingCell->pos.y;
    } else {
        DebugLog::getStream() << " | StartCell: NULL";
    }
    DebugLog::getStream() << std::endl;
}

//////////////////////////////////////////        clone              //////////////////////////////////////////

GameObject* Spring::clone() const
{
    return new Spring(*this);
}

//////////////////////////////////////////    containsCell          //////////////////////////////////////////

bool Spring::containsCell(int x, int y) const
{
    for (const SpringCell& cell : cells) {
        if (cell.pos.x == x && cell.pos.y == y) {
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////      getCellAt           //////////////////////////////////////////

SpringCell* Spring::getCellAt(int x, int y)
{
    for (SpringCell& cell : cells) {
        if (cell.pos.x == x && cell.pos.y == y) {
            return &cell;
        }
    }
    return nullptr;
}

//////////////////////////////////////////    tryCompress           //////////////////////////////////////////

bool Spring::tryCompress(int playerX, int playerY, Direction playerMoveDir)
{
    SpringCell* currentCell = getCellAt(playerX, playerY);

    // Player left spring - reset compression
    if (currentCell == nullptr) {
        if (isPlayerCompressing) {
            resetCompression();
        }
        return false;
    }

    // First entry to spring
    if (!isPlayerCompressing) {
        // Must enter at start cell
        if (!currentCell->startPoint) {
            return false;  // Wrong entry point - pass through
        }

        // Must move toward anchor (match compressionDir)
        if (playerMoveDir != compressionDir) {
            return false;  // Wrong direction - pass through
        }

        // Valid entry - start compressing
        isPlayerCompressing = true;
        playerLastPosition = Point(playerX, playerY);
        currentCell->collapsed = true;
        compressedCellCount = 1;

        DebugLog::getStream() << "[SPRING_COMPRESS_START] Player entered spring at ("
                              << playerX << "," << playerY << ") moving "
                              << static_cast<int>(playerMoveDir) << std::endl;
        return true;
    }

    // Already compressing - check if player moved to new cell
    if (playerX == playerLastPosition.x && playerY == playerLastPosition.y) {
        return true;  // Same cell, no change
    }

    // Collapse current cell if not already collapsed
    if (!currentCell->collapsed) {
        currentCell->collapsed = true;
        compressedCellCount++;
        playerLastPosition = Point(playerX, playerY);

        DebugLog::getStream() << "[SPRING_COMPRESS] Cell collapsed at ("
                              << playerX << "," << playerY << ") | Total: "
                              << compressedCellCount << "/" << cells.size() << std::endl;
    }

    return true;
}

//////////////////////////////////////////  isFullyCompressed       //////////////////////////////////////////

bool Spring::isFullyCompressed() const
{
    return compressedCellCount == static_cast<int>(cells.size());
}

//////////////////////////////////////////  calculateLaunch         //////////////////////////////////////////

Spring::LaunchData Spring::calculateLaunch() const
{
    LaunchData result;
    result.shouldLaunch = false;
    result.velocityX = 0;
    result.velocityY = 0;
    result.frames = 0;

    if (compressedCellCount == 0) {
        return result;
    }

    result.shouldLaunch = true;
    result.frames = compressedCellCount;  // Duration = compression level

    // Launch away from wall (opposite of compressionDir)
    switch (compressionDir) {
        case Direction::RIGHT:
            result.velocityX = -compressedCellCount;  // Launch LEFT
            break;
        case Direction::LEFT:
            result.velocityX = compressedCellCount;   // Launch RIGHT
            break;
        case Direction::DOWN:
            result.velocityY = -compressedCellCount;  // Launch UP
            break;
        case Direction::UP:
            result.velocityY = compressedCellCount;   // Launch DOWN
            break;
        default:
            result.shouldLaunch = false;
    }

    return result;
}

//////////////////////////////////////////  resetCompression        //////////////////////////////////////////

void Spring::resetCompression()
{
    if (compressedCellCount > 0) {
        DebugLog::getStream() << "[SPRING_RESET] Resetting spring compression from "
                              << compressedCellCount << " cells" << std::endl;
    }

    compressedCellCount = 0;
    isPlayerCompressing = false;
    playerLastPosition = Point(-1, -1);

    for (SpringCell& cell : cells) {
        cell.collapsed = false;
    }
}

//////////////////////////////////////////   updateVisuals          //////////////////////////////////////////

void Spring::updateVisuals(Room* room)
{
    if (room == nullptr) return;

    for (const SpringCell& cell : cells) {
        char displayChar = cell.collapsed ? '~' : '#';
        room->setCharAt(cell.pos.x, cell.pos.y, displayChar);
        gotoxy(cell.pos.x, cell.pos.y);
        std::cout << displayChar << std::flush;
    }
}

