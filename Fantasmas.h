#pragma once

class Fantasmas {
public:
    int posX, posY, direccion;
    bool comibles;
    Fantasmas() {
        posX = 0;
        posY = 0;
        direccion = 0;
        comibles = false;
    }
    virtual void movimientoNormal() = 0;
    virtual void movimientoPersecucion() = 0;
    virtual void movimientoHuida() = 0;
    int* posiciones();
    bool matarPacman(int*, bool);
    void regeneracion();
    bool movimientoValido(int*[]);
};
class Blinky : public Fantasmas {
public:
    void movimientoNormal();
    void movimientoPersecucion();
    void movimientoHuida();
};
class Clyde : public Fantasmas {
public:
    void movimientoNormal();
    void movimientoPersecucion();
};
class Inky : public Fantasmas {
public:
    void movimientoNormal();
    void movimientoPersecucion();
};
class Pinky : public Fantasmas {
public:
    void movimientoNormal();
    void movimientoPersecucion();
};
//Implementacion de los metodos de la clase Fantasmas
int* Fantasmas::posiciones() {
    int* posXY = new int[2];
    posXY[0] = posX;
    posXY[1] = posY;
    return posXY;
}
bool Fantasmas::matarPacman(int* posXY_pacman, bool poderPacman) {
    return (posX == posXY_pacman[0] && posY == posXY_pacman[1] && !poderPacman);
}
bool Fantasmas::movimientoValido(int *matrizJuego[]) {
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
void Fantasmas::regeneracion() {
    comibles = false;
}
//Implementacion de los metodos de la clase Blinky
void Blinky::movimientoNormal() {

}
void Blinky::movimientoPersecucion() {

}
//Implementacion de los metodos de la clase Clyde
void Clyde::movimientoNormal() {

}
void Clyde::movimientoPersecucion() {

}
//Implementacion de los metodos de la clase Inky
void Inky::movimientoNormal() {

}
void Inky::movimientoPersecucion() {

}
//Implementacion de los metodos de la clase Pinky
void Pinky::movimientoNormal() {

}
void Pinky::movimientoPersecucion() {

}
