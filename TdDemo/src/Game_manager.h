#ifndef _GAME_MANAGER_H_
#include "manager.h"
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>

class Game_manager : public Manager<Game_manager>
{
	friend class Manager<Game_manager>;
public:
	int run(int argc, char* argv) {
		Uint64 last_time = SDL_GetPerformanceCounter();
		const Uint64 freq = SDL_GetPerformanceFrequency();
		int fps = 60;
		while(! is_quit) {
			while (SDL_PollEvent(&event)) {
				on_input();
			}
			Uint64 current_time = SDL_GetPerformanceCounter();
			double delta_time = (double)(current_time - last_time) / freq;
			last_time = current_time;
			if(delta_time*1000<1000.0/fps) {
				SDL_Delay((Uint32)(1000.0 / fps - delta_time * 1000));
			}
			on_update(delta_time);

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			on_render();
			SDL_RenderPresent(renderer);
		}
		return 0;
	}
protected:
	Game_manager() {
		init_assert(!SDL_Init(SDL_INIT_EVERYTHING),u8"SDL³õÊ¼»¯Ê§°Ü");
		init_assert((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG), u8"SDL_image³õÊ¼»¯Ê§°Ü");
		init_assert(!TTF_Init(), u8"SDL_ttf³õÊ¼»¯Ê§°Ü");
		init_assert(Mix_Init(MIX_INIT_MP3), u8"SDL_mixer³õÊ¼»¯Ê§°Ü");

		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		SDL_SetHint(SDL_HINT_IME_SHOW_UI,"1");

		window = SDL_CreateWindow("Tower Defense Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
		init_assert(window, u8"´´½¨´°¿ÚÊ§°Ü");
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
		init_assert(renderer, u8"´´½¨äÖÈ¾Æ÷Ê§°Ü");
	
	}
	~Game_manager() {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		
		Mix_Quit();
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
	}
private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	SDL_Event event;
	bool is_quit = false;	

private:
	void init_assert(bool flag,const char* err_msg) {
		if (flag) return;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", err_msg, window);	
		exit(-1);
	}
	void on_input() {

	}
	void on_update(double delta_time) {

	}
	void on_render() {

	}

};
#define _GAME_MANAGER_H_
#endif // _GAME_MANAGER_H_
