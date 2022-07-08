#include "parser.h"
#include "windows_event_handler.h"

#include "windows_handler.h"
#include "executor.h"
#include <readline/readline.h>
#include <readline/history.h>

int mainloop();

int SIZE = 64;



// SDL_Surface *copied=NULL;

int (*funcs[]) (char **) = {
	&cimp_load,
	&cimp_open,
	&cimp_transfer,
	&cimp_save,
	&cimp_close,
	&cimp_exit,
	&cimp_grayscale,
	&cimp_negative,
	&cimp_blackwhite,
	&cimp_fillrect,
	&cimp_rotate,
	&cimp_resize,
	&cimp_copy,
	&cimp_cut,
	&cimp_paste,
	&cimp_select,
	&cimp_horizontal,
	&cimp_vertical,
	&cimp_alpha,
	&cimp_brightness,
	&cimp_crop,
	&cimp_zoom,
	&cimp_replace,
	&cimp_delete,
	&cimp_help
};

char *func_names[] = {
	"load",
	"open",
	"transfer",
	"save",
	"close",
	"exit",
	"grayscale",
	"negative",
	"blackwhite",
	"fillrect",
	"rotate",
	"resize",
	"copy",
	"cut",
	"paste",
	"select",
	"horizontal",	
	"vertical",
	"alpha",
	"brightness",
	"crop",
	"zoom",
	"replace",
	"delete",
	"help"
};


int main(int n, char** s){
	// SDL_Thread *thread;
	// int threadReturnValue;
	// thread = SDL_CreateThread(handle_windows(NULL), "TestThread", (void *)NULL);
  // if (NULL == thread) {
  //     printf("SDL_CreateThread failed: %s\n", SDL_GetError());
  // } else {
  //     SDL_WaitThread(thread, &threadReturnValue);
  //     printf("Thread returned value: %d\n", threadReturnValue);
  // }


	init_windows_table();
	init_images_table();
	is_selected = 0;
	selected_win_id = -1;
	free_selected_points_no = 0;
	mainloop();
	return 0;
}


int mainloop(){
	char *line;
  while(1){
    char *prompt;
    prompt="$>";
    line = readline(prompt);
    traitement_ligne_de_commande(line);
    free(line);
  }
	exit(0);
}



int funcs_num(){
	return sizeof(func_names)/sizeof(char*);
}
