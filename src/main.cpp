#include <iostream>
#include "SDL2/SDL_main.h"
#include "SDL2/SDL.h"

#define HEX2RGBA(hex) ((hex >> 24) & 0xFF), ((hex >> 16) & 0xFF), ((hex >> 8) & 0xFF), (hex & 0xFF)

static SDL_bool sdl_quit = SDL_FALSE;
static SDL_Event sdl_event;
static SDL_Window *sdl_window;
static SDL_Renderer *sdl_renderer;
int width = 1000;
int height = 1000;

int main_line_width = 10;
int main_square_side = (width - 2*main_line_width) / 3;

int lower_line_width = 5;
int lower_square_side = (main_square_side - 2*main_line_width) / 3;

constexpr unsigned int line_color = 0xcfcec0ff;
constexpr unsigned int fill_color = 0x0d0d0cff;

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

static int sdl_fillrect(int x, int y, int w, int h, unsigned int color) {
     const SDL_Rect rect = {x, y, w, h};
     SDL_SetRenderDrawColor(sdl_renderer, HEX2RGBA(color));
     return SDL_RenderFillRect(sdl_renderer, &rect);
}

int main(int argc, char* argv[]) {
     SDL_Init(SDL_INIT_VIDEO);
     sdl_window = SDL_CreateWindow("Super Tic Tac Toe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
     sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_PRESENTVSYNC);

     while (!sdl_quit) {
          while (SDL_PollEvent(&sdl_event)) {
               sdl_process_event(&sdl_event);
          }
          SDL_RenderClear(sdl_renderer);

          /* Add a background of the color of the lines*/

          sdl_fillrect(0, 0, width, height, fill_color);

          /* Add the 9 grids */

          for (int i = 0; i < 3; i++) {
               for (int j = 0; j < 3; j++) {
                    int x_start = i * (main_square_side + main_line_width);
                    int y_start = j * (main_square_side + main_line_width);

                    // Dessiner les 9 petites cases à l'intérieur
                    for (int k = 0; k < 3; k++) {
                         for (int l = 0; l < 3; l++) {
                              int x_cell = x_start + k * (lower_square_side + lower_line_width);
                              int y_cell = y_start + l * (lower_square_side + lower_line_width);
                              sdl_fillrect(x_cell, y_cell, lower_square_side, lower_square_side, line_color);
                         }
                    }

               }
          }

          SDL_RenderPresent(sdl_renderer);
     }
     return 0;
}
