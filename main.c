#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h> 
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h> 
#include <SDL2/SDL_image.h>
#include "create_table.h"
#include <stdlib.h> 

#define WINDOW_TITLE "PACMAN"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600 
#define MAZE_OFFSET_Y 100
#define CELL_SIZE 16
#define EXIT_FAILURE 1 
#define EXIT_SUCCES 0 
#define SIZE_col 50
#define SIZE_lin 25 
#define WALL '#'
#define DOT '.' 
#define EMPTY ' '
#define TEXT_SIZE 24

struct ENTITY {
    int lin ; 
    int col ; 
    int dx ; 
    int dy ; 
    uint32_t last_move_time ; 
} ; 

struct Game {
   SDL_Window *window ; 
   SDL_Renderer *renderer ; 
   SDL_Texture *Ghost_Texture ; 
   SDL_Texture *Pacman_icon ; 
   SDL_Texture *Cherry_Texture ;
   TTF_Font *Text_Font ;  
   SDL_Color Text_Color ; 

   SDL_Texture *Score_Texture ; 
   SDL_Rect Score_Rect ; 
   int score ; 

   struct ENTITY PACMAN ; 
   struct ENTITY GHOST[2] ; 
    
}; 

void game_cleanup(struct Game *game) ; 
bool sdl_initialize(struct Game *game) ;
bool load_media(struct Game *game) ; 
void draw_map(struct Game *game , char **table); 
bool load_score_texture(struct Game *game) ; 
void update_poz_pacman(struct Game *game, char **table);

int main (void) {
    struct Game game ; 
    char **table ; 

    game.window=NULL ; 
    game.renderer=NULL ; 
    game.Ghost_Texture=NULL ; 
    game.Pacman_icon=NULL ; 
    game.Cherry_Texture=NULL ; 
    game.Text_Font=NULL ; 
    game.Text_Color=(SDL_Color){255,255,255,255} ; 
    game.Score_Texture=NULL ; 
    game.score=0 ; 
    game.PACMAN=(struct ENTITY){1,1,0,0,0} ; 
    game.GHOST[0]=(struct ENTITY){16,10,0,0,0} ; 
    game.GHOST[1]=(struct ENTITY){16,39,0,0,0} ; 

    if(!sdl_initialize(&game)){
       game_cleanup(&game) ; 
       exit(EXIT_FAILURE) ; 
    } 

    if(!load_media(&game) || !load_score_texture(&game)){
       game_cleanup(&game) ; 
       exit(EXIT_FAILURE) ; 
    } 

    if((table=create_table())==NULL){  // creez tabla (matrice) , alocata dinamic
     game_cleanup(&game) ; 
     exit(EXIT_FAILURE) ; 
    }

    bool running = true ; 
    SDL_Event event ; 
    SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255); // setez background negru 

    while (running) {
      while(SDL_PollEvent(&event)) { //procesez fiecare eveniment aparut

         switch(event.type) {
            case SDL_QUIT: 
             running=false ; 
             break ; 

            case SDL_KEYDOWN: 
             switch(event.key.keysym.scancode) {
            
                case SDL_SCANCODE_ESCAPE: 
                    running=false ;
                    break ; 
                case SDL_SCANCODE_UP :
                  game.PACMAN.dx=0 ; 
                  game.PACMAN.dy=-1 ; 
                  break ; 
                case SDL_SCANCODE_DOWN : 
                  game.PACMAN.dx=0 ; 
                  game.PACMAN.dy=+1 ;
                  break ; 
                case SDL_SCANCODE_RIGHT : 
                  game.PACMAN.dx=+1 ; 
                  game.PACMAN.dy=0 ;
                  break ; 
                case SDL_SCANCODE_LEFT :
                  game.PACMAN.dx=-1 ; 
                  game.PACMAN.dy=0 ;
                  break ; 
                default:
                    break ; 

            }
            
            default:
                break ; 

         }
      }
        
     SDL_RenderClear(game.renderer) ; 

     update_poz_pacman(&game , table) ; 
     draw_map(&game , table) ; 

     SDL_RenderPresent(game.renderer) ; 
     SDL_Delay(12) ; 

    }
  
    game_cleanup(&game) ;
    free_table(table) ; 
    return 0 ; 
} 

bool sdl_initialize (struct Game *game) {
 if(SDL_Init(SDL_INIT_EVERYTHING) != 0 ) {
    fprintf(stderr, "Eroare , nu s-a putut initializa SDL . Motivul : %s \n" , SDL_GetError()) ; 
    return false ;  
 }

 if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)!=IMG_INIT_PNG) {
    printf("Eroare , nu s-au putut genera imaginile: %s\n", IMG_GetError());
    return false;
}
 if(TTF_Init()) {
   printf("Eroare , nu s-au putut genera imaginile: %s\n", TTF_GetError());
    return false;
 }

 game->window=SDL_CreateWindow(WINDOW_TITLE , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , SCREEN_WIDTH , SCREEN_HEIGHT , 0) ; 
 if(!game->window) {
    fprintf(stderr, "NU S-A PUTUT CREA FEREASTRA. MOTIVUL : %s \n" , SDL_GetError()) ; 
    return false ; 
 }

 game->renderer=SDL_CreateRenderer(game->window , -1 , 0) ; 
  if(!game->renderer){
     fprintf(stderr, "NU S-A PUTUT CREA RENDERER-UL . MOTIVUL : %s \n" , SDL_GetError()) ; 
     return false ; 
  }
 
 return true ; 
}

void game_cleanup(struct Game *game) {
   if(game->Score_Texture!=NULL) {
      SDL_DestroyTexture(game->Score_Texture) ; 
      game->Score_Texture=NULL ; 
   }
   if(game->Text_Font!=NULL) {
        TTF_CloseFont(game->Text_Font) ; 
        game->Text_Font=NULL ; 
    }
   if(game->Cherry_Texture!=NULL) {
        SDL_DestroyTexture(game->Cherry_Texture) ; 
        game->Cherry_Texture=NULL ; 
    }
   if(game->Pacman_icon!=NULL) {
        SDL_DestroyTexture(game->Pacman_icon) ; 
        game->Pacman_icon=NULL ; 
    }
    if(game->Ghost_Texture!=NULL) {
        SDL_DestroyTexture(game->Ghost_Texture) ; 
        game->Ghost_Texture=NULL ; 
    }
    if(game->renderer !=NULL) {
        SDL_DestroyRenderer(game->renderer) ; 
        game->renderer=NULL ; 
    }
    if(game->window!=NULL) { 
        SDL_DestroyWindow(game->window) ; 
        game->window=NULL ; 
    }
  TTF_Quit() ; 
  IMG_Quit();
  SDL_Quit() ; 
}
void draw_map(struct Game *game , char **table){
 SDL_Rect cel , pct ; 

 for (int i=0 ; i<SIZE_lin ; i++) {
   for(int j=0 ; j<SIZE_col ; j++) {
       cel.x = j* CELL_SIZE ; 
       cel.y = i* CELL_SIZE + MAZE_OFFSET_Y; 
       cel.w = CELL_SIZE ; 
       cel.h = CELL_SIZE ; 
       
       switch(table[i][j]) {
         case WALL : 
            SDL_SetRenderDrawColor(game->renderer , 0 , 0 , 255 ,255);
            SDL_RenderFillRect(game->renderer , &cel) ; 
            break ; 
         case DOT : 
            SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(game->renderer, &cel);
            SDL_SetRenderDrawColor(game->renderer , 255,255,255,255) ; 
            pct.x= cel.x+CELL_SIZE/2 - 2 ; 
            pct.y= cel.y+CELL_SIZE/2 - 2 ; 
            pct.w=4 ; 
            pct.h=4 ; 
            SDL_RenderFillRect(game->renderer, &pct) ; 
            break ; 
         case CHERRY:
            SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(game->renderer, &cel);
            SDL_RenderCopy(game->renderer , game->Cherry_Texture , NULL , &cel);
            break ; 
         default : 
            SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(game->renderer, &cel);
            break ; 
       }
       
      
       if (i == game->PACMAN.lin && j == game->PACMAN.col) {
         SDL_RenderCopy(game->renderer, game->Pacman_icon, NULL, &cel);
         }

      for (int g = 0; g < 2; g++) {
         if (i == game->GHOST[g].lin && j == game->GHOST[g].col) {
            SDL_RenderCopy(game->renderer, game->Ghost_Texture, NULL, &cel);
         }
      }

   }
 }
   SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
   SDL_RenderFillRect(game->renderer, &game->Score_Rect);
   SDL_RenderCopy(game->renderer , game->Score_Texture , NULL , &game->Score_Rect);
}
bool load_media(struct Game *game) {
  game->Ghost_Texture=IMG_LoadTexture(game->renderer , "Images_PACMAN/ghost_pacman.png");
  if(!game->Ghost_Texture) {
    fprintf(stderr, "NU S-A PUTUT CREA TEXTURA FANTOMELOR . MOTIVUL : %s \n" , IMG_GetError()) ; 
     return false ; 
  }
  game->Pacman_icon=IMG_LoadTexture(game->renderer , "Images_PACMAN/PACMAN.png");
  if(!game->Pacman_icon) {
    fprintf(stderr, "NU S-A PUTUT CREA TEXTURA FANTOMELOR . MOTIVUL : %s \n" , IMG_GetError()) ; 
     return false ; 
  }
   game->Cherry_Texture=IMG_LoadTexture(game->renderer , "Images_PACMAN/CHERRY.png");
  if(!game->Cherry_Texture) {
    fprintf(stderr, "NU S-A PUTUT CREA TEXTURA CIRESELOR . MOTIVUL : %s \n" , IMG_GetError()) ; 
     return false ; 
  }
   game->Text_Font = TTF_OpenFont("Font_txt/Arial.ttf" , TEXT_SIZE) ;
   if(!game->Text_Font) {
       fprintf(stderr, "NU S-A PUTUT CREA FONTUL DE TEXT . MOTIVUL : %s \n" , TTF_GetError()) ; 
        return false ; 
   } 


  return true ; 
}
bool load_score_texture(struct Game *game) {
 char score[64] ;
 snprintf(score , sizeof(score) , "SCORE : %d" , game->score) ; 
 
 if(game->Score_Texture!=NULL) {
   SDL_DestroyTexture(game->Score_Texture) ; 
   game->Score_Texture=NULL ; 
 } 

 SDL_Surface *surface = TTF_RenderText_Blended(game->Text_Font , score , game->Text_Color) ; 
 if(!surface){
   fprintf(stderr , "SUPRAFATA DE SCOR NU A PUTUT FI GENERATA. MOTIVUL: %s \n" , TTF_GetError()) ; 
   return false ; 
 }

 game->Score_Texture=SDL_CreateTextureFromSurface(game->renderer , surface) ; 
 if(!game->Score_Texture) {
   fprintf(stderr , "TEXTURA SCORULUI NU A PUTUT FI GENERATA. MOTIVUL: %s \n" , SDL_GetError()) ; 
   SDL_FreeSurface(surface) ; 
   return false ; 
 }
 
  game->Score_Rect.w = surface->w;
  game->Score_Rect.h = surface->h;
  game->Score_Rect.x = 10;
  game->Score_Rect.y = 10;

   SDL_FreeSurface(surface);
    return true;
}
void update_poz_pacman(struct Game *game, char **table){
  int next_lin = game->PACMAN.lin + game->PACMAN.dy ; 
  int next_col = game->PACMAN.col + game->PACMAN.dx ; 

  uint32_t current_time ; 
  current_time = SDL_GetTicks() ; 

  if(current_time-game->PACMAN.last_move_time<100) {
    return ; 
  }

  game->PACMAN.last_move_time=current_time ; 
  
  if(next_lin <0 || next_lin>=SIZE_lin || next_col<0 || next_col>=SIZE_col) {
     return ; 
  }
   
  if(table[next_lin][next_col]==WALL) {
   return ; 
  }

  if(table[next_lin][next_col]==DOT) {
    game->score+=10 ; 
    table[next_lin][next_col]=EMPTY ; 
    load_score_texture(game) ; 
  }

   game->PACMAN.lin=next_lin ;
   game->PACMAN.col=next_col ; 

}