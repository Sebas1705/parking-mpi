#include <mpi.h>
#include "Admin.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

#define PLANTAS 1
#define PLAZAS 6

int main(int argc, char **argv){
    
    

    int rank;
    MPI_Status estado;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank==0){
        Admin admin(PLANTAS,PLAZAS);
        int inMessage[3];//Rank,tipo_t,entrar-salir  
        MPI_Barrier(MPI_COMM_WORLD);
        while(1){
            MPI_Recv(inMessage,3,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&estado);
            if(inMessage[2]==0){
                admin.entrarParking(inMessage[0],(inMessage[1]==0)?COCHE:CAMION);
            }else{
                admin.salirParking(inMessage[0],(inMessage[1]==0)?COCHE:CAMION);
            }
        }
        MPI_Finalize();
    }else{
        srand(time(NULL));
        while(1){
            sleep(rand()%5+5);
            int outMessage[]={rank,COCHE,0};
            MPI_Send(outMessage,3,MPI_INT,0,0,MPI_COMM_WORLD);
            sleep(rand()%5+5);
            outMessage[2]=1;
            MPI_Send(outMessage,3,MPI_INT,0,0,MPI_COMM_WORLD);
        } 
        MPI_Finalize();
    }
}