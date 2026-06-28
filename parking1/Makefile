all: admin car run

admin: Admin.cpp Admin.h
	mpiCC Admin.cpp Admin.h -o admin

car: Car.cpp
	mpiCC Car.cpp -o car

run: admin car
	mpirun --hostfile hostfile.config -np 1 admin 10 : -np 10 car
