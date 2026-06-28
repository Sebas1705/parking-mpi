#ifndef __Admin__h
#define __Admin__h


class Admin
{

    int spots;
    int available;
    int *occupied;

    /*Finds a parking spot and returns its index*/
    int getSpot(int rank,bool libre);


    /*Funcion que imprime las occupiedaciones*/
    void printOccupancy();
    
public:

    /*Admin class constructor*/
    Admin(int spots);
    
    /*Requests a spot; returns 0 on success, -1 if no spot available*/
    int enterParking(int rank);

    /*Removes vehicle from parking; returns 0 on success, -1 on error*/
    int exitParking(int rank);

    /*Admin class destructor*/
    ~Admin();
};

#endif