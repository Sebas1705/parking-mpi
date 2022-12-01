#include "mpi.h"
#include <iostream>
using namespace std;
int main(int argc, char *argv[]) {
 int rank, size, contador;
 MPI_Status estado;
 MPI_Init(&argc, &argv);
 MPI_Comm_size(MPI_COMM_WORLD, &size);
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 //Sólo el proceso 0
 if(rank == 0){
 MPI_Send(&rank //referencia al vector de elementos a enviar
,1 // tamaño del vector a enviar
,MPI_INT // Tipo de dato que envias
,rank+1 // pid del proceso destino
 ,0 //etiqueta
 ,MPI_COMM_WORLD); //Comunicador por el que se manda
 } else{
 MPI_Recv(&contador // Referencia al vector donde se almacenara lo recibido
 ,1 // tamaño del vector a recibir
 ,MPI_INT // Tipo de dato que recibe
 ,rank-1 // pid del proceso origen de la que se recibe
 ,0 // etiqueta
 ,MPI_COMM_WORLD // Comunicador por el que se recibe
 ,&estado); // estructura informativa del estado
 cout<<"Soy el proceso "<<rank<<" y he recibido "<<contador<<endl;
 contador++;
 //El último proceso no envía, sólo recibe
 if(rank != size-1)
 MPI_Send(&contador, 1 ,MPI_INT ,rank+1 , 0 ,MPI_COMM_WORLD);
 }
 MPI_Finalize();
 return 0;
}
