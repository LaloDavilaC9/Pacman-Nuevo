#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <allegro.h>
#include <algorithm>
#define AZUL makecol(51, 153, 255)//Color predefinido
#define NEGRO makecol(0, 0, 0)//Color predefido
using namespace std;
class Jugadores {
private:
    unsigned int vidas, puntos, id;
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
	string getNom() {return nombreU;}
	string getPass() {return password;}
    unsigned int verPuntos() { return puntos; };
};

class HistorialJugadores {
    vector <Jugadores> tRegistros;
public:
    HistorialJugadores() {};
    Jugadores *  registroEnArchivo();
    void modificarInformacion(unsigned int, unsigned int, unsigned int);
    vector <Jugadores> Ganadores();
};

bool compare(Jugadores& x, Jugadores& y);
//Miembos de la clase Jugadores
Jugadores::Jugadores() {
    this->vidas = 3;
    this->puntos = 0;
   	this->id = 0;
    this->nombreU = " ";
    this->password = " ";
}
bool Jugadores::verificarRepeticionesUsuario() {
    fstream archivo;
    Jugadores aux;
    archivo.open("DatosJugadores.dat", ios::binary | ios::in);
	if(!archivo)//No existe un archivo, por lo tanto no hay una repetició
		return false;
    archivo.seekg(0, ios::beg);
	while (!archivo.eof()) {
        archivo.read((char*)&aux, sizeof(Jugadores));
        if (aux.nombreU == this->nombreU) {
			archivo.close();
			return true;
		}
    }
    archivo.close();
    return false;
}
//Miembros de la clase HistorialJugadores
Jugadores *HistorialJugadores::registroEnArchivo(){
	char ASCII, p[255];
	int newkey,indice=0;
	bool salida=false,salida2=false,usuarioLibre=false;
	string auxNomb="", auxPass="";
	BITMAP *registro=create_bitmap(900, 600),*continuar=create_bitmap(900, 600);
	Jugadores *jug=new Jugadores ();
	fstream archivo;
	archivo.open("Elementos\\DatosJugadores.dat",ios::binary | ios::in | ios::out);//Abrimos el archivo existente
	if(!archivo)//El archivo no existe, hay que crearlo
		archivo.open("Elementos\\DatosJugadores.dat",ios::binary|ios::out);//Lo crea
	registro=load_bitmap("Elementos\\FotoRegistroInicio.bmp", NULL);
	continuar=load_bitmap("Elementos\\FotoRegistro.bmp", NULL);

	blit(registro, screen, 0, 0, 0, 0, 900, 600);
	clear_keybuf();//Borramos el buffer de entrada del teclado
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
						if(indice<25-1) {
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
	archivo.seekg(0, ios::end);
    canReg=archivo.tellg()/sizeof(jug);
    archivo.seekg(ios::beg);
	jug->setID(canReg+1);
	jug->setVidas(3);
    archivo.write((char*)&jug, sizeof(jug));
    archivo.close();
	return jug;	
}
void HistorialJugadores::modificarInformacion(unsigned int id, unsigned int nVidas, unsigned int nPuntos) {
    Jugadores nuevosDatos;
    std::fstream archivo;
    int pos;
    archivo.open("DatosJugadores.dat", ios::binary | ios::in | ios::out);
    if (!archivo) {
        cout << "Hay un error en el archivo de almacenamiento" << endl;
        return;
    }
    while (!archivo.eof()) {
        pos = archivo.tellg();
        archivo.read((char*)&nuevosDatos, sizeof(nuevosDatos));
        if (archivo) {
            if (id == nuevosDatos.verID()) {
                archivo.seekg(pos);
                nuevosDatos.setPuntos(nPuntos);
                nuevosDatos.setVidas(nVidas);
                archivo.write((char*)&nuevosDatos, sizeof(nuevosDatos));
                break;
            }
        }
    }
}
vector <Jugadores> HistorialJugadores::Ganadores() {
    std::fstream archivo;
    std::vector <Jugadores> aux, ganadores;
    Jugadores lector;
    archivo.open("DatosJugadores.dat", ios::binary | ios::in);
    while (!archivo.eof()) {
        archivo.read((char*)&lector, sizeof(lector));
        aux.push_back(lector);
    }
    sort(aux.begin(), aux.end(), compare);
    for (int i = 0; i < 3; i++) {
        ganadores.push_back(aux[i]);
    }
    return ganadores;
}

//Funcion auxiliar de comparacion de puntajes
bool compare(Jugadores& x, Jugadores& y) {
    return x.verPuntos() > y.verPuntos();
}
