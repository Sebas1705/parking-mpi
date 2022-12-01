#include "Admin.h"
#include <iostream>
#include <stdlib.h>

void Admin::print(){

}

int* Admin::getPlazaLibre(tipo_t tipo){
    return NULL;
}

Admin::Admin(int plantas,int plazas){
    this->plazas = plazas;
    this->plantas = plantas;
    this->ocup=(int**)malloc(sizeof(int)*plazas*plantas);
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
}