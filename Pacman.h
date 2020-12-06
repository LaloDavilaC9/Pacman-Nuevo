#pragma once

class Pacman {
	private:
		int posX, posY, direccion;
		bool poder;
	public:
		Pacman();
		void movimiento();
		bool movimientoValido(int *[]);
		bool tienePoder(int *[]);
		bool getPoder(){return this->poder;}
		void setPoder(bool p){this->poder=p;}
};

Pacman::Pacman() {
	this->posX = 0;
	this->posY = 0;
	this->direccion = 0;
	this->poder=false;
}
void Pacman::movimiento() {

}
bool Pacman::movimientoValido(int* matrizJuego[]) {
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
bool Pacman::tienePoder(int *matrizJuego[]) {
	return (matrizJuego[posX][posY]==6);
}
