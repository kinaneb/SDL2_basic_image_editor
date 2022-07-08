/* ****************************************************** */
/* BSHARA Kinan                                           */
/* rasterIE program                                       */
/* windows_handler.c                                      */
/* ****************************************************** */

#include "windows_handler.h"

#define WIN_WID 400
int win_tabe_size = 10;
int image_tabe_size = 10;


// void tt()
// {
//   // SDL_Thread *thread;
// 	int threadReturnValue;
// 	// thread = SDL_CreateThread(handle_windows(NULL), "TestThread", (void *)NULL);
//   if (NULL == thread) {
//       printf("SDL_CreateThread failed: %s\n", SDL_GetError());
//   } else {
//       SDL_WaitThread(thread, &threadReturnValue);
//       printf("Thread returned value: %d\n", threadReturnValue);
//   }
// }


char *my_itoa(int n)
{
  int x = n;
  int numDigits = 0;
  do
  {
    ++numDigits;
    x = x / 10;
  }while ( x );
  char *str;
  str = malloc(sizeof(char)*(numDigits+1));
  if(!str)
  {
    printf("%s\n", "my itoa malloc Error!");
    return NULL;
  }
  memset(str+numDigits,'\0',1);
  if(0>sprintf(str, "%d", n))
  {
    printf("%s\n","naming window id Error!" );
  }
  return str;
}


void init_windows_table()
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  // initialize SDL with video flag 'video as all screen configration not as video'
  {
    //if sth go wrong add error to SDL_Log and exit
  	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init Error: %s\n", SDL_GetError());
    return;
  }
  /*table of opening windows*/
  //opened_window * windows_table = (opened_window*)malloc(sizeof(opened_window)*win_tabe_size);
  windows_table = malloc(sizeof(opened_window)*win_tabe_size);
  if(windows_table==NULL)
  {
    printf("init windows table malloc ERROR!");
  }

  SDL_DisplayMode dm;
  if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
  {
       SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
       return ;
  }
  screen_w = dm.w;
  screen_h = dm.h;
  // tt();
}

void init_images_table()
{
    // load support for the JPG and PNG image formats
  int flags=IMG_INIT_JPG|IMG_INIT_PNG;
  int initted=IMG_Init(flags);
  if((initted&flags) != flags)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "IMG_Init: Failed to init required jpg and png support!\n");
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "IMG_Init: %s\n", IMG_GetError());
    // handle error
  }
  images_table = malloc(sizeof(loaded_img)*image_tabe_size);
  if(!images_table)
  {
    printf("init images table malloc ERROR!");
  }
  handle_window();
}


/*opened windows counter*/
int win_count = 0;
int img_count = 0;

SDL_Window *new_window(const char* title, int posX, int posY, int width, int height)
{
  SDL_Window *win = NULL; //pointer of type SDL_Window
  win = SDL_CreateWindow(title, posX, posY, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
  // create SDL Window with title 'rasterIE !', position x = '100', y = '100', width = '640', height = '480'
  if (!win)
  //if sth go wrong add error to SDL_Log return NULL
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateWindow Error: %s\n", SDL_GetError());
  	SDL_Quit(); // to free memory used by SDL
  	return NULL;
  }
  return win;
}

SDL_Renderer *rendring_window(SDL_Window *win)
{
  SDL_Renderer *ren = NULL;//pointer of type SDL_Renderer
  ren = SDL_CreateRenderer(win, -1, 0 );
  // create SDL renderer associated with SDL window 'win', the index of the rendering driver
  // or like in this example = '-1' which mean the first that meet this requirements

  if (!ren)
  //if sth go wrong add error to SDL_Log and exit
  {
  	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
  	return NULL;
  }
  SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);

  return ren;
}

char *load_image(char *path)
{
  SDL_Surface *loaded_image;//pointer of type SDL_Surface
  loaded_image = IMG_Load(path);
  if (!loaded_image)
  //if sth go wrong add error to SDL_Log and exit
  {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "error loading image : %s\n", IMG_GetError() );
		return NULL;
	}
  loaded_image = SDL_ConvertSurfaceFormat(loaded_image, SDL_PIXELFORMAT_ARGB8888,0  );
  if(img_count >= image_tabe_size)
  {
    expend_images_table();
    return load_image(path);
  }
  else
  {
    int id = img_count++;
    images_table[id].id = id;
    images_table[id].img = loaded_image;
    images_table[id].img_no = my_itoa(id+1);
    images_table[id].path = copy_string(path);
    images_table[id].saved = 1;
  	return images_table[id].img_no;
  }
  return NULL;
}

int transfer_loaded_image(int win_id, char *img_no)
{
  int image_id = atoi(img_no) -1;
  windows_table[win_id].img_id = image_id;
  if (image_id < 0) return 1;
  if(!get_window(win_id))
  {
    return 2;
  }
  int i = refresh_window(win_id);
  if (i == 1) return 1;
  if (i == 3) return 3;
  windows_table[win_id].img_id = image_id;
  return 0;
}

int refresh_window(int win_id)
{
  SDL_Texture *tex = NULL;//pointer of type SDL_Texture
  if(!images_table[windows_table[win_id].img_id].img) return 1;
  SDL_RenderClear(windows_table[win_id].ren);
  tex = SDL_CreateTextureFromSurface(windows_table[win_id].ren, images_table[windows_table[win_id].img_id].img);
  int w = (screen_w < images_table[windows_table[win_id].img_id].img->w) ? screen_w : images_table[windows_table[win_id].img_id].img->w;
  int h = (screen_h < images_table[windows_table[win_id].img_id].img->h) ? screen_h : images_table[windows_table[win_id].img_id].img->h;
  SDL_SetWindowSize(windows_table[win_id].win, w, h);

  // upload the image at 'bmp' the memory to thr renderer 'ren'
  if (!tex)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
    return 3;
  }

  SDL_RenderClear(windows_table[win_id].ren);
  SDL_RenderCopy(windows_table[win_id].ren, tex, NULL, NULL);
  SDL_RenderPresent(windows_table[win_id].ren);
  SDL_DestroyTexture(tex);
  SDL_UpdateWindowSurface( windows_table[win_id].win );
  handle_window();
  return 0;
}

SDL_Texture *upload_image(SDL_Renderer *ren, char *path)
{
  SDL_Texture *tex = NULL;//pointer of type SDL_Texture
	tex = IMG_LoadTexture(ren, path);
	if(tex == NULL)
  {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "loading image Error: %s\n", IMG_GetError());
		return NULL;
	}
	return tex;
}

int close_window(int win_id)
{
  // id, img_id = -10 ==> so window is already closed
  if (get_window(win_id))
  {
	if(windows_table[win_id].id == -10){return -1;}
    else
      {
        SDL_DestroyWindow(windows_table[win_id].win);
        SDL_DestroyRenderer(windows_table[win_id].ren);
        windows_table[win_id].id = -10;
        windows_table[win_id].img_id = -10;
        return 1;
    }
  }
  return 0;
}

void open_new_window()
{
	// pthread_create(&event_thread, NULL, handle_windows, NULL);
  if(win_count >= win_tabe_size)
  {
    expend_windows_table();
    open_new_window();
  }
  else
  {
    int id = win_count++;
    char win_name[12] = "Window: ";
    strcat(win_name, my_itoa(id+1));
    SDL_Window *win = new_window(win_name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_WID, WIN_WID);
    if (!win)
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "opening new window Error %s\n", SDL_GetError() );
      return;
    }
    SDL_Renderer *ren = rendring_window(win);
    if (!ren)
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "opening new window renderer Error %s\n", SDL_GetError());
      SDL_DestroyWindow(win);
      return;
    }
    windows_table[id].id = id;
    windows_table[id].win = win;
    windows_table[id].ren = ren;
    windows_table[id].img_id = -5; // -5 ==> no image linked yet
  }
}

void expend_windows_table()
{
  win_tabe_size = win_tabe_size*2;

  opened_window *temp = realloc(windows_table, sizeof(opened_window)*win_tabe_size);
  if(!temp)
  {
    printf("%s\n", "expend windows table realloc Error");
  }
  else
  {
    windows_table = temp;
  }
}

void expend_images_table()
{
  image_tabe_size = image_tabe_size*2;

  loaded_img *temp = realloc(images_table, sizeof(loaded_img)*image_tabe_size);
  if(!temp)
  {
    printf("%s\n", "expend images table realloc Error");
  }
  else
  {
    images_table = temp;
  }
}

SDL_Window *get_window(int id)
{
  return (id >= win_count)?NULL : windows_table[id].win;
}


SDL_Surface *get_surface_by_id(int win_id)
{
  return (get_window(win_id)) ? images_table[windows_table[win_id].img_id].img : NULL;
}

char *get_image_path_by_window(int win_id)
{
  return (win_id > win_count)?NULL : images_table[windows_table[win_id].img_id].path;
}

char *copy_string(char *string)
{
  char *temp = malloc(sizeof(char)*strlen(string));
  temp = strcpy(temp, string);
  if (!temp)
  {
    printf("%s\n", "copy string malloc Error");
    return NULL;
  }
  return temp;
}


int get_image_id_from_no(char *img_no)
{
  for (int i = 0; i < img_count; i++)
  {
    if (strcmp(images_table[i].img_no, img_no) == 0)
    return i;
  }
  return -1;
}

SDL_Surface *get_current_window_surface(int win_id)
{
  return (get_window(win_id)) ? images_table[windows_table[win_id].img_id].img : NULL;
}

SDL_Renderer *get_current_window_renderer(int win_id)
{
  return (get_window(win_id)) ? windows_table[win_id].ren : NULL;
}

int get_window_id_from_title(const char *title)
{
  return (atoi(&title[7])-1);
}

int remove_image_from_memory(char *img_no)
{
  int id = get_image_id_from_no(img_no);
  if(id != -1)
  {
    if(!images_table[id].saved)
    {
      return -2;
    }
    else
    {
      images_table[id].id = -10;// ==> image already removed
      free(images_table[id].img_no);
      free(images_table[id].path);
      SDL_FreeSurface(images_table[id].img);
      images_table[id].saved = -10;// ==> image already removed
    }
    return 0;
  }
  return -1;
}





void edit(int win_id, SDL_Surface *image)
{
	 images_table[windows_table[win_id].img_id].img = image;
}












void close_all_windows()
{
  for (int i = 0; i < win_count; i++)
  {
    if((windows_table[i].win) && (windows_table[i].id != -10))
    {
      close_window(i);
    }
  }
  printf("good bye\n");
}
