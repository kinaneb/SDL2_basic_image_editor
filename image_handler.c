#include "image_handler.h"

int abs(int x)
{
  if(x<0){return -x;}
  return x;
}


void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
  int bytpixel = surface->format->BytesPerPixel;
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bytpixel;
  switch(bytpixel) {
    case 1:
    *p = pixel;
    break;
    case 2:
    *(Uint16 *)p = pixel;
    break;
    case 3:
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
      p[0] = (pixel >> 16) & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = pixel & 0xff;
    } else {
      p[0] = pixel & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = (pixel >> 16) & 0xff;
    }
    break;
    case 4:
    *(Uint32 *)p = pixel;
    break;
  }
}


void reset_selected_rect()
{
  is_selected = 0;
  selected_rect.y = 0;
  selected_rect.x = 0;
  selected_rect.y = 0;
  selected_rect.h = 0;
  selected_rect.w = 0;
  selected_win_id = -1;
  free_selected_points_no = 0;
  // SDL_FreeSurface(select);
}

SDL_Color get_pixel_color(int win_id, int x, int y)
{
  SDL_Color c;
  SDL_Surface *surface = get_surface_by_id(win_id);
  SDL_PixelFormat *fmt;
  Uint32 temp;
  //Uint8 red, green, blue, alpha;

  fmt = surface->format;
  int i = ( y * (surface->w) ) + x;
  SDL_LockSurface(surface);
  Uint32 *pixels = (Uint32 *) surface->pixels;
  Uint32 pixel = pixels[i];
  SDL_UnlockSurface(surface);

  /* Get Red component */
  temp = pixel & fmt->Rmask;  /* Isolate red component */
  temp = temp >> fmt->Rshift; /* Shift it down to 8-bit */
  temp = temp << fmt->Rloss;  /* Expand to a full 8-bit number */
  c.r = (Uint8)temp;

  /* Get Green component */
  temp = pixel & fmt->Gmask;  /* Isolate green component */
  temp = temp >> fmt->Gshift; /* Shift it down to 8-bit */
  temp = temp << fmt->Gloss;  /* Expand to a full 8-bit number */
  c.g = (Uint8)temp;

  /* Get Blue component */
  temp = pixel & fmt->Bmask;  /* Isolate blue component */
  temp = temp >> fmt->Bshift; /* Shift it down to 8-bit */
  temp = temp << fmt->Bloss;  /* Expand to a full 8-bit number */
  c.b = (Uint8)temp;

  /* Get Alpha component */
  temp = pixel & fmt->Amask;  /* Isolate alpha component */
  temp = temp >> fmt->Ashift; /* Shift it down to 8-bit */
  temp = temp << fmt->Aloss;  /* Expand to a full 8-bit number */
  c.a = (Uint8)temp;
  return c;
}


//SDL_Point mouse_position
int select_rect(int win_id, SDL_Point a, SDL_Point b)
{
  SDL_Rect image_full_rect;
  image_full_rect.x = 0;
  image_full_rect.y = 0;
  image_full_rect.w = images_table[windows_table[win_id].img_id].img->w;
  image_full_rect.h = images_table[windows_table[win_id].img_id].img->h;
  if (!SDL_PointInRect(&a, &image_full_rect)) return 1;
  if (!SDL_PointInRect(&b, &image_full_rect)) return 2;
  else
  {
    int min_x , min_y, max_x, max_y, h, w;
    if(a.x < b.x)
    {
      min_x = (a.x);
      max_x = (b.x);
    }else{
      min_x = (b.x);
      max_x = (a.x);
    }
    if(a.y < b.y)
    {
      min_y = (a.y);
      max_y = (b.y);
    }else{
      min_y = (b.y);
      max_y = (a.y);
    }
    selected_rect.x = min_x;
    selected_rect.y = min_y;
    selected_rect.h = max_y - min_y;
    selected_rect.w = max_x - min_x;
    is_selected = 1;
    selected_win_id = win_id;
    SDL_Renderer * rend = get_current_window_renderer(win_id);
    SDL_Surface *temp = get_current_window_surface(win_id);
    temp_s = drawRect(temp);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, temp_s);
    SDL_RenderCopy(windows_table[win_id].ren, tex, NULL, NULL);
    SDL_RenderPresent(windows_table[win_id].ren);
    SDL_DestroyTexture(tex);
    SDL_FreeSurface(temp_s);
    handle_window();
  }
  return 0;
}

int select_pixels_by_color(int win_id, SDL_Point x, int d)
{
  new_list();
  images_table[windows_table[win_id].img_id].img = SDL_ConvertSurfaceFormat(get_surface_by_id(win_id), SDL_PIXELFORMAT_RGBA32,0  );
  SDL_Rect image_borders;
  image_borders.x = 0;
  image_borders.y = 0;
  image_borders.h = images_table[windows_table[win_id].img_id].img->h;
  image_borders.w = images_table[windows_table[win_id].img_id].img->w;
  //printf("w = %d, h = %d \n", image_borders.h, image_borders.w);
  if (!SDL_PointInRect(&x, &image_borders)) return 1;
  SDL_Color c = get_pixel_color(win_id, x.x, x.y);
  SDL_Color t ;
  for(int i = 0; i < image_borders.w; i++)
  {
    for(int j = 0; j < image_borders.h; j++)
    {
      t = get_pixel_color(win_id, i, j);
      int r_diff = abs(t.r - c.r);
      int g_diff = abs(t.g - c.g);
      int b_diff = abs(t.b - c.b);
      int a_diff = abs(t.a - c.a);
      if((r_diff <d ) && (g_diff <d) && (b_diff <d) && (a_diff <d))
      {
        add_point_to_list( new_point(i, j));
      }
    }
  }
  push_to_free_selected();
  is_selected = 2;
  return 0;
}

free_point *new_point(int x, int y)
{
  free_point *np = malloc(sizeof(free_point));
  if(!np)
  {
    printf("malloc new points Error!");
    return NULL;
  }
  np->point.x = x;
  np->point.y = y;
  np->next = NULL;
  return np;
}

void new_list()
{
  p_list = malloc(sizeof(points_list));
  if(!p_list)
  {
    printf("malloc points list Error!");
  }else
  {
    p_list->point_count = 0;
    p_list->head = NULL;
  }
}

void add_point_to_list( free_point *p)
{
  p_list->point_count++;
  free_point *temp = p_list->head;
  p_list->head = p;
  p->next = temp;
}

void push_to_free_selected()
{
  free_selected_points_no = p_list->point_count;
  free_selected = malloc( sizeof(SDL_Point) * (p_list->point_count) );
  if(!free_selected)
  {
    printf("malloc selected points list Error!");
    return ;
  }
  for(; p_list->point_count > 0 ; p_list->point_count--)
  {
    free_selected[p_list->point_count-1]=p_list->head->point;
    free_point *temp = p_list->head;
    p_list->head = temp->next;
    free(temp);
  }
  free(p_list);
}


SDL_Surface *BlackWhite(SDL_Surface *surface){
  Uint32 *pixels = surface->pixels;
  SDL_LockSurface(surface);
  for(int j=selected_rect.y;j<selected_rect.y+selected_rect.h;j++){
    for(int i=selected_rect.x;i<selected_rect.x+selected_rect.w;i++){
      Uint32 pixel = pixels[j * surface->w +i];
      Uint8 r = pixel >> 16 & 0xFF;
      Uint8 g = pixel >> 8 & 0xFF;
      Uint8 b = pixel & 0xFF;
      int v= r+g+b;
      if(v>(255*3/2))
      pixel=SDL_MapRGB(surface->format,255,255,255);
      else
      pixel=SDL_MapRGB(surface->format,0,0,0);
      pixels[j*surface->w+i] = pixel;
    }
  }
  SDL_UnlockSurface(surface);
  return surface;
}

SDL_Surface *GrayScale(SDL_Surface *surface){
  Uint32 *pixels = surface->pixels;
  SDL_LockSurface(surface);
  for(int j=selected_rect.y;j<selected_rect.y+selected_rect.h;j++){
    for(int i=selected_rect.x;i<selected_rect.x+selected_rect.w;i++){
      Uint32 pixel = pixels[j * surface->w +i];
      Uint8 r = pixel >> 16 & 0xFF;
      Uint8 g = pixel >> 8 & 0xFF;
      Uint8 b = pixel & 0xFF;
      int v = (r+g+b)/3;
      pixel = (0xFF << 24) | (v << 16) | (v << 8) | v;
      pixels[j*surface->w+i] = pixel;
    }
  }
  SDL_UnlockSurface(surface);
  return surface;
}

SDL_Surface *Negative(SDL_Surface *surface){
  Uint32 *pixels = surface->pixels;
  // SDL_Rect rect = selected_rect;
  // printf(" %d %d %d %d ",selected_rect.x,selected_rect.y,selected_rect.h,selected_rect.w);
  SDL_LockSurface(surface);
  for(int j=selected_rect.y;j<selected_rect.y+selected_rect.h;j++){
    for(int i=selected_rect.x;i<selected_rect.x+selected_rect.w;i++){
      Uint32 pixel = pixels[j * surface->w +i];
      Uint8 r = pixel >> 16 & 0xFF;
      Uint8 g = pixel >> 8 & 0xFF;
      Uint8 b = pixel & 0xFF;
      pixel=SDL_MapRGB(surface->format,256-r,256-g,256-b);
      pixels[j*surface->w+i] = pixel;
    }
  }
  SDL_UnlockSurface(surface);
  return surface;
}

SDL_Surface *drawRect(SDL_Surface *surface){
  SDL_Surface *temp = SDL_CreateRGBSurface(surface->flags,
  surface->w,
  surface->h,
  surface->format->BitsPerPixel,
  surface->format->Rmask,
  surface->format->Gmask,
  surface->format->Bmask,
  surface->format->Amask);
  int valeur = 100;
  SDL_BlitSurface(surface, NULL, temp, NULL);
    SDL_LockSurface(temp);
    for(int i = selected_rect.x; i < (selected_rect.x + selected_rect.w); i++){
      for(int j = selected_rect.y; j < (selected_rect.y + selected_rect.h); j++){
        Uint8 r,g,b;
        SDL_GetRGB(getpixel(temp,i,j),temp->format,&r,&g,&b);
        r=check_color(r,valeur);
        g=check_color(g,valeur);
        b=check_color(b,valeur);
        Uint32 w=SDL_MapRGB(temp->format,r,g,b);
        putpixel(temp,i,j,w);
      }
   }
   SDL_UnlockSurface(temp);
   return temp;
}

SDL_Surface *FillRect(SDL_Surface *surface, SDL_Color color){
  Uint32 *pixels = surface->pixels;
  int startx=0, endx=0, starty=0, endy=0;
  startx = selected_rect.x;
  starty = selected_rect.y;
  endx = startx + selected_rect.w;
  endy = starty + selected_rect.h;
  for(int j=starty;j<endy;j++){
    for(int i=startx;i<endx;i++){
      Uint32 pixel = pixels[j * surface->w +i];
      pixel=SDL_MapRGB(surface->format,color.r,color.g,color.b);
      pixels[j*surface->w+i] = pixel;
    }
  }
  return surface;
}

void rotation(SDL_Renderer *ren, SDL_Texture *tex, double angle){
  SDL_RenderCopyEx(ren,tex,NULL,NULL,angle,NULL,SDL_FLIP_NONE);
}

SDL_Surface *crop_image(SDL_Surface *image, SDL_Rect rect)
{
  SDL_Surface *temp = SDL_CreateRGBSurface(
    image->flags,
    rect.w,
    rect.h,
    image->format->BitsPerPixel,
    image->format->Rmask,
    image->format->Gmask,
    image->format->Bmask,
    image->format->Amask
  );
  int b = SDL_BlitSurface(image, &rect, temp, NULL);
  if(b<0)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "crop blit surface Error %s\n", SDL_GetError() );
    return NULL;
  }
  return temp;
}

SDL_Surface *zoom_image(SDL_Surface *image, SDL_Rect rect)
{
  SDL_Surface *temp = crop_image(image, rect);
  if (!temp) return NULL;
  SDL_Surface *zoom = SDL_CreateRGBSurface(
    image->flags,
    image->w,
    image->h,
    image->format->BitsPerPixel,
    image->format->Rmask,
    image->format->Gmask,
    image->format->Bmask,
    image->format->Amask
  );
  int b = SDL_BlitScaled(temp, NULL, zoom, NULL);
  if(b < 0)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "zoom blit surface Error %s\n", SDL_GetError() );
    return NULL;
  }
  image->pixels = zoom->pixels;
  return zoom;
}

SDL_Surface *resize_image(SDL_Surface *image, SDL_Rect new_size)
{
  SDL_Surface *temp = SDL_CreateRGBSurface(
    image->flags,
    new_size.w,
    new_size.h,
    image->format->BitsPerPixel,
    image->format->Rmask,
    image->format->Gmask,
    image->format->Bmask,
    image->format->Amask
  );
  int b = SDL_BlitScaled(image, NULL, temp, NULL);
  if(b<0)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "crop blit surface Error %s\n", SDL_GetError() );
    return NULL;
  }
  return temp;
}

void copy(SDL_Surface *image)
{
  // printf(" copy \n");
  if(image==NULL)
  {
    if(selected_win_id==-1){
      printf("Nothing selected!\n");
      return;
    }
    image = get_current_window_surface(selected_win_id);
  }
  if(is_selected != 1)
  {
    selected_rect.x = 0;
    selected_rect.y = 0;
    selected_rect.w = image->w;
    selected_rect.h = image->h;
  }
  copied = SDL_CreateRGBSurface(
    image->flags,
    selected_rect.w,
    selected_rect.h,
    image->format->BitsPerPixel,
    image->format->Rmask,
    image->format->Gmask,
    image->format->Bmask,
    image->format->Amask
  );
  SDL_BlitSurface(image, &selected_rect, copied, NULL);
}

void paste(int win_id, SDL_Point p)
{
  SDL_Surface *image = get_current_window_surface(win_id);
  SDL_Rect rect;
  rect.x = p.x;
  rect.y = p.y;
  rect.w = copied->w;
  rect.h = copied->h;
  SDL_BlitSurface(copied, NULL, image, &rect);
  reset_selected_rect();
}

void cut(SDL_Surface *image)
{
  if(!image)
  {
    if(selected_win_id==-1){
      printf("Nothing selected!\n");
      return;
    }
    image = get_current_window_surface(selected_win_id);
  }
  if(is_selected != 1)
  {
    printf(" test not selected");
    selected_rect.x = 0;
    selected_rect.y = 0;
    selected_rect.w = image->w;
    selected_rect.h = image->h;
  }
  SDL_Rect temp;
  temp.x = selected_rect.x;
  temp.y = selected_rect.y;
  temp.w = selected_rect.w;
  temp.h = selected_rect.h;
  copy(image);
  SDL_Color color = {255,255,255,0};
  FillRect(image, color);
}

Uint32 getpixel(SDL_Surface *surface, int x,int y)
{
    int bytpixel = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bytpixel;
    switch(bytpixel) {
      /*here p is the adress to the pixel we want to retrieve*/
    case 1:
        return *p;
    case 2:
        return *(Uint16 *)p;
    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
    case 4:
        return *(Uint32 *)p;
    default:
        return 0;
    }
}

int Flip_Vertical(SDL_Surface *surface){
	Uint32 * pixels = (Uint32 *)surface->pixels;
  Uint32 * pixels2 = (Uint32 *)surface->pixels;
	Uint32 pixel, p2;
        for (int y = 0; y < surface->h/2; y++)
        {
            for (int x = 0; x < surface->w; x++)
            {
              pixel = pixels2[y * surface->w + x];
	          p2 = pixels[(surface->h -y -1)*surface->w + x ];
              pixels[(surface->h -y -1)*surface->w + x ] = pixel;
		      pixels2[y * surface->w + x]= p2;
            }
        }
    return 0;

  }

int Flip_Horizontal(SDL_Surface *surface){
  Uint32 *pixels = (Uint32 *)surface->pixels;
  Uint32 *reversePixels = (Uint32 *)surface->pixels;
  Uint32 pixel;
  for (int y = 0; y < surface->h; y++)
  {
    for (int x = 0; x < surface->w; x++)
    {
      pixel = reversePixels[y * surface->w + x];
      pixels[y * surface->w - x] = pixel;
    }
  }
  return 0;
}

Uint8 check_color(Uint8 c,int i){
  if(c+i>255)    return 255;
  else if(c+i<0) return 0;
  else           return c+i;
}

void brightness(SDL_Surface *surf,int valeur){
   int startx=0,starty=0,endx=0,endy=0;
   startx = selected_rect.x;
   starty = selected_rect.y;
   endx = startx + selected_rect.w;
   endy = starty + selected_rect.h;
    SDL_LockSurface(surf);
    for(int i=startx;i<endx;i++){
      for(int j=starty;j<endy;j++){
	 Uint8 r,g,b;
	 SDL_GetRGB(getpixel(surf,i,j),surf->format,&r,&g,&b);
	 r=check_color(r,valeur);
	 g=check_color(g,valeur);
	 b=check_color(b,valeur);
	 Uint32 w=SDL_MapRGBA(surf->format,r,g,b,valeur);
	 putpixel(surf,i,j,w);

     }
   }
   SDL_UnlockSurface(surf);
}

void alpha(SDL_Surface *surface,int valeur){
   int startx=0,starty=0,endx=0,endy=0;
   startx = selected_rect.x;
   starty = selected_rect.y;
   endx = startx + selected_rect.w;
   endy = starty + selected_rect.h;
    SDL_LockSurface(surface);
    Uint32 *pixels = (Uint32 *) surface->pixels;
    for(int i=startx;i<endx;i++){
      for(int j=starty;j<endy;j++){
              Uint32 pixel = pixels[j * surface->w +i];
              Uint8 r = pixel >> 16 & 0xFF;
              Uint8 g = pixel >> 8 & 0xFF;
              Uint8 b = pixel & 0xFF;
              pixel=SDL_MapRGBA(surface->format,r,g,b,valeur);
              pixels[j*surface->w+i] = pixel;
     }
   }
   SDL_UnlockSurface(surface);
}

void select_f(int win_id)
{
  reset_selected_rect();
  refresh_window(win_id);
  SDL_SetWindowInputFocus(get_window(win_id));
  SDL_Event win_e;
  SDL_Point a, b;
  int end = 2;
  while(end)
  {
    SDL_PollEvent(&win_e);
    {
      if ((win_e.type == SDL_MOUSEBUTTONDOWN) && (end == 2) )
      {
        SDL_GetMouseState(&a.x, &a.y);
        end--;
      }
      if (win_e.type == SDL_MOUSEBUTTONUP)
      {
        SDL_GetMouseState(&b.x, &b.y);
        end--;
      }
      select_rect(win_id,  a,  b);
    }
  }
}


void replace_color(
 SDL_Surface *surface,
 SDL_Color oldColor, SDL_Color newColor, int d){
 Uint32 *pixels = surface->pixels;
 for(int j=selected_rect.y;j<selected_rect.y+selected_rect.h;j++){
		for(int i=selected_rect.x;i<selected_rect.x+selected_rect.w;i++){
     Uint8 r,g,b;
     SDL_GetRGB(getpixel(surface,i,j),surface->format,&r,&g,&b);
     int r_diff = abs(r - oldColor.r);
     int g_diff = abs(g - oldColor.g);
     int b_diff = abs(b - oldColor.b);
     if((r_diff <= d ) && (g_diff <= d) && (b_diff <= d))
     {
       Uint32 pixel = pixels[j*surface->w +i];
       pixel=SDL_MapRGB(surface->format,newColor.r,newColor.g,newColor.b);
   		 pixels[j*surface->w+i] = pixel;
     }
		}
	}
}

void delete(SDL_Surface *image)
{
  if(!image)
  {
    if(selected_win_id==-1){
      printf("Nothing selected!\n");
      return;
    }
    image = get_current_window_surface(selected_win_id);
  }
  if(is_selected != 1)
  {
    printf(" test not selected");
    selected_rect.x = 0;
    selected_rect.y = 0;
    selected_rect.w = image->w;
    selected_rect.h = image->h;
  }
  printf("delete\n");
  Uint32 *pixels = image->pixels;
  int startx=0, endx=0, starty=0, endy=0;
  startx = selected_rect.x;
  starty = selected_rect.y;
  endx = startx + selected_rect.w;
  endy = starty + selected_rect.h;
  for(int j=starty;j<endy;j++){
    for(int i=startx;i<endx;i++){
      Uint32 pixel = pixels[j * image->w +i];
      pixel=SDL_MapRGB(image->format,255,255,255);
      pixels[j*image->w+i] = pixel;
    }
  }
}
