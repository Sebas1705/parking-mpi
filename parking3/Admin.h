#ifndef __Admin__h
#define __Admin__h

typedef enum Tipos{COCHE=0,CAMION=1} tipo_t;

class Admin
{

    int plantas;
    int plazas;
    int libres;
    int **ocup;

    /*Funcion que busca en el parking y devuelve la plaza y la planta*/
    int* getPlazas(int rank,tipo_t tipo,bool libre);

    /*Funcion que imprime las ocupaciones*/
    void printOcups();
    
public:

    /*Constructor de la clase Admin*/
    Admin(int plazas, int plantas);
    
    /*Funcion que pide plaza y devuelve 0 o -1 si se pudo entrar*/
    int entrarParking(int rank,tipo_t tipo);

    /*Funcion que hace salir al vehiculo y devuelve 0 o -1 si no se pudo salir*/
    int salirParking(int rank,tipo_t tipo);

    /*Destructor de la clase admin*/
    ~Admin();
};

#endif