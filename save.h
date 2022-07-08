#ifndef  save_h
#define  save_h

/*******************Un fichier.h****************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void save_jpg_format(SDL_Surface *image,char *newName);

void save_png_format(SDL_Surface *image,char *newName);

void save_bmp_format(SDL_Surface *image,char *newName);

#endif
