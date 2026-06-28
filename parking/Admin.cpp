#include "Admin.h"
#include <mpi.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int* Admin::getSpots(int rank,VehicleType type,bool libre){
    int *x=(int*)malloc(sizeof(int)*2);
    for(int i=0;i<floors;i++){
        for(int j=0;j<((type==CAR)?spots:spots-1);j++){
            if(type==CAR){
                if(libre&&occupied[i][j]==0){
                    x[0]=i;
                    x[1]=j;
                    return x;
                }else if(!libre&&occupied[i][j]==rank){
                    x[0]=i;
                    x[1]=j;
                    return x;
                }
            }else{
                if(libre&&occupied[i][j]==0&&occupied[i][j+1]==0){
                    x[0]=i;
                    x[1]=j;
                    return x;
                }else if(!libre&&occupied[i][j]==rank){
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

void Admin::printOccupancy(){
    std::cout<<"Parking:\n";
    for(int i=0;i<floors;i++){
        std::cout<<"Floor "<<i<<": ";
        for(int j=0;j<spots;j++){
            std::cout<<"["<<occupied[i][j]<<"]";
        }
        std::cout<<"\n";
    }
}

Admin::Admin(int nFloors,int nSpots): 
    floors(nFloors), 
    spots(nSpots),
    available(nFloors*nSpots),
    occupied((int**)malloc(sizeof(int*)*floors)){
    for(int i=0;i<nPlantas;i++)occupied[i]=(int*)malloc(sizeof(int)*spots);
}

int Admin::enterParking(int rank,VehicleType type){
    int* a=getSpots(rank,type,true);
    if(a==NULL)return -1;
    if(type==CAR)occupied[a[0]][a[1]]=rank;
    else{
        occupied[a[0]][a[1]]=rank;
        occupied[a[0]][a[1]+1]=rank;
        available--;
    }
    available--;
    std::cout<<"\nEntrada: "<<((type==CAR)?"Coche":"Camion")<<" "<<rank<<" parked at spot "<<a[0]<<","<<a[1];
    std::cout<<". Plazas available: "<<available<<std::endl;
    printOccupancy();
    return 0;
}

int Admin::exitParking(int rank,VehicleType type){
    int *a=getSpots(rank,type,false);
    if(a==NULL)return -1;
    if(type==CAR)occupied[a[0]][a[1]]=0;
    else{
        occupied[a[0]][a[1]]=rank;
        occupied[a[0]][a[1]+1]=rank;
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
    int floors = atoi(argv[2]);

    Admin admin(floors,spots);
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