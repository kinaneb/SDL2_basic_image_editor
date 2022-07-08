#ifndef image_handler_h
#define image_handler_h

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "windows_handler.h"


SDL_Surface *copied;
SDL_Surface *temp_s;

int is_selected;             // = 0 if nothing is selected, 1 if rectangular selected, 2 if free points is selected
SDL_Rect selected_rect;      // rectangle selected by select_rect
SDL_Point *free_selected;    // points selected by free_select
int free_selected_points_no; // nomber of selected points via
int selected_win_id;         // = -1 if nothing is selected, index of window where was selected

struct free_point {
    SDL_Point point;
    struct free_point * next;
};
typedef struct free_point free_point;

struct points_list {
  int point_count;
  free_point * head;
};
typedef struct points_list points_list;

points_list *p_list;

free_point *new_point(int x, int y);

void new_list();

void add_point_to_list( free_point *p);

// return 0 if success, 1 if point a is out of image in win_id, 2 if b
int select_rect(int win_id, SDL_Point a, SDL_Point b);

// return 0 if success, 1 if point a is out of image in win_id, 2 if b
int free_select(int win_id, SDL_Point *points);

int select_pixels_by_color(int win_id, SDL_Point x, int d);

SDL_Color get_pixel_color(int win_id, int x, int y);

void push_to_free_selected();

void print_selected();

SDL_Surface *BlackWhite(SDL_Surface *surface);
SDL_Surface *GrayScale(SDL_Surface *surface);
SDL_Surface *Negative(SDL_Surface *surface);
SDL_Surface *FillRect(SDL_Surface *surface, SDL_Color c);

/* copy SDL_Rect copy if already selected full size surface else */
void copy(SDL_Surface *surface);

/* past SDL_Rect copy if already selected full size surface else */
void paste(int win_id, SDL_Point p);

/* cut SDL_Rect copy if already selected full size surface else */
void cut(SDL_Surface *surface);
Uint32 getpixel(SDL_Surface *surface, int x,int y);
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
int Flip_Horizontal(SDL_Surface *surface);
int Flip_Vertical(SDL_Surface *surface);
void rotation(SDL_Renderer *ren, SDL_Texture *tex, double angle);
Uint8 check_color(Uint8 c,int i);

/* reset selected rect and related variables */
void reset_selected_rect();

/* crop image surface at the size of rect, if error NULL */
SDL_Surface *crop_image(SDL_Surface *image, SDL_Rect rect);

/* agrandissement zoom function zooming the rect heeping the size of image*/
SDL_Surface *zoom_image(SDL_Surface *image, SDL_Rect rect);

/* resize image with the size of new_size rect */
SDL_Surface *resize_image(SDL_Surface *image, SDL_Rect new_size);

void brightness(SDL_Surface *surf,int valeur);

Uint8 ckeck_color(Uint8 c,int i);

SDL_Surface *drawRect(SDL_Surface *surface);

void alpha(SDL_Surface *surface,int valeur);

void select_f(int win_id);

void replace_color(
 SDL_Surface *surface,
 SDL_Color oldColor, SDL_Color newColor, int d);

void delete(SDL_Surface *surface);



#endif
