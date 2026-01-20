////////////////////////////////////////      INCLUDES & FORWARDS       //////////////////////////////////////////

#include "Switch.h"
#include "Room.h"

//////////////////////////////////////////         toggle         //////////////////////////////////////////

void Switch::toggle()
{
    isOn = !isOn;
    if (isOn)
    {
        sprite = '/';
        type = ObjectType::SWITCH_ON;
    }
    else
    {
        sprite = '\\';
        type = ObjectType::SWITCH_OFF;
    }
}

//////////////////////////////////////////         setIsOn         //////////////////////////////////////////

void Switch::setIsOn(bool on)
{
    isOn = on;
    sprite = isOn ? '/' : '\\';
    type = isOn ? ObjectType::SWITCH_ON : ObjectType::SWITCH_OFF;
}

//////////////////////////////////////////         onInteract         //////////////////////////////////////////

bool Switch::onInteract(Player *player, Room *room)
{
    if (player == nullptr || room == nullptr) return false;

    toggle();
    draw();
    room->updatePuzzleState();

    return true;
}