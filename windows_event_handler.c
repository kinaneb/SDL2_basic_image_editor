/* ****************************************************** */
/* BSHARA Kinan                                           */
/* rasterIE program                                       */
/* windows_event_handler.c                                */
/* ****************************************************** */



#include "windows_event_handler.h"
void mac_problem()
{
  SDL_Event win_e;
  SDL_PollEvent(&win_e);
  // if(win_e.type == SDL_WINDOWEVENT){}
}

// void handle_window(SDL_Event *win_e)
void handle_window(void)
{
  SDL_Event win_e;
  SDL_PollEvent(&win_e);
    switch(win_e.type)
    {
      case SDL_WINDOWEVENT :
      { // this block just scopes 'targetWindow' and 'title'
        SDL_Window* selectedWindow = SDL_GetWindowFromID(win_e.window.windowID);
        const char *title = SDL_GetWindowTitle(selectedWindow);
        switch(win_e.window.event)
        {
          case SDL_WINDOWEVENT_FOCUS_GAINED :
          // tell which window gained focus

          break;

          case SDL_WINDOWEVENT_FOCUS_LOST :
          // tell which window lost focus

          break;

          case SDL_WINDOWEVENT_CLOSE:
          close_window(get_window_id_from_title(title));
          break;
        }
      }
      break;
      case SDL_QUIT :
      break;
    }
}




// void *handle_windows(void *arg)
// void handle_windows()
// {
//   SDL_Surface *delete;
//   int pi = 0;
//   int exit_window = 0;
//   while(!exit_window)
//   {
//     SDL_Event win_e;
//     while(!exit_window && SDL_PollEvent(&win_e))
//     {
//       switch(win_e.type)
//       {
//         case SDL_WINDOWEVENT :
//         { // this block just scopes 'targetWindow' and 'title'
//           SDL_Window* selectedWindow = SDL_GetWindowFromID(win_e.window.windowID);
//           const char *title = SDL_GetWindowTitle(selectedWindow);
//           switch(win_e.window.event)
//           {
//             case SDL_WINDOWEVENT_FOCUS_GAINED :
//             // tell which window gained focus
//
//             break;
//
//             case SDL_WINDOWEVENT_FOCUS_LOST :
//             // tell which window lost focus
//
//             break;
//
//             case SDL_WINDOWEVENT_CLOSE:
//             close_window(get_window_id_from_title(title));
//             break;
//           }
//         }
//         break;
//         case SDL_QUIT :
//         exit_window=1;
//         break;
//       }
//     }
//   }
//   SDL_Quit();
//   // return NULL;
// }
