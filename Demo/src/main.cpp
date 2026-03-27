#define SDL_MAIN_HANDLED

#include<iostream>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
#include<SDL2_gfxPrimitives.h>
#include <windows.h>

void initial() {
	HMODULE hMod = GetModuleHandleA("SDL2.dll");
	if (hMod == NULL) {
		std::cout << "SDL2.dll not loaded\n";
	}

	char path[MAX_PATH];
	DWORD len = GetModuleFileNameA(hMod, path, MAX_PATH);
	if (len > 0) {
		std::cout << "SDL2.dll loaded from: " << path << std::endl;
	}
	else {
		std::cout << "Failed to get module path\n";
	}
}

int main() {
	initial();
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_AVIF);
	Mix_Init(MIX_INIT_MP3);
	TTF_Init();
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	SDL_Window* window = SDL_CreateWindow(u8"Test Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	int fps = 60
		;

	SDL_Event event;
	SDL_Color color = { 255,255,255,255 };
	TTF_Font* font = TTF_OpenFont("resource/ipix.ttf", 32);
	Mix_Music* music = Mix_LoadMUS("resource/music.mp3");
	Uint64 last_counter = SDL_GetPerformanceCounter();
	Uint64 frequency = SDL_GetPerformanceFrequency();
	SDL_Surface* suf_img = IMG_Load("resource/pyke.jpg");
	SDL_Texture* tex_img = SDL_CreateTextureFromSurface(renderer, suf_img);

	SDL_Surface* suf_text = TTF_RenderUTF8_Blended(font, u8"把他们全拖下水", color);
	SDL_Texture* tex_text = SDL_CreateTextureFromSurface(renderer, suf_text);

	Mix_FadeInMusic(music, -1, 1500);
	SDL_Point cursor_pos = { 0,0 };
	
	SDL_Rect img_rect, text_rect;
	img_rect.w = suf_img->w;
	img_rect.h = suf_img->h;
	text_rect.h = suf_text->h;
	text_rect.w = suf_text->w;
	 
	bool is_quit = false;
	while (!is_quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) is_quit = true;
			if (event.type == SDL_MOUSEMOTION) {
				cursor_pos.x = event.motion.x;
				cursor_pos.y = event.motion.y;
			}
		}
		Uint64 current_counter = SDL_GetPerformanceCounter();
		double delta = (double)(current_counter - last_counter) / frequency;
		last_counter = current_counter;
		if (delta * 1000 < 1000.0 / fps) SDL_Delay((Uint32)(1000.0 / fps - delta * 1000.0));
		img_rect.x = cursor_pos.x;
		img_rect.y = cursor_pos.y;
		text_rect.x = cursor_pos.x;
		text_rect.y = cursor_pos.y;
		
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, tex_img, nullptr, &img_rect);
		filledCircleRGBA(renderer, cursor_pos.x, cursor_pos.y, 50, 0, 255, 255, 125);
		SDL_RenderCopy(renderer, tex_text, nullptr, &text_rect);
		SDL_RenderPresent(renderer);
	}

	return 0;
}
