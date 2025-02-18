#include <iostream>
#include "SDL2/SDL_main.h"
#include "SDL2/SDL.h"

static SDL_bool sdl_quit = SDL_FALSE;
static SDL_Event sdl_event;
static SDL_Window *sdl_window;
static SDL_Renderer *sdl_renderer;
int width = 800;
int height = 600;

bool is_mouse_down = false;
bool is_mouse_just_released = false;

int mouse_down_x, mouse_down_y;

static void sdl_process_event(SDL_Event *event) {
     switch (event->type) {
          case SDL_QUIT:
               sdl_quit = SDL_TRUE;
               break;
          // Detect when mouse is clicked
          case SDL_MOUSEBUTTONDOWN:
               is_mouse_down = true;
               mouse_down_x = event->button.x;
               mouse_down_y = event->button.y;
               SDL_Log("Click");
               break;
          default:
               break;
     }
}

static int sdl_fillrect(int x, int y, int w, int h, int color) {
     const SDL_Rect rect = {x, y, w, h};
     SDL_SetRenderDrawColor(sdl_renderer, HEX2RGBA(color));
     return SDL_RenderFillRect(sdl_renderer, &rect);
}

int main(int argc, char* argv[]) {
     SDL_Init(SDL_INIT_VIDEO);
     sdl_window = SDL_CreateWindow("Hello World", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE);
     sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_PRESENTVSYNC);

     while (!sdl_quit) {
          while (SDL_PollEvent(&sdl_event)) {
               sdl_process_event(&sdl_event);
          }
     }
     return 0;
}
