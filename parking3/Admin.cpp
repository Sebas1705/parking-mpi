#include "Admin.h"
#include <mpi.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

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
    if(a==NULL)return -1;
    if(tipo==COCHE)ocup[a[0]][a[1]]=0;
    else{
        ocup[a[0]][a[1]]=0;
        ocup[a[0]][a[1]+1]=0;
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
    int plantas = atoi(argv[2]);

    Admin admin(plantas,plazas);
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