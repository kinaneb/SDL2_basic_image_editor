/* ****************************************************** */
/* BSHARA Kinan                                           */
/* rasterIE program                                       */
/* windows_handler.h                                      */
/* ****************************************************** */

#ifndef WINDOWS_HANDLER_H
#define WINDOWS_HANDLER_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "windows_event_handler.h"

/* structure for handeling the windosw */
struct opened_window
{
  int id ;// window Id in windows_table
  int img_id;// associated image in images_table -10 if removed, -5 not associated yet
  SDL_Window *win;// window pointer
  SDL_Renderer *ren;// associated renderer pointer

};
typedef struct opened_window opened_window;

/* structure for handeling the images */
struct loaded_img
{
  int id;// image id in the images_table
  char *img_no;// user image number 'char * so can be changed by user in funture command'
  char *path;// image path
  SDL_Surface *img;// SDL_Surface pointer
  int saved;// if == 0 not saved 1 saved
};
typedef struct loaded_img loaded_img;

/* maximum number of opening windows */
int win_tabe_size;//initial size of windows table
int image_tabe_size;//initial size of images table
int win_count;// windows counter
int img_count;// images counter
int screen_h;// screen height
int screen_w;// screen width

pthread_t event_thread;

opened_window *windows_table;// opened windows table
loaded_img *images_table;// loaded images table

/* return pointer for new window posX, posY, width, height */
SDL_Window *new_window(const char* title, int posX, int posY, int width, int height);

/* create SDL renderer associated to SDL window 'win' */
SDL_Renderer *rendring_window(SDL_Window *win);

/* load image at path to the memory */
char * load_image(char *path);

/* upload loaded image has img_no in memory to the renderer associated to window table at win_id position
* return value: 0 success, 1 path (image not loaded in the memory), 2 window id not exit */
int transfer_loaded_image(int win_id, char *img_no);

/* upload image from path to the renderer */
SDL_Texture *upload_image(SDL_Renderer *ren, char *path);

/* upload loaded image to the renderer
* return value: 0 success, 1 error */
int close_window(int win_id);

/*opening new window, add it's pointer to windows_table at win_count position and increase the counter*/
void open_new_window();

/* initialization of windows_table and screen size */
void init_windows_table();

/* initialization of images_table */
void init_images_table();

/* double the size of windows_table */
void expend_windows_table();

/* double the size of images_table */
void expend_images_table();

/* return window's pointer at position id in windows_table if id < win_count else NULL */
SDL_Window *get_window(int id);

/* return image surface's pointer associated to window at win_id position, if win_id id < win_count else NULL */
SDL_Surface *get_surface_by_id(int win_id);

/* return image's path associated to window at win_id position, if win_id id < win_count else NULL */
char *get_image_path_by_window(int id);

/* aux function return a copy of string */
char *copy_string(char *string);

/* return image position at images_table of image has img_no if exist, -1 else */
int get_image_id_from_no(char *img_no);

/* return window's associated image surface if exit, else NULL */
SDL_Surface *get_current_window_surface(int win_id);

/* return window's associated image surface if exit, else NULL */
SDL_Renderer *get_current_window_renderer(int win_id);

/* update window in position win_id with the associated image,
* return 0 if success, 1 if image linked to this window == NULL, 2 if error CreateTextureFromSurface */
int refresh_window(int win_id);

/* return window id from window title */
int get_window_id_from_title(const char *title);

/* return value: 0 if success, -1 if no image has this img_no, -2 if image is not saved */
int remove_image_from_memory(char *img_no);

/* close all not closed windows */
void close_all_windows();

void edit(int win_id, SDL_Surface *image);

#endif
