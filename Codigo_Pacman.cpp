# include <iostream>
# include <string>
# include <algorithm>
# include <vector>
# include <fstream>
# include <ctime>
# include "Fantasmas.h"
# include "Impresiones.h"
# include "Jugadores.h"
# include "PacMan.h"
# include <allegro5/allegro.h>
# include <allegro5/allegro_primitives.h>

using namespace std;

int main(void) {
    if (!al_init()) {
        cout << "No es posible iniciar allegro" << endl;
        return -1;
    }
    ALLEGRO_DISPLAY* Pantalla = NULL;
    ALLEGRO_EVENT_QUEUE* eventos = NULL;
    ALLEGRO_BITMAP* portada = NULL;

    Pantalla = al_create_display(960, 660);
    if (!Pantalla) {
        cout << "No es posible cargar la ventana de inicio" << endl;
        return -1;
    }
   
    al_install_keyboard();
    al_init_primitives_addon();

    portada = al_create_bitmap(960, 660);
}

