/*
		
	3° A I.S.C UAA en Programación II							22 de diciembre de 2020	 
										Integrantes del Equipo
	Autores del código
			Beltrán Arroyo Luis
			Dávila Campos Eduardo
			Delgado Luna Víctor Emilio
			Gaytan Sánchez Brayan de Jesús
			Sánchez Arias José Emiliano
						
	Descripción general del código: Nuestro proyecto "Pacman" es un juego que consiste en utilizar a un personaje redondo cuyo objetivo es comer la mayor cantidad de puntos
	en diversos mapas, sin ser comido por los fantasmas que sirven de obstáculos en el juego. 
	En nuestra versión del clásico juego, para pasar de nivel hay que comer todos los puntos disponibles en el mapa sin ser asesinado. Contamos con 3 vidas y un archivo binario
	que registra nuestra información para poder cargar desde el nivel en el que nos quedamos. Ojo, sólo se carga el nivel completo, no los puntos que ha comido. En cada cambio de 
	nivel hay un guardado automático. 
	Cada función está comentada por dentro.

	En nuestra matriz Juego los valores se tomaran como los siguientes: 0 = Pacman  1=Margen y bloques 2=Paso libre 3=Indefinido (antes usado como casa de fantasmas)
	4=Frutas 5=P.chicos 6=P.grandes 7= Fantasma1 8= Fantasma2 9=Fantasma3  10=Fantasma4 
	
	DETALLES PARA FACILITAR LA REVISIÓN DEL PROFE:
	Puntos: Modificar la constante "Puntos" en "Pacman.hpp" ayudará a que sean necesarios menos puntos para pasar de nivel.
	Nivel Máximo: Modificar la constante "NivelMáximo" en "Mapa.hpp" ayudará a que pueda decidir qué nivel es el final.
*/

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


