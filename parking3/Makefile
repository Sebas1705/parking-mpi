all: admin coche camion run

admin: Admin.cpp Admin.h
	mpiCC Admin.cpp Admin.h -o admin

coche: Coche.cpp
	mpiCC Coche.cpp -o coche

camion: Camion.cpp
	mpiCC Camion.cpp -o camion

run: admin coche camion
	mpirun --hostfile hostfile.config -np 1 admin 10 4 : -np 4 coche : -np 4 camion

