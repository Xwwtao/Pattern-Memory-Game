#include "View_Game.h"
#include "splashkit.h"
#include <string>

using std::to_string;

//draw wrong flash
void draw_wrong_flash(const GameData &g)
{
    if (g.state != GameState::FEEDBACK) return;
    if (g.feedbackCorrect) return;

    double t = timer_ticks(g.patternTimer) / 1000.0;
    if (t > g.wrongFlashSeconds) return;

    color c = rgba_color(255, 0, 0, g.wrongFlash);
    int thickness = 12;

    fill_rectangle(c, 0, 0, screen_width(), thickness);
    fill_rectangle(c, 0, screen_height() - thickness, screen_width(), thickness);
    fill_rectangle(c, 0, 0, thickness, screen_height());
    fill_rectangle(c, screen_width() - thickness, 0, thickness, screen_height());
}

//click feedback
void draw_click_feedback(const GameData &g)
{
    if (g.state != GameState::FEEDBACK) return;
    if (g.feedbackIndex < 0) return;

    Cell* c = g.grid[g.feedbackIndex];
    c->draw_highlight(g.feedbackCorrect ? COLOR_GREEN : COLOR_RED);
}

//draw grid
void draw_grid(const GameData &g)
{
    for (Cell* c : g.grid)
        c->draw();
}

//draw pattern overlay
void draw_pattern_overlay(const GameData &g)
{
    if (g.state != GameState::SHOW_PATTERN) return;
    if (g.patternSeq.empty()) return;

    int idx = g.patternSeq[g.showIndex];
    g.grid[idx]->draw_highlight(COLOR_BLUE);
}

//draw UI
void draw_ui(const GameData &g)
{
    draw_text("Player: " + g.playerName, COLOR_BLUE, 10, 10);
    draw_text("Level: " + to_string(g.level), COLOR_BLACK, 10, 30);
    draw_text("Score: " + to_string(g.score), COLOR_BLACK, 10, 50);
    draw_text("Lives: " + to_string(g.lives), COLOR_BLACK, 10, 70);

    if (g.state == GameState::INPUT_NAME)
    {
        draw_text("Welcome to Pattern Memory!", COLOR_BLACK, 300, 300);
        draw_text("Please Type Your Name: ", COLOR_BLACK, 300, 340);

        draw_rectangle(COLOR_GRAY, 300, 370, 200, 30);
        draw_text(g.playerName + "_", COLOR_BLUE, 310, 378);

        draw_text("Press ENTER to start", COLOR_DARK_GRAY, 300, 420);
        draw_text("Press UP to view leaderboard", COLOR_DARK_GRAY, 300, 440);
        draw_text("Press ESC to leave the game", COLOR_DARK_GRAY, 300, 460);
    }
    else if (g.state == GameState::READY)
        draw_text("Press SPACE to start", COLOR_BLACK, 10, 90);
    else if (g.state == GameState::WAIT_INPUT)
        draw_text("Click the pattern in order!", COLOR_BLACK, 10, 90);
    else if (g.state == GameState::GAME_OVER)
    {
        draw_text("Game Over!", COLOR_RED, 10, 100);
        draw_text("Final Score: " + to_string(g.score), COLOR_BLACK, 10, 140);
        draw_text("Reached Level: " + to_string(g.level), COLOR_BLACK, 10, 160);
        draw_text("Press SPACE to restart", COLOR_BLACK, 10, 200);
        draw_text("Press UP to view leaderboard", COLOR_BLUE, 10, 220);
        draw_text("Press N to set a new player", COLOR_GREEN, 10, 240);
        draw_text("Press ESC to leave the game", COLOR_DARK_GRAY, 10, 260);
    }
    else if (g.state == GameState::SHOW_LEADERBOARD)
    {
        draw_text("=== Leaderboard ===", COLOR_BLACK, 320, 200);

        auto entries = g.leaderboard.get_entries();
        int y = 240;
        for (const auto &e : entries)
        {
            draw_text(e.name + " ..... " + to_string(e.score), COLOR_BLACK, 320, y);
            y += 20;
        }

        draw_text("Press SPACE to play again", COLOR_RED, 300, 500);
        draw_text("Press N to set a new player", COLOR_GREEN, 300, 520);
        draw_text("Press ESC to leave the game", COLOR_DARK_GRAY, 300, 540);
    }
    else if (!g.statusText.empty() &&
             timer_ticks(g.statusTimer) / 1000.0 < g.statusSeconds)
    {
        draw_text(g.statusText, COLOR_RED, 10, 130);
    }
}
