#include <iostream>
#include "Admin.h"
using namespace std;

int main()
{
    Admin admin(1,6);
    admin.entrarParking(1,COCHE);
    admin.entrarParking(2,COCHE);
    admin.entrarParking(3,COCHE);
    admin.entrarParking(4,COCHE);
    admin.entrarParking(5,COCHE);
    admin.salirParking(3,COCHE);
    admin.entrarParking(6,COCHE);
    admin.salirParking(2,COCHE);
    admin.salirParking(1,COCHE);
    admin.entrarParking(101,CAMION);
    return 0;
}
