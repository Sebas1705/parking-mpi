#include "AdminSoloPlazas.h"
#include <mpi.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int Admin::getPlaza(int rank,tipo_t tipo,bool libre){
    int x;
    for(int i=0;i<plazas;i++){
        if(tipo==COCHE){
            if(libre&&ocup[i]==0) return i;
            else if(!libre&&ocup[i]==rank) return i;
        }else{
            if(libre&&ocup[i]==0&&ocup[i+1]==0) return i;
            else if(!libre&&ocup[i]==rank) return i;
        }
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
    ocup((int*)malloc(sizeof(int)*plazas)){
    for(int i=0;i<nPlazas;i++)ocup[i]=0;
}

int Admin::entrarParking(int rank,tipo_t tipo){
    int a=getPlaza(rank,tipo,true);
    if(a==-1)return -1;
    if(tipo==COCHE)ocup[a]=rank;
    else{
        ocup[a]=rank;
        ocup[a+1]=rank;
        libres--;
    }
    libres--;
    std::cout<<"\nEntrada: "<<((tipo==COCHE)?"Coche":"Camion")<<" "<<rank<<" aparca en "<<a;
    std::cout<<". Plazas libres: "<<libres<<std::endl;
    printOcups();
    return 0;
}

int Admin::salirParking(int rank,tipo_t tipo){
    int a=getPlaza(rank,tipo,false);
    if(a==-1)return -1;
    if(tipo==COCHE)ocup[a]=0;
    else{
        ocup[a]=0;
        ocup[a+1]=0;
        libres++;
    }
    libres++;
    std::cout<<"\nSalida: "<<((tipo==COCHE)?"Coche":"Camion")<<" "<<rank<<" saliendo";
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
    int inMessage[3]; // {rank,modo,tipo_t}

    MPI_Barrier(MPI_COMM_WORLD);

    while(1){
        MPI_Recv(inMessage,3,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&estado);
        
        //Entrada al parking:
        if(inMessage[1]==0){
            int asignado=admin.entrarParking(inMessage[0],(inMessage[2]==0)?COCHE:CAMION);
            if(asignado==-1){
                if(inMessage[2]==0)printf("Coche %d no ha podido aparcar.\n", inMessage[0]);
                else if(inMessage[2]==1)printf("Camion %d no ha podido aparcar.\n", inMessage[0]);
            }
            MPI_Send(&asignado,1,MPI_INT,inMessage[0],0,MPI_COMM_WORLD);
        }
        //Salida del parking:
        else if(inMessage[1]==1){
            int salida=admin.salirParking(inMessage[0],(inMessage[2]==0)?COCHE:CAMION);
            if(salida==-1){
                if(inMessage[2]==0)printf("Coche %d no pudo dejar el parking.\n", inMessage[0]);
                else if(inMessage[2]==1)printf("Camion %d no pudo dejar el parking.\n", inMessage[0]);
            }
        }
        sleep(1);
    }

    MPI_Finalize();
}