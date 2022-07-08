
#include "save.h"

void save_jpg_format(SDL_Surface *image,char *newName)
 {

   IMG_SaveJPG(image,newName,100);


}

void save_png_format(SDL_Surface *image,char *newName)
  {


   IMG_SavePNG(image, newName);

}


void save_bmp_format(SDL_Surface *image,char *newName)
{
  /* SDL_SaveBMP retourne 0 en cas de succes et une valeur negatif en cas d'echec*/
   SDL_SaveBMP(image, newName);


}
