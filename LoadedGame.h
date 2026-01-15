#pragma once

#include "Game.h"
#include "Recorder.h"
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class LoadedGame : public Game {

    RecordedSteps steps;

    // For silent mode result comparison
    std::vector<GameEvent> expectedEvents;
    size_t expectedEventIndex;
    bool testPassed;
    std::string testFailureDetails;

    // For quit handling
    long quitCycle;  // -1 if no quit, otherwise cycle to quit at

public:
    LoadedGame(const string &filename, bool silent = false);
    LoadedGame(int argc, char* argv[]);  // New: parses args and initializes
    void handleInput() override;
    void run() override;
    void gameLoop() override;

protected:
    // Event verification overrides (for silent mode)
    void recordScreenChange(int roomId) override;
    void recordLifeLost(int playerId) override;
    void recordRiddleAttempt(const std::string& question, int answer, bool correct) override;
    void recordQuit() override;

private:
    ErrorCode loadActions(const string& filename) { return steps.loadFromFile(filename); }
    ErrorCode loadExpectedResults(const string& filename);
    bool verifyEvent(const GameEvent& actual);
    void testFailed(const std::string& details);
    void checkMissedEvents();
    void showQuitScreen();
    bool shouldQuit() const { return (quitCycle >= 0 && cycleCount >= static_cast<unsigned long>(quitCycle)); }
};