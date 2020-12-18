#pragma once
class Mapa;

class Fantasmas {
public:
    int id, posI, posJ, direccion;
    bool comibles;
    Fantasmas(){}
    Fantasmas(int id,int i, int j) {
    	this->id=id;
       	this->posI = i;
        this->posJ = j;
        this->direccion = 0;
        this->comibles = false;
    }
    virtual void movimientoNormal(Mapa &, int) = 0;
    //virtual void movimientoPersecucion() = 0;
    //virtual void movimientoHuida() = 0;
    //int* posiciones();
    //bool matarPacman(int*, bool);
    //void regeneracion();
    //bool movimientoValido(int*[]);
    void sacarFantasmas(Mapa &);
};

#include "Mapa.hpp"
void Fantasmas::sacarFantasmas(Mapa &mapa){
	//En esta función, sacamos a los fantasmas de forma validada para que los ponga en la primer posición disponible
	int i=7,j,k=12,band=0;
	//buscarPosicion(matrizJuego,numReiniciar,vecPos);
	mapa.setMatrizJuego(this->posI,this->posJ,2);//Posición borrada
	for(j=12;j<18;j++){
		if(mapa.getMatrizJuego(i,j)==2 || mapa.getMatrizJuego(i,j)==5){//Posición libre para avanzar			
			mapa.setMatrizJuego(i,j,this->id);
			band=1;
			return;
		}
		else if(mapa.getMatrizJuego(k,j)==2 || mapa.getMatrizJuego(k,j)==5){
			mapa.setMatrizJuego(k,j,this->id);
			band=1;
			return;
		}
	}
	j=12;
	k=17;
	if(band==0){
		for(i=7;i<13;i++){
			if(mapa.getMatrizJuego(i,j)==2 || mapa.getMatrizJuego(i,j)==5 ){//Posición libre para avanzar
				mapa.setMatrizJuego(i,j,this->id);
				return;
			}			
			else if(mapa.getMatrizJuego(i,k)==2 || mapa.getMatrizJuego(i,k)==5){
				mapa.setMatrizJuego(i,k,this->id);
				return;
			}
		}
	}
}

class Blinky : public Fantasmas {
public:
    void movimientoNormal();
    void movimientoPersecucion();
    void movimientoHuida();
};
class Clyde : public Fantasmas { //ID = 10  Naranja
	public:
		Clyde() : Fantasmas(){}
		Clyde(int id,int i, int j) : Fantasmas(id,i,j){}
    	void movimientoNormal(Mapa &mapa, int vez){
    		if(vez==6)//Lo sacamos de su casa por primera vez
    			this->sacarFantasmas(mapa);
    		else if(vez>6){//Comienza su movimiento normal
    			
			}
		}
		/*
    	void movimientoPersecucion(){
    		cout<<"Holi"<<endl;
		}*/
};

/*
class Inky : public Fantasmas {
public:
    void movimientoNormal();
    void movimientoPersecucion();
};
class Pinky : public Fantasmas {
public:
    void movimientoNormal();
    void movimientoPersecucion();
};
*/
//Implementacion de los metodos de la clase Fantasmas
/*int* Fantasmas::posiciones() {
    int* posXY = new int[2];
    posXY[0] = posX;
    posXY[1] = posY;
    return posXY;
}*/
/*bool Fantasmas::matarPacman(int* posXY_pacman, bool poderPacman) {
    return (posX == posXY_pacman[0] && posY == posXY_pacman[1] && !poderPacman);
}
bool Fantasmas::movimientoValido(int *matrizJuego[]) {
    switch (direccion) {
        case 1:
            return (matrizJuego[posX - 1][posY] == 2 || matrizJuego[posX - 1][posY] == 5);
            break;
        case 2:
            return (matrizJuego[posX + 1][posY] == 2 || matrizJuego[posX + 1][posY] == 5);
            break;
        case 3:
            return (matrizJuego[posX][posY - 1] == 2 || matrizJuego[posX][posY - 1] == 5);
            break;
        case 4:
            return (matrizJuego[posX][posY + 1] == 2 || matrizJuego[posX][posY + 1] == 5);
            break;
        default:
            return false;
            break;
    }
}
void Fantasmas::regeneracion() {
    comibles = false;
}
//Implementacion de los metodos de la clase Blinky
void Blinky::movimientoNormal() {

}
void Blinky::movimientoPersecucion() {

}
//Implementacion de los metodos de la clase Clyde
void Clyde::movimientoNormal() {

}
void Clyde::movimientoPersecucion() {

}
//Implementacion de los metodos de la clase Inky
void Inky::movimientoNormal() {

}
void Inky::movimientoPersecucion() {

}
//Implementacion de los metodos de la clase Pinky
void Pinky::movimientoNormal() {

}
void Pinky::movimientoPersecucion() {

}*/
