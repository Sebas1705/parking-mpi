#include "Admin.h"
#include <iostream>
#include <stdlib.h>



int* Admin::getPlazas(int rank,tipo_t tipo,bool libre){
    int *x=(int*)malloc(sizeof(int)*2);
    for(int i=0;i<plantas;i++){
        for(int j=0;j<((tipo==COCHE)?plazas:plazas-1);j++){
            if(tipo==COCHE){
                if(libre&&ocup[i][j]==0){
                    x[0]=i;
                    x[1]=j;
                    return x;
                }else if(!libre&&ocup[i][j]==rank){
                    x[0]=i;
                    x[1]=j;
                    return x;
                }
            }else{
                if(libre&&ocup[i][j]==0&&ocup[i][j+1]==0){
                    x[0]=i;
                    x[1]=j;
                    return x;
                }else if(!libre&&ocup[i][j]==rank){
                    x[0]=i;
                    x[1]=j;
                    return x;
                }
            }
        }
    }
    free(x);
    return NULL;
}

void Admin::printOcups(){
    std::cout<<"Parking:\n";
    for(int i=0;i<plantas;i++){
        std::cout<<"Planta nº"<<i<<": ";
        for(int j=0;j<plazas;j++){
            std::cout<<"["<<ocup[i][j]<<"]";
        }
        std::cout<<"\n";
    }
}

Admin::Admin(int nPlantas,int nPlazas): 
    plantas(nPlantas), 
    plazas(nPlazas),
    libres(nPlantas*nPlazas),
    ocup((int**)malloc(sizeof(int*)*plantas)){
    for(int i=0;i<nPlantas;i++)ocup[i]=(int*)malloc(sizeof(int)*plazas);
}

int Admin::entrarParking(int rank,tipo_t tipo){
    int* a=getPlazas(rank,tipo,true);
    if(a==NULL)return -1;
    if(tipo==COCHE)ocup[a[0]][a[1]]=rank;
    else{
        ocup[a[0]][a[1]]=rank;
        ocup[a[0]][a[1]+1]=rank;
        libres--;
    }
    libres--;
    std::cout<<"\nEntrada: "<<((tipo==COCHE)?"Coche":"Camion")<<" "<<rank<<" aparca en "<<a[0]<<","<<a[1];
    std::cout<<". Plazas libres: "<<libres<<std::endl;
    printOcups();
    return 0;
}

int Admin::salirParking(int rank,tipo_t tipo){
    int *a=getPlazas(rank,tipo,false);
    if(tipo==COCHE)ocup[a[0]][a[1]]=0;
    else{
        ocup[a[0]][a[1]]=rank;
        ocup[a[0]][a[1]+1]=rank;
        libres++;
    }
    libres++;
    std::cout<<"\nSalida: "<<((tipo==COCHE)?"Coche":"Camion")<<" "<<rank<<" saliendo";
    std::cout<<". Plazas libres: "<<libres<<std::endl;
    printOcups();
    return 0;
}

int Admin::getPlantas(){
    return this->plantas;
}

int Admin::getPlazas(){
    return this->plazas;
}

int Admin::getLibres(){
    return this->libres;
}

Admin::~Admin(){
    free(this->ocup);
}