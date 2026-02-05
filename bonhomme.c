#include "bonhomme.h"

int NB_CREATURE;


struct s_muscle{
    char mobility;
    char strenght;
    char speed;
    int consumption;
};


struct s_limb{
    float length;
    float ax;
    float ay;
    float bx;
    float by;
    struct s_limb * proximal;
    muscle p_muscle;
    struct s_limb *  distal;
    muscle d_muscle;
};


struct s_creature{
    int id;
    limb * body;
    int body_size;
    float energy;
    int x;
    int y;
};

muscle create_muscle(char mobility, char strenght,char speed){
    muscle m = malloc(sizeof(struct s_muscle));
    if(!m){
        fprintf(stderr,"Erreur création muscle (%c,%c,%c)\n",mobility,strenght,speed);
        return NULL;
    }
    m->mobility = mobility;
    m->speed = speed;
    m->strenght = strenght;
    m->consumption = (strenght*speed*speed)/2;
    return m;
}

void delete_muscle(muscle * m){
    if(*m){
        free(*m);
        *m = NULL;
    }
}


limb create_limb(float ax, float ay, float bx, float by){
    limb  l = malloc(sizeof(struct s_limb));
    if(!l){
        fprintf(stderr,"Erreur création limb(%f,%f,%f,%f)\n",ax,ay,bx,by);
        return NULL;
    }
    l->length = sqrt((ax-bx)*(ax-bx)+(ay-by)*(ay-by));
    l->ax = ax;
    l->ay = ay;
    l->bx = bx;
    l->by = by;
    l->proximal = NULL;
    l->p_muscle = NULL;
    l->distal = NULL;
    l-> d_muscle = NULL;
    return l;
}

void delete_limb(limb * l){
    delete_muscle(&((*l)->d_muscle));
    delete_muscle(&((*l)->p_muscle));
    free(*l);
    (*l)=NULL;
}

float random_coord(void){
    return (float)rand()/(float)(RAND_MAX)*5.0;
}

creature create_creature(int body_size, float energy, int x, int y){
    creature c = malloc(sizeof(struct s_creature));
    limb * body = malloc(sizeof(limb)*body_size);
    c->body_size = body_size;
    srand((unsigned int)time(NULL));
    for(int i = 0; i<body_size;i++){
        body[i] = create_limb(random_coord(),random_coord(),random_coord(),random_coord());
    }
    if(!c || !body){
        free(c);
        free(body);
        fprintf(stderr,"Erreur création créature(%d)\n",NB_CREATURE);
        return NULL;
    }
    c->id = NB_CREATURE++;
    c->body = body;
    c->body_size = body_size;
    c->energy = energy;
    c->x = x;
    c->y = y;
    return c;
}

void delete_creature(creature* c){
    for(int i = 0; i<(*c)->body_size;i++){
        delete_limb(&((*c)->body[i]));
    }
    free((*c)->body);
    free(*c);
    (*c)=NULL;
}

void draw_creature(creature c, SDL_Renderer* renderer){
    limb l;
    for(int i = 0; i<c->body_size;i++){
        l = c->body[i];
        if(!l){
            fprintf(stderr,"Erreur accès limb\n");
        }else{
            SDL_RenderDrawLine(renderer,(int)l->ax + c->x,(int)l->ay + c->y,(int)l->bx + c->x,(int)l->by + c->y);
        }

    }
}

void draw_circle(int x, int y, int r, SDL_Renderer* renderer){
    int xp = x-r;
    int yp = y-r;
    int size = (2*r+1)*(2*r+1);
    int tmp = 0;
    SDL_Point * points = malloc(sizeof(SDL_Point)*size);
    for(int i = 0; i<size;i++){
        if(xp==x+r){
            xp = x-r;
            yp++;
        }
        if((xp-x)*(xp-x) + (yp-y)*(yp-y) <= r*r){
            points[tmp].x = xp;
            points[tmp].y = yp;
            tmp++;
            
        }
        xp++;
    }
    SDL_RenderDrawPoints(renderer, points, tmp);
    free(points);
}

void draw_limb(limb l, SDL_Renderer* renderer){
    draw_circle(l->ax, l->ay, 10, renderer);
    draw_circle(l->bx, l->by, 10, renderer);
    SDL_RenderDrawLine(renderer,l->ax, l->ay,l->bx, l->by);
}

limb get_limb(creature c, int id){
    return c->body[id];
}