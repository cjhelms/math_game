#include "Engine.h"
#include "Main_Game.h"
#include "Main_Menu.h"
#include <stdio.h>

int main(int argc, char* args[])
{
	Engine game;
	Main_Game main_game;
	Main_Menu main_menu;

	game.init();
	game.load_media();

	while(!game.get_quit())
	{
		if (game.get_running_main_game())
		{
			main_game.set_high_score(main_menu.get_high_score());
			main_game.set_initials(main_menu.get_initials());
			main_game.init(&game);

			while (game.get_running_main_game() && !game.get_quit())
			{
				main_game.play(&game);
			}
		}

		else if (game.get_running_main_menu()) 
		{
			main_menu.set_score(main_game.get_current_score());
			main_menu.init(&game);

			while (game.get_running_main_menu() && !game.get_quit())
			{
				main_menu.play(&game);
			}
		}
	}
	return 0;
}
