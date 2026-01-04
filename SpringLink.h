#pragma once

#include "StaticObjects.h"
#include "Point.h"
#include "Constants.h"

// Forward declaration
class Spring;
class Room;

class SpringLink : public StaticObject
{
private:
    Spring* parentSpring;
    int linkIndex;
    bool collapsed;

public:
    // Constructor
    SpringLink(const Point& pos, Spring* parent, int index);

    // GameObject interface
    GameObject* clone() const override;
    const char* getName() const override { return "SpringLink"; }
    bool isBlocking() const override { return false; }
    bool onExplosion() override;

    // SpringLink specific methods
    Spring* getParentSpring() const { return parentSpring; }
    void setParentSpring(Spring* parent) { parentSpring = parent; }
    int getLinkIndex() const { return linkIndex; }
    bool isCollapsed() const { return collapsed; }
    void collapse(Room* room);
    void reset(Room* room);
    bool isStartLink() const { return linkIndex == 0; }
};
