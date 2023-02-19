#include <SDL2/SDL.h>

int main(int argc, char **argv) {
    int ret = SDL_Init(SDL_INIT_VIDEO);
    printf("SDL_Init() returned: %d\n", ret);
    SDL_Quit();
    return 0;
}
