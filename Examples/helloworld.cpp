#include "mpi.h"
#include <iostream>
using namespace std;
int main(int argc, char *argv[]) {
 int rank, size, contador;
 MPI_Status status;
 MPI_Init(&argc, &argv);
 MPI_Comm_size(MPI_COMM_WORLD, &size);
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 //Only process 0
 if(rank == 0){
 MPI_Send(&rank //pointer to the data buffer to send
,1 // send buffer size
,MPI_INT // MPI data type being sent
,rank+1 // destination process rank
 ,0 //message tag
 ,MPI_COMM_WORLD); //communicator
 } else{
 MPI_Recv(&contador // receive buffer
 ,1 // receive buffer size
 ,MPI_INT // MPI data type being received
 ,rank-1 // source process rank
 ,0 // message tag
 ,MPI_COMM_WORLD // communicator
 ,&status); // estructura informativa del status
 cout<<"I am process "<<rank<<" and I received "<<contador<<endl;
 contador++;
 //The last process only receives, does not send
 if(rank != size-1)
 MPI_Send(&contador, 1 ,MPI_INT ,rank+1 , 0 ,MPI_COMM_WORLD);
 }
 MPI_Finalize();
 return 0;
}
