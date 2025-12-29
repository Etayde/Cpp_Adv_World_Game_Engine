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

