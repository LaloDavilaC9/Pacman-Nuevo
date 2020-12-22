#include <iostream>
#include <allegro.h>
#include "Funciones.hpp"
#include "Mapa.hpp"
using namespace std;
int main(){
	int Opcion, Salir=4;
	init();//Función de Allegro para iniciarlo
	do {
		Opcion=Portada();//Mandamos llamar a la interfaz de pportada
		menu(Opcion);//Controlamos todo el menú
	} while (Opcion!=Salir);
	deinit();
	return 0;
}
END_OF_MAIN();


