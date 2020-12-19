#ifndef PACMAN_H_INCLUDED
#define PACMAN_H_INCLUDED
#define PUNTOS 0 //Importante no usar un valor menor a 0 ni mayor a 270

class Mapa;
class Pacman {
	private:
		int posI, posJ, direccion,contPoder,mato;
		bool poder,muerto,comibles[4];
	public:
		Pacman();
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
};

Pacman::Pacman() {
	this->posI = 14;
	this->posJ = 14;
	this->direccion = 0;
	this->poder=false;
	this->muerto=false;
	this->contPoder=0;
	this->mato=0;
	for(int i=0;i<4;i++)
		this->comibles[i]=false;
}

#include "Mapa.hpp"

void Pacman::movimiento(Mapa &mapa,Jugadores &jugadorActual) {
	int auxI,auxJ,x,buscar;
	if(key[KEY_UP] || key[KEY_W]) {
		//*pDir=4;//Dirección de la imagen de Pacman -> ¿A dónde apunta Pacman?
		mapa.setDirecciones(0,4);
		//this->posX--;
		this->direccion=1;
		//PosI--;//Al moverse hacia arriba las filas disminuyen. A partir de este dato, checamos todas las validaciones
		//x=verificarTeleport(matrizJuego, PosI, PosJ, 0,0);//Checamos si el Pacman se moverá o no al teleport
		x=movimientoValido(mapa);
		if(x) {//Se puede mover a donde desea el jugador
			//if(matrizJuego[PosI][PosJ]==1 || matrizJuego[PosI][PosJ]==3){}//Quiere decir que el PASO NO es libre	
			//else {//Pacman si se puede mover, por lo tanto hay que validar que su muerte se de
				//puntaje(matrizJuego,PosI,PosJ,pcomida,puntuacionTotal,pCambioNivel,registroUsuarios,pIdentificacion,pFrutas,pVidas,dificultad);//Checamos y acutalizamos puntaje en pantalla
				this->puntaje(mapa);
				this->tienePoder(mapa);
				//queCome(matrizJuego,pcomida,PosI,PosJ,dificultad);//Ya sabemos si el puntero de la comida está o no activo
				if(/*morirPacman(matrizJuego,PosI,PosJ,pvez,pcomida,pQuien)==0*/!morirPacman(mapa)){//Pacman no morirá en este movimiento
					if(mapa.getSonido()==0){//Quiere decir que el usuario está permitiendo el sonido del movimiento del pacman
						//play_sample(sonido,200,150,1000,0);
					}
					mapa.setMatrizJuego(this->posI,this->posJ,2);//[this->PosI][this->PosJ]=2;//Borramos la posición anterior, el pacman ya comió	por eso dejamos un 2		  
		           	mapa.setMatrizJuego(this->posI-1,this->posJ,0);//[this->PosI-1][this->PosJ]=0; //Se hace el movimiento del pacman, cambiándolo de posición
					this->posI-=1;
				}
				else{//Pacman podría perder una vida, pero hay que validar primero
					auxI=this->posI;
					auxJ=this->posJ;
					switch(direccion){
						case 1://Arriba
							auxI=this->posI-1;
							break;
						case 2://Abajo
							auxI=this->posI+1;
							break;
						case 3://Izquierda
							auxJ=this->posJ-1;
							break;
						case 4://Derecha
							auxJ=this->posJ+1;
							break;
					}
					buscar=mapa.getMatrizJuego(auxI,auxJ);//[auxI][auxJ];//Ya estamos enterados de qué Fantasma fue el que provocó que Pacman estuviera en esta instancia. El "*pQuien" se modificó en la función morirPacman, pero por practicidad lo metemos a una variable tipo INT
					if(!this->poder){//El poder está desactivado, por lo tanto Pacman pierde una vida
						//play_sample(muertePacman,200,150,1000,0);//Se activa el sonido de la muerte del Pacman
						//reiniciarFantasmas(matrizJuego,pvez);//Metemos a todos los fantasmas a su casa para que podamos sacarlos con orden
						//reiniciarPacman(matrizJuego,pVidas);//Pacman regresa a su punto de origen
						//jugadorActual.setVidas(jugadorActual.getVidas()-1);
						//mapa.setMatrizJuego(14,14,0);//[14][14]=0;//Reseteamos a pacman	
						//mapa.setMatrizJuego(this->posI,this->posJ,2);//[this->posI][this->posJ]=2;//Se borra la posición en donde estaba pacman. Pacman ya había comido, por lo tanto se le deja un espacio libre=2	
						//this->posI=14;
						//this->posJ=14;
						this->setMuerto(true);
					}
					else if(this->poder && this->comibles[buscar-7]){//El poder está activado, por lo tanto Pacman puede seguir y comerse al fantasma que le indicó el "*pQuien"
						//play_sample(muerteFantasma,200,150,1000,0);//Se activa el sonido de la muerte de un fantasma
						//meterFantasmas(matrizJuego,buscar);//Metemos a su casa al fantasma "*pQuien" que se le asignó a la variable buscar.
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
					if(mapa.getSonido()==0){//Quiere decir que el usuario está permitiendo el sonido del movimiento del pacman
						//play_sample(sonido,200,150,1000,0);
					}
					mapa.setMatrizJuego(this->posI,this->posJ,2);//[this->posI][this->posJ]=2;//Borramos la posición anterior, el pacman ya comió	por eso dejamos un 2		  
		            mapa.setMatrizJuego(this->posI,this->posJ-1,0);//[this->posI][this->posJ-1]=0; //Se hace el movimiento del pacman, cambiándolo de posición
					this->posJ-=1;
				}
				else{//Pacman podría perder una vida, pero hay que validar primero
					auxI=this->posI;
					auxJ=this->posJ;
					switch(direccion){
						case 1://Arriba
							auxI=this->posI-1;
							break;
						case 2://Abajo
							auxI=this->posI+1;
							break;
						case 3://Izquierda
							auxJ=this->posJ-1;
							break;
						case 4://Derecha
							auxJ=this->posJ+1;
							break;
					}
					buscar=mapa.getMatrizJuego(auxI,auxJ);//[auxI][auxJ];//Ya estamos enterados de qué Fantasma fue el que provocó que Pacman estuviera en esta instancia. El "*pQuien" se modificó en la función morirPacman, pero por practicidad lo metemos a una variable tipo INT
					if(!this->poder){//El poder está desactivado, por lo tanto Pacman pierde una vida
						//play_sample(muertePacman,200,150,1000,0);//Se activa el sonido de la muerte del Pacman
						/*mapa.setMatrizJuego(14,14,0);//[14][14]=0;//Reseteamos a pacman	
						mapa.setMatrizJuego(this->posI,this->posJ,2);//[this->posI][this->posJ]=2;//Se borra la posición en donde estaba pacman. Pacman ya había comido, por lo tanto se le deja un espacio libre=2	
						this->posI=14;
						this->posJ=14;*/
						this->setMuerto(true);
					}
					else if(this->poder && this->comibles[buscar-7]){//El poder está activado, por lo tanto Pacman puede seguir y comerse al fantasma que le indicó el "*pQuien"
						//play_sample(muerteFantasma,200,150,1000,0);//Se activa el sonido de la muerte de un fantasma
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
					if(mapa.getSonido()==0){//Quiere decir que el usuario está permitiendo el sonido del movimiento del pacman
						//play_sample(sonido,200,150,1000,0);
					}
					mapa.setMatrizJuego(this->posI,this->posJ,2);//Borramos la posición anterior, el pacman ya comió	por eso dejamos un 2		  
		            mapa.setMatrizJuego(this->posI,this->posJ+1,0);//Se hace el movimiento del pacman, cambiándolo de posición
					this->posJ+=1;
				}
				else{//Pacman podría perder una vida, pero hay que validar primero
					auxI=this->posI;
					auxJ=this->posJ;
					switch(direccion){
						case 1://Arriba
							auxI=this->posI-1;
							break;
						case 2://Abajo
							auxI=this->posI+1;
							break;
						case 3://Izquierda
							auxJ=this->posJ-1;
							break;
						case 4://Derecha
							auxJ=this->posJ+1;
							break;
					}
					buscar=mapa.getMatrizJuego(auxI,auxJ);//[auxI][auxJ];//Ya estamos enterados de qué Fantasma fue el que provocó que Pacman estuviera en esta instancia. El "*pQuien" se modificó en la función morirPacman, pero por practicidad lo metemos a una variable tipo INT
					if(!this->poder){//El poder está desactivado, por lo tanto Pacman pierde una vida
						//play_sample(muertePacman,200,150,1000,0);//Se activa el sonido de la muerte del Pacman
						/*mapa.setMatrizJuego(14,14,0);//[14][14]=0;//Reseteamos a pacman	
						mapa.setMatrizJuego(this->posI,this->posJ,2);//[this->posI][this->posJ]=2;//Se borra la posición en donde estaba pacman. Pacman ya había comido, por lo tanto se le deja un espacio libre=2	
						this->posI=14;
						this->posJ=14;*/
						this->setMuerto(true);
					}
					else if(this->poder && this->comibles[buscar-7]){//El poder está activado, por lo tanto Pacman puede seguir y comerse al fantasma que le indicó el "*pQuien"
						//play_sample(muerteFantasma,200,150,1000,0);//Se activa el sonido de la muerte de un fantasma
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
					if(mapa.getSonido()==0){//Quiere decir que el usuario está permitiendo el sonido del movimiento del pacman
						//play_sample(sonido,200,150,1000,0);
					}
					mapa.setMatrizJuego(this->posI,this->posJ,2);//Borramos la posición anterior, el pacman ya comió	por eso dejamos un 2		  
		            mapa.setMatrizJuego(this->posI+1,this->posJ,0);//Se hace el movimiento del pacman, cambiándolo de posición
					this->posI+=1;
				}
				else{//Pacman podría perder una vida, pero hay que validar primero
					auxI=this->posI;
					auxJ=this->posJ;
					switch(direccion){
						case 1://Arriba
							auxI=this->posI-1;
							break;
						case 2://Abajo
							auxI=this->posI+1;
							break;
						case 3://Izquierda
							auxJ=this->posJ-1;
							break;
						case 4://Derecha
							auxJ=this->posJ+1;
							break;
					}
					buscar=mapa.getMatrizJuego(auxI,auxJ);//[auxI][auxJ];//Ya estamos enterados de qué Fantasma fue el que provocó que Pacman estuviera en esta instancia. El "*pQuien" se modificó en la función morirPacman, pero por practicidad lo metemos a una variable tipo INT
					if(!this->poder){//El poder está desactivado, por lo tanto Pacman pierde una vida
						//play_sample(muertePacman,200,150,1000,0);//Se activa el sonido de la muerte del Pacman
						/*mapa.setMatrizJuego(14,14,0);//[14][14]=0;//Reseteamos a pacman	
						mapa.setMatrizJuego(this->posI,this->posJ,2);//[this->posI][this->posJ]=2;//Se borra la posición en donde estaba pacman. Pacman ya había comido, por lo tanto se le deja un espacio libre=2	
						this->posI=14;
						this->posJ=14;*/
						this->setMuerto(true);
					}
					else if(this->poder && this->comibles[buscar-7]){//El poder está activado, por lo tanto Pacman puede seguir y comerse al fantasma que le indicó el "*pQuien"
						//play_sample(muerteFantasma,200,150,1000,0);//Se activa el sonido de la muerte de un fantasma
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
	    		mapa.setMatrizJuego(10,29,0);//[10][29]=0;//Ponemos a Pacman en su nueva posición
				mapa.setMatrizJuego(10,0,2);//mapa.matrizJuego[10][0]=2;//Borra la posición anterior
				mapa.setMatrizJuego(10,1,2);//mapa.matrizJuego[10][1]=2;//Borra la posición ANTE Anterior
				this->posI=10;
				this->posJ=29;
				return false;
			}
	        return (mapa.getMatrizJuego(this->posI,this->posJ-1)!=1);//(mapa.matrizJuego[posX][posY - 1] != 1 );
	        break;
	    case 4://Derecha, verificación de Teleport de Derecha a Izquierda
	    	if(this->posI==10 && this->posJ+1==29){
	    		mapa.setMatrizJuego(10,0,0);//mapa.matrizJuego[10][0]=0;//Ponemos a Pacman en su nueva posición
				mapa.setMatrizJuego(10,29,2);//mapa.matrizJuego[10][29]=2;//Borra la posición anterior
				mapa.setMatrizJuego(10,28,2);//mapa.matrizJuego[10][28]=2;//Borra la posición ANTE Anterior
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
	int auxI,auxJ;
	auxI=this->posI;
	auxJ=this->posJ;
	if(this->poder)//Tiene poder, debemos incrementarle al contador
		this->contPoder++;
	switch(direccion){
		case 1://Arriba
			auxI=this->posI-1;
			break;
		case 2://Abajo
			auxI=this->posI+1;
			break;
		case 3://Izquierda
			auxJ=this->posJ-1;
			break;
		case 4://Derecha
			auxJ=this->posJ+1;
			break;
	}
	if(mapa.getMatrizJuego(auxI,auxJ)==6){
		this->contPoder=0;
		this->poder=true;
		for(int i=0;i<4;i++)
			this->comibles[i]=true;
		mapa.setPausaF(0,0);
		mapa.setPausaF(1,0);
		mapa.setPausaF(2,0);
		mapa.setPausaF(3,0);
	}
	if(this->contPoder==100){//Se acabó el poder
		mapa.setPausaF(0,0);
		mapa.setPausaF(1,0);
		mapa.setPausaF(2,0);
		mapa.setPausaF(3,0);//Provoca que los fantasmas ya puedan estar asustados  en caso de que el Pacman coma
		for(int i=0;i<4;i++)
			this->comibles[i]=false;
		this->poder=false;
		this->contPoder=0;
	}
		
		
	//return (matrizJuego[auxI][auxJ]==6);
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
	
	switch(direccion){
		case 1://Arriba
			auxI=this->posI-1;
			break;
		case 2://Abajo
			auxI=this->posI+1;
			break;
		case 3://Izquierda
			auxJ=this->posJ-1;
			break;
		case 4://Derecha
			auxJ=this->posJ+1;
			break;
			
	}
	
    if(mapa.getMatrizJuego(auxI,auxJ)==7 || mapa.getMatrizJuego(auxI,auxJ)==8 || mapa.getMatrizJuego(auxI,auxJ)== 9 || mapa.getMatrizJuego(auxI,auxJ)==10){//Si pacman se ha comido una pastilla y hay un fantasma...
	 	if(this->poder){//El poder está activo
	 		mapa.setPuntuacionTotal(mapa.getPuntuacionTotal()+20);
	 		//*puntuacionTotal+=20;	
		}
    }
	else if(mapa.getMatrizJuego(auxI,auxJ) == 4){//Si es igual a donde hay una fruta...
		mapa.setFrutas(mapa.getFrutas()+1);
		mapa.setPuntuacionTotal(mapa.getPuntuacionTotal()+15);
		//*pFrutas=*pFrutas+1; //Si pFrutas=1 -> Fruta 1   pFrutas=3 -> Fruta 2   pFrutas=3 -> Fruta 3
    	//*puntuacionTotal+=15;
	} else if(mapa.getMatrizJuego(auxI,auxJ) == 5){//Si pacman se ha  un puntito...
		//*puntuacionTotal+=5;comido
		mapa.setPuntuacionTotal(mapa.getPuntuacionTotal()+5);
    }
    else if (mapa.getMatrizJuego(auxI,auxJ)==6){//Si pacman se ha comido un punto grande...
    	mapa.setPuntuacionTotal(mapa.getPuntuacionTotal()+10);
    	//*puntuacionTotal+=10;
    }	
	if(cont<=PUNTOS){//Se tiene que activar un cambio de mapa y se guardan los puntos en el archivo
		//puntos=*puntuacionTotal;
		//registrarPuntaje(registroUsuarios,id,puntos,pVidas);//Aplica el guardado con los puntos actuales + los puntos que tiene registrados
		//*pCambioNivel=1;//Se activa el cambio de nivel
		cout<<"Contador = "<<cont<<endl;
		mapa.setCambioNivel(true);
	}

}

bool Pacman::morirPacman(Mapa &mapa){
	int auxI,auxJ;
	auxI=this->posI;
	auxJ=this->posJ;
	switch(direccion){
		case 1://Arriba
			auxI=this->posI-1;
			break;
		case 2://Abajo
			auxI=this->posI+1;
			break;
		case 3://Izquierda
			auxJ=this->posJ-1;
			break;
		case 4://Derecha
			auxJ=this->posJ+1;
			break;
	}
	if(mapa.getMatrizJuego(auxI,auxJ)==7 || mapa.getMatrizJuego(auxI,auxJ)==8 || mapa.getMatrizJuego(auxI,auxJ)==9 || mapa.getMatrizJuego(auxI,auxJ)==10)//Pacman podría morir, hay que verificar la información en el movimiento del Pacman
		return true;
	return false;
	
}
#endif
