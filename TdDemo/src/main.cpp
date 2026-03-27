#define SDL_MAIN_HANDLED
#include<iostream>
#include"Game_manager.h"


int main(int argc, char* argv){
	return  Game_manager::instance()->run(argc, argv);
}
