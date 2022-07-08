#ifndef executor_h
#define executor_h

#include "parser.h"
#include "windows_handler.h"
#include "image_handler.h"
#include "save.h"



//Pointeur sur les fonctions
extern int (*funcs[]) (char **);

//fonction qui compte le Nombre de commandes
extern int funcs_num();

//Tableau contenant le nom des function à appliquer (ex : Copy/cut/save..)
extern char *func_names[];



void extractCoords(char *coords, int *x, int *y);

void extractRGB(char *coords, Uint8 *r, Uint8 *g, Uint8 *b);

// fonction qui execute une commande grace a une liste d'arguments passés en parametre
int exec_command(char **args);


int cimp_exit(char **args);
int cimp_open(char **args);
int cimp_load(char **args);
int cimp_transfer(char **args);
int cimp_close(char **args);
int cimp_save(char **args);
int cimp_grayscale(char **args);
int cimp_blackwhite(char **args);
int cimp_fillrect(char **args);
int cimp_negative(char **args);
int cimp_rotate(char **args);
int cimp_vertical(char **args);
int cimp_horizontal(char **args);
int cimp_resize(char **args);
int cimp_copy(char **args);
int cimp_cut(char **args);
int cimp_paste(char **args);
int cimp_select(char **args);
int cimp_alpha(char **args);
int cimp_brightness(char **args);
int cimp_crop(char **args);
int cimp_zoom(char **args);
int cimp_replace(char **args);
int cimp_delete(char **args);
int cimp_help(char **args);	


#endif
