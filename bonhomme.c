#include <stdio.h>
#include <stdlib.h>

int NB_CREATURE;

typedef struct s_muscle{
    char mobility;
    char strenght;
    char speed;
    int consumption;
}muscle;


typedef struct s_limb{
    float length;
    float ax;
    float ay;
    float bx;
    float by;
    limb * proximal;
    muscle * p_muscle;
    limb * distal;
    muscle * d_muscle;
}limb;


typedef struct s_creature{
    int id;
    limb ** body;
    int body_size;
    float energy;
}creature;

muscle * create_muscle(char mobility, char strenght,char speed){
    muscle * m = malloc(sizeof(muscle));
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

void delete_muscle(muscle ** m){
    free(*m);
    (*m)=NULL;
}

limb * create_limb(float ax, float ay, float bx, float by){
    limb * l = malloc(sizeof(limb));
    if(!l){
        fprintf(stderr,"Erreur création limb(%f,%f,%f,%f)\n",ax,ay,bx,by);
        return NULL;
    }
    l->length = 
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

void delete_limb(limb ** l){
    delete_muscle(&((*l)->d_muscle));
    delete_muscle(&((*l)->p_muscle));
    free(*l);
    (*l)=NULL;
}


creature * create_creature(int body_size, float energy){
    creature * c = malloc(sizeof(creature));
    limb ** body = malloc(sizeof(limb *)*body_size);
    for(int i = 0; i<c->body_size;i++){
        body[i] = create_limb()
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
    return c;
}

void delete_creature(creature ** c){
    for(int i = 0; i<(*c)->body_size;i++){
        delete_limb(&((*c)->body[i]));
    }
    free(*c);
    (*c)=NULL;
}