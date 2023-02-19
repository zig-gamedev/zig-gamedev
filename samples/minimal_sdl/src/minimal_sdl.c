#include <SDL2/SDL.h>

int main(int argc, char **argv) {
    int ret = SDL_Init(SDL_INIT_EVERYTHING);
    printf("SDL_Init() returned: %d\n", ret);
    SDL_Window* win = SDL_CreateWindow("Untitle", 100, 100, 400, 400, 0);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
