#pragma once

#include "Engine.h"
#include <string>
#include <iostream>
#include <fstream>

const int NUM_MAIN_MENU_BUTTONS = 4,
	NUM_HIGH_SCORES = 3;

class Main_Menu
{
private:
	bool display_main_menu, 
		display_high_score,
		change_initials;
	char user_input;
	int initials_counter;
	Button main_menu_button[NUM_MAIN_MENU_BUTTONS], 
		high_score_button;
	SDL_Color color_title = { 0, 100, 200, 255 }, 
		color_options = { 0, 0, 0, 255 },
		color_erase = { 255, 255, 255, 255 };
	High_Score score, 
		high_score[NUM_HIGH_SCORES];
	std::fstream high_score_document;

public:
	Main_Menu();

	//Initializes main menu
	void init(Engine* g);

	//Plays main menu
	void play(Engine* g);

	//Draws main menu screen
	void draw_main_menu(Engine* g);

	//Draws high score screen
	void draw_high_score(Engine* g);

	//Handle button actions
	void handle_buttons(Engine* g);

	std::string get_initials();
	High_Score get_high_score();
	void set_score(High_Score s);
};

Main_Menu::Main_Menu()
{
	display_high_score = change_initials = false;
	display_main_menu = true;
	user_input = '\0';
	initials_counter = 0;

	high_score_document.open("high_scores.txt");

	high_score_document >> high_score[0].initials;
	high_score_document >> high_score[0].score;

	high_score_document >> high_score[1].initials;
	high_score_document >> high_score[1].score;

	high_score_document >> high_score[2].initials;
	high_score_document >> high_score[2].score;

	high_score_document.close();
}

void Main_Menu::init(Engine* g)
{
	display_high_score = change_initials = false;
	display_main_menu = true;
	initials_counter = 0;

	bool new_high_score = false;
	High_Score temp_a, temp_b;

	for (int i = 0; i < NUM_HIGH_SCORES; i++)
	{	
		if (score.score > high_score[i].score && !new_high_score)
		{
			temp_a.set_equal(high_score[i]);
			high_score[i].set_equal(score);
			new_high_score = true;
		}
		
		if (new_high_score && i < 2)
		{
			temp_b.set_equal(high_score[i+1]);
			high_score[i+1].set_equal(temp_a);
			temp_a.set_equal(temp_b);
		}
		else if(i == 2)
		{
			high_score_document.open("high_scores.txt");

			high_score_document << high_score[0].initials << " ";
			high_score_document << high_score[0].score << std::endl;

			high_score_document << high_score[1].initials << " ";
			high_score_document << high_score[1].score << std::endl;

			high_score_document << high_score[2].initials << " ";
			high_score_document << high_score[2].score << std::endl;

			high_score_document.close();
		}
	}

	for (int i = 0; i < NUM_HIGH_SCORES - 1; i++)
	{
		for (int j = 0; j < NUM_HIGH_SCORES - i - 1; j++)
		{
			if (high_score[j].score < high_score[j + 1].score)
			{
				std::swap(high_score[j], high_score[j + 1]);
			}
		}
	}

	draw_main_menu(g);
}

void Main_Menu::play(Engine* g)
{	
	user_input = g->handle_key_event();

	SDL_Rect erase_initials = { 175, 450, 300, g->get_height() };

	if (change_initials && user_input != '\0')
	{
		
		score.initials.at(initials_counter) = user_input;
		g->draw_rect(&erase_initials, &color_erase);
		g->render_text(score.initials, color_options, 175, 450);
		g->update();

		initials_counter++;

		if (initials_counter == 3)
		{
			change_initials = false;
			initials_counter = 0;
		}
	}

	handle_buttons(g);
}

void Main_Menu::draw_main_menu(Engine* g)
{
	g->clear_screen();

	g->render_text("The", color_title, 100, 150);
	g->render_text("Math", color_title, 150, 200);
	g->render_text("Game", color_title, 200, 250);

	g->render_text("Play", color_options, 500, 100);
	main_menu_button[0].set_button(g->texture_outline(), PLAY_BUTTON);

	g->render_text("High-Score", color_options, 500, 200);
	main_menu_button[1].set_button(g->texture_outline(), HIGH_SCORE_BUTTON);

	g->render_text("Quit", color_options, 500, 300);
	main_menu_button[2].set_button(g->texture_outline(), QUIT_BUTTON);

	g->render_text("Change Initials", color_options, 100, 400);
	main_menu_button[3].set_button(g->texture_outline(), CHANGE_INITIALS_BUTTON);

	g->render_text(score.initials, color_options, 175, 450);

	g->update();
}

void Main_Menu::draw_high_score(Engine* g)
{
	g->clear_screen();
	
	g->render_text("Back", color_options, 650, 450);
	high_score_button.set_button(g->texture_outline(), BACK_BUTTON);

	g->render_text(high_score[0].initials, color_options, 300, 100);
	g->render_text(high_score[1].initials, color_options, 300, 200);
	g->render_text(high_score[2].initials, color_options, 300, 300);

	g->render_text(std::to_string(high_score[0].score), color_options, 400, 100);
	g->render_text(std::to_string(high_score[1].score), color_options, 400, 200);
	g->render_text(std::to_string(high_score[2].score), color_options, 400, 300);

	g->update();
}

void Main_Menu::handle_buttons(Engine* g)
{
	for (int i = 0; display_main_menu && i < NUM_MAIN_MENU_BUTTONS; i++)
	{
		if (g->handle_mouse_event(&main_menu_button[i]))
		{
			switch (main_menu_button[i].type)
			{
				case PLAY_BUTTON: g->set_running_main_game(true);
					g->set_running_main_menu(false);
					break;
				case HIGH_SCORE_BUTTON: display_high_score = true;
					display_main_menu = false;
					draw_high_score(g);
					break;
				case QUIT_BUTTON: g->set_quit(true);
					break;
				case CHANGE_INITIALS_BUTTON: change_initials = true;
					break;
			}
		}
	}

	if (display_high_score && g->handle_mouse_event(&high_score_button))
	{
		if (high_score_button.type == BACK_BUTTON)
		{
			display_main_menu = true;
			display_high_score = false;
			draw_main_menu(g);
		}
	}
}

High_Score Main_Menu::get_high_score()
{
	return high_score[0];
}

std::string Main_Menu::get_initials()
{
	return score.initials;
}

void Main_Menu::set_score(High_Score s)
{
	score.set_equal(s);
}