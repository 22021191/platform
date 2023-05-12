#include"BaseFunc.h"
#include"Map.h"
#include"Player.h"
#include"Game.h"
#include"Menu.h"
using namespace std;

bool Init();

int main(int argc, char* argv[]) {
	Init();
	Game* game = new Game();
	game->GameLoop();
	return 0;
}

bool Init() {
	bool success = true;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Khong the khoi tao SDL. Loi: " << SDL_GetError() << std::endl;
		success = false;
	}
	else {
		window = SDL_CreateWindow("Mystery Guy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			std::cout << "Khong the tao cua so game! SDL error: " << SDL_GetError() << std::endl;
			success = false;
		}
		else {
			renderer = SDL_CreateRenderer(window, -1, 0);
			if (renderer == NULL) {
				std::cout << "Khong the tao but ve! SDL error: " << SDL_GetError() << std::endl;
				success = false;
			}
			else {
				if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
					std::cout << "Khong the khoi dong SDL_image! SDL_image error: " << IMG_GetError() << std::endl;
					success = false;
				}
				if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
					std::cout << "Khong the khoi dong SDL_mixer! SDl_mixer error: " << Mix_GetError() << std::endl;
					success = false;
				}
				if (TTF_Init() == -1) {
					std::cout << "Khong the khoi dong SDL_ttf! SDL_ttf error: " << TTF_GetError() << std::endl;
					success = false;
				}
			}
		}
	}
	fGame = TTF_OpenFont("ARLRDBD.ttf", 24);
	jump = Mix_LoadWAV("Sounds/Jump.wav");
	Click = Mix_LoadWAV("Sounds/click.wav");
	return success;
}