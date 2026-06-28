#include "Admin.h"
#include <mpi.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int Admin::getSpot(int rank,VehicleType type,bool libre){
    int x;
    for(int i=0;i<spots;i++){
        if(type==CAR){
            if(libre&&occupied[i]==0) return i;
            else if(!libre&&occupied[i]==rank) return i;
        }else{
            if(libre&&occupied[i]==0&&occupied[i+1]==0) return i;
            else if(!libre&&occupied[i]==rank) return i;
        }
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
    for(int i=0;i<nPlazas;i++)occupied[i]=0;
}

int Admin::enterParking(int rank,VehicleType type){
    int a=getSpot(rank,type,true);
    if(a==-1)return -1;
    if(type==CAR)occupied[a]=rank;
    else{
        occupied[a]=rank;
        occupied[a+1]=rank;
        available--;
    }
    available--;
    std::cout<<"\nEntrada: "<<((type==CAR)?"Coche":"Camion")<<" "<<rank<<" parked at spot "<<a;
    std::cout<<". Plazas available: "<<available<<std::endl;
    printOccupancy();
    return 0;
}

int Admin::exitParking(int rank,VehicleType type){
    int a=getSpot(rank,type,false);
    if(a==-1)return -1;
    if(type==CAR)occupied[a]=0;
    else{
        occupied[a]=0;
        occupied[a+1]=0;
        available++;
    }
    available++;
    std::cout<<"\nSalida: "<<((type==CAR)?"Coche":"Camion")<<" "<<rank<<" leaving";
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
    int inMessage[3]; // {rank,modo,VehicleType}

    MPI_Barrier(MPI_COMM_WORLD);

    while(1){
        MPI_Recv(inMessage,3,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status);
        
        //Parking entry:
        if(inMessage[1]==0){
            int assigned=admin.enterParking(inMessage[0],(inMessage[2]==0)?CAR:TRUCK);
            if(assigned==-1){
                if(inMessage[2]==0)printf("Car %d could not park.\n", inMessage[0]);
                else if(inMessage[2]==1)printf("Truck %d could not park.\n", inMessage[0]);
            }
            MPI_Send(&assigned,1,MPI_INT,inMessage[0],0,MPI_COMM_WORLD);
        }
        //Parking exit:
        else if(inMessage[1]==1){
            int salida=admin.exitParking(inMessage[0],(inMessage[2]==0)?CAR:TRUCK);
            if(salida==-1){
                if(inMessage[2]==0)printf("Car %d could not exit the parking.\n", inMessage[0]);
                else if(inMessage[2]==1)printf("Truck %d could not exit the parking.\n", inMessage[0]);
            }
        }
        sleep(1);
    }

    MPI_Finalize();
}