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
constexpr unsigned int highlight = 0x0000ff55;

bool is_mouse_down = false;
bool is_mouse_just_released = false;

int mouse_down_x, mouse_down_y;

const char CIRCLE = 1;
const char CROSS = -1;
char turn = CIRCLE;
int last_move[4] = {4, 4, 4, 4}; /* This is not a possible value so we'll know it's the first move */

char winner = 0;

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

char winner_grid(char grid[3][3]) {
     // Check for vertical lines
     for (int x = 0; x < 3; x++) {
          if (grid[x][0] != 0 && grid[x][1] == grid[x][0] && grid[x][1] == grid[x][2]) {
               return grid[x][0];
          }
     }

     for (int y = 0; y < 3; y++) {
          if (grid[0][y] != 0 && grid[1][y] == grid[0][y] && grid[1][y] == grid[2][y]) {
               return grid[0][y];
          }
     }

     /* Check diagonals*/
     if ((grid[0][0] != 0 && grid[1][1] == grid[0][0] && grid[0][0] == grid[2][2]) || (grid[2][0] != 0 && grid[1][1] == grid[2][0] && grid[2][0] == grid[0][2])) {
          return grid[1][1];
     };

     return 0;
}

char is_little_won(char board[3][3][3][3], int last_move[4]) {
     char (*grid)[3] = board[last_move[0]][last_move[1]];

     return winner_grid(grid);
}

char is_whole_won(char board[3][3][3][3]) {
     char grid[3][3];
     for (int X = 0; X < 3; X++) {
          for (int Y = 0; Y < 3; Y++) {
               int move[4] = {X, Y, 0, 0};
               grid[X][Y] = is_little_won(board, move);
          }
     }

     return winner_grid(grid);
}

bool is_grid_playable(int X, int Y, char board[3][3][3][3], int last_move[4]) {
     int move[4] = {X, Y, X, Y};

     if (last_move[0] == 4) {
          return true;
     }

     if (is_little_won(board, move) != 0) {
          return false;
     }

     if (last_move[2] == X && last_move[3] == Y) {
          /* Right grid */
          return true;
     } else {
          if (is_little_won(board, last_move) != 0) {
               return true;
          } else {
               return false;
          }
     }
}

void make_move(int mouse_x, int mouse_y, char (&turn), char(&board)[3][3][3][3], int (&last_move)[4]) {
     int boardX = mouse_x / (main_square_side + main_line_width);
     int boardY = mouse_y / (main_square_side + main_line_width);

     int boardx = (mouse_x % (main_square_side + main_line_width)) / (lower_square_side + lower_line_width);
     int boardy = (mouse_y % (main_square_side + main_line_width)) / (lower_square_side + lower_line_width);

     SDL_Log("boardX = %d, boardY = %d", boardX, boardY);
     SDL_Log("boardx = %d, boardy = %d", boardx, boardy);
     SDL_Log("turn = %c", turn);

     // Vérification de l'intérieur des limites
     if (boardX < 0 || boardX >= 3 || boardY < 0 || boardY >= 3 ||
         boardx < 0 || boardx >= 3 || boardy < 0 || boardy >= 3) {
          return;
         }

     // Vérifier si la case est déjà occupée
     if (board[boardX][boardY][boardx][boardy] != 0) {
          return;
     }

     // Vérifier si la grille est jouable
     if (!is_grid_playable(boardX, boardY, board, last_move)) {
          return;
     }

     // Placer le symbole et mettre à jour `last_move`
     board[boardX][boardY][boardx][boardy] = turn;
     last_move[0] = boardX;
     last_move[1] = boardY;
     last_move[2] = boardx;
     last_move[3] = boardy;

     turn = -turn;
}


void import_image(const char *path, SDL_Texture* (&texture_var)) {
     SDL_Surface* surface = IMG_Load(path);
     if (surface == NULL) {
          SDL_Log("IMG_Load: %s", IMG_GetError());
          return;
     }

     texture_var = SDL_CreateTextureFromSurface(sdl_renderer, surface);
     SDL_FreeSurface(surface);
}

void import_image(const char *path, SDL_Texture* (&texture_var), int (&image_x), int (&image_y)) {
     import_image(path, texture_var);
     SDL_QueryTexture(texture_var, NULL, NULL, &image_x, &image_y);
}

void initialize_board(char(&board)[3][3][3][3]) {
     for (auto & X : board) {
          for (auto & Y : X) {
               for (auto & x : Y) {
                    for (char & y : x) {
                         y = 0;
                    }
               }
          }
     }
};

int get_all_possible_moves(char board[3][3][3][3], int last_move[4], int (&all_moves)[81][4]) {
     int i = 0;
     for (int X = 0; X < 3; X++) {
          for (int Y = 0; Y < 3; Y++) {
               if (!is_grid_playable(X, Y, board, last_move)) {
                    continue;
               }

               for (int y = 0; y < 3; y++) {
                    for (int x = 0; x < 3; x++) {
                         int move[4] = {X, Y, x, y};
                         if (board[move[0]][move[1]][move[2]][move[3]] == 0) {
                              for (int k = 0; k < 4; k++) {
                                   all_moves[i][k] = move[k];
                              }
                              i++;
                         }
                    }
               }
          }
     }

     return i;
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

     int circle_x, circle_y;
     int cross_square_x, cross_square_y;
     SDL_Texture *circle_texture, *cross_texture;
     import_image("assets/Circle.png", circle_texture, circle_x, circle_y);
     import_image("assets/Cross.png", cross_texture, cross_square_x, cross_square_y);

     char board[3][3][3][3];
     initialize_board(board);

     while (!sdl_quit) {
          while (SDL_PollEvent(&sdl_event)) {
               sdl_process_event(&sdl_event);
          }

          winner = is_whole_won(board);

          if (turn == CIRCLE and not winner) {
               // Make a move at random
               int all_moves[81][4];
               int number_possible_moves = get_all_possible_moves(board, last_move, all_moves);
               int random_idx = rand() % number_possible_moves;
               int mouse_x_sim;
               int mouse_y_sim;
               mouse_x_sim = all_moves[random_idx][0] * (main_square_side + main_line_width);
               mouse_x_sim += all_moves[random_idx][2] * (lower_square_side + lower_line_width);
               mouse_y_sim = all_moves[random_idx][1] * (main_square_side + main_line_width);
               mouse_y_sim += all_moves[random_idx][3] * (lower_square_side + lower_line_width);
               make_move(mouse_x_sim, mouse_y_sim, turn, board, last_move);
          }

          if (is_mouse_just_released and not winner) {
               make_move(mouse_down_x, mouse_down_y, turn, board, last_move);
               is_mouse_just_released = !is_mouse_just_released;
          }
          SDL_RenderClear(sdl_renderer);

          /* Add a background of the color of the lines*/

          sdl_fillrect(0, 0, width, height, fill_color);

          /* Add the 9 grids */

          if (not winner) {
               for (int X = 0; X < 3; X++) {
                    for (int Y = 0; Y < 3; Y++) {
                         int x_start = X * (main_square_side + main_line_width);
                         int y_start = Y * (main_square_side + main_line_width);

                         // Dessiner les 9 petites cases à l'intérieur
                         for (int x = 0; x < 3; x++) {
                              for (int y = 0; y < 3; y++) {
                                   int x_cell = x_start + x * (lower_square_side + lower_line_width);
                                   int y_cell = y_start + y * (lower_square_side + lower_line_width);

                                   if (is_grid_playable(X, Y, board, last_move)) {
                                        if (board[X][Y][x][y] == 0) {
                                             sdl_fillrect(x_cell, y_cell, lower_square_side, lower_square_side, highlight);
                                        } else {
                                             sdl_fillrect(x_cell, y_cell, lower_square_side, lower_square_side, line_color);
                                        }
                                   } else {
                                        sdl_fillrect(x_cell, y_cell, lower_square_side, lower_square_side, line_color);
                                   }

                                   SDL_Rect rect;
                                   rect.x = x_cell;
                                   rect.y = y_cell;
                                   rect.w = lower_square_side;
                                   rect.h = lower_square_side;
                                   if (board[X][Y][x][y] == 1) {
                                        SDL_RenderCopy(sdl_renderer, circle_texture, NULL, &rect);
                                   } else if (board[X][Y][x][y] == -1) {
                                        SDL_RenderCopy(sdl_renderer, cross_texture, NULL, &rect);
                                   }
                              }
                         }

                         SDL_Rect rect2;
                         rect2.x = x_start;
                         rect2.y = y_start;
                         rect2.w = main_square_side;
                         rect2.h = main_square_side;
                         int move[4] = {X, Y, X, Y};
                         if (is_little_won(board, move) == CIRCLE) {
                              SDL_RenderCopy(sdl_renderer, circle_texture, NULL, &rect2);
                         } else if (is_little_won(board, move) == CROSS) {
                              SDL_RenderCopy(sdl_renderer, cross_texture, NULL, &rect2);
                         }
                    }
               }
          } else {
               SDL_Rect rect;
               rect.x = main_square_side;
               rect.y = main_square_side;
               rect.w = main_square_side;
               rect.h = main_square_side;

               if (winner == CIRCLE) {
                    SDL_RenderCopy(sdl_renderer, circle_texture, NULL, &rect);
               } else {
                    SDL_RenderCopy(sdl_renderer, cross_texture, NULL, &rect);
               }
          }
          SDL_RenderPresent(sdl_renderer);
     }
     return 0;
}
