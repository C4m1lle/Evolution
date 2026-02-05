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
/* =======================
   Fonctions muscle
   ======================= */
muscle create_muscle(char mobility, char strenght, char speed);
void delete_muscle(muscle * m);

/* =======================
   Fonctions limb
   ======================= */
limb create_limb(float ax, float ay, float bx, float by);
void delete_limb(limb * l);
limb get_limb(creature c, int id);
/* =======================
   Utilitaires
   ======================= */
float random_coord(void);

/* =======================
   Fonctions creature
   ======================= */
creature create_creature(int body_size, float energy, int x, int y);
void delete_creature(creature * c);


void draw_creature(creature c, SDL_Renderer* renderer);
void draw_circle(int x, int y, int r, SDL_Renderer* renderer);
void draw_limb(limb l, SDL_Renderer* renderer);
#endif /* CREATURE_H */
