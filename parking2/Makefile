all: admin coche camion run

admin: AdminSoloPlazas.cpp AdminSoloPlazas.h
	mpiCC AdminSoloPlazas.cpp AdminSoloPlazas.h -o admin

coche: Coche.cpp
	mpiCC Coche.cpp -o coche

camion: Camion.cpp
	mpiCC Camion.cpp -o camion

run: admin coche camion
	mpirun --hostfile hostfile.config -np 1 admin 10 : -np 4 coche : -np 4 camion


