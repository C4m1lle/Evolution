#include "bonhomme.h"

int NB_CREATURE;


struct s_muscle{
    char mobility;
    char strenght;
    char speed;
    int consumption;
    limb proximal_limb;
    limb distal_limb;
};


struct s_point{
    int x,y;
    float vx,vy;
};

struct s_limb{
    float length;
    dot first;
    dot second;
};


struct s_creature{
    int id;
    limb * body;
    dot * body_joints;
    int body_size;
    float energy;
    int x;
    int y;
};

dot create_dot(int x, int y){
    dot d = malloc(sizeof(struct s_point));
    d->x = x;
    d->y = y;
    d->vx = 0.0;
    d->vy = 0.0;
    return d;
}

void delete_dot(dot * d){
    if(*d){
        free(*d);
        *d = NULL;
    }
}

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


limb create_limb(dot first, dot second){
    limb l = malloc(sizeof(struct s_limb));
    if(!l){
        fprintf(stderr,"Erreur création limb\n");
        return NULL;
    }
    l->length = sqrt((first->x-second->x)*(first->x-second->x)+(first->y-second->y)*(first->y-second->y));
    l->first = first;
    l->second = second;
    return l;
}

void delete_limb(limb * l){
    free(*l);
    (*l)=NULL;
}

float random_coord(void){
    return (float)rand()/(float)(RAND_MAX)*300.0;
}
int random_int(int max){
    return rand()%max;
}
int random_int_nor(int max, int forbid){
    int c;
    while((c=rand()%max)==forbid);
    return c;
}
float scal(float ux, float uy, float vx, float vy){
    return ux*vx+uy*vy;
}

int u_proj_v(float ux, float uy, float vx, float vy, float * ox, float * oy){
    float n2 = (sqrt(vx*vx+vy*vy))*(sqrt(vx*vx+vy*vy));
    (*ox) = (scal(ux,uy,vx,vy)/n2)*(vx);
    (*oy) = (scal(ux,uy,vx,vy)/n2)*(vy);
    return 0;
}

void transmit_motion(dot A, dot B, float length, float stiffness){
    float dx = B->x - A->x;
    float dy = B->y - A->y;
    float len = sqrt(dx*dx + dy*dy);

    float ux = dx / len;
    float uy = dy / len;
    float nx = -uy;
    float ny =  ux;

    float v_para  = B->vx*ux + B->vy*uy;
    float v_ortho = B->vx*nx + B->vy*ny;

    float vx_out = v_para * ux + (v_ortho * stiffness) * nx;
    float vy_out = v_para * uy + (v_ortho * stiffness) * ny;

    A->vx += vx_out;
    A->vy += vy_out;

    // contrainte
    float k = length / len;
    B->x = A->x + dx * k;
    B->y = A->y + dy * k;
}


creature create_creature(int body_size, float energy, int x, int y){
    creature c = malloc(sizeof(struct s_creature));
    dot * body_joints = malloc(sizeof(dot)*body_size);
    limb * body = malloc(sizeof(limb)*(body_size-1));
    c->body_size = body_size;
    //srand((unsigned int)time(NULL));
    for(int i = 0; i<body_size;i++){
        body_joints[i] = create_dot(random_coord(),random_coord());
        body_joints[i]->vx = 10.0;
    }
    for(int i = 0; i<body_size-1;i++){
        body[i] = create_limb(body_joints[i],body_joints[i+1]);
    }
 
    if(!c || !body){
        free(c);
        free(body);
        fprintf(stderr,"Erreur création créature(%d)\n",NB_CREATURE);
        return NULL;
    }
    c->id = NB_CREATURE++;
    c->body = body;
    c->body_joints = body_joints;
    c->body_size = body_size;
    c->energy = energy;
    c->x = x;
    c->y = y;
    return c;
}
void update_creature(creature c){
    for(int i = 0; i<c->body_size;i++){
        c->body_joints[i]->x += (int)c->body_joints[i]->vx;
        c->body_joints[i]->y += (int)c->body_joints[i]->vy;
        c->body_joints[i]->vx/=1.4;
        c->body_joints[i]->vy/=1.4;
        
    }
    limb l;
    for(int i = 0; i<c->body_size-1;i++){
        l = c->body[i];
        transmit_motion(l->first,l->second,l->length,0.1);
    }
}

void delete_creature(creature* c){
    for(int i = 0; i<((*c)->body_size-1);i++){
        delete_limb(&((*c)->body[i]));
        delete_dot(&((*c)->body_joints[i]));
    }
    delete_dot(&((*c)->body_joints[(*c)->body_size-1]));
    free((*c)->body);
    free((*c)->body_joints);
    free(*c);
    (*c)=NULL;
}

void draw_creature(creature c, SDL_Renderer* renderer){
    limb l;
    for(int i = 0; i<c->body_size;i++){
        if(!c->body_joints[i]){
            fprintf(stderr,"Erreur accès dot\n");
        }else{
            draw_dot(c->body_joints[i], renderer);
        }
        
    }
    for(int i = 0; i<(c->body_size-1);i++){
        l = c->body[i];
        if(!l){
            fprintf(stderr,"Erreur accès limb\n");
        }else{
            draw_limb(l,renderer);
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
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawLine(renderer,l->first->x, l->first->y,l->second->x, l->second->y);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
}
void draw_dot(dot d, SDL_Renderer* renderer){
    draw_circle(d->x, d->y, 10, renderer);
}



/*
int main(void){
    creature billy = create_creature(5,5.0,500,500);
    delete_creature(&billy);
    //creature billy2 = create_creature(5,5.0,500,500);
    //delete_creature(&billy2);
}
    */