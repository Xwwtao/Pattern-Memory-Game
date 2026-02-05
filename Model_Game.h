#pragma once
#include "splashkit.h"
#include <vector>
#include <string>

#include "Model_Cell.h"
#include "Model_Leaderboard.h"

using std::vector;
using std::string;

enum class GameState
{
    INPUT_NAME,
    READY,
    SHOW_PATTERN,
    WAIT_INPUT,
    FEEDBACK,
    RESULT,
    GAME_OVER,
    SHOW_LEADERBOARD
};

struct GameData
{
    // dynamic grid
    int gridSize = 2;
    vector<Cell*> grid;

    // pattern sequence
    int currentStep = 0;
    vector<int> patternSeq;

    // gameplay
    int level = 1;
    int score = 0;
    int lives = 3;
    int showIndex = 0;

    // state machine
    GameState state = GameState::INPUT_NAME;

    // timing
    string patternTimer = "patternTimer";
    double showSeconds = 1.5;
    double resultSeconds = 1;
    double stepSeconds = 0.8;

    // click feedback
    int feedbackIndex = -1;
    bool feedbackCorrect = false;
    double feedbackSeconds = 0.3;
    bool roundFinished = false;

    // warning/status
    string statusText = "";
    double statusSeconds = 0.6;
    string statusTimer = "statusTimer";
    double wrongFlashSeconds = 0.25;
    int wrongFlash = 80;

    // player info
    string playerName = "";
    Leaderboard leaderboard;
};

// memory management helper
void cleanup_grid(GameData &g);

// grid and hit test
void generate_grid(GameData &g, double startX, double startY, double cellSize);
void generate_centered_grid(GameData &g, double cellSize);
int  hit_test_cell(const GameData &g, double mx, double my);

// difficulty settings
int  pattern_length_for_level(int level);
int  grid_size_for_level(int level);

// pattern and click handling
void generate_pattern(GameData &g, int length);
void set_status(GameData &g, const string &msg);
void handle_click(GameData &g, int clickedIndex);

// state machine update
void update_state(GameData &g);
