#include "Obstacle.h"
#include "Room.h"


void Obstacle::initialize(const std::vector<ObstacleBlock*>& obstacleBlocks)
{
    blocks = obstacleBlocks;
    weight = static_cast<int>(blocks.size());
    for (size_t i = 0; i < blocks.size(); i++)
    {
        blocks[i]->setBlockIndex(static_cast<int>(i));
    }
}

bool Obstacle::move(Direction dir, Room* room)
{
    // Calculate new positions for all blocks
    std::vector<Point> newPositions;
    for (ObstacleBlock* block : blocks)
    {
        Point pos = block->getPosition();
        switch (dir)
        {
        case Direction::UP:
            if (room)
            pos.y -= 1;
            break;
        case Direction::DOWN:
            pos.y += 1;
            break;
        case Direction::LEFT:
            pos.x -= 1;
            break;
        case Direction::RIGHT:
            pos.x += 1;
            break;
        default:
            break;
        }
        newPositions.push_back(pos);
    }

    // Check if all new positions are free
    for (const Point& pos : newPositions)
    {
        if (room->isCellBlocking(pos.x, pos.y))
        {
            return false; // Movement blocked
        }
    }

    // Move all blocks to new positions
    for (size_t i = 0; i < blocks.size(); i++)
    {
        blocks[i]->setPosition(newPositions[i]);
    }

    return true; // Movement successful
}
