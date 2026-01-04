#include "SpringLink.h"
#include "Spring.h"
#include "Room.h"
#include "Console.h"
#include "DebugLog.h"
#include <iostream>

// Constructor
SpringLink::SpringLink(const Point& pos, Spring* parent, int index)
    : StaticObject(pos, '#', ObjectType::SPRING_LINK),
      parentSpring(parent),
      linkIndex(index),
      collapsed(false)
{
    DebugLog::getStream() << "[SPRINGLINK_CREATE] Link#" << linkIndex
                          << " created at (" << pos.x << "," << pos.y << ")" << std::endl;
}

// Clone
GameObject* SpringLink::clone() const
{
    return new SpringLink(*this);
}

// Handle explosion - destroy entire spring
bool SpringLink::onExplosion()
{
    if (parentSpring)
    {
        parentSpring->destroyAllLinks();
    }
    return true;
}

// Collapse this link and update visual
void SpringLink::collapse(Room* room)
{
    if (collapsed)
    {
        DebugLog::getStream() << "[SPRINGLINK_COLLAPSE] Link#" << linkIndex
                              << " at (" << position.x << "," << position.y
                              << ") already collapsed - skipping" << std::endl;
        return;
    }

    collapsed = true;
    sprite = ' ';  // Change sprite to space (or '~' for visual feedback)

    DebugLog::getStream() << "[SPRINGLINK_COLLAPSE] Link#" << linkIndex
                          << " at (" << position.x << "," << position.y
                          << ") collapsed - sprite changed to ' '" << std::endl;

    // Update room's character map and display
    if (room != nullptr)
    {
        room->setCharAt(position.x, position.y, sprite);
        gotoxy(position.x, position.y);
        std::cout << sprite << std::flush;
        DebugLog::getStream() << "[SPRINGLINK_COLLAPSE] Visual updated in room" << std::endl;
    }
    else
    {
        DebugLog::getStream() << "[SPRINGLINK_COLLAPSE] WARNING: Room is null, visual not updated!" << std::endl;
    }
}

// Reset to uncompressed state
void SpringLink::reset(Room* room)
{
    if (!collapsed)
    {
        DebugLog::getStream() << "[SPRINGLINK_RESET] Link#" << linkIndex
                              << " at (" << position.x << "," << position.y
                              << ") not collapsed - skipping reset" << std::endl;
        return;
    }

    collapsed = false;
    sprite = '#';  // Back to normal spring visual

    DebugLog::getStream() << "[SPRINGLINK_RESET] Link#" << linkIndex
                          << " at (" << position.x << "," << position.y
                          << ") reset - sprite changed to '#'" << std::endl;

    // Update room's character map and display
    if (room != nullptr)
    {
        room->setCharAt(position.x, position.y, sprite);
        gotoxy(position.x, position.y);
        std::cout << sprite << std::flush;
        DebugLog::getStream() << "[SPRINGLINK_RESET] Visual updated in room" << std::endl;
    }
    else
    {
        DebugLog::getStream() << "[SPRINGLINK_RESET] WARNING: Room is null, visual not updated!" << std::endl;
    }
}
