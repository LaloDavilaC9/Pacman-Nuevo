#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <allegro.h>
#include <algorithm>
#define AZUL makecol(250, 196, 30)//Color predefinido
#define NEGRO makecol(0, 0, 0)//Color predefido

using namespace std;
struct Jugador{
	unsigned int vidas, puntos, id,nivel;
	char nombreU[25], password[25];
};
class Jugadores {
private:
    unsigned int vidas, puntos, id,nivel;
    string nombreU, password;
public:
    Jugadores();
    bool verificarRepeticionesUsuario();
    unsigned int verID() { return id; }
	void setNombre(string nombre) {nombreU = nombre;}
	void setPassword(string pass) {password = pass;}
    void setVidas(unsigned int nVidas) { vidas = nVidas; }
    void setPuntos(unsigned int score) { puntos = score; }
	void setID(unsigned int iD) {id = iD;}
	void setNivel(unsigned int niv){this->nivel=niv;}
	string getNom() {return nombreU;}
	string getPass() {return password;}
	unsigned int getId(){return this->id;}
	unsigned int getVidas(){return this->vidas;}
    unsigned int getPuntos() { return puntos; }
    unsigned int getNivel(){return this->nivel;}
    
};
class HistorialJugadores {
    vector <Jugadores> tRegistros;
public:
    HistorialJugadores() {};
    Jugadores *  registroEnArchivo();
    Jugadores * iniciarSesion();
    bool validarContrasena(Jugadores *);
    void modificarInformacion(Jugadores,int);
    void ganadores();
};
bool compare(Jugador & x, Jugador& y);
Jugadores::Jugadores() {
    this->vidas = 3;
    this->puntos = 0;
   	this->id = 0;
    this->nombreU = " ";
    this->password = " ";
    this->nivel=1;
}
bool Jugadores::verificarRepeticionesUsuario() {
    fstream archivo;
    Jugador aux;
    archivo.open("Elementos\\DatosJugadores.dat", ios::binary | ios::in);
	if(!archivo)//No existe un archivo, por lo tanto no hay una repetició
		return false;
    archivo.seekg(0, ios::beg);
	while (!archivo.eof()) {
        archivo.read((char*)&aux, sizeof(Jugador));
        if (strcmp(aux.nombreU,this->nombreU.c_str())==0) {
			archivo.close();
			return true;
		}
    }
    archivo.close();
    return false;
}
Jugadores *HistorialJugadores::registroEnArchivo(){
	Jugador auxiliarArchivo;
	unsigned int canReg;
	char ASCII, p[255];
	int newkey,indice=0;
	bool salida=false,salida2=false,usuarioLibre=false;
	string auxNomb="", auxPass="";
	BITMAP *registro=create_bitmap(900, 600),*continuar=create_bitmap(900, 600);
	Jugadores *jug=new Jugadores ();
	fstream archivo;
	archivo.open("Elementos\\DatosJugadores.dat",ios::binary | ios::in | ios::out);//Abrimos el archivo existente
	if(!archivo)
		 archivo.open("Elementos\\DatosJugadores.dat",ios::binary|ios::out);//Lo crea
	registro=load_bitmap("Elementos\\FotoRegistroInicio.bmp", NULL);
	continuar=load_bitmap("Elementos\\FotoRegistro.bmp", NULL);
	blit(registro, screen, 0, 0, 0, 0, 900, 600);
	clear_keybuf();//Borramos el buffer de entrada del teclado
	strcpy(p,"                       ");
	while(!salida) {
		do {
			vline(screen, (indice*8)+335, 99, 107, AZUL);//Aparece el cursor para escribir
			if(keypressed()) {//Detectamos si se pulsa una tecla
				 newkey = readkey();//Almacenamos el valor
				 ASCII = newkey & 0xff;//Convertimos lo almacenado
				if(ASCII>=32 && ASCII <=126) {//Validamos el ingreso de solo caracteres permitidos
					if(indice<12-1) {//Validamos que el usuario no ingrese más caracteres de los permitidos
						p[indice]=ASCII;//Metemos el caracter ingresado al vector
						indice++;
						p[indice]='\0';//Asignamos el fin de línea
					}
					else
						p[indice+1]=' ';
				}
				else if(ASCII==8){//Está retrocediendo
					p[indice]=' ';//Se borra la posición actual
					if(indice!=0){
						indice--;//Cambiamos de índice
						p[indice]=' ';//Se borra la posición actual
					}
				}
				else if(ASCII==27){//El usuario aprieta Escape, hay que retornar
					jug->setID(-1);//Mandamos -1 para evitar que al dar escape, entre al motor del juego
					return jug;
				}
			}
			if(ASCII==13){//El usuario le dió ENTER
				//Verificamos si ese nombre ya lo está usando algún otro usuario
				if(jug->verificarRepeticionesUsuario())//Usuario repetido
					textprintf_ex(screen, font, 335, 139, AZUL,0, "¡Ups! Nombre ya existente, intente con uno diferente");
				else{
					textprintf_ex(screen, font, 335, 139, AZUL,0, "                                                    ");
					usuarioLibre=true;
				}
			}
			auxNomb=p;
			jug->setNombre(auxNomb);
			textout_ex(screen, font, p, 335, 99,AZUL, 0);
			memset(p,255,'\0');
		} while(!usuarioLibre);//Sale cuando se le da un enter y el usuario es válido	
		indice=0;
		clear_keybuf();
		strcpy(p,"                       ");
		do {
			vline(screen, indice*8+335, 210, 218, AZUL);
			if(keypressed()) {
				 newkey = readkey();
				 ASCII =newkey & 0xff;
				 if(ASCII==13){
				 	salida2=true;
			 	 }
				 else{
				 	if(ASCII>=32 && ASCII <=126) {
						if(indice<25) {
							p[indice]=ASCII;
							indice++;
							p[indice]='\0';
						}
						else
							p[indice+1]=' ';
				 	}
					 else if(ASCII==8){//Está retrocediendo
						p[indice-1]=' ';//Se borra la posición actual
						if(indice!=0){
							indice--;//Cambiamos de índice
						}
					}
					else if(ASCII==27){//Apretó Escape, hay que retornar
						jug->setID(-1);//Mandamos -1 para evitar que al dar escape, entre al motor del juego
						return jug;
					}	
				}
			}
			auxPass=p;
			jug->setPassword(auxPass);
			textout_ex(screen, font, p, 335, 210,AZUL, 0);
			memset(p,255,'\0');
		} 
		while(salida2==false);
		
		if(jug->getNom().size()!=0 && jug->getPass().size()!=0) {
			salida=true;
		}//Salimos del ciclo que controla la funciï¿½n
	}
	archivo.seekp(0, ios::end);
    canReg=archivo.tellg()/sizeof(Jugador);
	jug->setID(canReg+1);
	jug->setVidas(3);
	jug->setPuntos(0);
	strcpy(auxiliarArchivo.nombreU,jug->getNom().c_str());
	strcpy(auxiliarArchivo.password,jug->getPass().c_str());
	auxiliarArchivo.puntos=jug->getPuntos();
	auxiliarArchivo.vidas=jug->getVidas();
	auxiliarArchivo.id=jug->getId();
	archivo.seekp(0, ios::end);
    archivo.write((char*)&auxiliarArchivo, sizeof(Jugador));
    archivo.close();
	return jug;	
}
Jugadores *HistorialJugadores::iniciarSesion(){
	Jugador auxiliarArchivo;
	char ASCII, p[255];
	int newkey,indice=0;
	bool salida=false,salida2=false,usuarioLibre=false,contrasenaCorrecta=false;
	string auxNomb="", auxPass="";
	BITMAP *registro=create_bitmap(900, 600),*continuar=create_bitmap(900, 600);
	Jugadores *jug=new Jugadores ();
	fstream archivo;
	archivo.open("Elementos\\DatosJugadores.dat",ios::binary | ios::in | ios::out);//Abrimos el archivo existente
	if(!archivo)
		 archivo.open("Elementos\\DatosJugadores.dat",ios::binary|ios::out);//Lo crea
	registro=load_bitmap("Elementos\\FotoRegistroInicio.bmp", NULL);
	continuar=load_bitmap("Elementos\\FotoRegistro.bmp", NULL);

	blit(registro, screen, 0, 0, 0, 0, 900, 600);
	clear_keybuf();//Borramos el buffer de entrada del teclado
	strcpy(p,"                       ");
	while(!salida) {
		do {
			vline(screen, (indice*8)+335, 99, 107, AZUL);//Aparece el cursor para escribir
			if(keypressed()) {//Detectamos si se pulsa una tecla
				newkey = readkey();//Almacenamos el valor
				ASCII = newkey & 0xff;//Convertimos lo almacenado
				if(ASCII>=32 && ASCII <=126) {//Validamos el ingreso de solo caracteres permitidos
					if(indice<20-1) {//Validamos que el usuario no ingrese más caracteres de los permitidos
						p[indice]=ASCII;//Metemos el caracter ingresado al vector
						indice++;
						p[indice]='\0';//Asignamos el fin de línea
					}
					else
						p[indice+1]=' ';
				}
				else if(ASCII==8){//Está retrocediendo
					p[indice]=' ';//Se borra la posición actual
					if(indice!=0){
						indice--;//Cambiamos de índice
						p[indice]=' ';//Se borra la posición actual
					}
				}
				else if(ASCII==27){//El usuario aprieta Escape, hay que retornar
					jug->setID(-1);//Mandamos -1 para evitar que al dar escape, entre al motor del juego
					return jug;
				}
			}
			if(ASCII==13){//El usuario le dió ENTER
				//Verificamos si ese nombre ya lo está usando algún otro usuario
				if(!jug->verificarRepeticionesUsuario())//Usuario Inexistente
					textprintf_ex(screen, font, 335, 139, AZUL,-1, "¡Ups! nombre no existente, intente con uno válido");
					//textout(screen, font, "¡Ups! nombre no existente, intente con uno válido", 335, 139, AZUL);
				else{
					textprintf_ex(screen, font, 335, 139, AZUL,-1,  "                                                    ");
					//textout(screen, font, "                                                    ", 335, 139, AZUL);//Borramos la leyenda del "Ups"
					usuarioLibre=true;
				}
			}
			auxNomb=p;
			jug->setNombre(auxNomb);
			//textprintf_ex(screen, font, 335, 99, AZUL,-1, "%s",p);
			textout_ex(screen, font, p, 335, 99,AZUL, 0);
			//textout(screen, font, p, 335, 99, AZUL);
			memset(p,255,'\0');
		} while(!usuarioLibre);//Sale cuando se le da un enter y el usuario es válido	
		indice=0;
		clear_keybuf();
		strcpy(p,"                       ");
		do {
			vline(screen, indice*8+335, 210, 218, AZUL);
			if(keypressed()) {
				 newkey = readkey();
				 ASCII =newkey & 0xff;
				 if(ASCII==13){
				 	if(this->validarContrasena(jug)){//Contraseñacorrecta, puede salir
				 		salida2=true;
						contrasenaCorrecta=true;
					}
					else{
						textprintf_ex(screen, font, 335, 245, AZUL,-1, "¡Ups! Clave incorrecta, intente de nuevo");
						//textout(screen, font, "¡Ups! Clave incorrecta, intente de nuevo", 335, 245, AZUL);
					}

			 	 }
				 else{
				 	if(ASCII>=32 && ASCII <=126) {
						if(indice<25) {
							p[indice]=ASCII;
							indice++;
							p[indice]='\0';
						}
						else
							p[indice+1]=' ';
				 	}
					 else if(ASCII==8){//Está retrocediendo
						p[indice-1]=' ';//Se borra la posición actual
						if(indice!=0){
							indice--;//Cambiamos de índice
						}
					}
					else if(ASCII==27){//Apretó Escape, hay que retornar
						jug->setID(-1);//Mandamos -1 para evitar que al dar escape, entre al motor del juego
						return jug;
					}	
				}
			}
			auxPass=p;
			jug->setPassword(auxPass);
			//textprintf_ex(screen, font, 335, 210, AZUL,-1, "%s",p);
			textout_ex(screen, font, p, 335, 210,AZUL, 0);
			//textout(screen, font, p, 335, 210, AZUL);
			memset(p,255,'\0');
		} 
		while(salida2==false && !contrasenaCorrecta);
		jug->setPuntos(0);
		if(jug->getNom().size()!=0 && jug->getPass().size()!=0) {
			salida=true;
		}//Salimos del ciclo que controla la funciï¿½n
	}
    archivo.close();
	return jug;	
}
bool HistorialJugadores::validarContrasena(Jugadores *jug){
	fstream archivo;
    Jugador aux;
    archivo.open("Elementos\\DatosJugadores.dat", ios::binary | ios::in);
	if(!archivo)//No existe un archivo, por lo tanto no hay una repetició
		return false;
    archivo.seekg(0, ios::beg);
	while (!archivo.eof()) {
        archivo.read((char*)&aux, sizeof(Jugador));
        if (strcmp(aux.nombreU,jug->getNom().c_str())==0) {
        	if(strcmp(aux.password,jug->getPass().c_str())==0){
        			jug->setID(aux.id);
        			jug->setPuntos(aux.puntos);
        			jug->setNivel(aux.nivel);
        			jug->setVidas(aux.vidas);
					archivo.close();
					return true;
			}
		}
    }
    archivo.close();
    return false;
}
void HistorialJugadores::modificarInformacion(Jugadores jugadorCambio,int puntosExtra) {
    Jugador nuevosDatos;
    fstream archivo;
    int pos;
    archivo.open("Elementos\\DatosJugadores.dat", ios::binary | ios::in | ios::out);
    if (!archivo)
        return;
    archivo.seekg(0,ios::end);
	int tamano=archivo.tellg()/sizeof(Jugador);
    archivo.seekg(0,ios::beg);

    for(int i=0;i<tamano;i++){
       	archivo.read(reinterpret_cast<char*>(&nuevosDatos), sizeof(Jugador));
      	if(jugadorCambio.getId()==nuevosDatos.id){
      		cout<<"Nuevos datos trae "<<nuevosDatos.puntos<<" + "<<jugadorCambio.getPuntos()<<" + "<<puntosExtra<<endl;
            nuevosDatos.puntos+=jugadorCambio.getPuntos()+puntosExtra;
            nuevosDatos.vidas=jugadorCambio.getVidas();
            nuevosDatos.nivel=jugadorCambio.getNivel();
            nuevosDatos.id=jugadorCambio.getId();
            strcpy(nuevosDatos.nombreU,jugadorCambio.getNom().c_str());
            strcpy(nuevosDatos.password,jugadorCambio.getPass().c_str());
      		archivo.seekp( i * sizeof(Jugador) , ios::beg);//Estoy en el registro indicado
      		archivo.write(reinterpret_cast<char*>(&nuevosDatos),sizeof(Jugador));
      		break;
		}
	}
}
void HistorialJugadores::ganadores() {
	BITMAP *buffer=create_bitmap(900, 600);
    BITMAP *mejoresPuntuaciones=load_bitmap("Elementos\\MejoresPuntuaciones.bmp", NULL);
    BITMAP *regresar=load_bitmap("Elementos\\MejoresPuntuacionesRegreso.bmp", NULL);
    BITMAP *advertencia=load_bitmap("Elementos\\advertencia.bmp",NULL);
    bool salir=false;
    FONT *font1=load_font("Elementos\\letritas.pcx", NULL, NULL);
    fstream archivo;
    vector <Jugador> aux;
    Jugador lector;
    archivo.open("Elementos\\DatosJugadores.dat", ios::binary | ios::in);
	if(!archivo){//El archivo no se pudo abrir o no existe, por lo tanto no hay rank que mostrar
		do{
			blit(advertencia,screen,0,0,0,0,900,600);
		}while(!key[KEY_ESC]);
		return;
	}
	archivo.seekg(0,ios::end);
	int tamano=archivo.tellg()/sizeof(Jugador);
    archivo.seekg(0,ios::beg);

    for(int i=0;i<tamano;i++){
       	archivo.read((char*)&lector, sizeof(Jugador));
        aux.push_back(lector);
	}

    sort(aux.begin(), aux.end(), compare);
    while (!salir) {
        blit(buffer, screen, 0, 0, 0, 0, 900, 600);
        if(mouse_x>=17 && mouse_x<=400 && mouse_y>=495 && mouse_y<=581) {
            blit(regresar, buffer, 0, 0, 0, 0, 900, 600);        
            if(mouse_b & 1)
                salir=true;
        } 
		else 
            blit(mejoresPuntuaciones, buffer, 0, 0, 0, 0, 900, 600);
        
        if(aux.size()==1){
     	   	textprintf_ex(buffer, font1, 340, 350, makecol(253, 214, 94),-1, "%s",aux[0].nombreU);
        	textprintf_ex(buffer, font1, 340, 375, makecol(255, 255, 255),-1, "%i",aux[0].puntos);
		}
        else if(aux.size()==2){
          	textprintf_ex(buffer, font1, 405, 350, makecol(253, 214, 94),-1, "%s",aux[0].nombreU);
        	textprintf_ex(buffer, font1, 405, 375, makecol(255, 255, 255),-1, "%i",aux[0].puntos);
            textprintf_ex(buffer, font1, 187, 410, makecol(237, 127, 52),-1, "%s",aux[1].nombreU);
        	textprintf_ex(buffer, font1, 187, 435, makecol(255, 255, 255),-1, "%i",aux[1].puntos);
		}
		else if(aux.size()>=3){
            textprintf_ex(buffer, font1, 340, 350, makecol(253, 214, 94),-1, "%s",aux[0].nombreU);
        	textprintf_ex(buffer, font1, 340, 375, makecol(255, 255, 255),-1, "%i",aux[0].puntos);
            textprintf_ex(buffer, font1, 187, 410, makecol(237, 127, 52),-1, "%s",aux[1].nombreU);
        	textprintf_ex(buffer, font1, 187, 435, makecol(255, 255, 255),-1, "%i",aux[1].puntos);
        	textprintf_ex(buffer, font1, 540, 400, makecol(181, 185, 196),-1, "%s",aux[2].nombreU);
        	textprintf_ex(buffer, font1, 540, 425, makecol(255, 255, 255),-1, "%i",aux[2].puntos);
		}
    }
    destroy_bitmap(buffer);
}
bool compare(Jugador& x, Jugador& y) {
	//Funcion auxiliar de comparacion de puntajes
    return x.puntos > y.puntos;
}
