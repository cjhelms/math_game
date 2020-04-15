#pragma once

#include "Engine.h"
#include <string>
#include <stdlib.h>
#include <sstream>

const int NUM_MAIN_GAME_BUTTONS = 3, 
	TIME_LIMIT = 5000;

class Main_Game
{
private:
	SDL_Color color_words = { 0, 0, 0, 255 }, 
		color_equation = { 0, 100, 200, 255 },
		color_game_over = { 200, 0, 0, 255 }, 
		color_erase = { 255, 255, 255, 255 };
	SDL_Rect erase_equation, 
		erase_answer, 
		erase_score, 
		erase_high_score;
	char user_input;
	int score,
		user_answer_int, 
		solution, 
		coeff_a, 
		coeff_b, 
		timer, 
		width;
	High_Score new_high_score,
		high_score,
		current_score;
	std::string user_answer, 
		equation;
	std::stringstream convert;
	Button button[NUM_MAIN_GAME_BUTTONS];
	bool user_correct, 
		game_over, 
		start;
	
public:
	Main_Game();

	//Initializes main game
	void init(Engine* g);

	//Plays main game
	void play(Engine* g);

	//Creates new equation
	void new_equation();

	//Handles button actions
	void handle_buttons(Engine* g);

	void set_initials(std::string i);
	High_Score get_current_score();
	void set_high_score(High_Score hs);
};

Main_Game::Main_Game()
{
	user_input = '\0';
	user_answer = "";
	user_answer_int = 0;
	user_correct = game_over = start = false;
}

void Main_Game::init(Engine* g)
{
	g->clear_screen();

	g->render_text("Enter the solution within 5 seconds to earn a point!", color_words, 50, 50);
	
	g->render_text("Current Score: ", color_words, 100, 125);
	erase_score.x = g->get_width() + 100;
	erase_score.y = 125;
	erase_score.w = 300;
	erase_score.h = g->get_height();
	g->render_text(std::to_string(score), color_equation, erase_score.x, erase_score.y);

	g->render_text("Current High Score: ", color_words, 100, 175);
	erase_high_score.x = g->get_width() + 100;
	erase_high_score.y = 175;
	erase_high_score.w = 300;
	erase_high_score.h = g->get_height();
	g->render_text(std::to_string(high_score.score), color_equation, erase_high_score.x, erase_high_score.y);

	g->render_text("Main Menu", color_words, 650, 350);
	button[0].set_button(g->texture_outline(), MAIN_MENU_BUTTON);

	g->render_text("Try Again", color_words, 650, 400);
	button[1].set_button(g->texture_outline(), TRY_AGAIN_BUTTON);

	g->render_text("Quit", color_words, 650, 450);
	button[2].set_button(g->texture_outline(), QUIT_BUTTON);

	srand(SDL_GetTicks());
	user_answer = "";
	score = 0;
	user_correct = game_over = start = false;

	g->render_text("Press ENTER to begin", color_words, 100, 250);

	g->update();
}

void Main_Game::play(Engine* g)
{
	user_input = g->handle_key_event();

	if (!start)
	{
		if (user_input == '\r')
		{
			SDL_Rect begin = { 100, 250, 300, g->get_height() };
			g->draw_rect(&begin, &color_erase);
			
			new_equation();
			g->render_text(equation, color_equation, 100, 250);
			width = g->get_width();

			erase_equation.x = 100;
			erase_answer.x = 100 + width;
			erase_equation.y = erase_answer.y = 250;
			erase_equation.h = erase_answer.h = g->get_height();
			erase_equation.w = width;
			erase_answer.w = 300;

			g->update();

			timer = SDL_GetTicks();
			start = true;
		}
	}
	else if (user_input == '\b' && user_answer.length() > 0 && !game_over)
	{
		user_answer.pop_back();

		g->draw_rect(&erase_answer, &color_erase);

		if (user_answer.length() > 0)
		{
			g->render_text(user_answer, color_equation, 100 + width, 250);
		}

		g->update();
	}
	else if (user_input == '\r' && !game_over)
	{
		convert.clear();
		convert.str(user_answer);
		convert >> user_answer_int;
		user_answer = "";

		if (user_answer_int == solution)
		{
			g->draw_rect(&erase_answer, &color_erase);
			user_correct = true;
		}
		else
		{
			g->render_text("Game Over!", color_game_over, 100, 325);
			g->render_text("Incorrect answer :(", color_game_over, 100, 375);
			g->render_text("Correct answer was ", color_game_over, 100, 425);
			g->render_text(std::to_string(solution), color_game_over, 400, 425);

			current_score.score = score;

			game_over = true;
		}
	}
	else if (user_input != '\0' && user_answer.length() < 5 && !game_over)
	{
		user_answer.push_back(user_input);

		g->draw_rect(&erase_answer, &color_erase);
		g->render_text(user_answer, color_equation, 100 + width, 250);
		g->update();
	}

	if (user_correct && SDL_GetTicks() - timer < TIME_LIMIT && start)
	{
		timer = SDL_GetTicks();
		score++;

		if (score > high_score.score)
		{
			high_score.score = score;
		}

		g->draw_rect(&erase_score, &color_erase);
		g->draw_rect(&erase_high_score, &color_erase);
		g->render_text(std::to_string(score), color_equation, erase_score.x, erase_score.y);
		g->render_text(std::to_string(high_score.score), color_equation, erase_high_score.x, erase_high_score.y);

		g->draw_rect(&erase_equation, &color_erase);
		new_equation();
		g->render_text(equation, color_equation, 100, 250);
		width = g->get_width();

		erase_equation.w = width;
		erase_answer.x = 100 + width;
		
		g->update();
		
		user_correct = false;
	}
	else if (SDL_GetTicks() - timer >= TIME_LIMIT && start && !game_over)
	{
		g->render_text("Game Over!", color_game_over, 100, 325);
		g->render_text("Out of time :(", color_game_over, 100, 375);
		g->render_text("Correct answer was ", color_game_over, 100, 425);
		g->render_text(std::to_string(solution), color_game_over, 400, 425);

		current_score.score = score;

		game_over = true;
	}

	handle_buttons(g);
} 

void Main_Game::new_equation()
{
	coeff_a = rand() % 100;
	coeff_b = rand() % 100;

	switch (rand() % 3)
	{
		case 0: equation = std::to_string(coeff_a) + " + " + std::to_string(coeff_b) + " = ";
			solution = coeff_a + coeff_b;
			break;
		case 1: equation = std::to_string(coeff_a) + " - " + std::to_string(coeff_b) + " = ";
			solution = coeff_a - coeff_b;
			break;
		case 2: coeff_a %= 12;
			coeff_b %= 12;
			equation = std::to_string(coeff_a) + " * " + std::to_string(coeff_b) + " = ";
			solution = coeff_a * coeff_b;
			break;
	}
}

void Main_Game::handle_buttons(Engine* g)
{
	for (int i = 0; i < NUM_MAIN_GAME_BUTTONS; i++)
	{
		if (g->handle_mouse_event(&button[i]))
		{
			switch (button[i].type)
			{
				case MAIN_MENU_BUTTON: g->set_running_main_game(false);
					g->set_running_main_menu(true);
					current_score.score = score;
					score = 0;
					break;
				case TRY_AGAIN_BUTTON: init(g);
					break;
				case QUIT_BUTTON: g->set_quit(true);
					break;
			}
		}
	}
}

void Main_Game::set_initials(std::string i)
{
	current_score.initials = i;
}

High_Score Main_Game::get_current_score()
{
	return current_score;
}

void Main_Game::set_high_score(High_Score hs)
{
	hs = high_score;
}