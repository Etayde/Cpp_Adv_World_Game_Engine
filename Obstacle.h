#pragma once

////////////////////////////////////////      INCLUDES & FORWARDS       //////////////////////////////////////////

#include "StaticObjects.h"
#include <vector>

class Room;
class Player;

///////////////////////////////////////////      ObstacleBlock        //////////////////////////////////////////

class ObstacleBlock : public StaticObject
{
private:
    Obstacle* parentObstacle;
    int blockIndex;
public:
    ObstacleBlock(const Point &pos, Obstacle* parent)
        : StaticObject(pos, 'O', ObjectType::OBSTACLE_BLOCK), parentObstacle(parent), blockIndex(-1) {}

    Obstacle* getParent() const { return parentObstacle; }
    void setBlockIndex(int index) { blockIndex = index; }
    int getBlockIndex() const { return blockIndex; }
    GameObject* clone() const override
    {
        return new ObstacleBlock(*this);
    }
    const char* getName() const override { return "ObstacleBloack"; }
    bool isBlocking() const override { return true; }
};

//////////////////////////////////////////        Obstacle         //////////////////////////////////////////

// An movable object that blocks movement
class Obstacle
{
private:
    std::vector<ObstacleBlock*> blocks;
    int weight = blocks.size(); // Weight based on number of blocks
public:
    Obstacle(): blocks() {};

    void initialize(const std::vector<ObstacleBlock*>& obstacleBlocks);

    int getWeight() const { return weight; }
    const std::vector<ObstacleBlock*>& getBlocks() const { return blocks; }
    bool canBeMoved(int force) const { return force >= weight; }
    bool move(Direction dir, Room* room);
};
