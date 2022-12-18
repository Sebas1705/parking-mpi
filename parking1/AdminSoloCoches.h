#ifndef __AdminSoloCoches__h
#define __AdminSoloCoches__h


class Admin
{

    int plazas;
    int libres;
    int *ocup;

    /*Funcion que busca en el parking y devuelve la plaza */
    int getPlaza(int rank,bool libre);


    /*Funcion que imprime las ocupaciones*/
    void printOcups();
    
public:

    /*Constructor de la clase Admin*/
    Admin(int plazas);
    
    /*Funcion que pide plaza y devuelve 0 o -1 si se pudo entrar*/
    int entrarParking(int rank);

    /*Funcion que hace salir al vehiculo y devuelve 0 o -1 si no se pudo salir*/
    int salirParking(int rank);

    /*Destructor de la clase admin*/
    ~Admin();
};

#endif