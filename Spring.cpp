#include "Spring.h"
#include "Player.h"
#include "DebugLog.h"

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
    for (const Point& cellPos : springCells) {
        SpringCell cell;
        cell.pos = cellPos;
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

bool Spring::isCompressing(const Player& p, int checkX, int checkY) const{
    Point checkPos(checkX, checkY);

    // Check if player is on ANY spring cell
    bool onSpringCell = false;
    for (const SpringCell& cell : cells) {
        if (checkPos == cell.pos) {
            onSpringCell = true;
            break;
        }
    }

    if (!onSpringCell) {
        return false;  // Not on spring at all
    }

    // If already fully compressed, always return true
    if (compressed) {
        DebugLog::getStream() << "[SPRING_CHECK] Player at " << checkX << "," << checkY
                              << " | State: FULLY_COMPRESSED"
                              << std::endl;
        return true;
    }

    // If spring is actively compressing (compressionState > 0), continue compressing
    if (compressionState > 0) {
        DebugLog::getStream() << "[SPRING_CHECK] Player at " << checkX << "," << checkY
                              << " | State: ACTIVELY_COMPRESSING"
                              << " | ComprState: " << compressionState << "/" << cells.size()
                              << std::endl;
        return true;
    }

    // First cell: check direction matches
    bool dirMatch = (p.getCurrentDirection() == compressionDir);
    DebugLog::getStream() << "[SPRING_CHECK] Player at " << checkX << "," << checkY
                          << " | State: FIRST_CELL_CHECK"
                          << " | DirMatch: " << (dirMatch ? "YES" : "NO")
                          << " | PlayerDir: " << static_cast<int>(p.getCurrentDirection())
                          << " | ComprDir: " << static_cast<int>(compressionDir)
                          << std::endl;

    if (dirMatch && onSpringCell) {
        return true;
    }

    return false;
}

void Spring::compressCell() {
    this->cells[compressionState].collapsed = true;

    DebugLog::getStream() << "[SPRING_COMPRESS] Spring@" << position.x << "," << position.y
                          << " | Cell: " << compressionState << "/" << cells.size()
                          << " | CellPos: " << cells[compressionState].pos.x << "," << cells[compressionState].pos.y
                          << " | NewState: " << (compressionState + 1)
                          << " | Collapsed: YES"
                          << std::endl;

    compressionState++;

    // Set compressed flag when all cells are collapsed
    if (compressionState >= static_cast<int>(cells.size())) {
        compressed = true;
    }
}

void Spring::reset() {
    for (auto& cell : cells) {
        cell.collapsed = false;
    }
    compressionState = 0;
    compressed = false;
}

void Spring::launch(Player* p){

    DebugLog::getStream() << "[SPRING_LAUNCH] Player:" << p->playerId
                          << " at " << p->getPosition().x << "," << p->getPosition().y
                          << " | ComprDir: " << static_cast<int>(compressionDir)
                          << " | ComprState: " << compressionState
                          << std::endl;

    Direction launchDir = Direction::STAY;
    switch (this->compressionDir) {
        case Direction::UP:
            launchDir = Direction::DOWN;
            break;
        case Direction::DOWN:
            launchDir = Direction::UP;
            break;
        case Direction::LEFT:
            launchDir = Direction::RIGHT;
            break;
        case Direction::RIGHT:
            launchDir = Direction::LEFT;
            break;
        default:
            launchDir = Direction::STAY;
            break;
    }
    p->setDirection(launchDir, compressionState);
    p->launchFramesRemaining = compressionState;

    DebugLog::getStream() << "[SPRING_LAUNCH_SET] LaunchDir: " << static_cast<int>(launchDir)
                          << " | Force: " << compressionState
                          << " | LaunchFrames: " << p->launchFramesRemaining
                          << std::endl;

    this->reset();

}

