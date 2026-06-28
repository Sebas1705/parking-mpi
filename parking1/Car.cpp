#include <mpi.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv){
    
    //Mode: 0-enter | 1-exit
    int outMessage[2]; // {rank,modo}
    int assigned=-1;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Comm_rank(MPI_COMM_WORLD,&outMessage[0]);
    outMessage[1] = 0;//mode: enter
    srand(outMessage[0]*time(NULL));

    while(1){
        //Wait for an available spot:
        while(assigned==-1){
            MPI_Send(outMessage,2,MPI_INT,0,0,MPI_COMM_WORLD);
            MPI_Recv(&assigned,1,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status);
        }

        //Switch mode:
        outMessage[1]=1;//Switch to exit mode
        sleep(rand()%9+1);//Sleep 1-10 seconds
        MPI_Send(outMessage,2,MPI_INT,0,0,MPI_COMM_WORLD);
        
        //Reset variables:
        outMessage[1]=0;
        assigned=-1;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}