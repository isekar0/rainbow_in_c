#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
// #include <stdint.h>

struct RGB_tuple {
    uint8_t r;
    uint8_t g;
    uint8_t b;
}; 

struct RGB_tuple cycle_through_rgb(struct RGB_tuple rgb_tuple, const double frequency) {
    int amplitude = 128, center = 127;
    static int increment = 0;
    increment += 1;
    increment = increment % 360;

    rgb_tuple.r = SDL_sin(frequency * increment)*127 + 128;
    rgb_tuple.g = SDL_sin(frequency * increment + 2)*127 + 128;
    rgb_tuple.b = SDL_sin(frequency * increment + 4)*127 + 128;

    return rgb_tuple;
}

int main(int argc, char* argv[]) {

    double frequency = 0;

    if (argc > 1) {
        char *endptr;
        frequency = SDL_strtod(argv[1], &endptr);

        if (*endptr != '\0') {
            return 1;
        }
    }
    else {
        frequency = 0.01;
    }

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    

    bool init_is_successful = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    if (!init_is_successful) {
        SDL_Log("SDL_init error: %s", SDL_GetError());
        return -1;
    }
    window = SDL_CreateWindow("Rainbow", 640, 640, 0);
    if (window == NULL) {
        SDL_Log("SDL_CreateWindow: %s", SDL_GetError());
        return -2;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == NULL) {
        SDL_Log("SDL_CreateRenderer: %s", SDL_GetError());
        return -3;
    }

    SDL_Log("SDL3 Initialized");

    
    SDL_Event event;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT: SDL_Log("SDL3 Event Quit"); quit = 1; break;

            }
        }
        struct RGB_tuple rgb_tuple = cycle_through_rgb(rgb_tuple, frequency);
        SDL_SetRenderDrawColor(renderer, rgb_tuple.r, rgb_tuple.g, rgb_tuple.b, 255);
        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(1);
    }

    SDL_Log("SDL3 Shutdown");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}