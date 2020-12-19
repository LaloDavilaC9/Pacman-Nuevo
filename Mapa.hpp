#ifndef MAPA_H_INCLUDED
	#include "Jugadores.hpp"
#define MAPA_H_INCLUDED

#define NIVELMAXIMO 10 //Importante no usar un valor menor a 2 ni mayor a 10
#define VELOCIDAD 200 //Importante no usar valores negativos
#define AZUL makecol(51, 153, 255)//Color predefinido
#define NEGRO makecol(0, 0, 0)//Color predefido

class Pacman;
class Clyde;

class Mapa {
	private:
		int matrizJuego[20][30], dificultad,direccionesVisuales[5],frutas,sonido,puntuacionTotal, pPausaF[4];
		int frutaI,frutaJ;
		bool cambioNivel;
		BITMAP *vectorPintarMapa[9],*vectorMotorJuego[15],*buffer;
		FONT *font1, *font2, *font3;
	public:
		Mapa();
		void mapa_1();
		void mapa_2();
		void mapa_3();
		//int menuPrincipal();
		//int menuSecundario();
		void seleccionarMapa(int);
		void dibujarMapa(int,bool);
		void ponerFruta();
		void motorJuego(Jugadores *jugadorActual);
		void setDirecciones(int pos,int valor){this->direccionesVisuales[pos]=valor;}
		void setPuntuacionTotal(int valor){this->puntuacionTotal=valor;}
		int getPuntuacionTotal(){return this->puntuacionTotal;}
		void setFrutas(int valor){this->frutas=valor;}
		int getFrutas(){return this->frutas;}
		void setCambioNivel(bool valor){this->cambioNivel=valor;}
		void setPausaF(int pos, int valor){this->pPausaF[pos]=valor;}
		void setMatrizJuego(int i, int j, int valor){this->matrizJuego[i][j]=valor;}
		int getMatrizJuego(int i, int j){return this->matrizJuego[i][j];}
		int getSonido(){return this->sonido;}
		void reiniciarFantasmas(Clyde &,Pacman &);
};

Mapa::Mapa() {
	int i,j;
	for (i = 0; i < 20; i++) {
		for (j = 0; j < 30; j++) {
			matrizJuego[i][j] = 0;
		}
	}
	for(i=0;i<5;i++)
		this->direccionesVisuales[i]=1;
	for(i=0;i<4;i++)
		this->pPausaF[i]=0;
	this->frutas=0;
	this->frutaI=-1;
	this->frutaJ=-1;
	this->sonido=0;//Significa sonido Activado
	this->puntuacionTotal=0;	
	this->dificultad=0;
	this->cambioNivel=false;
	this->vectorPintarMapa[0]=load_bitmap("Elementos\\Bloques_1.bmp", NULL);
	this->vectorPintarMapa[1]=load_bitmap("Elementos\\Espacio.bmp", NULL);
	this->vectorPintarMapa[2]=load_bitmap("Elementos\\PuntosChicos.bmp", NULL);
	this->vectorPintarMapa[3]=load_bitmap("Elementos\\PuntosGrandes.bmp", NULL);
	this->vectorPintarMapa[4]=load_bitmap("Elementos\\FantasmaBlinky_D.bmp", NULL);
	this->vectorPintarMapa[5]=load_bitmap("Elementos\\FantasmaInky_D.bmp", NULL);
	this->vectorPintarMapa[6]=load_bitmap("Elementos\\FantasmaPinky_D.bmp", NULL);
	this->vectorPintarMapa[7]=load_bitmap("Elementos\\FantasmaClyde_D.bmp", NULL);
	this->vectorPintarMapa[8]=load_bitmap("Elementos\\CuerpoPacman_I.bmp", NULL);
	this->vectorMotorJuego[1]=load_bitmap("Elementos\\Vidas1.bmp",NULL);
	this->vectorMotorJuego[2]=load_bitmap("Elementos\\Vidas2.bmp",NULL);
	this->vectorMotorJuego[3]=load_bitmap("Elementos\\Vidas3.bmp",NULL);
	this->vectorMotorJuego[4]=load_bitmap("Elementos\\1Frutas.bmp",NULL);
	this->vectorMotorJuego[5]=load_bitmap("Elementos\\2Frutas.bmp",NULL);
	this->vectorMotorJuego[6]=load_bitmap("Elementos\\3Frutas.bmp",NULL);
	this->vectorMotorJuego[7]=load_bitmap("Elementos\\4Frutas.bmp",NULL);
	this->vectorMotorJuego[8]=load_bitmap("Elementos\\5Frutas.bmp",NULL);
	this->vectorMotorJuego[9]=load_bitmap("Elementos\\advertenciasalir.bmp",NULL);
	this->vectorMotorJuego[10]=load_bitmap("Elementos\\cargando1.bmp",NULL);
	this->vectorMotorJuego[11]=load_bitmap("Elementos\\cargando2.bmp",NULL);
	this->vectorMotorJuego[12]=load_bitmap("Elementos\\cargando3.bmp",NULL);
	this->vectorMotorJuego[13]=load_bitmap("Elementos\\cargando0.bmp", NULL);
	this->vectorMotorJuego[14]=load_bitmap("Elementos\\perdiste.bmp", NULL);
	this->vectorMotorJuego[0]=load_bitmap("Elementos\\Felicidades.bmp", NULL);
	this->font1=load_font("Elementos\\letritas.pcx", NULL, NULL);
	this->font2=load_font("Elementos\\letritas2.pcx", NULL, NULL);
	this->font3=load_font("Elementos\\LetritasPoder.pcx",NULL,NULL);
	this->buffer=create_bitmap(960, 660);
}
void Mapa::mapa_1() {
	//vector<vector<Nodo>> redNodos;
	int matrizJuegoAux[20][30]{
		//   0 1 2 3 4 5 6 7 8 9 10        15        20        25      29
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},//0
			{1,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,1},//1
			{1,2,1,2,1,1,2,1,1,1,1,2,1,2,1,1,2,1,2,1,1,1,1,2,1,1,2,1,2,1},//2									
			{1,2,1,2,1,1,2,1,1,2,2,2,1,2,2,2,2,1,2,2,2,1,1,2,1,1,2,1,2,1},//3
			{1,2,1,2,1,1,2,2,2,2,1,2,1,2,1,1,2,1,2,1,2,2,2,2,1,1,2,1,2,1},//4										
			{1,2,2,2,2,2,2,1,2,1,1,2,1,2,2,2,2,1,2,1,1,2,1,2,2,2,2,2,2,1},//5											
			{1,2,1,1,2,1,1,1,2,2,1,2,1,2,1,1,2,1,2,1,2,2,1,1,1,2,1,1,2,1},//6
			{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},//7
			{1,2,1,1,1,2,1,2,1,1,1,1,2,1,1,1,1,2,1,1,1,1,2,1,2,1,1,1,2,1},//8
			{1,2,1,1,1,2,1,2,1,1,1,1,2,1,9,8,1,2,1,1,1,1,2,1,2,1,1,1,2,1},//9
			{2,2,2,2,2,2,1,2,2,1,1,1,2,1,10,7,1,2,1,1,1,2,2,1,2,2,2,2,2,2},//10
			{1,2,1,1,1,2,1,1,2,2,2,2,2,1,1,1,1,2,2,2,2,2,1,1,2,1,1,1,2,1},//11
			{1,2,1,2,2,2,2,2,2,1,1,1,2,2,2,2,2,2,1,1,1,2,2,2,2,2,2,1,2,1},//12
			{1,2,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,2,1},//13
			{1,2,1,2,1,2,2,2,1,2,2,2,2,2,0,2,2,2,2,2,2,1,2,2,2,1,2,1,2,1},//14
			{1,2,1,2,2,2,1,2,2,2,1,1,2,1,1,1,1,2,1,1,2,2,2,1,2,2,2,1,2,1},//15
			{1,2,1,2,1,2,2,2,1,2,1,1,2,2,2,2,2,2,1,1,2,1,2,2,2,1,2,1,2,1},//16
			{1,2,1,2,1,1,2,1,1,2,1,1,2,1,1,1,1,2,1,1,2,1,1,2,1,1,2,1,2,1},//17
			{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},//18
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},//19
	};
	for (int i = 0; i < 20; i++) {//Este FOR reemplaza los 2 por 5 y 6 para comenzar el mapa lleno de puntos.
		for (int j = 0; j < 30; j++) {
			if ((i == 1 && j == 1) || (i == 1 && j == 28) || (i == 18 && j == 1) || (i == 18 && j == 28)) {
				matrizJuegoAux[i][j] = 6;//Asigna los puntos grandes
			}
			else if (matrizJuegoAux[i][j] == 2) {//Asigna los puntos chicos
				matrizJuegoAux[i][j] = 5;
			}
		}
	}
	for (int i = 0; i < 20; i++) {//Hacemos una copia directa hacia la matriz de Juego
		for (int j = 0; j < 30; j++) {
			this->matrizJuego[i][j] = matrizJuegoAux[i][j];
		}
	}
}
void Mapa::mapa_2() {
	//Se carga el mapa tipo 2
	int matrizJuegoAux[20][30] = {
		//1 2 3 4 5 6 7 8 9 1 1 2 3 4 5 6 7 8 9 2 1 2 3 4 5 6 7 8 9 3
		 {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},//1
		 {1,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,1},//2
		 {1,2,1,1,1,1,2,1,2,1,2,1,1,2,2,2,2,1,1,2,1,2,1,2,1,1,1,1,2,1},//3									
		 {1,2,1,2,2,2,2,1,2,1,2,1,1,2,1,1,2,1,1,2,1,2,1,2,2,2,2,1,2,1},//4
		 {1,2,1,2,1,1,2,1,2,2,2,1,2,2,2,2,2,2,1,2,2,2,1,2,1,1,2,1,2,1},//5										
		 {1,2,1,2,1,1,2,1,2,1,2,1,1,2,1,1,2,1,1,2,1,2,1,2,1,1,2,1,2,1},//6											
		 {1,2,2,2,2,2,2,2,2,2,2,1,1,2,1,1,2,1,1,2,2,2,2,2,2,2,2,2,2,1},//7
		 {1,2,1,1,1,1,1,2,1,1,2,2,2,2,2,2,2,2,2,2,1,1,2,1,1,1,1,1,2,1},//8
		 {1,2,2,2,2,2,2,2,2,2,2,1,2,1,1,1,1,2,1,2,2,2,2,2,2,2,2,2,2,1},//9
		 {1,2,1,2,1,1,1,1,1,1,2,1,2,1,9,8,1,2,1,2,1,1,1,1,1,1,2,1,2,1},//10
		 {2,2,2,2,2,2,2,2,2,2,2,1,2,1,10,7,1,2,1,2,2,2,2,2,2,2,2,2,2,2},//11
		 {1,2,1,1,1,1,1,1,1,1,2,2,2,1,1,1,1,2,2,2,1,1,1,1,1,1,1,1,2,1},//12
		 {1,2,1,2,2,2,2,2,2,1,2,1,2,2,2,2,2,2,1,2,1,2,2,2,2,2,2,1,2,1},//13
		 {1,2,2,2,1,1,1,1,2,1,2,1,2,1,1,1,1,2,1,2,1,2,1,1,1,1,2,2,2,1},//14
		 {1,2,1,2,2,2,2,2,2,2,2,2,2,2,0,2,2,2,2,2,2,2,2,2,2,2,2,1,2,1},//15
		 {1,2,1,2,1,1,1,2,1,2,1,2,1,1,1,1,1,1,2,1,2,1,2,1,1,1,2,1,2,1},//16
		 {1,2,1,2,2,2,2,2,1,2,2,2,1,2,2,2,2,1,2,2,2,1,2,2,2,2,2,1,2,1},//17
		 {1,2,1,1,1,1,1,2,1,2,1,2,1,2,1,1,2,1,2,1,2,1,2,1,1,1,1,1,2,1},//18
		 {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},//19
		 {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},//20
	};
	for (int i = 0; i < 20; i++) {//Este FOR reemplaza los 2 por 5 y 6 para comenzar el mapa lleno de puntos.
		for (int j = 0; j < 30; j++) {
			if ((i == 1 && j == 1) || (i == 1 && j == 28) || (i == 18 && j == 1) || (i == 18 && j == 28)) {
				matrizJuegoAux[i][j] = 6;//ASIGNACIï¿½N DE LOS PUNTOS GRANDES
			}
			else if (matrizJuegoAux[i][j] == 2) {
				matrizJuegoAux[i][j] = 5;
			}
		}
	}

	for (int i = 0; i < 20; i++) {//Hacemos una copia directa
		for (int j = 0; j < 30; j++) {
			this->matrizJuego[i][j] = matrizJuegoAux[i][j];
		}
	}
}
void Mapa::mapa_3() {
	//Se carga el mapa tipo 3
	int matrizJuegoAux[20][30] = {
		//0 1 2 3 4 5 6 7 8 9 1 1 2 3 4 5 6 7 8 9 2 1 2 3 4 5 6 7 8 9 
		 {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},//0
		 {1,1,1,2,2,1,1,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,1,1,2,2,1,1,1},//1
		 {1,1,2,2,2,1,2,2,1,2,1,1,1,2,2,2,2,1,1,1,2,1,2,2,1,2,2,2,1,1},//2									
		 {1,2,2,1,2,1,2,1,1,2,2,1,1,2,1,1,2,1,1,2,2,1,1,2,1,2,1,2,2,1},//4
		 {1,2,1,1,2,1,2,1,1,2,1,2,1,2,1,1,2,1,2,1,2,1,1,2,1,2,1,1,2,1},//5										
		 {1,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,1},//6											
		 {1,2,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,2,1},//7
		 {1,2,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,1},//8
		 {1,2,1,2,1,1,1,2,1,2,1,1,2,1,1,1,1,2,1,1,2,1,2,1,1,1,2,1,2,1},//9
		 {1,2,1,2,1,1,1,2,1,2,2,2,2,1,9,10,1,2,2,2,2,1,2,1,1,1,2,1,2,1},//10
		 {2,2,2,2,1,1,1,2,1,1,1,1,2,1,8,7,1,2,1,1,1,1,2,1,1,1,2,2,2,2},//11
		 {1,2,1,2,1,1,1,2,1,1,1,1,2,1,1,1,1,2,1,1,1,1,2,1,1,1,2,1,2,1},//12
		 {1,2,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,1},//13
		 {1,2,1,2,1,2,1,2,1,2,1,1,1,1,1,1,1,1,1,1,2,1,2,1,2,1,2,1,2,1},//14
		 {1,2,1,2,2,2,2,2,1,2,2,2,2,2,0,2,2,2,2,2,2,1,2,2,2,2,2,1,2,1},//15
		 {1,2,2,2,1,2,1,2,1,2,2,1,2,1,1,1,1,2,1,2,2,1,2,1,2,1,2,2,2,1},//15
		 {1,1,1,1,2,2,2,2,2,2,1,1,2,1,2,2,1,2,1,1,2,2,2,2,2,2,1,1,1,1},//16
		 {1,2,2,1,2,1,1,1,1,2,1,1,2,2,2,2,2,2,1,1,2,1,1,1,1,2,1,2,2,1},//17
		 {1,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,1},//18
		 {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},//19
	};
	for (int i = 0; i < 20; i++) {//Este FOR reemplaza los 2 por 5 y 6 para comenzar el mapa lleno de puntos.
		for (int j = 0; j < 30; j++) {
			if ((i == 1 && j == 3) || (i == 1 && j == 26) || (i == 17 && j == 2) || (i == 17 && j == 28)) {
				matrizJuegoAux[i][j] = 6;//ASIGNACIï¿½N DE LOS PUNTOS GRANDES
			}
			else if (matrizJuegoAux[i][j] == 2) {
				matrizJuegoAux[i][j] = 5;
			}
		}
	}

	for (int i = 0; i < 20; i++) {//Hacemos una copia directa
		for (int j = 0; j < 30; j++) {
			this->matrizJuego[i][j] = matrizJuegoAux[i][j];
		}
	}
}
void Mapa::seleccionarMapa(int nivel) {
	switch (nivel) {
		case 1:
		case 2:
		case 3:
			mapa_1();
			break;
		case 4:
		case 5:
		case 6:
			mapa_2();
			break;
		case 7:
		case 8:
		case 9:
			mapa_3();
			break;
	}
}
void Mapa::dibujarMapa(int nivel,bool poderActivo) {
		switch(nivel){//Seleccionamos la forma del diseño visual de los mapas
		case 1:
			this->vectorPintarMapa[0]=load_bitmap("Elementos\\Bloques_1.bmp", NULL);
			break;
		case 2:
			this->vectorPintarMapa[0]=load_bitmap("Elementos\\Bloques_2.bmp", NULL);
			break;
		case 3:
			this->vectorPintarMapa[0]=load_bitmap("Elementos\\Bloques_3.bmp", NULL);
			break;
		case 4:
			this->vectorPintarMapa[0]=load_bitmap("Elementos\\Bloques_4.bmp", NULL);
			break;
		case 5:
			this->vectorPintarMapa[0]=load_bitmap("Elementos\\Bloques_5.bmp", NULL);
			break;
		case 6:
			this->vectorPintarMapa[0]=load_bitmap("Elementos\\Bloques_6.bmp", NULL);
			break;
		case 7:
			this->vectorPintarMapa[0]=load_bitmap("Elementos\\Bloques_7.bmp", NULL);
			break;
		case 8:
			this->vectorPintarMapa[0]=load_bitmap("Elementos\\Bloques_8.bmp", NULL);
			break;
		case 9:
			this->vectorPintarMapa[0]=load_bitmap("Elementos\\Bloques_9.bmp", NULL);
			break;
	}
	
	switch(this->direccionesVisuales[0]){//Seleccionamos la dirección en pantalla del Pacman
		case 1://izquierda
			this->vectorPintarMapa[8]=load_bitmap("Elementos\\CuerpoPacman_I.bmp", NULL);
			break;
		case 2://derecha
			this->vectorPintarMapa[8]=load_bitmap("Elementos\\CuerpoPacman_II.bmp", NULL);
			break;
		case 3://abajo
			this->vectorPintarMapa[8]=load_bitmap("Elementos\\CuerpoPacman_III.bmp", NULL);
		break;
		case 4://arriba
			this->vectorPintarMapa[8]=load_bitmap("Elementos\\CuerpoPacman_IIII.bmp", NULL);
		break;
		case 5://cuerpo completo
			this->vectorPintarMapa[8]=load_bitmap("Elementos\\CuerpoPacman_V.bmp", NULL);
			break;
	}
	
	switch(this->direccionesVisuales[1]){//Seleccionamos la dirección en pantalla del Fantasma 1
		case 1://izquierda
			vectorPintarMapa[4]=load_bitmap("Elementos\\FantasmaBlinky_I.bmp", NULL);
			break;
		case 2://derecha
			vectorPintarMapa[4]=load_bitmap("Elementos\\FantasmaBlinky_D.bmp", NULL);
			break;
		case 3://abajo
			vectorPintarMapa[4]=load_bitmap("Elementos\\FantasmaBlinky_Abajo.bmp", NULL);
		break;
		case 4://arriba
			vectorPintarMapa[4]=load_bitmap("Elementos\\FantasmaBlinky_Arriba.bmp", NULL);
		break;
	}
	
	switch(this->direccionesVisuales[2]){//Seleccionamos la dirección en pantalla del Fantasma 2
		case 1://izquierda
			vectorPintarMapa[5]=load_bitmap("Elementos\\FantasmaInky_I.bmp", NULL);
			break;
		case 2://derecha
			vectorPintarMapa[5]=load_bitmap("Elementos\\FantasmaInky_D.bmp", NULL);
			break;
		case 3://abajo
			vectorPintarMapa[5]=load_bitmap("Elementos\\FantasmaInky_Abajo.bmp", NULL);
		break;
		case 4://arriba
			vectorPintarMapa[5]=load_bitmap("Elementos\\FantasmaInky_Arriba.bmp", NULL);
		break;
	}
	
	switch(this->direccionesVisuales[3]){//Seleccionamos la dirección en pantalla del Fantasma 3
		case 1://izquierda
			vectorPintarMapa[6]=load_bitmap("Elementos\\FantasmaPinky_I.bmp", NULL);
			break;
		case 2://derecha
			vectorPintarMapa[6]=load_bitmap("Elementos\\FantasmaPinky_D.bmp", NULL);
			break;
		case 3://abajo
			vectorPintarMapa[6]=load_bitmap("Elementos\\FantasmaPinky_Abajo.bmp", NULL);
		break;
		case 4://arriba
			vectorPintarMapa[6]=load_bitmap("Elementos\\FantasmaPinky_Arriba.bmp", NULL);
		break;
	}
	
	switch(this->direccionesVisuales[4]){//Seleccionamos la dirección en pantalla del Fantasma 4
		case 1://izquierda
			vectorPintarMapa[7]=load_bitmap("Elementos\\FantasmaClyde_I.bmp", NULL);
			break;
		case 2://derecha
			vectorPintarMapa[7]=load_bitmap("Elementos\\FantasmaClyde_D.bmp", NULL);
			break;
		case 3://abajo
			vectorPintarMapa[7]=load_bitmap("Elementos\\FantasmaClyde_Abajo.bmp", NULL);
		break;
		case 4://arriba
			vectorPintarMapa[7]=load_bitmap("Elementos\\FantasmaClyde_Arriba.bmp", NULL);
		break;
	}
	
	switch(this->frutas){//Seleccionamos la fruta que corresponda
		case 0:
			vectorPintarMapa[9]=load_bitmap("Elementos\\Fruta 1.bmp",NULL);
			break;
		case 1:
			vectorPintarMapa[9]=load_bitmap("Elementos\\Fruta 2.bmp",NULL);
			break;
		case 2:
			vectorPintarMapa[9]=load_bitmap("Elementos\\Fruta 3.bmp",NULL);
			break;
		case 3:
			vectorPintarMapa[9]=load_bitmap("Elementos\\Fruta 4.bmp",NULL);
			break;
		default:
			vectorPintarMapa[9]=load_bitmap("Elementos\\Fruta 5.bmp",NULL);
			break;
	}

	if(poderActivo){//El poder está activado, hay que imprimir a los fantasmas asustados siempre y cuando no estén pausados
		 if(pPausaF[0]==0){
		 	vectorPintarMapa[4]=load_bitmap("Elementos\\Fantasmas_Asustados.bmp", NULL);
		 }
		 if(pPausaF[1]==0){
		 	vectorPintarMapa[5]=load_bitmap("Elementos\\Fantasmas_Asustados.bmp", NULL);
		 }
		 if(pPausaF[2]==0){
		 	vectorPintarMapa[6]=load_bitmap("Elementos\\Fantasmas_Asustados.bmp", NULL);
		 }
		 if(pPausaF[3]==0){
		 	vectorPintarMapa[7]=load_bitmap("Elementos\\Fantasmas_Asustados.bmp", NULL);
		 }
	}
	
	for(int i=0;i<20;i++){
		for(int j=0;j<30;j++){
			if(matrizJuego[i][j]==0){//PacMan
				draw_sprite(buffer, vectorPintarMapa[8], j*30, i*30+35);
			}else if(matrizJuego[i][j]==1 || matrizJuego[i][j]==3){//Bloques
				draw_sprite(buffer, vectorPintarMapa[0], j*30, i*30+35);
			}
			else if(matrizJuego[i][j]==2){//Paso Libre
				draw_sprite(buffer, vectorPintarMapa[1], j*30, i*30+35);
			}
			else if(matrizJuego[i][j]==4){//Frutas
				draw_sprite(buffer, vectorPintarMapa[9], j*30, i*30+35);
			}else if(matrizJuego[i][j]==5){//Puntos  chicos
				draw_sprite(buffer, vectorPintarMapa[2], j*30, i*30+35);
			}else if(matrizJuego[i][j]==6){//Puntos  grandes
				draw_sprite(buffer, vectorPintarMapa[3], j*30, i*30+35);
			}else if(matrizJuego[i][j]==7){//Fantasma Blinky
				draw_sprite(buffer, vectorPintarMapa[4], j*30, i*30+35);
			}else if(matrizJuego[i][j]==8){//Fantasma Inky
				draw_sprite(buffer, vectorPintarMapa[5], j*30, i*30+35);
			}else if(matrizJuego[i][j]==9){//Fantasma Pinky
				draw_sprite(buffer, vectorPintarMapa[6], j*30, i*30+35);
			}else if(matrizJuego[i][j]==10){//Fantasma Clyde
				draw_sprite(buffer, vectorPintarMapa[7], j*30, i*30+35);
			}
		}
	}
}
void Mapa::ponerFruta() {
	int i, j;
	srand(time(NULL));
	do {
		i = rand() % 17 + 1;
		j = rand() % 27 + 1;
	} while (matrizJuego[i][j] != 5);
	matrizJuego[i][j] = 4;
	this->frutaI=i;
	this->frutaJ=j;
}

#include "Pacman.hpp"
#include "Fantasmas.hpp"
void Mapa::motorJuego(Jugadores *jugadorActual){
	//Creamos a Pacman
	Pacman pacman;
	Clyde clyde(10,10,14);
	HistorialJugadores auxiliarArchivo;
	int l=0,l1=0,l2=0,vez=0,vezFruta=0,vezCargar=0,puntosScoreFinal,newkey,newkey2;
	char ASCII,ASCII2;
	bool x=false,perdedor=false,ganador=false;
	do{//Incia el Nivel
		//play_sample(efecto,200,150,1000,0);//Efecto de sonido de inicio
		this->seleccionarMapa(jugadorActual->getNivel());//Generamos el mapa con el nivel actual
		//cout<<"El jugador que se registra es "<<jugadorActual->getNom()<<" y size de "<<jugadorActual->getNom().size()<<endl;
		this->dibujarMapa(jugadorActual->getNivel(),pacman.getPoder());//Cargamos todo al mapa
		blit(buffer, screen, 0, 0, 0, 0, 960, 660);//Pintamos lo cargado previamente
		textprintf(buffer, font1, 400, 0, makecol(255, 255, 255), "Listo? ");//Texto de ¿listo? para antes de comenzar
		do{
			blit(buffer, screen, 0, 0, 0, 0, 960, 660);//Pintamos el mapa dentro de este ciclo para que la palabra "listo" aparezca algunos segundos antes de empezar a jugar
			l++;
		}while(l!=900);
		l=0;
		do{//Ciclo interno  a realizar mientras no haya un cambio de nivel o un perdedor
			pacman.movimiento(*this,*jugadorActual);
			if(pacman.getMato()!=0){//El pacman acaba de matar a alguien
				if(pacman.getMato()==1){//Mató al fantasma 7
				}
				else if(pacman.getMato()==4){
					clyde.posI=10;
					clyde.posJ=14;
					this->matrizJuego[10][14]=10;
					clyde.sacarFantasmas(*this);
				}
				pacman.setMato(0);
			}
			if(pacman.getMuerto())
				cout<<"YAA MURIOO"<<endl;
			if(!pacman.getMuerto()){
				clyde.movimientoNormal(*this,vez,pacman);
			}
			if(pacman.getMuerto()){
				this->reiniciarFantasmas(clyde, pacman);
				
					cout<<"Despues de reiniciar Pos I "<<clyde.getI()<<" Pos J "<<clyde.getJ()<<endl;

				vez=0;
				pacman.setMuerto(false);
				jugadorActual->setVidas(jugadorActual->getVidas()-1);
				auxiliarArchivo.modificarInformacion(*jugadorActual,0);// Esta validación impide al usuario hacer trampas de salirse a la mitad de un nivel para poder reiniciar sus vidas y le impide incrementar de forma tramposa su puntaje
			}
			//movimientoPacman(matrizJuego,vecPos,pvez,pcomida,pVidas,puntuacionTotal,pCambioNivel,pDir,registroUsuarios,pIdentificacion,pFrutas,pQuien,pPausaF,dificultad,sonidoActivo);//Le permitimos al Pacman moverse
			//fantasmas(matrizJuego,vez,vecPos,pvez,pcomida,pVidas,pDirF1,pDirF2,pDirF3,pDirF4,pPausaF,dificultad);//Llama a los 4 fantasmas, según corresponda por el valor del "pvez#"
			if(vez%4==0){//Este if controla la animación del Pacman comiendo
				this->direccionesVisuales[0]=5;
			}
			clear(buffer);//Borramos el buffer
			if(keypressed()){//Detectamos si el usuario apretó la tecla de "escape" para pausar el juego
				newkey = readkey();
				ASCII = newkey & 0xff;
				clear_keybuf();
				if(ASCII==27){//Apretó Escape
					do{//Este ciclo se ejecuta mientras el usuario no decida qué hacer en el menú de pausa
						blit(vectorMotorJuego[9], buffer, 0, 0, 0, 0, 960, 620);//Advertencia de salida	
						blit(buffer, screen, 0, 0, 0, 0, 960, 660);
						if(keypressed()){//Confirmación del usuario para proceder
							newkey2=readkey();
							ASCII2 = newkey2 & 0xff;
							if(ASCII2==13){//Apretó enter,se retorna porque el usuario ya no quiso seguir jugando a pesar de la advertencia
								auxiliarArchivo.modificarInformacion(*jugadorActual,0);// Esta validación impide al usuario hacer trampas de salirse a la mitad de un nivel para poder reiniciar sus vidas y le impide incrementar de forma tramposa su puntaje
								clear(this->buffer);
								return;
							}
							else if(ASCII2==27){//apretó Escape, se cancela la operación
								clear(buffer);
								x=true;//Sale de este ciclo para seguir la ejecución normal del motor de juego
							}
							else if(ASCII2==115){//apretó la tecla S para activar/desactivar el sonido
								if(this->sonido==0)//Significa que el sonido estaba activado, por lo tanto el usuario quiere desactivarlo
									this->sonido=1;//Significa que el usuario no quiere escuchar el sonido de comida
								else//Significa que el sonido estaba desactivado, por lo tanto el usuario quiere activarlo
									this->sonido=0;//Significa que el usuario quiere volver a escuchar el sonido
							}
						}
					}while(!x);
					x=false;
				}	
			}
				
			switch(frutas){//Con esto, el programa detecta qué fruta(s) va a mostrar en el Score (es la colección de las frutas).
				case 0:
					break;
				case 1:
					draw_sprite(buffer, vectorMotorJuego[4], 495, 0);
					break;
				case 2:
					draw_sprite(buffer, vectorMotorJuego[5], 495, 0);
					break;
				case 3:
					draw_sprite(buffer, vectorMotorJuego[6], 495, 0);
					break;
				case 4:
					draw_sprite(buffer, vectorMotorJuego[7], 495, 0);
					break;
				default:
					draw_sprite(buffer, vectorMotorJuego[8], 495, 0);
					break;
			}	
			if(jugadorActual->getVidas()==3)//Aparecen las vidas completas en el Score
				draw_sprite(buffer, vectorMotorJuego[3], 815, 0);
			else if(jugadorActual->getVidas()==2){//Aparecen sólo dos vidas completas en el Score
				if(l2==1){//Con esto sabemos que el Pacman ha perdido su primer vida y se le indica de nuevo la palabra "listo" antes de volver a jugar
					do{
						textprintf(screen, font1, 400, 0, makecol(255, 255, 255), "Listo?      ");
						l++;
					}while(l!=900);
					l=0;
					l2=2;
				}
				draw_sprite(buffer, vectorMotorJuego[2], 815, 0);
			}
			else if(jugadorActual->getVidas()==1){//Con esto sabemos que el Pacman ha perdido su segunda vida y se le indica de nuevo la palabra "listo" antes de volver a jugar
				if(l1==1){
					do{
						textprintf(screen, font1, 400, 0, makecol(255, 255, 255), "Listo?      ");
						l++;
					}while(l!=900);
					l=0;
					l1=2;
				}
				draw_sprite(buffer, vectorMotorJuego[1],815, 0);
			}
			if(vezFruta==100)//Generemos la fruta si la repetición es la 100
				ponerFruta();
			if(vezFruta==250){//Tenemos que desaparecer la fruta en caso de que no se la haya comido Pacman
				if(this->frutaI!=-1 && this->frutaJ!=-1){//Si la encontró
					matrizJuego[this->frutaI][this->frutaJ]=2;//Borramos a la fruta
					this->frutaI=-1;
					this->frutaJ=-1;
				}
				//Si no la encuentra quiere decir que el Pacman se la comió
			}
			//pintarMapa(matrizJuego,buffer,pDir,pcomida,pDirF1,pDirF2,pDirF3,pDirF4,pFrutas,*pNivel,pPausaF,dificultad,vectorPintarMapa);//Cargamos todo lo visual al mapa
			this->dibujarMapa(jugadorActual->getNivel(),pacman.getPoder());
			blit(buffer, screen, 0, 0, 0, 0, 960, 660);//Pintamos lo cargado previamente
			textprintf(screen, font1, 0, 0, makecol(255, 153, 51), "Score: %i", this->puntuacionTotal);//Es el encargado de mostrar el Score
			textprintf(screen, font1, 400, 0, makecol(255, 255, 255), "Nivel: %i", jugadorActual->getNivel());//Es el encargado de mostrar el nivel actual
			textprintf(screen, font3, 690, 0, makecol(255, 255, 255), "Poder: %ir", this->dificultad);//"rep" se refiere a "repeticiones" por lo que 60, por ejemplo, significa que si Pacman toma el poder en ese momento, tardará 60 repeticiones del juego para que termine el poder
			textprintf(screen, font1, 200, 0, makecol(255, 255, 255), "[ESC] Pausa");//"rep" se refiere a "repeticiones" por lo que 60, por ejemplo, significa que si Pacman toma el poder en ese momento, tardará 60 repeticiones del juego para que termine el poder
			vezFruta++;
			vez++;
			rest(VELOCIDAD);//Maneja la velocidad del juego. Entre más alto el parámetro, más lento el juego
			if(jugadorActual->getVidas()<=0){//se terminó el juego
				//play_midi(perderMusic,1);//Se activa la música de derrota
				perdedor=true;//Permitimos la salida de este ciclo y del siguiente
			}
			if(this->cambioNivel){//Habrá cambio de nivel, hay que guardar
				puntosScoreFinal=this->puntuacionTotal;//Por fines prácticos, hacemos un respaldo de la puntuación total del nivel	
				jugadorActual->setPuntos(jugadorActual->getPuntos()+puntosScoreFinal);
				jugadorActual->setNivel(jugadorActual->getNivel()+1);
				auxiliarArchivo.modificarInformacion(*jugadorActual,0);
				cout<<"Se supone que se guarda la info"<<endl;
			}
		
		} while(!perdedor && !this->cambioNivel);//El ciclo interno, de juego por mapa
		//*pcomida=0;//Reiniciamos el contador de comida por si estuviera prendido, que no avance con poder al siguiente nivel
		pacman.setPoder(false);
		vezFruta=0;//Reiniciamos este contador para que podamos volver a imprimir la fruta en el siguiente.
		//cambioNivel=0;//Reiniciamos a cero esta variable para que el ciclo interno se siga dando
		this->cambioNivel=false;
		//*puntuacionTotal=0;//Reseteamos la puntuación
		this->puntuacionTotal=0;
		//*pNivel+=1;//Aumentamos el nivel en 1
			cout<<"Llega"<<endl;
		if(perdedor){//Reiniciamos el nivel del usuario a 1 y le borramos su puntaje
			jugadorActual->setNivel(1);
			jugadorActual->setVidas(3);
			jugadorActual->setPuntos(0);
			auxiliarArchivo.modificarInformacion(*jugadorActual,0);
		}
		else if(jugadorActual->getNivel()==NIVELMAXIMO){//El usuario ganó el juego
			set_volume(70,70);
			//play_midi(ganarMusic,1);//Inicia la música de ganador
			ganador=true;//Permitimos la salida de este ciclo
		}
		else
			auxiliarArchivo.modificarInformacion(*jugadorActual,0);//guardarNivel(registroUsuarios,pNivel,pIdentificacion,false);//Quiere decir que es cambio de nivel, y hay que guardar
		clear(this->buffer);
		do{//Este switch es el encargado de la animación de guardado
			switch(vezCargar){
				case 0:
					blit(vectorMotorJuego[12], buffer, 0, 0, 0, 0, 960, 660);
					break;
				case 400:
					blit(vectorMotorJuego[11], buffer, 0, 0, 0, 0, 960, 660);
					break;
				case 800:
					blit(vectorMotorJuego[10], buffer, 0, 0, 0, 0, 960, 660);
					break;
				case 1200:
					blit(vectorMotorJuego[13], buffer, 0, 0, 0, 0, 960, 660);
					break;
			}
			vezCargar++;
			blit(buffer, screen, 0, 0, 0, 0, 960, 660);
		}
		while(vezCargar<1201);
		vezCargar=0;
		vez=0;
		clear(buffer);	
	} while(perdedor==false && ganador==false);//Ganador se activa cuando se termina el nivel 9
	
	if(ganador==true){//El usuario ganó el Juego y, por lo tanto, debemos de mostrarle una pantalla que lo indique
		l=jugadorActual->getVidas();
		jugadorActual->setNivel(1);//Le reiniciamos el nivel a 1, para que pueda seguir jugando ahora desde el inicio
		jugadorActual->setVidas(3);//Le reiniciamos sus vidas a 3, es justo porque ya ganó el juego
		auxiliarArchivo.modificarInformacion(*jugadorActual,0);//Se guarda por última vez la información del usuario	
		clear_keybuf();//Borramos el buffer de entrada del teclado
		clear(this->buffer);
		do{
			blit(vectorMotorJuego[0], buffer, 0, 0, 0, 0, 900, 660);
			textprintf(buffer, font2, 480, 160, makecol(255, 0, 0), "%i", puntosScoreFinal);
			textprintf(buffer, font2, 570, 235, makecol(255, 0, 0), "%i", l);
			blit(buffer, screen, 0, 0, 0, 0, 960, 660);			
		}while(!key[KEY_ESC]);
		blit(buffer, screen, 0, 0, 0, 0, 960, 660);	
		l=0;
	}
	else if(perdedor==true){//El usuario perdió el juego y, por lo tanto, debemos de mostrarle una pantalla que lo indique
			clear_keybuf();//Borramos el buffer de entrada del teclado
				clear(buffer);
				do{
					blit(vectorMotorJuego[14], buffer, 0, 0, 0, 0, 900, 660);
					blit(buffer, screen, 0, 0, 0, 0, 960, 660);
				}while(!key[KEY_ESC]);
	}
	set_volume(45, 45);//Reducimos el nivel de volumen al original
	clear_keybuf();//Borramos el buffer*/
	clear(this->buffer);
	//}while(true);
}

void Mapa::reiniciarFantasmas(Clyde &clyde, Pacman &pacman){
	this->matrizJuego[clyde.getI()][clyde.getJ()]=2;
	clyde.setPos(10,14);
	this->matrizJuego[10][14]=10;
	this->matrizJuego[pacman.getI()][pacman.getJ()]=2;
	pacman.setPos(14,14);
	this->matrizJuego[14][14]=0;
}
#endif
