/*
		
	3� A I.S.C UAA en Programaci�n II							22 de diciembre de 2020	 
										Integrantes del Equipo
	Autores del c�digo
			Beltr�n Arroyo Luis
			D�vila Campos Eduardo
			Delgado Luna V�ctor Emilio
			Gaytan S�nchez Brayan de Jes�s
			S�nchez Arias Jos� Emiliano
						
	Descripci�n general del c�digo: Nuestro proyecto "Pacman" es un juego que consiste en utilizar a un personaje redondo cuyo objetivo es comer la mayor cantidad de puntos
	en diversos mapas, sin ser comido por los fantasmas que sirven de obst�culos en el juego. 
	En nuestra versi�n del cl�sico juego, para pasar de nivel hay que comer todos los puntos disponibles en el mapa sin ser asesinado. Contamos con 3 vidas y un archivo binario
	que registra nuestra informaci�n para poder cargar desde el nivel en el que nos quedamos. Ojo, s�lo se carga el nivel completo, no los puntos que ha comido. En cada cambio de 
	nivel hay un guardado autom�tico. 
	Cada funci�n est� comentada por dentro.

	En nuestra matriz Juego los valores se tomaran como los siguientes: 0 = Pacman  1=Margen y bloques 2=Paso libre 3=Indefinido (antes usado como casa de fantasmas)
	4=Frutas 5=P.chicos 6=P.grandes 7= Fantasma1 8= Fantasma2 9=Fantasma3  10=Fantasma4 
	
	DETALLES PARA FACILITAR LA REVISI�N DEL PROFE:
	Puntos: Modificar la constante "Puntos" en "Pacman.hpp" ayudar� a que sean necesarios menos puntos para pasar de nivel.
	Nivel M�ximo: Modificar la constante "NivelM�ximo" en "Mapa.hpp" ayudar� a que pueda decidir qu� nivel es el final.
*/

#include <iostream>
#include <allegro.h>
#include "Funciones.hpp"
#include "Mapa.hpp"
using namespace std;
int main(){
	int Opcion, Salir=4;
	init();//Funci�n de Allegro para iniciarlo
	do {
		Opcion=Portada();//Mandamos llamar a la interfaz de pportada
		menu(Opcion);//Controlamos todo el men�
	} while (Opcion!=Salir);
	deinit();
	return 0;
}
END_OF_MAIN();


