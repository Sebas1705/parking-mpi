#include "Admin.h"
#include <iostream>
#include <stdlib.h>

void Admin::print(){

}

int* Admin::getPlazaLibre(tipo_t tipo){
    return NULL;
}

Admin::Admin(int nPlantas,int nPlazas): 
    plantas(nPlantas), 
    plazas(nPlazas),
    ocup((int**)malloc(sizeof(int*)*plantas)){
    for(int i=0;i<nPlantas;i++)ocup[i]=(int*)malloc(sizeof(int)*plazas);
}

int Admin::pedirPlaza(tipo_t tipo){
    return 0;
}

void Admin::printOcups(){
    std::cout<<"->Ocupaciones del parking:\n";
    for(int i=0;i<plantas;i++){
        std::cout<<"Planta nº"<<i<<": ";
        for(int j=0;j<plazas;j++){
            std::cout<<"["<<ocup[i][j]<<"]";
        }
        std::cout<<"\n";
    }
    std::cout<<"->Fin Ocupaciones\n";
}

int Admin::getPlantas(){
    return this->plantas;
}

int Admin::getPlazas(){
    return this->plazas;
}

Admin::~Admin(){
    free(this->ocup);
}