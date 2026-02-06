#ifndef CREATURE_H
#define CREATURE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>


typedef struct s_muscle * muscle;
typedef struct s_limb * limb;
typedef struct s_creature * creature;
typedef struct s_point * dot;
/* =======================
   Fonctions muscle
   ======================= */
muscle create_muscle(char mobility, char strenght, char speed);
void delete_muscle(muscle * m);

/* =======================
   Fonctions limb
   ======================= */
limb create_limb(dot first, dot second);
void delete_limb(limb * l);
/* =======================
   Utilitaires
   ======================= */
float random_coord(void);
int random_int(int max);
/* =======================
   Fonctions creature
   ======================= */
creature create_creature(int body_size, float energy, int x, int y);
void delete_creature(creature * c);


void draw_creature(creature c, SDL_Renderer* renderer);
void draw_circle(int x, int y, int r, SDL_Renderer* renderer);
void draw_limb(limb l, SDL_Renderer* renderer);
void draw_dot(dot d, SDL_Renderer* renderer);
#endif /* CREATURE_H */
