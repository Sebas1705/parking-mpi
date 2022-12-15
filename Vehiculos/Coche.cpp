#include <mpi.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv){
    
    //Modo: 0-entrar | 1-salir
    //Tipo: 0-Coche | 1-Camion
    int outMessage[3]; // {rank,modo,tipo}
    int asignado=-1;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Comm_rank(MPI_COMM_WORLD,&outMessage[0]);
    outMessage[1] = 0;//modo: entrar
    outMessage[2] = 0;//tipo_t: Coche
    srand(outMessage[0]*time(NULL));

    while(1){
        //Esperar hueco:
        while(asignado==-1){
            MPI_Send(outMessage,3,MPI_INT,0,0,MPI_COMM_WORLD);
            MPI_Recv(&asignado,1,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status);
        }

        //Cambio de modo:
        outMessage[1]=1;//Cambiamos a salir
        sleep(rand()%9+1);//Dormimos de 1-10segs
        MPI_Send(outMessage, 3, MPI_INT, 0, 0, MPI_COMM_WORLD);
        
        //Reseteo de variables:
        outMessage[1]=0;
        asignado=-1;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}