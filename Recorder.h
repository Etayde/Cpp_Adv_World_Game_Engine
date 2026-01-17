#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "Constants.h"
#include "Player.h"

using namespace std;

//////////////////////////////////////////     GameEventType & GameEvent     /////////////////////////////////////////////

enum class GameEventType {
    SCREEN_CHANGE,
    LIFE_LOST,
    RIDDLE_ANSWERED,
    QUIT
};

struct GameEvent {
    unsigned long cycle;
    GameEventType type;
    int roomId;

    // For LIFE_LOST
    int playerId;

    // For RIDDLE_ANSWERED
    std::string question;
    int answerGiven;  // 1-4
    bool wasCorrect;

    // Default constructor
    GameEvent() : cycle(0), type(GameEventType::SCREEN_CHANGE), roomId(0),
                  playerId(0), answerGiven(0), wasCorrect(false) {}

    // Constructor for SCREEN_CHANGE
    GameEvent(unsigned long c, int room)
        : cycle(c), type(GameEventType::SCREEN_CHANGE), roomId(room),
          playerId(0), answerGiven(0), wasCorrect(false) {}

    // Constructor for LIFE_LOST
    GameEvent(unsigned long c, int room, int player)
        : cycle(c), type(GameEventType::LIFE_LOST), roomId(room),
          playerId(player), answerGiven(0), wasCorrect(false) {}

    // Constructor for RIDDLE_ANSWERED
    GameEvent(unsigned long c, int room, const std::string& q, int answer, bool correct)
        : cycle(c), type(GameEventType::RIDDLE_ANSWERED), roomId(room),
          playerId(0), question(q), answerGiven(answer), wasCorrect(correct) {}

    // Constructor for QUIT
    GameEvent(unsigned long c, int room, GameEventType quitType)
        : cycle(c), type(quitType), roomId(room),
          playerId(0), answerGiven(0), wasCorrect(false) {}

    void write(std::ostream& out) const;
    bool read(std::istream& in);
};

//////////////////////////////////////////     Action Conversion     /////////////////////////////////////////////

inline string actionToString(Action action);
inline Action stringToAction(const string& str);
struct ActionRecord
{
    unsigned long cycle;
    int playerId;
    Action action;

    int answer; // -1 if not a riddle answer

    // Default constructor
    ActionRecord() : cycle(0), playerId(0), action(Action::STAY), answer(-1) {}

    // Constructor from PlayerKeyBinding
    ActionRecord(unsigned long c, const PlayerKeyBinding &binding)
        : cycle(c), playerId(binding.playerID),
          action(binding.action), answer(-1) {}
    
    // Constructor for riddle answer
    ActionRecord(unsigned long c, int player, int ans)
        : cycle(c), playerId(player), action(Action::ANSWER_RIDDLE), answer(ans) {}

    // Serialize to output stream
    // Format: CYCLE: <cycle> PLAYER: <player> ACTION: <action>
    void write(ostream &output) const;

    // Deserialize from input stream (reads one line)
    // Returns true on success, false on error (no exceptions)
    bool read(istream &input);
};

class RecordedSteps
{
private:
    vector<ActionRecord> actions;
    size_t currActionIndex;

public:
    // Constructor
    RecordedSteps() : currActionIndex(0) {}

    // Add action to the vector
    void addAction(const ActionRecord& record) { actions.push_back(record); }

    // Load actions from file (calls read() for each line)
    ErrorCode loadFromFile(const string& filename);

    // Get current action (returns nullptr if no more actions)
    const ActionRecord* getCurrentAction() const;

    // Move to next action
    void advanceToNextAction() { if (currActionIndex < actions.size()) currActionIndex++; }

    // Check if there are more actions
    bool hasMoreActions() const { return currActionIndex < actions.size(); }

    // Get all actions for a specific cycle number
    vector<ActionRecord> getActionsForCycle(unsigned long cycle) const;

    ActionRecord getActionAt(size_t index) const { return actions[index]; }

    size_t getCurrIndex() const { return currActionIndex; }
    
};