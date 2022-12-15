all: parking1 parking2 parking3 vehiculos

parking1:
	cd parking1 && make && cd ..

parking2: 
	cd parking2 && make && cd ..

parking3: 
	cd parking1 && make && cd ..

vehiculos: 
	cd parking1 && make && cd ..