all: admin car truck run

admin: Admin.cpp Admin.h
	mpiCC Admin.cpp Admin.h -o admin

car: Car.cpp
	mpiCC Car.cpp -o car

truck: Truck.cpp
	mpiCC Truck.cpp -o truck

run: admin car truck
	mpirun --hostfile hostfile.config -np 1 admin 10 : -np 4 car : -np 4 truck
