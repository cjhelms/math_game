#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <iostream>

const int SCREEN_HEIGHT = 500, 
	SCREEN_WIDTH = 800;
enum Button_Type { PLAY_BUTTON, HIGH_SCORE_BUTTON, MAIN_MENU_BUTTON, QUIT_BUTTON, BACK_BUTTON, TRY_AGAIN_BUTTON, CHANGE_INITIALS_BUTTON, NULL_BUTTON };

struct High_Score
{
	int score;
	std::string initials;

	High_Score()
	{
		score = 0;
		initials = "---";
	}

	void set_equal(High_Score hs)
	{
		score = hs.score;
		initials = hs.initials;
	}

	void set_new_high_score(int s, std::string i)
	{
		score = s;
		initials = i;
	}
};

struct Button
{
	SDL_Rect loc;
	Button_Type type;

	Button()
	{
		loc.x = loc.y = loc.w = loc.h = 0;
		type = NULL_BUTTON;
	}

	void set_button(SDL_Rect rect, Button_Type t)
	{
		loc = rect;
		type = t;
	}
};

class Engine
{
private:
	SDL_Renderer* renderer;
	SDL_Window* window;
	SDL_Texture* text;
	SDL_Event e;
	TTF_Font* font;
	int width, 
		height, 
		x, 
		y;
	bool running_main_game, 
		running_main_menu, 
		quit;

public:
	Engine();
	~Engine();

	//Initializes game class
	void init();

	//Loads the font & converts to texture 'text'
	void load_media();

	//Handles user keyboard input; returns null char if no key has been pressed
	char handle_key_event();

	//Handles user mouse input; returns true if button activated
	bool handle_mouse_event(Button* b);

	//Renders string to the window with specified color & location
	void render_text(std::string message, SDL_Color text_color, int win_pos_x, int win_pos_y);

	//Returns rectangle that contains current texture in window
	SDL_Rect texture_outline();

	//Draws rectangles to window
	void draw_rect(SDL_Rect* win_pos, SDL_Color* color);

	//Erases all textures on window
	void clear_screen();

	//Updates all textures currently rendered to window
	void update();

	//Destroys old 'text' textures
	void clean();

	SDL_Event get_event();
	int get_width();
	int get_height();
	int get_x();
	int get_y();
	bool get_quit();
	bool get_running_main_game();
	bool get_running_main_menu();

	void set_quit(bool q);
	void set_running_main_game(bool run);
	void set_running_main_menu(bool run);
};

Engine::Engine()
{
	renderer = NULL;
	window = NULL;
	text = NULL;
	font = NULL;
	width = height = 1;
	running_main_game = quit = false;
	running_main_menu = true;
}

Engine::~Engine()
{
	SDL_DestroyWindow(window);
	window = NULL;

	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	TTF_CloseFont(font);
	font = NULL;

	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
}

void Engine::init()
{
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	window = SDL_CreateWindow("Math Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
							  SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (window == NULL || renderer == NULL)
	{
		printf("ERROR: Init has failed - %s\n", SDL_GetError());
	}
}

void Engine::load_media()
{
	font = TTF_OpenFont("ostrich-regular.ttf", 42);

	if (font == NULL)
	{
		printf("FAIL: load_media has failed - %s\n", TTF_GetError());
	}
}

char Engine::handle_key_event()
{
	char current_key = '\0';
	
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			quit = true;
		}
		else if (e.type == SDL_KEYDOWN)
		{
			if (e.key.keysym.sym == SDLK_0 || e.key.keysym.sym == SDLK_KP_0) { current_key = '0'; }
			if (e.key.keysym.sym == SDLK_1 || e.key.keysym.sym == SDLK_KP_1) { current_key = '1'; }
			if (e.key.keysym.sym == SDLK_2 || e.key.keysym.sym == SDLK_KP_2) { current_key = '2'; }
			if (e.key.keysym.sym == SDLK_3 || e.key.keysym.sym == SDLK_KP_3) { current_key = '3'; }
			if (e.key.keysym.sym == SDLK_4 || e.key.keysym.sym == SDLK_KP_4) { current_key = '4'; }
			if (e.key.keysym.sym == SDLK_5 || e.key.keysym.sym == SDLK_KP_5) { current_key = '5'; }
			if (e.key.keysym.sym == SDLK_6 || e.key.keysym.sym == SDLK_KP_6) { current_key = '6'; }
			if (e.key.keysym.sym == SDLK_7 || e.key.keysym.sym == SDLK_KP_7) { current_key = '7'; }
			if (e.key.keysym.sym == SDLK_8 || e.key.keysym.sym == SDLK_KP_8) { current_key = '8'; }
			if (e.key.keysym.sym == SDLK_9 || e.key.keysym.sym == SDLK_KP_9) { current_key = '9'; }
			if (e.key.keysym.sym == SDLK_a) { current_key = 'A'; }
			if (e.key.keysym.sym == SDLK_b) { current_key = 'B'; }
			if (e.key.keysym.sym == SDLK_c) { current_key = 'C'; }
			if (e.key.keysym.sym == SDLK_d) { current_key = 'D'; }
			if (e.key.keysym.sym == SDLK_e) { current_key = 'E'; }
			if (e.key.keysym.sym == SDLK_f) { current_key = 'F'; }
			if (e.key.keysym.sym == SDLK_g) { current_key = 'G'; }
			if (e.key.keysym.sym == SDLK_h) { current_key = 'H'; }
			if (e.key.keysym.sym == SDLK_i) { current_key = 'I'; }
			if (e.key.keysym.sym == SDLK_j) { current_key = 'J'; }
			if (e.key.keysym.sym == SDLK_k) { current_key = 'K'; }
			if (e.key.keysym.sym == SDLK_l) { current_key = 'L'; }
			if (e.key.keysym.sym == SDLK_m) { current_key = 'M'; }
			if (e.key.keysym.sym == SDLK_n) { current_key = 'N'; }
			if (e.key.keysym.sym == SDLK_o) { current_key = 'O'; }
			if (e.key.keysym.sym == SDLK_p) { current_key = 'P'; }
			if (e.key.keysym.sym == SDLK_q) { current_key = 'Q'; }
			if (e.key.keysym.sym == SDLK_r) { current_key = 'R'; }
			if (e.key.keysym.sym == SDLK_s) { current_key = 'S'; }
			if (e.key.keysym.sym == SDLK_t) { current_key = 'T'; }
			if (e.key.keysym.sym == SDLK_u) { current_key = 'U'; }
			if (e.key.keysym.sym == SDLK_v) { current_key = 'V'; }
			if (e.key.keysym.sym == SDLK_w) { current_key = 'W'; }
			if (e.key.keysym.sym == SDLK_x) { current_key = 'X'; }
			if (e.key.keysym.sym == SDLK_y) { current_key = 'Y'; }
			if (e.key.keysym.sym == SDLK_z) { current_key = 'Z'; }
			if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER) { current_key = '\r'; }
			if (e.key.keysym.sym == SDLK_MINUS || e.key.keysym.sym == SDLK_KP_MINUS) { current_key = '-'; }
			if (e.key.keysym.sym == SDLK_TAB) { current_key = '\t'; }
			if (e.key.keysym.sym == SDLK_BACKSPACE) { current_key = '\b'; }
			if (e.key.keysym.sym == SDLK_ESCAPE) { quit = true; }
		}
	}

	return current_key;
}

bool Engine::handle_mouse_event(Button* b)
{
	int mouse_x = 0, mouse_y = 0;
	bool activate = false;
	SDL_Rect underline = { b->loc.x, b->loc.y + b->loc.h, b->loc.w, 3 };

	SDL_GetMouseState(&mouse_x, &mouse_y);

	if (mouse_x > b->loc.x && mouse_x < b->loc.x + b->loc.w && mouse_y > b->loc.y && mouse_y < b->loc.y + b->loc.h)
	{
		if (e.type == SDL_MOUSEMOTION)
		{
			SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
			SDL_RenderFillRect(renderer, &underline);
			update();
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			activate = true;
		}
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &underline);
		update();
	}

	return activate;
}

void Engine::render_text(std::string message, SDL_Color text_color, int win_pos_x, int win_pos_y)
{
	clean();

	SDL_Surface* loaded_text = NULL;
	loaded_text = TTF_RenderText_Blended(font, message.c_str(), text_color);

	text = SDL_CreateTextureFromSurface(renderer, loaded_text);

	width = loaded_text->w;
	height = loaded_text->h;
	x = win_pos_x;
	y = win_pos_y;

	SDL_FreeSurface(loaded_text);
	loaded_text = NULL;

	SDL_Rect dst_loc = { x, y, width, height };

	SDL_RenderCopy(renderer, text, NULL, &dst_loc);

	if (text == NULL)
	{
		printf("FAIL: render_text failed - %s\n", SDL_GetError());
	}
}

SDL_Rect Engine::texture_outline()
{
	SDL_Rect outline = { x, y, width, height };
	return outline;
}

void Engine::draw_rect(SDL_Rect* win_pos, SDL_Color* color)
{
	SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
	SDL_RenderFillRect(renderer, win_pos);
}


void Engine::clear_screen()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
}

void Engine::update()
{
	SDL_RenderPresent(renderer);
}

void Engine::clean()
{
	if (text != NULL)
	{
		SDL_DestroyTexture(text);
		text = NULL;
	}
}

SDL_Event Engine::get_event()
{
	return e;
}

int Engine::get_width()
{
	return width;
}

int Engine:: get_height()
{
	return height;
}

int Engine::get_x()
{
	return x;
}

int Engine::get_y()
{
	return y;
}

bool Engine::get_quit()
{
	return quit;
}

bool Engine::get_running_main_game()
{
	return running_main_game;
}

bool Engine::get_running_main_menu()
{
	return running_main_menu;
}

void Engine::set_quit(bool q)
{
	quit = q;
}

void Engine::set_running_main_game(bool run)
{
	running_main_game = run;
}

void Engine::set_running_main_menu(bool run)
{
	running_main_menu = run;
}