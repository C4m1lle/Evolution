#include "io_reseaux.h"

struct s_neurone{
    float * i;
    float * w;
    float B;
    float O;
};

struct s_layer{
    neurone * layer;
    int size;
    int input_size;
    float ** output;
};

layer create_layer(int input_size, int size){
    layer l = malloc(sizeof(struct s_layer));
    if(!l){
        fprintf(stderr,"Erreur allocation layer\n");
        return NULL;
    }
    l->layer = malloc(sizeof(neurone)*size);
    if(!l->layer){
        fprintf(stderr,"Erreur allocation layer -> [tableau neurones]\n");
        return NULL;
    }
    l->size = size;
    for(int i = 0; i<size;i++){
        l->layer[i] = malloc(sizeof(struct s_neurone));
    }
    l->output = malloc(sizeof(float *)*size);
    for(int i = 0; i<size;i++){
        l->output[i] = &(l->layer[i]->O);
    }
    return l;
}

void compute_neurone(float ** l,int nb, neurone n){
    if(nb!=n->input_size){
        fprintf(stderr,"[Neuron computing] Size mismatch, try to input %d into a %d input sized neuron\n",nb,n->input_size);
        return;
    }
    float sum = n->B;
    for(int i = 0; i<input_size;i++){
        sum+=l[i]*(n->w[i]);
    }
    if(sum<=0){ // ReLu
        n->O = 0;
    }else{
        n->O = sum;
    }
}

void compute_layer(float ** l1, int size_l1, layer l2){
    for(int i = 0; i<l2->size;i++){
        compute_neurone(l1,size_l1,l2[i]);
    }
}
