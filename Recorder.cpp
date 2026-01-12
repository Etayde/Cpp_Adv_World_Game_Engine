#include "Recorder.h"
#include <fstream>

//////////////////////////////////////////    ActionRecord::write    /////////////////////////////////////////////

void ActionRecord::write(ostream &output) const
{
    output << "CYCLE: " << cycleNumber
           << " | PLAYER: " << playerId
           << " | ACTION: " << action << "\n";
}

//////////////////////////////////////////    ActionRecord::read    /////////////////////////////////////////////

bool ActionRecord::read(istream &input)
{
    string line;
    if (!getline(input, line)) {
        return false;  // Failed to read line
    }

    size_t currentPos = 0;

    // Parse CYCLE field
    if (!parseUnsignedLongField(line, currentPos, "CYCLE", cycleNumber)) {
        return false;
    }

    // Parse PLAYER field
    if (!parseIntField(line, currentPos, "PLAYER", playerId)) {
        return false;
    }

    // Parse ACTION field
    if (!parseStringField(line, currentPos, "ACTION", action, "")) {
        return false;
    }

    return true;  // Success
}

//////////////////////////////////////////    ActionRecord::jumpToFieldValue    /////////////////////////////////////////////

size_t ActionRecord::jumpToFieldValue(const string& line, size_t startPos, const string& fieldName) const
{
    string searchStr = fieldName + ": ";
    size_t pos = line.find(searchStr, startPos);
    if (pos == string::npos) {
        return string::npos;
    }
    return pos + searchStr.length();  // Return position after "FIELDNAME: "
}

//////////////////////////////////////////    ActionRecord::extractFieldValue    /////////////////////////////////////////////

string ActionRecord::extractFieldValue(const string& line, size_t startPos, const string& delimiter) const
{
    if (startPos == string::npos) {
        return "";
    }

    size_t endPos = line.find(delimiter, startPos);
    if (endPos == string::npos) {
        // No delimiter found, take rest of line
        string value = line.substr(startPos);
        // Trim trailing whitespace
        size_t end = value.find_last_not_of(" \t\r\n");
        return (end != string::npos) ? value.substr(0, end + 1) : value;
    }

    return line.substr(startPos, endPos - startPos);
}

//////////////////////////////////////////    RecordedSteps::loadFromFile    /////////////////////////////////////////////

bool RecordedSteps::loadFromFile(const string& filename)
{
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    actions.clear();
    currActionIndex = 0;

    while (file.peek() != EOF) {
        ActionRecord record;
        if (!record.read(file)) {
            file.close();
            return false;  // Parse error
        }
        actions.push_back(record);
    }

    file.close();
    return true;
}

//////////////////////////////////////////    RecordedSteps::getCurrentAction    /////////////////////////////////////////////

const ActionRecord* RecordedSteps::getCurrentAction() const
{
    if (currActionIndex >= actions.size()) {
        return nullptr;
    }
    return &actions[currActionIndex];
}

//////////////////////////////////////////    RecordedSteps::getActionsForCycle    /////////////////////////////////////////////

vector<ActionRecord> RecordedSteps::getActionsForCycle(unsigned long cycle) const
{
    vector<ActionRecord> result;
    for (const ActionRecord& record : actions) {
        if (record.cycleNumber == cycle) {
            result.push_back(record);
        }
    }
    return result;
}

//////////////////////////////////////////    ActionRecord::parseUnsignedLongField    /////////////////////////////////////////////

bool ActionRecord::parseUnsignedLongField(const string& line, size_t& currentPos, const string& fieldName, unsigned long& outValue) const
{
    size_t valuePos = jumpToFieldValue(line, currentPos, fieldName);
    if (valuePos == string::npos) {
        return false;  // Missing field
    }

    string valueStr = extractFieldValue(line, valuePos, " | ");
    if (valueStr.empty()) {
        return false;  // Empty value
    }

    try {
        outValue = stoul(valueStr);
    } catch (...) {
        return false;  // Invalid number format
    }

    currentPos = valuePos;  // Update position for next field
    return true;
}

//////////////////////////////////////////    ActionRecord::parseIntField    /////////////////////////////////////////////

bool ActionRecord::parseIntField(const string& line, size_t& currentPos, const string& fieldName, int& outValue) const
{
    size_t valuePos = jumpToFieldValue(line, currentPos, fieldName);
    if (valuePos == string::npos) {
        return false;  // Missing field
    }

    string valueStr = extractFieldValue(line, valuePos, " | ");
    if (valueStr.empty()) {
        return false;  // Empty value
    }

    try {
        outValue = stoi(valueStr);
    } catch (...) {
        return false;  // Invalid number format
    }

    currentPos = valuePos;  // Update position for next field
    return true;
}

//////////////////////////////////////////    ActionRecord::parseStringField    /////////////////////////////////////////////

bool ActionRecord::parseStringField(const string& line, size_t& currentPos, const string& fieldName, string& outValue, const string& delimiter) const
{
    size_t valuePos = jumpToFieldValue(line, currentPos, fieldName);
    if (valuePos == string::npos) {
        return false;  // Missing field
    }

    outValue = extractFieldValue(line, valuePos, delimiter);
    if (outValue.empty()) {
        return false;  // Empty value
    }

    currentPos = valuePos;  // Update position for next field
    return true;
}