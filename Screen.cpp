//////////////////////////////////////       INCLUDES & FORWARDS       //////////////////////////////////////////

#include "Screen.h"
#include "Point.h"

//////////////////////////////////////////         constructors         //////////////////////////////////////////

Screen::Screen()
{
    for (int i = 0; i < MAX_Y; i++)
      screen[i] = nullptr;
}

Screen::Screen(const char *layout[MAX_Y])
{
    for (int i = 0; i < MAX_Y; i++)
      screen[i] = layout[i];
}

Screen::Screen(const std::string layout[MAX_Y])
{
    for (int i = 0; i < MAX_Y; i++)
    {
      ownedData[i] = layout[i];
      screen[i] = ownedData[i].c_str();
    }
}

//////////////////////////////////////////         getCharAt          //////////////////////////////////////////

char Screen::getCharAt(int x, int y) const
{
    if (x >= 0 && x < MAX_X && y >= 0 && y < MAX_Y && screen[y] != nullptr)
      return screen[y][x];
    return 'W';
}

char Screen::getCharAt(const Point &p) const { return getCharAt(p.getX(), p.getY()); }

//////////////////////////////////////////          isWall            //////////////////////////////////////////

bool Screen::isWall(const Point &p) const { return getCharAt(p) == 'W'; }

//////////////////////////////////////////         isObject           //////////////////////////////////////////

bool Screen::isObject(const Point &p) const
{
    char c = getCharAt(p);
    return (c != ' ' && c != 'W');
}

//////////////////////////////////////////         objectIs           //////////////////////////////////////////

ObjectType Screen::objectIs(const Point &p) const
{
    char ch = getCharAt(p);

    switch (ch)
    {
    case '#':
        return ObjectType::SPRING;
    case '*':
        return ObjectType::OBSTACLE_BLOCK;
    case '!':
        return ObjectType::TORCH;
    case '@':
        return ObjectType::BOMB;
    case 'K':
        return ObjectType::KEY;
    case '\\':
        return ObjectType::SWITCH_OFF;
    case '/':
        return ObjectType::SWITCH_ON;
    case '?':
        return ObjectType::RIDDLE;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return ObjectType::DOOR;
    default:
        return ObjectType::AIR;
    }
}

//////////////////////////////////////////          draw            //////////////////////////////////////////

static void colorMenuChar(char c)
{
    if (c == ' ') return;
    set_color(Color::White);
}

void Screen::draw() const
{
    Renderer::clrscr();
    Renderer::gotoxy(0, 0);

    for (int i = 0; i < MAX_Y - 1; ++i)
    {
      if (screen[i] != nullptr)
      {
        for (int j = 0; screen[i][j] != '\0'; ++j)
        {
            char c = screen[i][j];
            colorMenuChar(c);
            Renderer::print(c);
            reset_color();
        }
        Renderer::print('\n');
      }
    }

    if (screen[MAX_Y - 1] != nullptr)
    {
        for (int j = 0; screen[MAX_Y - 1][j] != '\0'; ++j)
        {
            char c = screen[MAX_Y - 1][j];
            colorMenuChar(c);
            Renderer::print(c);
            reset_color();
        }
    }
    Renderer::flush();
}
