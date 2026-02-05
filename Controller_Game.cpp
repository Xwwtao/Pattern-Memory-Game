#include "splashkit.h"
#include "Controller_Game.h"
#include "Model_Game.h"
#include "View_Game.h"

void run_game()
{
    open_window("Pattern Memory", 800, 800);

    GameData g;

    //set timers
    create_timer(g.patternTimer);
    start_timer(g.patternTimer);

    create_timer(g.statusTimer);
    start_timer(g.statusTimer);

    //init grid
    generate_centered_grid(g, 90);
    generate_pattern(g, 2 + g.level);

    start_reading_text(rectangle_from(0, 0, 800, 800));

    while (!quit_requested())
    {
        process_events();

        if (key_typed(ESCAPE_KEY))
        {
            if (reading_text()) end_reading_text();
            break;
        }

        
        //input (Controller)
        if (g.state == GameState::INPUT_NAME)
        {
            if (reading_text()) g.playerName = text_input();

            if (key_typed(RETURN_KEY))
            {
                if (g.playerName.empty()) g.playerName = "Guest";
                end_reading_text();
                g.state = GameState::READY;
            }
            else if (key_typed(UP_KEY))
            {
                end_reading_text();
                g.state = GameState::SHOW_LEADERBOARD;
            }
        }
        else if (g.state == GameState::READY)
        {
            if (key_typed(SPACE_KEY))
            {
                g.level = 1;
                g.score = 0;
                g.lives = 3;

                g.gridSize = grid_size_for_level(g.level);
                generate_centered_grid(g, 90);
                generate_pattern(g, pattern_length_for_level(g.level));

                g.state = GameState::SHOW_PATTERN;
                reset_timer(g.patternTimer);
                start_timer(g.patternTimer);
            }
        }
        else if (g.state == GameState::WAIT_INPUT && mouse_clicked(LEFT_BUTTON))
        {
            int idx = hit_test_cell(g, mouse_x(), mouse_y());
            handle_click(g, idx);
        }
        else if (g.state == GameState::GAME_OVER)
        {
            if (key_typed(UP_KEY))
            {
                g.state = GameState::SHOW_LEADERBOARD;
            }
            else if (key_typed(SPACE_KEY))
            {
                string savedName = g.playerName;
                cleanup_grid(g);
                g = GameData();              // reset everything
                g.playerName = savedName;
                g.state = GameState::READY;
            }
            else if (key_typed(N_KEY))
            {
                cleanup_grid(g);
                g = GameData();
                g.state = GameState::INPUT_NAME;
                start_reading_text(rectangle_from(0, 0, 800, 800));
            }
        }
        else if (g.state == GameState::SHOW_LEADERBOARD)
        {
            if (key_typed(SPACE_KEY))
            {
                string savedName = g.playerName;
                cleanup_grid(g);
                g = GameData();
                g.playerName = savedName;
                g.state = GameState::READY;
            }
            else if (key_typed(N_KEY))
            {
                cleanup_grid(g);
                g = GameData();
                g.state = GameState::INPUT_NAME;
                start_reading_text(rectangle_from(0, 0, 800, 800));
            }
        }

        //update
        update_state(g);

        
        //draw (View)
        clear_screen(COLOR_WHITE);

        if (g.state != GameState::INPUT_NAME &&
            g.state != GameState::SHOW_LEADERBOARD &&
            g.state != GameState::GAME_OVER)
        {
            draw_grid(g);
            draw_pattern_overlay(g);
            draw_click_feedback(g);
            draw_wrong_flash(g);
        }

        draw_ui(g);
        refresh_screen(60);
    }

    cleanup_grid(g);
}
