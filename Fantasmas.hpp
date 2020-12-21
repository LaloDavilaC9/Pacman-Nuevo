#pragma once
#include <cmath>
#include <vector>
#include <algorithm>
class Mapa;
class Fantasmas {
public:
    int id, posI, posJ, direccion;
    bool comibles;
    SAMPLE *muertePacman,*muerteFantasma;//=load_sample("pacman-dies.wav");
    Fantasmas(){}
    Fantasmas(int id,int i, int j) {
    	this->id=id;
       	this->posI = i;
        this->posJ = j;
        this->direccion = 0;
        this->comibles = false;
        this->muertePacman=load_sample("Elementos\\pacman-dies.wav");
        this->muerteFantasma=load_sample("Elementos\\pacman-eating-ghost");
    }
    virtual void movimientoNormal(Mapa &, int,Pacman &) = 0;
    //virtual void movimientoPersecucion() = 0;
    //virtual void movimientoHuida() = 0;
    //int* posiciones();
    //bool matarPacman(int*, bool);
    //void regeneracion();
   	bool movimientoValido(Mapa &mapa);
    void sacarFantasmas(Mapa &);
    void setPos(int i, int j){
		this->posI=i;
		this->posJ=j;
	}
	int getI(){return this->posI;}
	int getJ(){return this->posJ;}
	int distancia(int iF, int jF, int i, int j) {
		return (abs(iF-i)+abs(jF-j));
	}
	int distanciaMenor(int dis[]) {
		int menor=dis[0];
		int indi=1;
		for(int i=0; i<4; i++) {
			if(dis[i]<menor) {
				menor=dis[i];
				indi=i+1;
			}		
		}
		return indi;
	}
	int distanciaMayor(int dis[]){
		vector <int> copiaDis;
		int menor=0;
		int indi=1;
		for(int i=0; i<4; i++) {
			copiaDis.push_back(dis[i]);
		}
		sort(copiaDis.begin(),copiaDis.end());
		for(int i=0;i<4;i++){
			if(copiaDis[1]==dis[i]){
				indi=i+1;
				break;
			}
			
		}
		return indi;
	}
};
#include "Mapa.hpp"
void Fantasmas::sacarFantasmas(Mapa &mapa){
	//En esta función, sacamos a los fantasmas de forma validada para que los ponga en la primer posición disponible
	int i=7,j,k=12,band=0;
	mapa.setMatrizJuego(this->posI,this->posJ,2);//Posición borrada
	for(j=12;j<18;j++){
		if(mapa.getMatrizJuego(i,j)==2 || mapa.getMatrizJuego(i,j)==5){//Posición libre para avanzar			
			mapa.setMatrizJuego(i,j,this->id);
			this->posI=i;
			this->posJ=j;
			band=1;
			return;
		}
		else if(mapa.getMatrizJuego(k,j)==2 || mapa.getMatrizJuego(k,j)==5){
			mapa.setMatrizJuego(k,j,this->id);
			this->posI=k;
			this->posJ=j;
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
				this->posI=i;
				this->posJ=j;
				return;
			}			
			else if(mapa.getMatrizJuego(i,k)==2 || mapa.getMatrizJuego(i,k)==5){
				mapa.setMatrizJuego(i,k,this->id);
				this->posI=i;
				this->posJ=k;
				return;
			}
		}
	}
}
class Clyde : public Fantasmas { //ID = 10  Naranja
	public:
		Clyde() : Fantasmas(){}
		Clyde(int id,int i, int j) : Fantasmas(id,i,j){}
    	void movimientoNormal(Mapa &mapa, int vez,Pacman &pacman){
    		int auxI=this->posI,auxJ=this->posJ,valorPre=2;
    		if(vez==6){//Lo sacamos de su casa por primera vez
    			this->sacarFantasmas(mapa);
    			mapa.setMatrizJuego(10,14,2);
			}
    		else if(vez>6){//Comienza su movimiento normal
    			srand(time(NULL));
				this->direccion=rand()%4+1;//Generamos random un número para que se mueva 1=Derecha 2=Izquierda 3=Arriba 4=Abajo
				if(this->movimientoValido(mapa)){//El fantasma se puede mover libremente
					switch(this->direccion){
	    				case 1://Arriba
	    					mapa.setDirecciones(4,4);//Apunta hacia arriba en el mapa
							this->posI-=1;
							break;
						case 2://Abajo
							mapa.setDirecciones(4,3);//Apunta hacia arriba en el mapa
							this->posI+=1;
							break;
						case 3://Izquierda
							mapa.setDirecciones(4,1);//Apunta hacia arriba en el mapa
							this->posJ-=1;
							
							break;
						case 4://Derecha
							mapa.setDirecciones(4,2);//Apunta hacia arriba en el mapa
							this->posJ+=1;
							break;
					}
					if(mapa.getMatrizJuego(this->posI,this->posJ)==0){//Se encontró con el Pacman, hay que checar si el poder del pacman está activo o no
						if(pacman.getPoder() && pacman.getComible(3)){//Se muere el fantasma
							this->comibles=false;
							mapa.setMatrizJuego(auxI,auxJ,2);//Borramos la posición anterior del fantasma
							this->posI=10;
							this->posJ=14;//Regresamos el fantasma a su casa
							mapa.setMatrizJuego(10,14,10);
							this->sacarFantasmas(mapa);
							pacman.setComibles(3,false);
							mapa.setPausaF(3,1);//Está normal el Fantasma, ya no está asustado
						}
						else{//El fantasma mata a Pacman, todos a sus casas y Pacman se reinicia
							this->posI=auxI;
							this->posJ=auxJ;
							pacman.setMuerto(true);	
							pacman.setPoder(false);
							play_sample(muertePacman,200,150,1000,0);//Se activa el sonido de la muerte del Pacman
						}
					}
					else{//Se encontró cualquier otra cosa
						valorPre=mapa.getMatrizJuego(this->posI,this->posJ);//Guardamos el valor que traía antes
						mapa.setMatrizJuego(this->posI,this->posJ,10);
						if(valorPre!=7 && valorPre!=8 && valorPre!=9 && valorPre!=10)
							mapa.setMatrizJuego(auxI,auxJ,valorPre);
						else
							mapa.setMatrizJuego(auxI,auxJ,2);
					}		
				}
			}
		}
};
class Blinky : public Fantasmas { //ID = 7 Rojo
public:
	Blinky(int id, int i, int j) : Fantasmas(id,i,j){}
    void movimientoNormal(Mapa &mapa, int vez,Pacman &pacman) {
    	int auxI=this->posI,auxJ=this->posJ,valorPre=2;
    	int auxPacmanI=0,auxPacmanJ=0;
    	int distancias[4]; //Guarda las distancias en los diferentes movimientos posibles
    		if(vez==21){//Lo sacamos de su casa por primera vez
    			this->sacarFantasmas(mapa);
    			mapa.setMatrizJuego(10,15,2);
			}
    		else if(vez>21 &&vez%2==0){//Comienza su movimiento normal
    			if(pacman.getComible(0)){//Pacman tiene poder y el fantasma está asustado porque se lo puede comer
    				auxPacmanI=5;
    				auxPacmanJ=5;
				}
    			if(mapa.getMatrizJuego(posI-1, posJ)!=1) 
    				distancias[0]=distancia(posI-1, posJ, pacman.getI()-auxPacmanI, pacman.getJ()-auxPacmanJ);//Arriba
    			else 
    				distancias[0]=9999;
    			if(mapa.getMatrizJuego(posI+1, posJ)!=1) 
    				distancias[1]=distancia(posI+1, posJ, pacman.getI()-auxPacmanI, pacman.getJ()-auxPacmanJ);//Abajo
    			else 
    				distancias[1]=9999;
    			if(mapa.getMatrizJuego(posI, posJ-1)!=1) 
    				distancias[2]=distancia(posI, posJ-1, pacman.getI()-auxPacmanI, pacman.getJ()-auxPacmanJ);//Izquierda
    			else 
    				distancias[2]=9999;
    			if(mapa.getMatrizJuego(posI, posJ+1)!=1) 
    				distancias[3]=distancia(posI, posJ+1, pacman.getI()-auxPacmanI, pacman.getJ()-auxPacmanJ);//Derecha
    			else 
    				distancias[3]=9999;
    			direccion=distanciaMenor(distancias);
    			cout<<direccion<<endl;
				if(this->movimientoValido(mapa)){//El fantasma se puede mover libremente
					switch(this->direccion){
	    				case 1://Arriba
	    					mapa.setDirecciones(1,4);//Apunta hacia arriba en el mapa
							this->posI-=1;
							break;
						case 2://Abajo
							mapa.setDirecciones(1,3);//Apunta hacia arriba en el mapa
							this->posI+=1;
							break;
						case 3://Izquierda
							mapa.setDirecciones(1,1);//Apunta hacia arriba en el mapa
							this->posJ-=1;
							
							break;
						case 4://Derecha
							mapa.setDirecciones(1,2);//Apunta hacia arriba en el mapa
							this->posJ+=1;
							break;
					}
					if(mapa.getMatrizJuego(this->posI,this->posJ)==0){//Se encontró con el Pacman, hay que checar si el poder del pacman está activo o no
						if(pacman.getPoder() && pacman.getComible(0)){//Se muere el fantasma
							this->comibles=false;
							mapa.setMatrizJuego(auxI,auxJ,2);//Borramos la posición anterior del fantasma
							this->posI=10;
							this->posJ=15;//Regresamos el fantasma a su casa
							mapa.setMatrizJuego(10,15,7);
							this->sacarFantasmas(mapa);
							pacman.setComibles(0,false);
							mapa.setPausaF(0,1);//Está normal el Fantasma, ya no está asustado
						}
						else{//El fantasma mata a Pacman, todos a sus casas y Pacman se reinicia
							this->posI=auxI;
							this->posJ=auxJ;
							pacman.setMuerto(true);	
							pacman.setPoder(false);
								play_sample(muertePacman,200,150,1000,0);//Se activa el sonido de la muerte del Pacman
						}
						
					}
					else{//Se encontró cualquier otra cosa
						valorPre=mapa.getMatrizJuego(this->posI,this->posJ);//Guardamos el valor que traía antes
						mapa.setMatrizJuego(this->posI,this->posJ,7);
						if(valorPre!=7 && valorPre!=8 && valorPre!=9 && valorPre!=10)
							mapa.setMatrizJuego(auxI,auxJ,valorPre);
						else
							mapa.setMatrizJuego(auxI,auxJ,2);
					}		
				} 		
			}
		}
};
class Pinky : public Fantasmas{//ID: 9
public:
	Pinky(int id, int i, int j) : Fantasmas(id,i,j){}
    void movimientoNormal(Mapa &mapa, int vez,Pacman &pacman) {
    	int auxI=this->posI,auxJ=this->posJ,valorPre=2;
    	int auxPacmanI=0,auxPacmanJ=0;

    	int distancias[4]; //Guarda las distancias en los diferentes movimientos posibles
    		if(vez==41){//Lo sacamos de su casa por primera vez
    			this->sacarFantasmas(mapa);
    			mapa.setMatrizJuego(9,14,2);
			}
    			
    		else if(vez>41 && vez%2==0){//Comienza su movimiento normal
	    		if(pacman.getComible(2)){//Pacman tiene poder y el fantasma está asustado porque se lo puede comer
	    			auxPacmanI=-5;
	    			auxPacmanJ=-5;
				}
    			if(pacman.getI()-auxPacmanI==this->posI){//Están en la misma fila
					if(pacman.getJ()-auxPacmanJ > this->posJ)//Quiere decir que el pacman  está a la derecha
						this->direccion=4;
					else if(pacman.getJ()-auxPacmanJ < this->posJ)//Quiere decir que el pacman está a la izquierda
						this->direccion=3;
				}
    			else if(pacman.getJ()-auxPacmanJ==this->posJ){
    				if(pacman.getI()-auxPacmanI > this->posI)//Pacman está abajo
						this->direccion=2;
					else if(pacman.getI()-auxPacmanI < this->posI)//Pacman está arriba
						this->direccion=1;
				}
				else{
					if(pacman.getJ()-auxPacmanJ > this->posJ)//Quiere decir que el pacman  está a la derecha
						this->direccion=4;
					else if(pacman.getJ()-auxPacmanI < this->posJ)//Quiere decir que el pacman está a la izquierda
						this->direccion=3;
					else if(pacman.getI()-auxPacmanI > this->posI)//Pacman está abajo
						this->direccion=2;
					else if(pacman.getI()-auxPacmanI < this->posI)//Pacman está arriba
						this->direccion=1;
				}
				if(!this->movimientoValido(mapa)){
					this->direccion=0;
					do{
						this->direccion++;
					}while(!this->movimientoValido(mapa));
				}
				if(this->movimientoValido(mapa)){//El fantasma se puede mover libremente
					switch(this->direccion){
	    				case 1://Arriba
	    					mapa.setDirecciones(3,4);//Apunta hacia arriba en el mapa
							this->posI-=1;
							break;
						case 2://Abajo
							mapa.setDirecciones(3,3);//Apunta hacia arriba en el mapa
							this->posI+=1;
							break;
						case 3://Izquierda
							mapa.setDirecciones(3,1);//Apunta hacia arriba en el mapa
							this->posJ-=1;
							break;
						case 4://Derecha
							mapa.setDirecciones(3,2);//Apunta hacia arriba en el mapa
							this->posJ+=1;
							break;
					}
					if(mapa.getMatrizJuego(this->posI,this->posJ)==0){//Se encontró con el Pacman, hay que checar si el poder del pacman está activo o no
						if(pacman.getPoder() && pacman.getComible(2)){//Se muere el fantasma
							this->comibles=false;
							mapa.setMatrizJuego(auxI,auxJ,2);//Borramos la posición anterior del fantasma
							this->posI=9;
							this->posJ=14;//Regresamos el fantasma a su casa
							mapa.setMatrizJuego(9,14,9);
							this->sacarFantasmas(mapa);
							pacman.setComibles(2,false);
							mapa.setPausaF(3,1);//Está normal el Fantasma, ya no está asustado
						}
						else{//El fantasma mata a Pacman, todos a sus casas y Pacman se reinicia
							this->posI=auxI;
							this->posJ=auxJ;
							pacman.setMuerto(true);	
							pacman.setPoder(false);
							play_sample(muertePacman,200,150,1000,0);//Se activa el sonido de la muerte del Pacman
						}
						
					}
					else{//Se encontró cualquier otra cosa
						valorPre=mapa.getMatrizJuego(this->posI,this->posJ);//Guardamos el valor que traía antes
						mapa.setMatrizJuego(this->posI,this->posJ,9);
						if(valorPre!=7 && valorPre!=8 && valorPre!=9 && valorPre!=10)
							mapa.setMatrizJuego(auxI,auxJ,valorPre);
						else
							mapa.setMatrizJuego(auxI,auxJ,2);
					}			
				}	
			}
		}
	
};
class Inky : public Fantasmas{//ID = 8
public:
	Inky(int id, int i, int j) : Fantasmas(id,i,j){}
    void movimientoNormal(Mapa &mapa, int vez,Pacman &pacman) {
    	int auxI=this->posI,auxJ=this->posJ,valorPre=2;
    	int auxPacmanI=0,auxPacmanJ=0;
    	int distancias[4]; //Guarda las distancias en los diferentes movimientos posibles
    		if(vez==59){//Lo sacamos de su casa por primera vez
    			this->sacarFantasmas(mapa);
    			mapa.setMatrizJuego(9,15,2);
			}
    		else if(vez>59 && vez%4==0){//Comienza su movimiento normal
    			if(pacman.getComible(1)){//Pacman tiene poder y el fantasma está asustado porque se lo puede comer
    				auxPacmanI=10;
    				auxPacmanJ=10;
				}
    			if(mapa.getMatrizJuego(posI-1, posJ)!=1) 
    				distancias[0]=distancia(posI-1, posJ, pacman.getI()-auxPacmanI, pacman.getJ()-auxPacmanJ);//Arriba
    			else 
    				distancias[0]=9999;
    			if(mapa.getMatrizJuego(posI+1, posJ)!=1) 
    				distancias[1]=distancia(posI+1, posJ, pacman.getI()-auxPacmanI, pacman.getJ()-auxPacmanJ);//Abajo
    			else 
    				distancias[1]=9999;
    			if(mapa.getMatrizJuego(posI, posJ-1)!=1) 
    				distancias[2]=distancia(posI, posJ-1, pacman.getI()-auxPacmanI, pacman.getJ()-auxPacmanJ);//Izquierda
    			else 
    				distancias[2]=9999;
    			if(mapa.getMatrizJuego(posI, posJ+1)!=1) 
    				distancias[3]=distancia(posI, posJ+1, pacman.getI()-auxPacmanI, pacman.getJ()-auxPacmanJ);//Derecha
    			else 
    				distancias[3]=9999;
    			direccion=distanciaMenor(distancias);
				if(this->movimientoValido(mapa)){//El fantasma se puede mover libremente
					switch(this->direccion){
	    				case 1://Arriba
	    					mapa.setDirecciones(2,4);//Apunta hacia arriba en el mapa
							this->posI-=1;
							break;
						case 2://Abajo
							mapa.setDirecciones(2,3);//Apunta hacia arriba en el mapa
							this->posI+=1;
							break;
						case 3://Izquierda
							mapa.setDirecciones(2,1);//Apunta hacia arriba en el mapa
							this->posJ-=1;
							
							break;
						case 4://Derecha
							mapa.setDirecciones(2,2);//Apunta hacia arriba en el mapa
							this->posJ+=1;
							break;
					}
					if(mapa.getMatrizJuego(this->posI,this->posJ)==0){//Se encontró con el Pacman, hay que checar si el poder del pacman está activo o no
						if(pacman.getPoder() && pacman.getComible(1)){//Se muere el fantasma
							this->comibles=false;
							mapa.setMatrizJuego(auxI,auxJ,2);//Borramos la posición anterior del fantasma
							this->posI=9;
							this->posJ=15;//Regresamos el fantasma a su casa
							mapa.setMatrizJuego(9,15,8);
							this->sacarFantasmas(mapa);
							pacman.setComibles(1,false);
							mapa.setPausaF(1,1);//Está normal el Fantasma, ya no está asustado
						}
						else{//El fantasma mata a Pacman, todos a sus casas y Pacman se reinicia
							this->posI=auxI;
							this->posJ=auxJ;
							pacman.setMuerto(true);	
							pacman.setPoder(false);
								play_sample(muertePacman,200,150,1000,0);//Se activa el sonido de la muerte del Pacman
						}
						
					}
					else{//Se encontró cualquier otra cosa
						valorPre=mapa.getMatrizJuego(this->posI,this->posJ);//Guardamos el valor que traía antes
						mapa.setMatrizJuego(this->posI,this->posJ,8);
						if(valorPre!=7 && valorPre!=8 && valorPre!=9 && valorPre!=10)
							mapa.setMatrizJuego(auxI,auxJ,valorPre);
						else
							mapa.setMatrizJuego(auxI,auxJ,2);
					}		
				} 		
			}
		}
};
bool Fantasmas::movimientoValido(Mapa &mapa) {
    switch (this->direccion) {
	    case 1://Arriba
	        return (mapa.getMatrizJuego(this->posI-1,this->posJ)!=1);
	        break;
	    case 2://Abajo
	        return (mapa.getMatrizJuego(this->posI+1,this->posJ)!=1);
	        break;
	    case 3://Izquierda, verificación de Teleport de Izquierda a Derecha
	    	if(this->posI==10 && this->posJ-1==0){
	    		mapa.setMatrizJuego(10,29,this->id);//Ponemos al fantasma en su nueva posición
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
	    		mapa.setMatrizJuego(10,0,this->id);//Ponemos al fantasma en su nueva posición
				mapa.setMatrizJuego(10,29,2);//Borra la posición anterior
				mapa.setMatrizJuego(10,28,2);//Borra la posición ANTE Anterior
				this->posI=10;
				this->posJ=0;
				return false;
			}
	        return (mapa.getMatrizJuego(this->posI,this->posJ+1)!=1);
	        break;
	    default:
	        return false;
	        break;
    }
}
