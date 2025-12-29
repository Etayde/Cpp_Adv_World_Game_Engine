#pragma once

//////////////////////////////////////       INCLUDES & FORWARDS       //////////////////////////////////////////

#include "Point.h"
#include "Constants.h"
#include <vector>

class SpringLink;
class Room;

//////////////////////////////////////////          Spring            //////////////////////////////////////////

// Spring manager class - coordinates SpringLink objects for compression and launch
class Spring
{
private:
    std::vector<SpringLink*> links;
    Point anchorPosition;
    Direction compressionDir;
    int compressedCount;

public:
    // Launch calculation result
    struct LaunchData {
        bool shouldLaunch;
        int velocityX;
        int velocityY;
        int frames;
    };

    // Constructor/Destructor
    Spring();
    ~Spring();

    // Initialization - call from Room during scanning
    void initialize(const std::vector<SpringLink*>& springLinks,
                   const Point& anchor,
                   Direction projectionDir);

    // Compression validation
    bool canCompressLink(int linkIndex, Direction playerDir) const;

    // Compression execution
    void compressLink(int linkIndex, Room* room);

    // State queries
    bool isFullyCompressed() const;
    int getCompressionLevel() const { return compressedCount; }
    int getLinkCount() const { return static_cast<int>(links.size()); }

    // Launch mechanics
    LaunchData calculateLaunch() const;
    void resetCompression(Room* room);

    // Getters
    Direction getCompressionDir() const { return compressionDir; }
};
