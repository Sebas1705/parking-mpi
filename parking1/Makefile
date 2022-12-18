all: admin coche run

admin: AdminSoloCoches.cpp AdminSoloCoches.h
	mpiCC AdminSoloCoches.cpp AdminSoloCoches.h -o admin

coche: Coche1.cpp
	mpiCC Coche1.cpp -o coche

run: admin coche
	mpirun --hostfile hostfile.config -np 1 admin 10 : -np 10 coche




