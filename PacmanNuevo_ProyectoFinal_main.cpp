#include <iostream>
#include "Funciones.hpp"
using namespace std;

int main(){
	init();
	int Opcion, Salir=4;
	do {
		Opcion=Portada();
		menu(Opcion);
	} while (Opcion!=Salir);
	deinit();
	return 0;
}
END_OF_MAIN();


