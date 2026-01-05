//////////////////////////////////////       INCLUDES & FORWARDS       /////////////////////////////////////////////

#include "Obstacle.h"
#include "GameObject.h"
#include "PickableObject.h"
#include "Room.h"
#include "Player.h"


void Obstacle::initialize(const std::vector<ObstacleBlock*>& obstacleBlocks, 
                            std::unordered_map<Point, std::vector<Point>>& neighbors)
{
    blocks = obstacleBlocks;
    weight = static_cast<int>(blocks.size());
    for (size_t i = 0; i < blocks.size(); i++)
    {
        blocks[i]->setBlockIndex(static_cast<int>(i));
    }
    initEdges(neighbors);
}

bool Obstacle::move(Direction dir, Room* room, int force)
{
    // Prevent multiple movements per frame
    if (movedThisFrame)
        return false;

    // Validate force requirement
    if (force < weight)
        return false;  // Insufficient force

    // Calculate offset based on direction
    int dx = 0, dy = 0;
    switch (dir)
    {
    case Direction::UP:
        dy = -1;
        break;
    case Direction::DOWN:
        dy = 1;
        break;
    case Direction::LEFT:
        dx = -1;
        break;
    case Direction::RIGHT:
        dx = 1;
        break;
    default:
        break;
    }

    // Check collision: only edge blocks matter for obstacle boundary
    for (ObstacleBlock* block : edges[dir])
    {
        Point pos = block->getPosition();
        if (room->isBlocked(pos.x + dx, pos.y + dy))
        {
            return false; // Movement blocked
        }
    }

    // Move ALL blocks by the same offset AND update visuals
    for (ObstacleBlock* block : blocks)
    {
        Point oldPos = block->getPosition();
        Point newPos(oldPos.x + dx, oldPos.y + dy);

        // Clear old position (both buffer and screen)
        room->setCharAt(oldPos.x, oldPos.y, ' ');
        gotoxy(oldPos.x, oldPos.y);
        std::cout << ' ';

        // Update block's logical position
        block->setPosition(newPos);

        // Set new position visually
        room->setCharAt(newPos.x, newPos.y, block->getSprite());
    }

    movedThisFrame = true;
    return true; // Movement successful
}


std::vector<Direction> ObstacleBlock::neighborsToEdgeDirections(std::unordered_map<Point, std::vector<Point>>& neighbors)
{
    int x = position.x;
    int y = position.y;

    std::vector<Direction> edgeDirections;
    std::vector<Point> neighborPositions = neighbors[Point(x, y)];

    if (neighborPositions.empty())
        return {};

    for (const Point& np : neighborPositions)
    {
        if (np.x == x && np.y == y - 1)
            edgeDirections.push_back(Direction::UP);
        else if (np.x == x && np.y == y + 1)
            edgeDirections.push_back(Direction::DOWN);
        else if (np.x == x - 1 && np.y == y)
            edgeDirections.push_back(Direction::LEFT);
        else if (np.x == x + 1 && np.y == y)
            edgeDirections.push_back(Direction::RIGHT);
    }

    return edgeDirections;
}

void Obstacle::initEdges(std::unordered_map<Point, std::vector<Point>>& neighbors)
{
    for (auto& block : blocks)
    {
        if (neighbors.find(block->getPosition()) != neighbors.end())
        {
            std::vector<Direction> blockEdges = block->neighborsToEdgeDirections(neighbors);
            for (const Direction& dir : blockEdges)
            {
                edges[dir].push_back(block);
            }
        }
    }
}

void Obstacle::resetPushState()
{
    accumulatedForce = 0;
    pushDirection = Direction::STAY;
    pushers.clear();
    movedThisFrame = false;
}

bool Obstacle::tryPush(Direction dir, int force, Room* room, Player* pusher)
{
    // First push this frame - initialize direction
    if (pushers.empty())
    {
        pushDirection = dir;
        accumulatedForce = force;
        pushers.push_back(pusher);

        // Check if this force alone is enough
        if (accumulatedForce >= weight)
            return move(dir, room, accumulatedForce);

        return false;  // Not enough force yet
    }

    // Subsequent push - must be same direction
    if (dir == pushDirection)
    {
        accumulatedForce += force;
        pushers.push_back(pusher);

        // Check if combined force is now enough
        if (accumulatedForce >= weight)
            return move(dir, room, accumulatedForce);
    }

    return false;  // Either wrong direction or still not enough force
}

//////////////////////////////////////////  ObstacleBlock::onExplosion  //////////////////////////////////////////

bool ObstacleBlock::onExplosion()
{
    if (parentObstacle)
    {
        parentObstacle->markForReconstruction();
    }
    return true;
}

//////////////////////////////////////////  Obstacle::reconstruct  //////////////////////////////////////////

void Obstacle::reconstruct(Room* room)
{
    // Get remaining active blocks
    std::vector<ObstacleBlock*> remaining;
    for (ObstacleBlock* block : blocks)
    {
        if (block && block->isActive())
            remaining.push_back(block);
    }

    if (remaining.empty())
    {
        // Will be cleaned up by Room
        return;
    }

    // Check if blocks are still connected
    std::vector<std::vector<ObstacleBlock*>> components = findConnectedComponents(remaining);

    if (components.size() == 1)
    {
        // Still one piece - reinitialize
        blocks = remaining;
        weight = static_cast<int>(blocks.size());
        for (size_t i = 0; i < blocks.size(); i++)
        {
            blocks[i]->setBlockIndex(static_cast<int>(i));
        }
        std::unordered_map<Point, std::vector<Point>> neighbors = buildNeighborsMap(remaining);
        initEdges(neighbors);
        resetPushState();
    }
    else
    {
        // Split into multiple - create new obstacles
        // Use first component for this obstacle
        blocks = components[0];
        weight = static_cast<int>(blocks.size());
        for (size_t i = 0; i < blocks.size(); i++)
        {
            blocks[i]->setBlockIndex(static_cast<int>(i));
        }
        std::unordered_map<Point, std::vector<Point>> neighbors = buildNeighborsMap(components[0]);
        initEdges(neighbors);
        resetPushState();

        // Create new obstacles for other components
        for (size_t j = 1; j < components.size(); j++)
        {
            Obstacle* newObs = new Obstacle();
            newObs->blocks = components[j];
            newObs->weight = static_cast<int>(components[j].size());

            for (size_t k = 0; k < components[j].size(); k++)
            {
                components[j][k]->setBlockIndex(static_cast<int>(k));
                components[j][k]->setParent(newObs);
            }

            std::unordered_map<Point, std::vector<Point>> newNeighbors = buildNeighborsMap(components[j]);
            newObs->initEdges(newNeighbors);
            newObs->resetPushState();

            room->addObstacle(newObs);
        }
    }

    needsReconstructionFlag = false;
}

//////////////////////////////////////////  findConnectedComponents  //////////////////////////////////////////


// Made with AI - I strated with a simpler obstacle reconstruction
// method which had bugs and multiple problems with it's logic.
// I then asked an AI to review my current algorithm and suggest a better solution.
// The AI then provided this BFS-based connected components approach,
// which worked great as far as I've tested, and therefore decided to keep altough 
// we didn't properly get to study and fully understand the BFS algorithm yet.
 
std::vector<std::vector<ObstacleBlock*>> Obstacle::findConnectedComponents(
    const std::vector<ObstacleBlock*>& blocks)
{
    std::vector<std::vector<ObstacleBlock*>> components;
    std::unordered_set<ObstacleBlock*> visited;

    for (ObstacleBlock* start : blocks)
    {
        if (!start || visited.count(start))
            continue;

        // BFS
        std::vector<ObstacleBlock*> component;
        std::queue<ObstacleBlock*> queue;
        queue.push(start);
        visited.insert(start);

        while (!queue.empty())
        {
            ObstacleBlock* curr = queue.front();
            queue.pop();
            component.push_back(curr);

            Point pos = curr->getPosition();
            Point neighbors[4] = {
                Point(pos.x+1, pos.y), Point(pos.x-1, pos.y),
                Point(pos.x, pos.y+1), Point(pos.x, pos.y-1)
            };

            for (const Point& np : neighbors)
            {
                for (ObstacleBlock* other : blocks)
                {
                    if (other && other->isActive() &&
                        other->getPosition() == np &&
                        !visited.count(other))
                    {
                        visited.insert(other);
                        queue.push(other);
                    }
                }
            }
        }

        components.push_back(component);
    }

    return components;
}

//////////////////////////////////////////  buildNeighborsMap  //////////////////////////////////////////

std::unordered_map<Point, std::vector<Point>> Obstacle::buildNeighborsMap(
    const std::vector<ObstacleBlock*>& blocks)
{
    std::unordered_map<Point, std::vector<Point>> neighbors;

    for (ObstacleBlock* block : blocks)
    {
        if (!block)
            continue;

        Point pos = block->getPosition();
        Point adjacent[4] = {
            Point(pos.x+1, pos.y), Point(pos.x-1, pos.y),
            Point(pos.x, pos.y+1), Point(pos.x, pos.y-1)
        };

        for (const Point& adj : adjacent)
        {
            bool isObstacle = false;
            for (ObstacleBlock* other : blocks)
            {
                if (other && other->getPosition() == adj)
                {
                    isObstacle = true;
                    break;
                }
            }

            if (!isObstacle)
            {
                neighbors[pos].push_back(adj);
            }
        }
    }

    return neighbors;
}