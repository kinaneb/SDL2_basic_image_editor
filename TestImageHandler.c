#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

#include <check.h>
#include "image_handler.h"
#include "windows_handler.h"


//Cette variable doit être initialisé avant chaque test!
SDL_Surface *surface;

void setup(void){
	surface = IMG_Load("anas.bmp");
	//Selected_rect doit avoir la taille de la surface ici
	 //selected_rect.x=.y=0
	//selected_rect.w = surface->w;
	//selected_rect.h = surface->h


}

void teardown(void){
	free(surface);
}

START_TEST(blackwhite_test){
	
	BlackWhite(surface);
	SDL_Surface *blacknwhite = IMG_Load("bwGIMP.png");
	//on compare la surface qu'on a modifé, avec une surface modifiée par GIMP ou Photoshop	
	//ck_assert_int_eq(COMPARE_SURFACES(surface,blacknwhite,20),1);
}END_TEST

START_TEST(grayscale_test){
	
	GrayScale(surface);
	SDL_Surface *grayscaled = IMG_Load("gsGIMP.png");
	
	//ck_assert_int_eq(COMPARE_SURFACES(surface,grayscaled,20),1);
}END_TEST


START_TEST(blackwhite_test){
	
	Negative(surface);
	SDL_Surface *negatived = IMG_Load("negGIMP.png");
	
	//ck_assert_int_eq(COMPARE_SURFACES(surface,negatived,20),1);
}END_TEST




//int main(void) here du cours
