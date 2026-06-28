#ifndef __Admin__h
#define __Admin__h

typedef enum VehicleTypes{CAR=0,TRUCK=1} VehicleType;

class Admin
{

    int spots;
    int available;
    int *occupied;

    /*Finds a spot (floor and column) and returns it*/
    int getSpot(int rank,VehicleType type,bool libre);

    /*Funcion que imprime las occupiedaciones*/
    void printOccupancy();
    
public:

    /*Admin class constructor*/
    Admin(int spots);
    
    /*Requests a spot; returns 0 on success, -1 if no spot available*/
    int enterParking(int rank,VehicleType type);

    /*Removes vehicle from parking; returns 0 on success, -1 on error*/
    int exitParking(int rank,VehicleType type);

    /*Admin class destructor*/
    ~Admin();
};

#endif