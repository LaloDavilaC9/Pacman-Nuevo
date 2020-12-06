#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <allegro.h>
#include <algorithm>
#define AZUL makecol(51, 153, 255)//Color predefinido
#define NEGRO makecol(0, 0, 0)//Color predefido
using namespace std;
struct Jugador{
	unsigned int vidas, puntos, id;
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
    char *convertir(string var){
    	char x[25];
    	for(int i=0;i<var.size();i++){
    		x[i]=var[i];
		}
		return x;
	}
};

class HistorialJugadores {
    vector <Jugadores> tRegistros;
public:
    HistorialJugadores() {};
    Jugadores *  registroEnArchivo();
    void modificarInformacion(Jugadores,int);
    void ganadores();
};

bool compare(Jugador & x, Jugador& y);

//Miembos de la clase Jugadores
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
//Miembros de la clase HistorialJugadores
Jugadores *HistorialJugadores::registroEnArchivo(){
	Jugador auxiliarArchivo;
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
					return jug;
				}
			}
			if(ASCII==13){//El usuario le dió ENTER
				//Verificamos si ese nombre ya lo está usando algún otro usuario
				if(jug->verificarRepeticionesUsuario())//Usuario repetido
					textout(screen, font, "¡Ups! nombre ya existente, intente con uno diferente", 335, 139, AZUL);
				else{
					textout(screen, font, "                                                    ", 335, 139, AZUL);//Borramos la leyenda del "Ups"
					usuarioLibre=true;
				}
			}
			auxNomb=p;
			jug->setNombre(auxNomb);
			textout(screen, font, p, 335, 99, AZUL);
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
						return jug;
					}	
				}
			}
			auxPass=p;
			jug->setPassword(auxPass);
			textout(screen, font, p, 335, 210, AZUL);
			memset(p,255,'\0');
		} 
		while(salida2==false);
		
		//*pIdentificacion=usuario.id=secuenciaId(registroUsuarios);//Verifica cuál fue el último ID 
		//*pVidas=3;//Inicializamos al usuario con 3 vidas

		if(jug->getNom().size()!=0 && jug->getPass().size()!=0) {
			salida=true;
		}//Salimos del ciclo que controla la funciï¿½n
	}

	unsigned int canReg;
	archivo.seekp(0, ios::end);
    canReg=archivo.tellg()/sizeof(Jugador);
	jug->setID(canReg+1);
	jug->setVidas(3);
	jug->setPuntos(0);
	strcpy(auxiliarArchivo.nombreU,jug->getNom().c_str());
	strcpy(auxiliarArchivo.password,jug->getPass().c_str());
	auxiliarArchivo.puntos=jug->getPuntos();
	auxiliarArchivo.vidas=jug->getVidas();
	archivo.seekp(0, ios::end);
    archivo.write((char*)&auxiliarArchivo, sizeof(Jugador));
    archivo.close();
	return jug;	
}
void HistorialJugadores::modificarInformacion(Jugadores jugadorCambio,int puntosExtra) {
    Jugadores nuevosDatos;
    fstream archivo;
    int pos;
    archivo.open("DatosJugadores.dat", ios::binary | ios::in | ios::out);
    if (!archivo) {
        //cout << "Hay un error en el archivo de almacenamiento" << endl;
        return;
    }
    while (!archivo.eof()) {
        pos = archivo.tellg();
        archivo.read((char*)&nuevosDatos, sizeof(nuevosDatos));
        if (archivo) {
            if (jugadorCambio.getId() == nuevosDatos.getId()) {
                archivo.seekg(pos);
                nuevosDatos.setPuntos(jugadorCambio.getPuntos()+puntosExtra);
                nuevosDatos.setVidas(jugadorCambio.getVidas());
                archivo.write((char*)&nuevosDatos, sizeof(nuevosDatos));
                break;
            }
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
	}
    archivo.seekg(0,ios::beg);
    while (!archivo.eof()) {
    	//archivo.read(reinterpret_cast<char*>(&lector), sizeof(Jugadores));
       	archivo.read((char*)&lector, sizeof(Jugador));
       	//cout<<"ID "<<lecto<<" Nombre: "<<lector.getNom()<<" puntos: "<<lector.getPuntos()<<endl;
        aux.push_back(lector);
	}
    sort(aux.begin(), aux.end(), compare);
    //cout<<"Entro y se refiere a  "<<aux[0].getNom()<<" con puntos de "<<aux[0].getPuntos();
    while (!salir) {
        blit(buffer, screen, 0, 0, 0, 0, 900, 600);
        if(mouse_x>=17 && mouse_x<=257 && mouse_y>=495 && mouse_y<=581) {
            blit(regresar, buffer, 0, 0, 0, 0, 900, 600);        
            if(mouse_b & 1) {
                salir=true;
            }
        } else {
            blit(mejoresPuntuaciones, buffer, 0, 0, 0, 0, 900, 600);
        }
        
        if(aux.size()==1){
          	textprintf(buffer, font1, 308, 200, makecol(255, 0, 0), "%s", aux[0].nombreU);
            textprintf(buffer, font1, 520, 200, makecol(255, 0, 0), "%i", aux[0].puntos);
            
		}
        else if(aux.size()==2){
            textprintf(buffer, font1, 308, 200, makecol(255, 0, 0), "%s", aux[0].nombreU);
           	textprintf(buffer, font1, 520, 200, makecol(255, 0, 0), "%i", aux[0].puntos);
           	textprintf(buffer, font1, 308, 240, makecol(0, 255, 0), "%s", aux[1].nombreU);
            textprintf(buffer, font1, 520, 240, makecol(0, 255, 0), "%i", aux[1].puntos);
		}
		else if(aux.size()>=3){
			textprintf(buffer, font1, 308, 200, makecol(255, 0, 0), "%s", aux[0].nombreU);
           	textprintf(buffer, font1, 520, 200, makecol(255, 0, 0), "%i", aux[0].puntos);
           	textprintf(buffer, font1, 308, 240, makecol(0, 255, 0), "%s", aux[1].nombreU);
            textprintf(buffer, font1, 520, 240, makecol(0, 255, 0), "%i", aux[1].puntos);
           	textprintf(buffer, font1, 308, 280, makecol(0, 0, 255), "%s", aux[2].nombreU);
       	 	textprintf(buffer, font1, 520, 280, makecol(0, 0, 255), "%i", aux[2].puntos);
		}
    }
    destroy_bitmap(buffer);
}

//Funcion auxiliar de comparacion de puntajes
bool compare(Jugador& x, Jugador& y) {
    return x.puntos > y.puntos;
}
