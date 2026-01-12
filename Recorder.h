#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "Constants.h"
#include "Player.h"

using namespace std;

// Convert Action enum to string
inline string actionToString(Action action) {
    switch (action) {
        case Action::MOVE_UP:    return "MOVE_UP";
        case Action::MOVE_DOWN:  return "MOVE_DOWN";
        case Action::MOVE_LEFT:  return "MOVE_LEFT";
        case Action::MOVE_RIGHT: return "MOVE_RIGHT";
        case Action::STAY:       return "STAY";
        case Action::DROP_ITEM:  return "DROP_ITEM";
        case Action::ESC:        return "ESC";
        default:                 return "UNKNOWN";
    }
}

// Convert string to Action enum (returns Action::STAY on invalid input)
inline Action stringToAction(const string& str) {
    if (str == "MOVE_UP")    return Action::MOVE_UP;
    if (str == "MOVE_DOWN")  return Action::MOVE_DOWN;
    if (str == "MOVE_LEFT")  return Action::MOVE_LEFT;
    if (str == "MOVE_RIGHT") return Action::MOVE_RIGHT;
    if (str == "STAY")       return Action::STAY;
    if (str == "DROP_ITEM")  return Action::DROP_ITEM;
    if (str == "ESC")        return Action::ESC;
    return Action::STAY;  // Default fallback for invalid strings
}

struct ActionRecord
{
    unsigned long cycleNumber;
    int playerId;
    string action;

    // Default constructor (needed for reading)
    ActionRecord() : cycleNumber(0), playerId(0), action("") {}

    // Constructor from PlayerKeyBinding (eliminates redundancy)
    ActionRecord(unsigned long cycle, const PlayerKeyBinding &binding)
        : cycleNumber(cycle), playerId(binding.playerID),
          action(actionToString(binding.action)) {}

    // Serialize to output stream
    // Format: CYCLE: <cycle> | PLAYER: <player> | ACTION: <action>
    void write(ostream &output) const;

    // Deserialize from input stream (reads one line)
    // Returns true on success, false on error (no exceptions)
    bool read(istream &input);

private:
    // Helper method to jump to field value position
    size_t jumpToFieldValue(const string& line, size_t startPos, const string& fieldName) const;

    // Helper method to extract field value between current position and delimiter
    string extractFieldValue(const string& line, size_t startPos, const string& delimiter) const;

    // Helper method to parse unsigned long field
    bool parseUnsignedLongField(const string& line, size_t& currentPos, const string& fieldName, unsigned long& outValue) const;

    // Helper method to parse integer field
    bool parseIntField(const string& line, size_t& currentPos, const string& fieldName, int& outValue) const;

    // Helper method to parse string field (no conversion needed)
    bool parseStringField(const string& line, size_t& currentPos, const string& fieldName, string& outValue, const string& delimiter) const;
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
    bool loadFromFile(const string& filename);

    // Get current action (returns nullptr if no more actions)
    const ActionRecord* getCurrentAction() const;

    // Move to next action
    void advanceToNextAction() { if (currActionIndex < actions.size()) { currActionIndex++; } }

    // Check if there are more actions
    bool hasMoreActions() const { return currActionIndex < actions.size(); }

    // Get all actions for a specific cycle number
    vector<ActionRecord> getActionsForCycle(unsigned long cycle) const;
};