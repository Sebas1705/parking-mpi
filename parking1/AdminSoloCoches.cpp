#include "AdminSoloCoches.h"
#include <mpi.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int Admin::getPlaza(int rank,bool libre){
    int x;
    for(int i=0;i<plazas;i++){
        if(libre&&ocup[i]==0) return i;
        else if(!libre&&ocup[i]==rank) return i;
    }
    return -1;
}

void Admin::printOcups(){
    std::cout<<"Parking: ";
    for(int i=0;i<plazas;i++){
        std::cout<<"["<<ocup[i]<<"]";
    }
    std::cout<<"\n";
}

Admin::Admin(int nPlazas):  
    plazas(nPlazas),
    libres(nPlazas),
    ocup(((int*)malloc(sizeof(int)*plazas))){
    for(int i=0;i<plazas;i++)ocup[i]=0;
}

int Admin::entrarParking(int rank){
    int a =getPlaza(rank,true);
    if(a==-1)return -1;
    ocup[a]=rank;
    libres--;
    std::cout<<"\nEntrada: Coche "<<rank<<" aparca en "<<a;
    std::cout<<". Plazas libres: "<<libres<<std::endl;
    printOcups();
    return 0;
}

int Admin::salirParking(int rank){
    int a=getPlaza(rank,false);
    if(a==-1)return -1;
    ocup[a]=0;
    libres++;
    std::cout<<"\nSalida: Coche "<<rank<<" saliendo";
    std::cout<<". Plazas libres: "<<libres<<std::endl;
    printOcups();
    return 0;
}

Admin::~Admin(){
    free(this->ocup);
}

int main(int argc, char** argv){
    //Argumentos pasados:
    int plazas = atoi(argv[1]);

    Admin admin(plazas);
    MPI_Status estado;

    MPI_Init(&argc, &argv);
    //Modo: 0-entrar | 1-salir
    int inMessage[2]; // {rank,modo}

    MPI_Barrier(MPI_COMM_WORLD);

    while(1){
        MPI_Recv(inMessage,2,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&estado);
        
        //Entrada al parking:
        if(inMessage[1]==0){
            int asignado=admin.entrarParking(inMessage[0]);
            if(asignado==-1)printf("Coche %d no ha podido aparcar.\n", inMessage[0]);
            MPI_Send(&asignado,1,MPI_INT,inMessage[0],0,MPI_COMM_WORLD);
        }
        //Salida del parking:
        else if(inMessage[1]==1){
            int salida=admin.salirParking(inMessage[0]);
            if(salida==-1) printf("Coche %d no pudo dejar el parking.\n", inMessage[0]);
        }
        sleep(1);
    }

    MPI_Finalize();
}