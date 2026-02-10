#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "bonhomme.h"

int main(void)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    int statut = EXIT_FAILURE;
    int running = 1;
    Uint32 last_time;
    Uint32 current_time;
    int frames = 0;
    char title[128];
    srand((unsigned int)time(NULL));
    if(0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }
    last_time = SDL_GetTicks();
    window = SDL_CreateWindow("Evolution v0.1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1080, 720, SDL_WINDOW_SHOWN);

    if(NULL == window)
    {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        goto Quit;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if(!renderer)
    {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        goto Quit;
    }

    creature billy = create_creature(5,3.0,500,500);
    //creature billy2 = create_creature(5,5.0,800,800);
    SDL_Event event;
    SDL_PollEvent(&event);
    int cpt = 0;
    while(running){
        
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        if(0 != SDL_RenderClear(renderer))
        {
            fprintf(stderr, "Erreur SDL_RenderClear : %s", SDL_GetError());
            goto Quit;
        }
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        update_creature(billy);
        draw_creature(billy,renderer);
        //draw_creature(billy2,renderer);

        


        frames++;
        current_time = SDL_GetTicks();
        if (current_time - last_time >= 1000) {
            float fps = frames * 1000.0f / (current_time - last_time);
            snprintf(title, sizeof(title),
                     "Evolution v0.2 - FPS: %.1f", fps);
        
            SDL_SetWindowTitle(window, title);
        
            frames = 0;
            last_time = current_time;
        }


        SDL_RenderPresent(renderer);
        SDL_Delay(100);
        cpt++;
    }
    delete_creature(&billy);
    //delete_creature(&billy2);
    statut = EXIT_SUCCESS;
    

Quit:
    if(renderer)
        SDL_DestroyRenderer(renderer);
    if(window)
        SDL_DestroyWindow(window);
    SDL_Quit();
        printf("%d frames rendered\n",cpt);
    return statut;
}
