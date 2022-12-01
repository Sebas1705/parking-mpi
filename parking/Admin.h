#ifndef __Admin__h
#define __Admin__h
#endif
#include <iostream>

typedef enum Tipos{COCHE,CAMION} tipo_t;

class Admin
{

    int plantas;
    int plazas;
    int **ocup;
    
    /*Funcion que imprime las plazas ocupadas y libres*/
    void print();

    /*Funcion que busca en el parking y devuelve la plaza y la planta*/
    int* getPlazaLibre(tipo_t tipo);

public:

    /*Constructor de la clase Admin*/
    Admin(int plazas, int plantas);
    
    /*Funcion que pide plaza y devuelve la plaza ocupada*/
    int pedirPlaza(tipo_t tipo);

    /*Funcion que imprime las ocupaciones*/
    void printOcups();

    /*Getter plazas*/
    int getPlazas();

    /*Getter plantas*/
    int getPlantas();

    /*Destructor de la clase admin*/
    ~Admin();
};