#include <iostream>
#include <allegro.h>
#include "Mapa.hpp"

#define AZUL makecol(51, 153, 255)//Color predefinido
#define NEGRO makecol(0, 0, 0)//Color predefido
using namespace std;
struct Guardado{//Esta estructura es la encargada de manejar los puntos, el nivel y las vidas de los usuarios para guardarlos en un archivo binario.
	int puntos=0;
	int nivel=1;
	int vidas=3;
};

struct Usuario{//Esta estructura se liga con la estructura de Guardado lo que nos permite tener toda la información del usuario en dos estructuras
	int id;
	char nombreUsuario[20];
	char contrasena[25];
	Guardado info;
};

void init() {
	//Configuraciones iniciales de Alegro
	MIDI *musica1;  
	int depth, res,cancion;
	allegro_init();
	depth = desktop_color_depth();
	if (depth == 0) 
		depth = 32;
	set_color_depth(depth);
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 900, 635, 0, 0);
	if (res != 0) {
		allegro_message(allegro_error);
		exit(-1);
	}
	set_window_title("Pac_Man Game");

	install_timer();
	install_keyboard();
	install_mouse();
	show_mouse(screen);
	
	//Instalación del sonido
	if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0) {
       allegro_message("Error: inicializando sistema de sonido\n%s\n", allegro_error);
       return;
    }
	set_volume(55, 55);
	//Elección de la canción del menú
	srand(time(NULL));
	cancion=rand()%8+1;
	switch(cancion){
		case 1:
			musica1=load_midi("Elementos\\sonora de margarita - que bello.mid");
			break;
		case 2:
			musica1 = load_midi("Elementos\\El_rey_leon_(Hakuna_matata).mid");
			break;
		case 3:
			musica1  = load_midi("Elementos\\Harry_Potter.mid");
			break;
		case 4:
			musica1  = load_midi("Elementos\\angeles_azules_-_amigos_nada_mas.mid");	
			break;
		case 5:
			musica1  = load_midi("Elementos\\Cazafantasmas.mid");	
			break;
		case 6:
			musica1  = load_midi("Elementos\\rata_de_2_patas.mid");
			break;
		case 7:
			musica1  = load_midi("Elementos\\luis miguel - culpable o no.mid");
			break;
		case 8:
			musica1  = load_midi("Elementos\\jefe_de_jefes.mid");
			break;
	}
	play_midi(musica1,1);//Reproducción de la canción del menú
}

void deinit() {
	clear_keybuf();
}

int Portada() {
	int opcion;
	bool salida=false;
	BITMAP *buffer;
	buffer=create_bitmap(960, 620); //Creación del lienzo
	//Carga de las imágenes necesarias para la portada
	BITMAP *Portada=load_bitmap("Elementos\\Portada.bmp", NULL);
	BITMAP *Portada1=load_bitmap("Elementos\\Portada1.bmp", NULL);
	BITMAP *Portada2=load_bitmap("Elementos\\Portada2.bmp", NULL);
	BITMAP *Portada3=load_bitmap("Elementos\\Portada3.bmp", NULL);
	BITMAP *Portada4=load_bitmap("Elementos\\Portada4.bmp", NULL);

	while(!salida) {
		blit(buffer, screen, 0, 0, 0, 0, 960, 620);
		//Verificación de la ubicaación del Mouse en pantalla
		if(mouse_x>=295 && mouse_x<=599 && mouse_y>=265 && mouse_y<=335) {
			blit(Portada1, buffer, 0, 0, 0, 0, 960, 620);		
			if(mouse_b & 1) {
				salida=true;
				opcion=1;
			}
		} else if(mouse_x>=195 && mouse_x<=717 && mouse_y>=360 && mouse_y<=429) {
			blit(Portada2, buffer, 0, 0, 0, 0, 960, 620);		
			if(mouse_b & 1) {
				salida=true;
				opcion=2;
			}
		} else if(mouse_x>=373 && mouse_x<=527 && mouse_y>=452 && mouse_y<=517) {
			blit(Portada3, buffer, 0, 0, 0, 0, 960, 620);		
			if(mouse_b & 1) {
				salida=true;
				opcion=3;
			}
		} else if(mouse_x>=388 && mouse_x<=503 && mouse_y>=541 && mouse_y<=593) {
			blit(Portada4, buffer, 0, 0, 0, 0, 960, 620);		
			if(mouse_b & 1) {
				salida=true;
				opcion=4;
			}
		} else {
			blit(Portada, buffer, 0, 0, 0, 0, 960, 620);
		}
	}
	clear(buffer);
	destroy_bitmap(buffer);
	//Retornamos la opción que eligió el usuario
	return opcion;
}

int MiniPortada() {
	int Opcion1;
	bool salida=false;
	BITMAP *buffer;
	buffer=create_bitmap(960, 620);
	//Carga de las imágenes necesarias para la miniportada
	BITMAP *MiniPortada=load_bitmap("Elementos\\MiniPortada.bmp", NULL);
	BITMAP *MiniPortada1=load_bitmap("Elementos\\MiniPortada1.bmp", NULL);
	BITMAP *MiniPortada2=load_bitmap("Elementos\\MiniPortada2.bmp", NULL);
	BITMAP *MiniPortada3=load_bitmap("Elementos\\MiniPortada3.bmp", NULL);
	while(!salida) {
		blit(buffer, screen, 0, 0, 0, 0, 960, 620);
		//Verificación de la posición del Mouse en pantalla
		if(mouse_x>=301 && mouse_x<=608 && mouse_y>=63 && mouse_y<=131) {
			blit(MiniPortada1, buffer, 0, 0, 0, 0, 960, 620);		
			if(mouse_b & 1) {
				salida=true;
				Opcion1=1;
			}
		} else if(mouse_x>=264 && mouse_x<=654 && mouse_y>=162 && mouse_y<=236) {
			blit(MiniPortada2, buffer, 0, 0, 0, 0, 960, 620);		
			if(mouse_b & 1) {
				salida=true;
				Opcion1=2;
			}
		} else if(mouse_x>=17 && mouse_x<=233 && mouse_y>=459 && mouse_y<=534) {
			blit(MiniPortada3, buffer, 0, 0, 0, 0, 960, 620);		
			if(mouse_b & 1) {
				salida=true;
				Opcion1=3;
			}
		} else {
			blit(MiniPortada, buffer, 0, 0, 0, 0, 960, 620);
		}
	}
	clear(buffer);
	destroy_bitmap(buffer);
	return Opcion1;
}

void menu(int Opcion) {
	Jugadores *jugador;
	HistorialJugadores registro;
	Mapa mapa;
	fstream registroUsuario;
	Usuario usuario;

	BITMAP *ayuda=load_bitmap("Elementos\\ayuda.bmp",NULL);
	char **nombresRank,ASCII;
	int op=0;
	bool salir=false;
	nombresRank=new char *[3];
	for(int j=0;j<3;j++){
		nombresRank[j]=new char [20];
	}
	int identificacion,*pIdentificacion,nivel,*pNivel,vidas,*pVidas;
	pIdentificacion=&identificacion;pNivel=&nivel;pVidas=&vidas;
	*pNivel=1;
	int *vecRank;
	vecRank=new int [3];
	BITMAP *advertencia=load_bitmap("Elementos\\advertencia.bmp",NULL);
	enum {Jugar=1, MejoresPuntuaciones, Ayuda, Salir};
	enum {NuevoJuego=1, ContinuarJuego, RegresarAlMenuPrincipal};
	int OpcionJuego;
	switch(Opcion) {
		do {
			case Jugar:
				do {
					OpcionJuego=MiniPortada();
					switch (OpcionJuego) {
						case NuevoJuego:
							jugador=registro.registroEnArchivo();//Un nuevo usuario fue registrado en el archivo
							if(jugador->getNom().size()!=0){
								mapa.motorJuego(jugador);
								//cout<<"HOLA"<<endl;
								//motorJuego(registroUsuario,pIdentificacion,pNivel,pVidas,vectorPintarMapa,vectorMotorJuego);	
							}
							break;
						case ContinuarJuego:
							//op=continuarProgreso(registroUsuario,pIdentificacion,pNivel,pVidas);
							if(op!=1){
								//motorJuego(registroUsuario,pIdentificacion,pNivel,pVidas,vectorPintarMapa,vectorMotorJuego);
							}
							break;
					}
				} while (OpcionJuego!=RegresarAlMenuPrincipal);
				break;
			case MejoresPuntuaciones:
				registro.ganadores();
				break;
			case Ayuda:
					do{
						blit(ayuda,screen,0,0,0,0,900,600);
					}while(!key[KEY_ESC]);
				break;
		}while (Opcion!=Salir);
	}
}

