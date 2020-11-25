#pragma once

class Pacman {
public:
	int posX, posY, direccion;
private:
	Pacman();
	void movimiento();
	bool movimientoValido(int *[]);
	bool tienePoder(int *[]);
};

Pacman::Pacman() {
	this->posX = 0;
	this->posY = 0;
	this->direccion = 0;
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