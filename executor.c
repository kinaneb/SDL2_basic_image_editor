#include "executor.h"



int exec_command(char **args){

	int nbr_fct_intern = funcs_num();
	int i;
	for (i=0;i<nbr_fct_intern; i++) {
		if(strcmp(args[0], func_names[i]) == 0) {
			return (*funcs[i])(args);
		}
	}

	return error(args[0]);

}

int cimp_exit(char **args){

	exit(0);
}


int cimp_open(char **args){
	//call windows_handler open new window
	open_new_window();

	printf("New window opened!\n");
	return 1;
}

int cimp_load(char **args){

	char *filename = strdup(args[1]);
	char *imgName=load_image(filename);
	if(imgName==NULL){
		printf("Error image does not exist!\n");
		return 0;
	}
	//If it's successfull :
	printf("Image %s successfully loaded with name : %s!\n",filename,imgName);

	return 1;
}

int cimp_transfer(char **args){
	char *filename = strdup(args[3]);

	//Load /path/to/moh.jpg
	int windowId = atoi(args[1]) -1 ;

	//Call the related function in the window handler with the two args (filename,Windowid)
	int res = transfer_loaded_image(windowId,filename);
	//If the image isn't loaded
	if(res==1){
		printf("The image %s is not loaded in memory! Please use the load command before!\n",filename);
		return 0;
	}
	//If the windowId is wrong
	else if(res==2){
		printf("Window with ID : %d does not exist!\n",windowId);
		return 0;
	}

	else if(res==3){
		printf("An error occured while processing the command!\n");
		return 0;
	}

	//If everything is correct
	else{
		printf("Image %s  transfered with success in Window : %d !\n",filename, windowId);
		return 1;
	}
}

int cimp_save(char **args){
	int windowId = atoi(args[1]) -1 ;
	SDL_Surface *image = get_current_window_surface(windowId);

	if(image==NULL){
		printf("Wrong window ID!\n");
		return 0;
	}
	//Si save fenetre blank
	char *filename;
	if(!args[2]){
		filename = get_image_path_by_window(windowId);
		printf("filename : %s\n",filename);
	}
	else
		filename = args[2];

	char *format = strrchr(filename,'.');
	if(strcmp(format+1,"jpg")==0 || strcmp(format+1,"jpeg")==0){
		save_jpg_format(image,filename);
		printf("Image saved succesfully!(jpg)\n");
		return 1;
	}

	if(strcmp(format+1,"png")==0){
		save_png_format(image,filename);
		printf("Image saved succesfully!(png)\n");
		return 1;
	}

	if(strcmp(format+1,"bmp")==0){
		save_bmp_format(image,filename);
		printf("Image saved succesfully!(bmp)\n");
		return 1;
	}

	return 1;
}


int cimp_select(char **args){

	SDL_Point p1,p2;
	int windowId = atoi(args[2])-1;
	SDL_Surface *image=get_current_window_surface(windowId);

	if(image){
		if(strcmp(args[1],"-c")==0){
			extractCoords(args[3],&p1.x,&p1.y);
			extractCoords(args[4],&p2.x,&p2.y);
			if(select_rect(windowId,p1,p2)==0){
				return 1;
			}
			else{
				printf("Coords out of bound!\n");
				return 0;
			}
		}
		if(strcmp(args[1],"-a")==0){
			p1.x = 1;
			p1.y = 1;
			p2.x = image->w-1;
			p2.y = image->h-1;
			if(select_rect(windowId,p1,p2)==0){
				return 1;
			}
			else{
				printf("Coords out of bound!\n");
				return 0;
			}
		}
		if(strcmp(args[1],"-f")==0){
			select_f(windowId);
			return 1;
		}
	}

	printf("Wrong window Id\n");
	return 0;

}


int cimp_close(char **args){
	int windowId = atoi(args[1]) -1 ;


	if(!close_window(windowId)){
		printf("Wrong window ID!\n");
		return 0;
	}
	printf("fermé : %d\n",close_window(windowId));
	return 1;
}


int cimp_grayscale(char **args){
	int windowId = atoi(args[1]) -1 ;
	SDL_Surface *surface = get_current_window_surface(windowId);
	SDL_Point p1,p2;
	if(surface){
		if(args[2]){
			extractCoords(args[2],&p1.x,&p1.y);//
			extractCoords(args[3],&p2.x,&p2.y);
		}
		else{
			p1.x = 1;
			p1.y = 1;
			p2.x = surface->w-1;
			p2.y=surface->h-1;
		}
		if(select_rect(windowId,p1,p2)==0){
			GrayScale(surface);
			refresh_window(windowId);
			return 1;
		}
		else {
			printf("Coords Out of bound!\n");
			return 0;
		}
	}
	printf("Wrong window ID");
	return 0;
}


int cimp_blackwhite(char **args){
	int windowId = atoi(args[1]) -1 ;
	SDL_Surface *surface = get_current_window_surface(windowId);
	SDL_Point p1,p2;
	if(surface){
		if(args[2]){
			extractCoords(args[2],&p1.x,&p1.y);//
			extractCoords(args[3],&p2.x,&p2.y);
		}
		else{
			p1.x = 1;
			p1.y = 1;
			p2.x = surface->w-1;
			p2.y=surface->h-1;
		}
		if(select_rect(windowId,p1,p2)==0){
			BlackWhite(surface);
			refresh_window(windowId);
			return 1;
		}
		else {
			printf("Coords Out of bound!\n");
			return 0;
		}
	}
	printf("Wrong window ID");
	return 0;
}


int cimp_negative(char **args){
	int windowId = atoi(args[1]) -1 ;
	SDL_Surface *surface = get_current_window_surface(windowId);
	SDL_Point p1,p2;
	if(surface){
		if(args[2]){
			extractCoords(args[2],&p1.x,&p1.y);//
			extractCoords(args[3],&p2.x,&p2.y);
		}
		else{
			p1.x = 1;
			p1.y = 1;
			p2.x = surface->w-1;
			p2.y=surface->h-1;
		}
		if(select_rect(windowId,p1,p2)==0){
			Negative(surface);
			refresh_window(windowId);
			return 1;
		}
		else {
			printf("Coords Out of bound!\n");
			return 0;
		}
	}
	printf("Wrong window ID");
	return 0;
}


int cimp_alpha(char **args){
	int windowId = atoi(args[1]) -1 ;
	SDL_Surface *surface = get_current_window_surface(windowId);
	SDL_Point p1,p2;
	int alphaValue=0;
	if(surface){
		if(args[2] && args[3]){
			extractCoords(args[2],&p1.x,&p1.y);
			extractCoords(args[3],&p2.x,&p2.y);
			alphaValue = atoi(args[4]);
		}
		else if (args[2] && ! args[3]){
			p1.x = 1;
			p1.y = 1;
			p2.x = surface->w-1;
			p2.y=surface->h-1;
			alphaValue = atoi(args[2]);
		}


		if(select_rect(windowId,p1,p2)==0){
			alpha(surface,alphaValue);
			refresh_window(windowId);
			return 1;
		}
		else {
			printf("Coords Out of bound!\n");
			return 0;
		}
	}
	printf("Wrong ID");
	return 0;

}




int cimp_crop(char **args){
	int windowId = atoi(args[1]) -1 ;
	SDL_Surface *image = get_current_window_surface(windowId);
	if(image==NULL){
		printf("Wrong window ID!\n");
		return 0;
	}
	int height = atoi(args[2]);
	int width = atoi(args[3]);
	SDL_Rect rect =  {0,0,height,width};
	image=crop_image(image,rect);

	edit(windowId,image);
	refresh_window(windowId);
	return 1;
}


int cimp_zoom(char **args){
	int windowId = atoi(args[1]) -1 ;
	SDL_Surface *image = get_current_window_surface(windowId);
	if(image==NULL){
		printf("Wrong window ID!\n");
		return 0;
	}
	int height = atoi(args[2]);
	int width = atoi(args[3]);
	SDL_Rect rect =  {0,0,height,width};
	image=zoom_image(image,rect);

	edit(windowId,image);
	refresh_window(windowId);
	return 1;
}



int cimp_brightness(char **args){
	int windowId = atoi(args[1]) -1 ;
	SDL_Surface *surface = get_current_window_surface(windowId);
	SDL_Point p1,p2;
	int brightnessValue=0;

	if(surface){
		if(args[2] && args[3]){
			extractCoords(args[2],&p1.x,&p1.y);
			extractCoords(args[3],&p2.x,&p2.y);
			brightnessValue = atoi(args[4]);
		}
		else if (args[2] && ! args[3]){
			p1.x = 1;
			p1.y = 1;
			p2.x = surface->w-1;
			p2.y=surface->h-1;
			brightnessValue = atoi(args[2]);
		}

		if(select_rect(windowId,p1,p2)==0){
			brightness(surface,brightnessValue);
			refresh_window(windowId);
			return 1;
		}
		else {
			printf("Coords Out of bound!\n");
			return 0;
		}
	}
	printf("Wrong ID");
	return 0;
}




int cimp_fillrect(char **args){
	int windowId = atoi(args[1]) -1 ;
	SDL_Color color;
	SDL_Point p1,p2;
	extractRGB(args[2], &color.r, &color.g, &color.b);
	SDL_Surface *surface = get_current_window_surface(windowId);
	if(surface){
		if(args[3]){
			extractCoords(args[3],&p1.x,&p1.y);
			extractCoords(args[4],&p2.x,&p2.y);
		}

		else{
			p1.x = 1;
			p1.y = 1;
			p2.x = surface->w -1;
			p2.y = surface->h -1 ;
		}

		select_rect(windowId,p1,p2);
		FillRect(surface,color);
		refresh_window(windowId);
		return 1;
	}


	printf("Wrong ID");
	return 0;
}

int cimp_rotate(char **args){
	// int windowId = atoi(args[1]) -1 ;
	// int angle = atoi(args[2]);
	//SDL_Renderer *ren = getRenderer(windowId);
	//SDL_Texture *tex = getTex(windowId);
	// rotation(ren,tex,angle)
	return 1;
}


int cimp_vertical(char **args){
	int windowId = atoi(args[1]) -1;
	SDL_Surface *surface = get_current_window_surface(windowId);
	if(surface){
		Flip_Vertical(surface);

		refresh_window(windowId);
		return 1;

	}
	printf("Wrong ID");
	return 0;
}

int cimp_horizontal(char **args){
	int windowId = atoi(args[1]) -1 ;

	SDL_Surface *surface= get_current_window_surface(windowId);
	if(surface){
		Flip_Horizontal(surface);

		refresh_window(windowId);

		return 1;
	}
	printf("Wrong ID");
	return 0;
}


int cimp_resize(char **args){
	int windowId = atoi(args[1]) -1 ;
	SDL_Surface *image = get_current_window_surface(windowId);
	if(image==NULL){
		printf("Wrong window ID!\n");
		return 0;
	}
	int height = atoi(args[2]);
	int width = atoi(args[3]);
	SDL_Rect rect =  {0,0,height,width};
	image=resize_image(image,rect);

	edit(windowId,image);
	refresh_window(windowId);
	return 1;
}

int cimp_replace(char **args){

	//replace {specify window} p1 p2 (r,g,b) (r,g,b) d

	int windowId = atoi(args[1]) -1 ;
	SDL_Surface *surface = get_current_window_surface(windowId);
	SDL_Color color1;
	SDL_Color color2;
	SDL_Point p1,p2;
	int difference;
	if(surface){


		if(args[5] && args[6]){
			extractCoords(args[2],&p1.x,&p1.y);
			extractCoords(args[3],&p2.x,&p2.y);
			extractRGB(args[4], &color1.r, &color1.g, &color1.b);
			extractRGB(args[5], &color2.r, &color2.g, &color2.b);
			difference= atoi(args[6]);
		}
		else if(!args[5]){
			p1.x = 1;
			p1.y = 1;
			p2.x = surface->w-1;
			p2.y=surface->h-1;
			extractRGB(args[2], &color1.r, &color1.g, &color1.b);
			extractRGB(args[3], &color2.r, &color2.g, &color2.b);
			difference = atoi(args[4]);
		}
		if(select_rect(windowId,p1,p2)==0){
			replace_color(surface,color1,color2,difference);
			refresh_window(windowId);
			return 1;
		}
		else{
			printf("Coords Out of bound!\n");
			return 0;
		}
	}
	printf("Wrong window id\n!");
	return 0;
}



int cimp_copy(char **args){
	if(!args[1]){
		copy(NULL);
		return 1;
	}

	SDL_Point p1,p2;
	int windowId = atoi(args[2])-1;
	SDL_Surface *image=get_current_window_surface(windowId);

	if(image){
		if(strcmp(args[1],"-c")==0){
			extractCoords(args[3],&p1.x,&p1.y);
			extractCoords(args[4],&p2.x,&p2.y);
			if(select_rect(windowId,p1,p2)==0){
				copy(image);
				return 1;
			}
			else{
				printf("Coords out of bound!\n");
				return 0;
			}
		}
		if(strcmp(args[1],"-a")==0){
			p1.x = 1;
			p1.y = 1;
			p2.x = image->w-1;
			p2.y = image->h-1;
			if(select_rect(windowId,p1,p2)==0){
				copy(image);
				return 1;
			}
			else{
				printf("Coords out of bound!\n");
				return 0;
			}
		}
		if(strcmp(args[1],"-f")==0){
			//Free Select
			return 1;
		}
	}

	printf("Wrong window Id\n");
	return 0;

}

int cimp_cut(char **args){
	if(!args[1]){
		cut(NULL);
		refresh_window(selected_win_id);
		return 1;
	}

	SDL_Point p1,p2;
	int windowId = atoi(args[2])-1;
	SDL_Surface *image=get_current_window_surface(windowId);

	if(image){
		if(strcmp(args[1],"-c")==0){
			extractCoords(args[3],&p1.x,&p1.y);
			extractCoords(args[4],&p2.x,&p2.y);
			if(select_rect(windowId,p1,p2)==0){
				cut(image);
				refresh_window(windowId);
				return 1;
			}
			else{
				printf("Coords out of bound!\n");
				return 0;
			}
		}
		if(strcmp(args[1],"-a")==0){
			p1.x = 1;
			p1.y = 1;
			p2.x = image->w-1;
			p2.y = image->h-1;
			if(select_rect(windowId,p1,p2)==0){
				cut(image);
				refresh_window(windowId);
				return 1;
			}
			else{
				printf("Coords out of bound!\n");
				return 0;
			}
		}
		if(strcmp(args[1],"-f")==0){
			//Free Select
			return 1;
		}
	}

	printf("Wrong window Id\n");
	return 0;
}

int cimp_paste(char **args){
	int windowId = atoi(args[1]) -1;
	SDL_Point p;
	if(args[2])
		extractCoords(args[2],&p.x,&p.y);
	else {
		p.x=0;
		p.y=0;
	}

	if(get_current_window_surface(windowId)){
		paste(windowId,p);
		refresh_window(windowId);
		return 1;
	}

	printf("Wrong id!\n");
	return 0;
}


int cimp_delete(char **args){
	if(!args[1]){
		delete(NULL);
		refresh_window(selected_win_id);
		return 1;
	}
	else {
		int windowId = atoi(args[1]) -1;
		SDL_Surface *image = get_current_window_surface(windowId);
		SDL_Point p1,p2;
		if(image){


		extractCoords(args[2],&p1.x,&p1.y);
		extractCoords(args[3],&p2.x,&p2.y);
		if(select_rect(windowId,p1,p2)==0){
			delete(image);
			refresh_window(windowId);
			return 1;
		}
		else{
			printf("Coords out of bound!\n");
			return 0;
		}
	}
	printf("Wrong window ID!\n");
	return 0;
	}
}

int cimp_help(char **args){
	printf("\n\nCIMP EDITON v1.0, all these commands a defined internally.\n\n");
	printf("load /path/to/Image \t\t Loads the Image in the buffer\n");
	printf("open \t\t Opens a new empty Window.\n");
	printf("transfer WindowNumber -img ImageName \t\t Transfers an image from the buffer into the window with index : WindowNumber, the imageName is given by load command upon loading\n");
	printf("save WindowNumber [path/toImage.(jpg,jpeg,png,bmp)] \t\t Saves the image in the WindowNumber'th window, ");
	printf("if path is specified, it creates a new image with the specified name and format. If not, it modifies the original image\n");
	printf("close windowNumber \t\t Closes the window with id : windownumber\n");
	printf("exit \t\t closes the program\n");
	printf("grayscale/negative/blackwhite windowNumber [(x1,y1) (x2,y2)] \t\t ");
	printf("Applies grayscale,negative or blackwhite effect on the image in window : Windownumber, if no coords are specified, it's applied on the whole image ");
	printf("Otherwise,it applies on the rectangle between the 2 specified points\n");
	printf("fillrect windowNumber (R,G,B) [(x1,y1) (x2,y2)] \t\t Fills the whole image with the (R,G,B) color, if points are specified, fills the rectangle between those points\n");
	printf("resize/crop/zoom windowNumber height width \t\t Resizes, crop or zoom the image in the window : WindowNumber according to the specified height and width\n");
	printf("select -c/-a/-f windowNumber \t\t -c windowNumber (x1,y1) (x2,y2) selects the rectanble between the two specified points\n");
	printf("\t\t\t\t\t-a select the whole image\n");
	printf("\t\t\t\t\t-f free selection with mouse\n");
	printf("copy/cut [-[c/a/f] windowNumber] \t\t Copies/Cuts the current selection. -c/-a/-f options acts the same as select\n");
	printf("horizontal/vertical windowNumber \t\t Applies a horizontal/vertical flip on the image\n");
	printf("alpha/brightness windowNumber value \t\t Changes the alpha/brightness to value\n");
	printf("delete [windowNumber (x1,y1) (x2,y2)]\t\t Deletes the current selection. If coords are specified, deletes the rectangle between those points\n");
	return 1;
}


void extractCoords(char *coords, int *x, int *y){

	char *token = strtok(coords,",");

	*x = atoi(token+1);
	token = strtok(NULL, ",");
	*y = atoi(token);
}

void extractRGB(char *coords, Uint8 *r, Uint8 *g, Uint8 *b){

	char *token = strtok(coords,",");

	*r = atoi(token+1);
	token = strtok(NULL, ",");
	*g = atoi(token);
	token = strtok(NULL,",");
	*b = atoi(token);
}
