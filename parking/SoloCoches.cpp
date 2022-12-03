#include "mpi.h"
#include <iostream>
#include <stdlib.h>

using namespace std;


int main(int argc, char **argv){
    
    //Argumentos del main: Plantas Plazas
    if(argc!=2){
        fprintf(stderr,"Error en el numero de parametros [Plantas] [Plazas]: %d, %d",&argv[1],&argv[2]);
        exit(1);
    } 

    int rank, size;
    MPI_Status estado;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
}