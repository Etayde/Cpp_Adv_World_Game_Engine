#pragma once

//////////////////////////////////////////       INCLUDES & FORWARDS       /////////////////////////////////////////////

#include "Console.h"
#include "Constants.h"
#include "Renderer.h"
#include <iostream>
#include <string>

class Point;

//////////////////////////////////////////          Screen       /////////////////////////////////////////////

class Screen
{
  const char *screen[MAX_Y];
  std::string ownedData[MAX_Y];

public:
  Screen();
  Screen(const char *layout[MAX_Y]);
  Screen(const std::string layout[MAX_Y]);
  
  char getCharAt(int x, int y) const;
  char getCharAt(const Point &p) const;

  bool isWall(int x, int y) const { return getCharAt(x, y) == 'W'; }
  bool isWall(const Point &p) const;

  bool isObject(const Point &p) const;

  ObjectType objectIs(const Point &p) const;

  void draw() const;
};
