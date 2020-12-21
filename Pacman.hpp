#ifndef PACMAN_H_INCLUDED
#define PACMAN_H_INCLUDED
#define PUNTOS 0 //Importante no usar un valor menor a 0 ni mayor a 270

class Mapa;
class Pacman {
	private:
		int posI, posJ, direccion,contPoder,mato,duracionPoder;
		bool poder,muerto,comibles[4];
	public:
		Pacman(int nivel);
		void movimiento(Mapa &mapa,Jugadores &);
		bool movimientoValido(Mapa &mapa);
		void tienePoder(Mapa &mapa);
		bool getPoder(){return this->poder;}
		void setPoder(bool p){this->poder=p;}
		void puntaje(Mapa &mapa);
		bool morirPacman(Mapa &mapa);
		void setPos(int i, int j){
			this->posI=i;
			this->posJ=j;
		}
		int getI(){return this->posI;}
		int getJ(){return this->posJ;}
		void setMuerto(bool x){
			this->muerto=x;
		}
		bool getMuerto(){return this->muerto;}
		void setComibles(int pos,bool x){
			this->comibles[pos]=x;
		}
		bool getComible(int x){return this->comibles[x];}
		int getMato(){return this->mato;}
		void setMato(int x){
			this->mato=x;
		}
		void asignarDireccion(int &auxI,int &auxJ){
			switch(this->direccion){
				case 1://Arriba
					auxI=this->posI-1;
					break;
				case 2://Abajo
					auxJ=this->posI+1;
					break;
				case 3://Izquierda
					auxJ=this->posJ-1;
					break;
				case 4://Derecha
					auxJ=this->posJ+1;
					break;
			}
		}
};

Pacman::Pacman(int nivel) {
	this->posI = 14;
	this->posJ = 14;
	this->direccion = 0;
	this->poder=false;
	this->muerto=false;
	this->contPoder=0;
	this->mato=0;
	for(int i=0;i<4;i++)
		this->comibles[i]=false;
	this->duracionPoder=50-(nivel*5);
}
#include "Mapa.hpp"
void Pacman::movimiento(Mapa &mapa,Jugadores &jugadorActual) {
	SAMPLE *sonido=load_sample("Elementos\\pacman avanza chido.wav");
	SAMPLE *muerteFantasma=load_sample("Elementos\\pacman-eating-ghost.wav");
	int auxI,auxJ,x,buscar;
	if(key[KEY_UP] || key[KEY_W]) {
		mapa.setDirecciones(0,4);
		this->direccion=1;
		x=movimientoValido(mapa);
		if(x) {//Se puede mover a donde desea el jugador
				this->puntaje(mapa);
				this->tienePoder(mapa);
				if(!morirPacman(mapa)){//Pacman no morirá en este movimiento
					if(mapa.getSonido()==0)//Quiere decir que el usuario está permitiendo el sonido del movimiento del pacman
						play_sample(sonido,200,150,1000,0);
					mapa.setMatrizJuego(this->posI,this->posJ,2);//[this->PosI][this->PosJ]=2;//Borramos la posición anterior, el pacman ya comió	por eso dejamos un 2		  
		           	mapa.setMatrizJuego(this->posI-1,this->posJ,0);//[this->PosI-1][this->PosJ]=0; //Se hace el movimiento del pacman, cambiándolo de posición
					this->posI-=1;
				}
				else{//Pacman podría perder una vida, pero hay que validar primero
					auxI=this->posI;
					auxJ=this->posJ;
					this->asignarDireccion(auxI,auxJ);
					buscar=mapa.getMatrizJuego(auxI,auxJ);//[auxI][auxJ];//Ya estamos enterados de qué Fantasma fue el que provocó que Pacman estuviera en esta instancia. El "*pQuien" se modificó en la función morirPacman, pero por practicidad lo metemos a una variable tipo INT
					if(!this->poder)//El poder está desactivado, por lo tanto Pacman pierde una vida
						this->setMuerto(true);
					else if(this->poder && this->comibles[buscar-7]){//El poder está activado, por lo tanto Pacman puede seguir y comerse al fantasma que le indicó el "*pQuien"
						play_sample(muerteFantasma,200,150,1000,0);//Se activa el sonido de la muerte de un fantasma
						mapa.setPausaF(buscar-7,1);//Encendemos la pausa al fantasma que corresponda. Buscar tiene valores a partir del 7 al 10, pero el vector pPausaF comienza en 0, por lo tanto a buscar le restamos 7 y ahora tenemos la posición indicada para pausar
						mapa.setMatrizJuego(this->posI,this->posJ,2);//[this->posI][this->posJ]=2;//Borramos la posición anterior a Pacman
						mapa.setMatrizJuego(this->posI-1,this->posJ,0);//[this->posI-1][this->posJ]=0;//Movemos a Pacman
						this->mato=buscar-6;
						this->posI-=1;
							this->comibles[buscar-7]=false;
					}
					else{
						this->setMuerto(true);
						this->setPoder(false);
					}
			}
		}
	}
	
	else if(key[KEY_LEFT] || key[KEY_A]){
		mapa.setDirecciones(0,1);//Visualmente hacia la izquierda
		this->direccion=3;
		x=movimientoValido(mapa);
		if(x) {//Se puede mover a donde desea el jugador
				this->puntaje(mapa);
				this->tienePoder(mapa);
				if(!morirPacman(mapa)){//Pacman no morirá en este movimiento
					if(mapa.getSonido()==0)//Quiere decir que el usuario está permitiendo el sonido del movimiento del pacman
						play_sample(sonido,200,150,1000,0);
					mapa.setMatrizJuego(this->posI,this->posJ,2);//[this->posI][this->posJ]=2;//Borramos la posición anterior, el pacman ya comió	por eso dejamos un 2		  
		            mapa.setMatrizJuego(this->posI,this->posJ-1,0);//[this->posI][this->posJ-1]=0; //Se hace el movimiento del pacman, cambiándolo de posición
					this->posJ-=1;
				}
				else{//Pacman podría perder una vida, pero hay que validar primero
					auxI=this->posI;
					auxJ=this->posJ;
					this->asignarDireccion(auxI,auxJ);
					buscar=mapa.getMatrizJuego(auxI,auxJ);//[auxI][auxJ];//Ya estamos enterados de qué Fantasma fue el que provocó que Pacman estuviera en esta instancia. El "*pQuien" se modificó en la función morirPacman, pero por practicidad lo metemos a una variable tipo INT
					if(!this->poder)//El poder está desactivado, por lo tanto Pacman pierde una vida
						this->setMuerto(true);
					else if(this->poder && this->comibles[buscar-7]){//El poder está activado, por lo tanto Pacman puede seguir y comerse al fantasma que le indicó el "*pQuien"
						play_sample(muerteFantasma,200,150,1000,0);//Se activa el sonido de la muerte de un fantasma
						mapa.setPausaF(buscar-7,1);//Encendemos la pausa al fantasma que corresponda. Buscar tiene valores a partir del 7 al 10, pero el vector pPausaF comienza en 0, por lo tanto a buscar le restamos 7 y ahora tenemos la posición indicada para pausar
						mapa.setMatrizJuego(this->posI,this->posJ,2);//[this->posI][this->posJ]=2;//Borramos la posición anterior a Pacman
						mapa.setMatrizJuego(this->posI,this->posJ-1,0);//[this->posI][this->posJ-1]=0;//Movemos a Pacman
						this->posJ-=1;
						this->mato=buscar-6;
							this->comibles[buscar-7]=false;
					}
					else{
						this->setMuerto(true);
						this->setPoder(false);
					}
			}
		}
	}
	
	else if(key[KEY_RIGHT] || key[KEY_D]){
		mapa.setDirecciones(0,2);//Visualmente hacia la derecha
		this->direccion=4;
		x=movimientoValido(mapa);
		if(x) {//Se puede mover a donde desea el jugador
				this->puntaje(mapa);
				this->tienePoder(mapa);
				if(!morirPacman(mapa)){//Pacman no morirá en este movimiento
					if(mapa.getSonido()==0)//Quiere decir que el usuario está permitiendo el sonido del movimiento del pacman
						play_sample(sonido,200,150,1000,0);
					mapa.setMatrizJuego(this->posI,this->posJ,2);//Borramos la posición anterior, el pacman ya comió	por eso dejamos un 2		  
		            mapa.setMatrizJuego(this->posI,this->posJ+1,0);//Se hace el movimiento del pacman, cambiándolo de posición
					this->posJ+=1;
				}
				else{//Pacman podría perder una vida, pero hay que validar primero
					auxI=this->posI;
					auxJ=this->posJ;
					this->asignarDireccion(auxI,auxJ);
					buscar=mapa.getMatrizJuego(auxI,auxJ);//[auxI][auxJ];//Ya estamos enterados de qué Fantasma fue el que provocó que Pacman estuviera en esta instancia. El "*pQuien" se modificó en la función morirPacman, pero por practicidad lo metemos a una variable tipo INT
					if(!this->poder)//El poder está desactivado, por lo tanto Pacman pierde una vida
						this->setMuerto(true);
					else if(this->poder && this->comibles[buscar-7]){//El poder está activado, por lo tanto Pacman puede seguir y comerse al fantasma que le indicó el "*pQuien"
						play_sample(muerteFantasma,200,150,1000,0);//Se activa el sonido de la muerte de un fantasma
						mapa.setPausaF(buscar-7,1);//Encendemos la pausa al fantasma que corresponda. Buscar tiene valores a partir del 7 al 10, pero el vector pPausaF comienza en 0, por lo tanto a buscar le restamos 7 y ahora tenemos la posición indicada para pausar
						mapa.setMatrizJuego(this->posI,this->posJ,2);//[this->posI][this->posJ]=2;//Borramos la posición anterior a Pacman
						mapa.setMatrizJuego(this->posI,this->posJ+1,0);//[this->posI][this->posJ-1]=0;//Movemos a Pacman
						this->posJ+=1;
						this->mato=buscar-6;
							this->comibles[buscar-7]=false;
					}
					else{
						this->setMuerto(true);
						this->setPoder(false);
					}
			}
		}
	}
	else if(key[KEY_DOWN] || key[KEY_S]){
		mapa.setDirecciones(0,3);//Visualmente hacia abajo
		this->direccion=2;
		x=movimientoValido(mapa);
		if(x) {//Se puede mover a donde desea el jugador
				this->puntaje(mapa);
				this->tienePoder(mapa);
				if(!morirPacman(mapa)){//Pacman no morirá en este movimiento
					if(mapa.getSonido()==0)//Quiere decir que el usuario está permitiendo el sonido del movimiento del pacman
						play_sample(sonido,200,150,1000,0);
					mapa.setMatrizJuego(this->posI,this->posJ,2);//Borramos la posición anterior, el pacman ya comió	por eso dejamos un 2		  
		            mapa.setMatrizJuego(this->posI+1,this->posJ,0);//Se hace el movimiento del pacman, cambiándolo de posición
					this->posI+=1;
				}
				else{//Pacman podría perder una vida, pero hay que validar primero
					auxI=this->posI;
					auxJ=this->posJ;
					this->asignarDireccion(auxI,auxJ);
					buscar=mapa.getMatrizJuego(auxI,auxJ);//[auxI][auxJ];//Ya estamos enterados de qué Fantasma fue el que provocó que Pacman estuviera en esta instancia. El "*pQuien" se modificó en la función morirPacman, pero por practicidad lo metemos a una variable tipo INT
					if(!this->poder)//El poder está desactivado, por lo tanto Pacman pierde una vida
						this->setMuerto(true);
					else if(this->poder && this->comibles[buscar-7]){//El poder está activado, por lo tanto Pacman puede seguir y comerse al fantasma que le indicó el "*pQuien"
						play_sample(muerteFantasma,200,150,1000,0);//Se activa el sonido de la muerte de un fantasma
						mapa.setPausaF(buscar-7,1);//Encendemos la pausa al fantasma que corresponda. Buscar tiene valores a partir del 7 al 10, pero el vector pPausaF comienza en 0, por lo tanto a buscar le restamos 7 y ahora tenemos la posición indicada para pausar
						mapa.setMatrizJuego(this->posI,this->posJ,2);//[this->posI][this->posJ]=2;//Borramos la posición anterior a Pacman
						mapa.setMatrizJuego(this->posI+1,this->posJ,0);//[this->posI][this->posJ-1]=0;//Movemos a Pacman
						this->posI+=1;
						this->mato=buscar-6;
						this->comibles[buscar-7]=false;
					}
					else{
						this->setMuerto(true);
						this->setPoder(false);
					}
			}
		}
	}
} 
bool Pacman::movimientoValido(Mapa &mapa) {
    switch (this->direccion) {
	    case 1://Arriba
	        return (mapa.getMatrizJuego(this->posI-1,this->posJ)!=1);   //[posX - 1][posY] != 1 );
	        break;
	    case 2://Abajo
	        return (mapa.getMatrizJuego(this->posI+1,this->posJ)!=1);//(mapa.matrizJuego[posX + 1][posY] != 1 );
	        break;
	    case 3://Izquierda, verificación de Teleport de Izquierda a Derecha
	    	if(this->posI==10 && this->posJ-1==0){
	    		mapa.setMatrizJuego(10,29,0);//Ponemos a Pacman en su nueva posición
				mapa.setMatrizJuego(10,0,2);//Borra la posición anterior
				mapa.setMatrizJuego(10,1,2);//Borra la posición ANTE Anterior
				this->posI=10;
				this->posJ=29;
				return false;
			}
	        return (mapa.getMatrizJuego(this->posI,this->posJ-1)!=1);
	        break;
	    case 4://Derecha, verificación de Teleport de Derecha a Izquierda
	    	if(this->posI==10 && this->posJ+1==29){
	    		mapa.setMatrizJuego(10,0,0);//Ponemos a Pacman en su nueva posición
				mapa.setMatrizJuego(10,29,2);//Borra la posición anterior
				mapa.setMatrizJuego(10,28,2);//Borra la posición ANTE Anterior
				this->posI=10;
				this->posJ=0;
				return false;
			}
	        return (mapa.getMatrizJuego(this->posI,this->posJ+1)!=1);//(mapa.matrizJuego[posX][posY + 1] != 1 );
	        break;
	    default:
	        return false;
	        break;
    }
}
void Pacman::tienePoder(Mapa &mapa) {
	SAMPLE *sonidoFruta=load_sample("Elementos\\pacman-fruta.wav");//Carga el sonido de la fruta
	int auxI,auxJ;
	auxI=this->posI;
	auxJ=this->posJ;
	if(this->poder)//Tiene poder, debemos decrementarle al contador
		this->contPoder--;
	this->asignarDireccion(auxI,auxJ);
	if(mapa.getMatrizJuego(auxI,auxJ)==6){
		this->contPoder=this->duracionPoder;
		this->poder=true;
		for(int i=0;i<4;i++)
			this->comibles[i]=true;
		mapa.setPausaF(0,0);
		mapa.setPausaF(1,0);
		mapa.setPausaF(2,0);
		mapa.setPausaF(3,0);
	}
	else if(mapa.getMatrizJuego(auxI,auxJ)==4){//Comió una fruta
		play_sample(sonidoFruta,200,150,1000,0);
	}
	if(this->contPoder==0){//Se acabó el poder
		mapa.setPausaF(0,0);
		mapa.setPausaF(1,0);
		mapa.setPausaF(2,0);
		mapa.setPausaF(3,0);//Provoca que los fantasmas ya puedan estar asustados  en caso de que el Pacman coma
		for(int i=0;i<4;i++)
			this->comibles[i]=false;
		this->poder=false;
		this->contPoder=this->duracionPoder;
	}
}
void Pacman::puntaje(Mapa &mapa){
	//Es la función que hacas las cuentas numéricas de los puntos y que además realiza el cambio de nivel cuando es necesario
	int cont=0,contadorRegistro=0,puntos=0;
	int auxI=this->posI,auxJ=this->posJ;
	for(int i=0;i<20;i++){//Este FOR cuenta el número total de puntos activos en el mapa. Son los puntos reales que existen. Es necesario esto para tener un control en tiempo real de los puntos existentes en el mapa, recordando que en algunos casos se agregan (por mecanismos propios del juego) puntos al mapa o se quitan
		for(int j=0;j<30;j++){
			if(mapa.getMatrizJuego(i,j)==5 || mapa.getMatrizJuego(i,j)==6 ){//Cuenta cuántos puntos hay 
				cont++;
			}
		}
	}
	this->asignarDireccion(auxI,auxJ);
    if(mapa.getMatrizJuego(auxI,auxJ)==7 || mapa.getMatrizJuego(auxI,auxJ)==8 || mapa.getMatrizJuego(auxI,auxJ)== 9 || mapa.getMatrizJuego(auxI,auxJ)==10){//Si pacman se ha comido una pastilla y hay un fantasma...
	 	if(this->poder)//El poder está activo
	 		mapa.setPuntuacionTotal(mapa.getPuntuacionTotal()+20);
    }
	else if(mapa.getMatrizJuego(auxI,auxJ) == 4){//Si es igual a donde hay una fruta...
		mapa.setFrutas(mapa.getFrutas()+1);
		mapa.setPuntuacionTotal(mapa.getPuntuacionTotal()+15);
	} else if(mapa.getMatrizJuego(auxI,auxJ) == 5)//Si pacman se ha  un puntito...
		mapa.setPuntuacionTotal(mapa.getPuntuacionTotal()+5);
    else if (mapa.getMatrizJuego(auxI,auxJ)==6)//Si pacman se ha comido un punto grande...
    	mapa.setPuntuacionTotal(mapa.getPuntuacionTotal()+10);
	if(cont<=PUNTOS)//Se tiene que activar un cambio de mapa y se guardan los puntos en el archivo
		mapa.setCambioNivel(true);
}
bool Pacman::morirPacman(Mapa &mapa){
	int auxI,auxJ;
	auxI=this->posI;
	auxJ=this->posJ;
	this->asignarDireccion(auxI,auxJ);
	if(mapa.getMatrizJuego(auxI,auxJ)==7 || mapa.getMatrizJuego(auxI,auxJ)==8 || mapa.getMatrizJuego(auxI,auxJ)==9 || mapa.getMatrizJuego(auxI,auxJ)==10)//Pacman podría morir, hay que verificar la información en el movimiento del Pacman
		return true;
	return false;
}
#endif
