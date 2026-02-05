#include "Model_Game.h"

//free heap memory for cells
void cleanup_grid(GameData &g)
{
    for (Cell* c : g.grid) delete c;
    g.grid.clear();
}

//generate grid
void generate_grid(GameData &g, double startX, double startY, double cellSize)
{
    cleanup_grid(g);

    for (int row = 0; row < g.gridSize; row++)
    {
        for (int col = 0; col < g.gridSize; col++)
        {
            int index = row * g.gridSize + col;

            // keep your original cell type
            Cell* cell = nullptr;
            //decide wich is basic or circle cell
            if (index % 2 == 0)
                cell = new SquareCell(startX + col * cellSize, startY + row * cellSize, cellSize, index);
            else
                cell = new CircleCell(startX + col * cellSize, startY + row * cellSize, cellSize, index);

            g.grid.push_back(cell);
            
        }
    }
}

void generate_centered_grid(GameData &g, double cellSize)
{
    double gridPixelSize = g.gridSize * cellSize;
    double startX = (screen_width() - gridPixelSize) / 2;
    double startY = (screen_height() - gridPixelSize) / 2;

    generate_grid(g, startX, startY, cellSize);
}

//mouse hit test
int hit_test_cell(const GameData &g, double mx, double my)
{
    for (Cell* c : g.grid)
    {
        if (c->contains(mx, my))
            return c->get_index();
    }
    return -1;
}

//pattern length for different level
int pattern_length_for_level(int level)
{
    return 2 + (level - 1) / 2;
}

//grid sizes for different level
int grid_size_for_level(int level)
{
    if (level <= 3)  return 2;
    if (level <= 6)  return 3;
    if (level <= 10) return 4;
    return 5;
}

//generate pattern sequence
void generate_pattern(GameData &g, int length)
{
    g.patternSeq.clear();
    g.patternSeq.reserve(length);

    while ((int)g.patternSeq.size() < length)
    {
        int idx = rnd((int)g.grid.size());
        bool used = false;

        for (int v : g.patternSeq)
        {
            if (v == idx)
            {
                used = true;
                break;
            }
        }

        if (!used)
            g.patternSeq.push_back(idx);
    }

    g.currentStep = 0;
    g.showIndex = 0;
}

void set_status(GameData &g, const string &m)
{
    g.statusText = m;
    reset_timer(g.statusTimer);
    start_timer(g.statusTimer);
}

//handle click in WAIT_INPUT
void handle_click(GameData &g, int clickedIndex)
{
    if (clickedIndex < 0) return; // click empty

    int expected = g.patternSeq[g.currentStep];

    g.feedbackIndex = clickedIndex;
    g.roundFinished = false;

    if (clickedIndex == expected)
    {
        g.feedbackCorrect = true;
        g.currentStep++;
        
        //depends on kind of cell
        g.score += g.grid[clickedIndex]->score_value();


        if (g.currentStep >= (int)g.patternSeq.size())
        {
            g.roundFinished = true;
            set_status(g, "Nice! Next level.");
        }
    }
    else
    {
        g.feedbackCorrect = false;
        g.lives--;
        set_status(g, "Wrong! Life -1.");
        g.roundFinished = true;
    }

    g.state = GameState::FEEDBACK;
    reset_timer(g.patternTimer);
    start_timer(g.patternTimer);
}

//state machine update
void update_state(GameData &g)
{
    if (g.state == GameState::SHOW_PATTERN)
    {
        double time = timer_ticks(g.patternTimer) / 1000.0;

        if (time >= g.stepSeconds)
        {
            g.showIndex++;
            reset_timer(g.patternTimer);
            start_timer(g.patternTimer);

            if (g.showIndex >= (int)g.patternSeq.size())
            {
                g.state = GameState::WAIT_INPUT;
                g.showIndex = 0;
                reset_timer(g.patternTimer);
                start_timer(g.patternTimer);
            }
        }
    }
    else if (g.state == GameState::RESULT)
    {
        if (timer_ticks(g.patternTimer) / 1000.0 >= g.resultSeconds)
        {
            g.level++;
            int newLen = pattern_length_for_level(g.level);
            g.gridSize = grid_size_for_level(g.level);

            generate_centered_grid(g, 90);
            generate_pattern(g, newLen);

            g.state = GameState::SHOW_PATTERN;
            g.showIndex = 0;
            reset_timer(g.patternTimer);
            start_timer(g.patternTimer);
        }
    }
    else if (g.state == GameState::FEEDBACK)
    {
        if (timer_ticks(g.patternTimer) / 1000.0 >= g.feedbackSeconds)
        {
            g.feedbackIndex = -1;

            if (g.lives <= 0)
            {
                g.state = GameState::GAME_OVER;
                g.leaderboard.add_score(g.playerName, g.score);
                return;
            }

            if (g.roundFinished)
            {
                g.state = GameState::RESULT;
                reset_timer(g.patternTimer);
                start_timer(g.patternTimer);
            }
            else
            {
                g.state = GameState::WAIT_INPUT;
            }
        }
    }
}
