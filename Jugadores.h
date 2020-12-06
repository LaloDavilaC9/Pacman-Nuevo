#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

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

bool compare(Jugadores& , Jugadores& );
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
    archivo.seekg(0, ios::end);
	if(archivo.tellg()==0) //Indica que el archivo está vacio
		return false;	//Por tanto, no habrá repetición de usuario
	while (!archivo.eof()) {
        archivo.read((char*)&aux, sizeof(aux));
        if (aux.nombreU == this->nombreU) {
			archivo.close();
			return true;
		}
    }
    archivo.close();
    return false;
}
//Miembros de la clase HistorialJugadores
Jugadores * HistorialJugadores::registroEnArchivo() {
	Jugadores jug;
    fstream archivo;
    archivo.open("DatosJugadores.dat", ios::binary | ios::app);
    if (!archivo) {
        cout << "No es posible ingresar datos." << endl;
        cout << "Hay un error en el archivo de almacenamiento" << endl;
        return jug;
    }
    
	//Valida un registro de usuarios de buena manera
	//Usuario usuario;
	char ASCII;
	int newkey,indice=0;
	BITMAP *registro=create_bitmap(900, 600);
	registro=load_bitmap("FotoRegistroInicio.bmp", NULL);
	BITMAP *continuar=create_bitmap(900, 600);
	continuar=load_bitmap("FotoRegistro.bmp", NULL);
	bool salida=false,salida2=false,usuarioLibre=false;
	string auxNomb="", auxPass="";
	blit(registro, screen, 0, 0, 0, 0, 900, 600);
	clear_keybuf();//Borramos el buffer de entrada del teclado
	while(!salida) {
		do {
			vline(screen, (indice*8)+335, 99, 107, AZUL);//Aparece el cursor para escribir
			if(keypressed()) {//Detectamos si se pulsa una tecla
				 newkey = readkey();//Almacenamos el valor
				 ASCII = newkey & 0xff;//Convertimos lo almacenado
				if(ASCII>=32 && ASCII <=126) {//Validamos el ingreso de solo caracteres permitidos
					if(indice<20-1) {//Validamos que el usuario no ingrese m�s caracteres de los permitidos
						auxNomb[indice]=ASCII;//Metemos el caracter ingresado al vector
						indice++;
						auxNomb[indice]='\0';//Asignamos el fin de l�nea
					}
				}
				else if(ASCII==8){//Est� retrocediendo
					auxNomb[indice]=' ';//Se borra la posici�n actual
					if(indice!=0){
						indice--;//Cambiamos de �ndice
						auxNomb[indice]=' ';//Se borra la posici�n actual
					}
				}
				else if(ASCII==27){//Apret� Escape, hay que retornar
					return 1;
				}
			}
			if(ASCII==13){//Verificamos si ya est� completo el nombre de usuario
				if(jug.verificarRepeticionesUsuario()/*validarUsuario(registroUsuarios,usuario.nombreUsuario)==1*/){//El usuario est� repetido, hay que indicarle al usuario
					textout(screen, font, "�Ups! nombre ya existente, intente con uno diferente", 335, 139, AZUL);
				}	
				else{
					textout(screen, font, "                                                    ", 335, 139, AZUL);//Borramos la leyenda del "Ups"
					usuarioLibre=true;
				}
			}
			jug.setNombre(auxNomb);
			textout(screen, font, auxNomb, 335, 99, AZUL);
		} while(usuarioLibre==false);//Sale cuando se le da un enter y el usuario es v�lido	
		indice=0;
		clear_keybuf();
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
							auxPass[indice]=ASCII;
							indice++;
							auxPass[indice]='\0';
						}
				 	}
					 else if(ASCII==8){//Est� retrocediendo
						auxPass[indice-1]=' ';//Se borra la posici�n actual
						if(indice!=0){
							indice--;//Cambiamos de �ndice
						}
					}
					else if(ASCII==27){//Apret� Escape, hay que retornar
						return 1;
					}	
				}
			}
			jug.setPassword(auxPass);
			textout(screen, font, auxPass, 335, 210, AZUL);
		} 
		while(salida2==false);
		
		//*pIdentificacion=usuario.id=secuenciaId(registroUsuarios);//Verifica cu�l fue el �ltimo ID 
		//*pVidas=3;//Inicializamos al usuario con 3 vidas
		
		/*FILE registroUsuarios = fopen("Usuarios.dat","rb+");
		if(!registroUsuarios){
			registroUsuarios=fopen("Usuarios.dat","a+");
		}
		fseek(registroUsuarios, 0, SEEK_END);//Colocamos el puntero en su �ltima posici�n
		fwrite(&usuario,sizeof(Usuario),1,registroUsuarios);
		rewind(registroUsuarios);
		fread(&usuario,sizeof(Usuario),1,registroUsuarios);
		do{
			fread(&usuario,sizeof(Usuario),1,registroUsuarios);
		}while(!(feof(registroUsuarios)));
		fclose(registroUsuarios);//Se recorre el archivo por precauci�n*/
		
		if(jug.getNom()!=" " && jug.getPass()!=" "/*strcmp(usuario.nombreUsuario, "NULL")!=0 && strcmp(usuario.contrasena, "NULL")!=0*/) {
			salida=true;
		}//Salimos del ciclo que controla la funci�n
	}
	//Aqui asignamos el numero de usuario del jugador
	//Los id comienzan en 1
	unsigned int canReg;
	archivo.seekg(0, ios::end);
    canReg=archivo.tellg()/sizeof(jug);
    archivo.seekg(ios::beg);
	jug.setID(canReg+1);
    archivo.write((char*)&jug, sizeof(jug));
    archivo.close();
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
std::vector <Jugadores> HistorialJugadores::Ganadores() {
    std::fstream archivo;
    std::vector <Jugadores> aux, ganadores;
    Jugadores lector;
    archivo.open("DatosJugadores.dat", ios::binary | ios::in);
    while (!archivo.eof()) {
        archivo.read((char*)&lector, sizeof(lector));
        aux.push_back(lector);
    }
    sort(begin(aux), end(aux), compare);
    for (int i = 0; i < 3; i++) {
        ganadores.push_back(aux[i]);
    }
    return ganadores;
}

//Funcion auxiliar de comparacion de puntajes
bool compare(Jugadores& x, Jugadores& y) {
    return x.verPuntos() > y.verPuntos();
}
