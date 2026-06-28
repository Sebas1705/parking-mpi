#include "Admin.h"
#include <mpi.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int Admin::getSpot(int rank,bool libre){
    int x;
    for(int i=0;i<spots;i++){
        if(libre&&occupied[i]==0) return i;
        else if(!libre&&occupied[i]==rank) return i;
    }
    return -1;
}

void Admin::printOccupancy(){
    std::cout<<"Parking: ";
    for(int i=0;i<spots;i++){
        std::cout<<"["<<occupied[i]<<"]";
    }
    std::cout<<"\n";
}

Admin::Admin(int nSpots):  
    spots(nSpots),
    available(nSpots),
    occupied((int*)malloc(sizeof(int)*spots)){
    for(int i=0;i<spots;i++)occupied[i]=0;
}

int Admin::enterParking(int rank){
    int a =getSpot(rank,true);
    if(a==-1)return -1;
    occupied[a]=rank;
    available--;
    std::cout<<"\nEntrada: Coche "<<rank<<" parked at spot "<<a;
    std::cout<<". Plazas available: "<<available<<std::endl;
    printOccupancy();
    return 0;
}

int Admin::exitParking(int rank){
    int a=getSpot(rank,false);
    if(a==-1)return -1;
    occupied[a]=0;
    available++;
    std::cout<<"\nSalida: Coche "<<rank<<" leaving";
    std::cout<<". Plazas available: "<<available<<std::endl;
    printOccupancy();
    return 0;
}

Admin::~Admin(){
    free(this->occupied);
}

int main(int argc, char** argv){
    //Command-line arguments:
    int spots = atoi(argv[1]);

    Admin admin(spots);
    MPI_Status status;

    MPI_Init(&argc, &argv);
    //Mode: 0-enter | 1-exit
    int inMessage[2]; // {rank,modo}

    MPI_Barrier(MPI_COMM_WORLD);

    while(1){
        MPI_Recv(inMessage,2,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status);
        
        //Parking entry:
        if(inMessage[1]==0){
            int assigned=admin.enterParking(inMessage[0]);
            if(assigned==-1)printf("Car %d could not park.\n", inMessage[0]);
            MPI_Send(&assigned,1,MPI_INT,inMessage[0],0,MPI_COMM_WORLD);
        }
        //Parking exit:
        else if(inMessage[1]==1){
            int salida=admin.exitParking(inMessage[0]);
            if(salida==-1) printf("Car %d could not exit the parking.\n", inMessage[0]);
        }
        sleep(1);
    }

    MPI_Finalize();
}