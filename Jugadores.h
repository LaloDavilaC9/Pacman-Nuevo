#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Jugadores {
private:
    unsigned int vidas, puntos, id;
    std::string nombreU, password;
public:
    Jugadores();
    void setDatos();
    bool verificarRepeticionesUsuario();
    unsigned int verID() { return id; }
    void setVidas(unsigned int nVidas) { vidas = nVidas; }
    void setPuntos(unsigned int score) { puntos = score; }
    unsigned int verPuntos() { return puntos; };
};

class HistorialJugadores {
    vector <Jugadores> tRegistros;
public:
    HistorialJugadores() {};
    void registroEnArchivo(Jugadores);
    void modificarInformacion(unsigned int, unsigned int, unsigned int);
    vector <Jugadores> Ganadores();
};
//Miembos de la clase Jugadores
Jugadores::Jugadores() {
    vidas = 3;
    puntos = 0;
    id = 0;
    nombreU = "";
    password = "";
}
void Jugadores::setDatos() {
CambioNombre:
    cout << "Nombre de usuario: ";
    fflush(stdin);
    getline(cin, this->nombreU);
    if (!verificarRepeticionesUsuario()) {
        cout << "Nombre de usuario NO disponible" << endl;
        cout << "Ingrese otro nombre de usuario" << endl;
        goto CambioNombre;
    }
    cout << "Contrase" << char(164) << "a: ";
    fflush(stdin);
    getline(cin, password);

}
bool Jugadores::verificarRepeticionesUsuario() {
    fstream archivo;
    Jugadores aux;
    archivo.open("DatosJugadores.bin", ios::binary | ios::in);
    while (!archivo.eof()) {
        archivo.read((char*)&aux, sizeof(aux));
        if (aux.nombreU == nombreU) return true;
    }
    archivo.close();
    return false;
}
//Funcion auxiliar de comparacion de puntajes
bool compare(Jugadores& x, Jugadores& y) {
    return x.verPuntos() > y.verPuntos();
}
//Miembros de la clase HistorialJugadores
void HistorialJugadores::registroEnArchivo(Jugadores jugActual) {
    fstream archivo;
    archivo.open("DatosJugadores.bin", ios::binary | ios::app);
    if (!archivo) {
        cout << "No es posible ingresar datos." << endl;
        cout << "Hay un error en el archivo de almacenamiento" << endl;
        return;
    }
    archivo.write((char*)&jugActual, sizeof(jugActual));
    archivo.close();
}
void HistorialJugadores::modificarInformacion(unsigned int id, unsigned int nVidas, unsigned int nPuntos) {
    Jugadores nuevosDatos;
    std::fstream archivo;
    int pos;
    archivo.open("DatosJugadores.bin", ios::binary | ios::in | ios::out);
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
    archivo.open("DatosJugadores.bin", ios::binary | ios::in);
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

