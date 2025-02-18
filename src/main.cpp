#include <iostream>
#include "SDL2/SDL_main.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

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

const char CIRCLE = 1;
const char CROSS = -1;
char turn = 1;

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
               SDL_Log("mouse_down_x = %d", mouse_down_x);
               SDL_Log("mouse_down_y = %d", mouse_down_y);
               break;
          case SDL_MOUSEBUTTONUP:
               if (is_mouse_down) {
                    is_mouse_just_released = !is_mouse_just_released;
               }
               is_mouse_down = false;
               SDL_Log("Click realesed");
               mouse_down_x = event->button.x;
               mouse_down_y = event->button.y;
               SDL_Log("mouse_down_x = %d", mouse_down_x);
               SDL_Log("mouse_down_y = %d", mouse_down_y);
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

void make_move(char (&board)[9*9], int mouse_x, int mouse_y, char (&turn)) {
     /* From mouse pos get x and y on the board*/
     int x, y;
     y = mouse_y / (lower_square_side + main_line_width);
     x = mouse_x / (lower_square_side + main_line_width);

     if (board[x*9 + y] == 0 ) {
          board[x*9 + y] = turn;
          turn = -turn;
     }
}

int main(int argc, char* argv[]) {
     SDL_Init(SDL_INIT_VIDEO);
     if (IMG_Init(IMG_INIT_PNG) == 0) {
         SDL_Log("IMG_Init Error: %s", IMG_GetError());
         SDL_Quit();
         return 1;
     }
     sdl_window = SDL_CreateWindow("Super Tic Tac Toe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
     sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_PRESENTVSYNC);

     SDL_Surface* circle_surface = IMG_Load("assets/Circle.png");
     SDL_Surface* cross_surface = IMG_Load("assets/Cross.png");

     SDL_Texture* circle_texture = SDL_CreateTextureFromSurface(sdl_renderer, circle_surface);
     SDL_Texture* cross_texture = SDL_CreateTextureFromSurface(sdl_renderer, cross_surface);

     SDL_FreeSurface(circle_surface);
     SDL_FreeSurface(cross_surface);

     int circle_x, circle_y;
     SDL_QueryTexture(circle_texture, NULL, NULL, &circle_x, &circle_y);

     int cross_square_x, cross_square_y;
     SDL_QueryTexture(cross_texture, NULL, NULL, &cross_square_x, &cross_square_y);


     char board[9*9];
     for (int x = 0; x < 9; x++) {
          for (int y = 0; y < 9; y++) {
               board[x*9+y] = 0;
          }
     }

     while (!sdl_quit) {
          while (SDL_PollEvent(&sdl_event)) {
               sdl_process_event(&sdl_event);
          }
          if (is_mouse_just_released) {
               make_move(board, mouse_down_x, mouse_down_y, turn);
               is_mouse_just_released = !is_mouse_just_released;
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

                              SDL_Rect rect;
                              rect.x = x_cell;
                              rect.y = y_cell;
                              rect.w = lower_square_side;
                              rect.h = lower_square_side;
                              if (board[(i*3 + k)*9 + (j*3 + l)] == 1) {
                                   SDL_RenderCopy(sdl_renderer, circle_texture, NULL, &rect);
                              } else if (board[(i*3 + k)*9 + (j*3 + l)] == -1) {
                                   SDL_RenderCopy(sdl_renderer, cross_texture, NULL, &rect);
                              }
                         }
                    }

               }
          }

          SDL_RenderPresent(sdl_renderer);
     }
     return 0;
}
