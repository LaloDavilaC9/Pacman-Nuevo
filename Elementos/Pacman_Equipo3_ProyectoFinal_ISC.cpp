/*
		
	2° A I.S.C UAA en Programción I									 5 de julio de 2020	 
										Integrantes del Equipo 3 
	Autores del código
			Dávila Campos Eduardo
			Delgado Luna Víctor Emilio
			Gaytan Sánchez Brayan de Jesús
						
	Descripción general del código: Nuestro proyecto "Pacman" es un juego que consiste en utilizar a un personaje redondo cuyo objetivo es comer la mayor cantidad de puntos
	en diversos mapas, sin ser comido por los fantasmas que sirven de obstáculos en el juego. 
	En nuestra versión del clásico juego, para pasar de nivel hay que comer todos los puntos disponibles en el mapa sin ser asesinado. Contamos con 3 vidas y un archivo binario
	que registra nuestra información para poder cargar desde el nivel en el que nos quedamos. Ojo, sólo se carga el nivel completo, no los puntos que ha comido. En cada cambio de 
	nivel hay un guardado automático. 
	Cada función está comentada por dentro.

	En nuestra matriz Juego los valores se tomaran como los siguientes: 0 = Pacman  1=Margen y bloques 2=Paso libre 3=Indefinido (antes usado como casa de fantasmas)
	4=Frutas 5=P.chicos 6=P.grandes 7= Fantasma1 8= Fantasma2 9=Fantasma3  10=Fantasma4 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <allegro.h>
#include <iostream>
//Importante: No hacer uso de la librería Windows.h porque genera errores con Allegro.h

/*La constante VELOCIDAD se puede modificar a gusto de la maestra para que la velocidad del juego se dé más o menos rápido. 
Entre más grande el número de la constante, más lento el juego. La recomendación es poner la velocidad en ->220<-, con incluso alguna saturación de audio.
Al jugar y presionar ESCAPE y apretar la tecla S, el sonido del movimiento del Pacman se desactivará 
Subir o disminuir la velocidad trae cambios al sonido del movimiento del pacman
La constante PUNTOS se puede modificar a gusto de la maestra para que tarde menos en pasar un nivel y pueda verificar todos los niveles disponibles. 
OJO Entre más grande sea el valor, menos puntos debe de comer para avanzar al siguiente nivel, y mientras más pequeño sea el valor, más puntos debe de comer para avanzar de nivel.
La constante NIVELMAXIMO se puede moficar a gusto de la maestra para que gane el juego en un nivel menor a diez. Ejemplo: Si pone NIVELMAXIMO = 5, al terminar el nivel 4, estará 
ganando el juego.
*/
#define NIVELMAXIMO 10 //Importante no usar un valor menor a 2 ni mayor a 10
#define PUNTOS 0 //Importante no usar un valor menor a 0 ni mayor a 270
#define VELOCIDAD 50 //Importante no usar valores negativos
#define COL 30 //Importante no modificar ->30<-
#define FIL 20 //Importante no modificar ->20<-
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

//Prototipos de las funciones
void motorJuego(FILE *registroUsuarios,int *pIdentificacion,int *pNivel,int *pVidas,BITMAP *vectorPintarMapa[],BITMAP *vectorMotorJuego[]);
void generacionMapa(int numNivel,int matrizJuego[][COL],int *pDificultad);
void OcultaCursor();
void pintarMapa(int matrizJuego[][COL],BITMAP *buffer,int *pDir,int *pcomida,int *pDirF1,int *pDirF2,int *pDirF3,int *pDirF4,int *pFrutas,int numNivel,int pPausaF[],int dificultad,BITMAP *vectorPintarMapa[]);
void puntaje(int matrizJuego[][COL],int posI,int posJ,int *pcomida,int *puntuacionTotal,int *pCambioNivel,FILE *registroUsuarios,int *pIdentificacion,int *pFrutas,int *pVidas,int dificultad);
void queCome(int matrizJuego[][COL],int *pcomida,int posI,int posJ,int dificultad);
void movimientoPacman(int matrizJuego[][COL],int vecPos[],int *pvez,int *pcomida,int *pVidas,int *puntuacionTotal,int *pCambioNivel,int *pDir,FILE *registroUsuarios, int *pIdentificacion,int *pFrutas,int *pQuien,int pPausaF[],int dificultad,int sonidoActivo);
void dibujarPacman(BITMAP *buffer,BITMAP *CuerpoPacman,int posXY[]);
void fantasmas(int matrizJuego[][COL],int vez,int vecPos[],int *pvez,int *pcomida,int *pvidas,int *pDirF1,int *pDirF2,int *pDirF3,int *pDirF4,int pPausaF[],int dificultad);
int valorPreFantasma(int matrizJuego[][COL],int posIF, int posJF);
void fantasmaRandom(int matrizJuego[][COL],int vez,int vecPos[],int *pvez,int *pcomida,int *pvidas,int *pDirF4,int pPausaF[],int dificultad);
void buscarPosicion(int matrizJuego[][COL],int tipoBusqueda,int vecPos[]);
int verificarTeleport(int matrizJuego[][COL],int posI,int posJ,int tipo,int valorPre);
void fantasmaFast(int matrizJuego[][COL],int vez,int vecPos[],int *pvez,int *pcomida,int *pvidas,int *pDirF1,int pPausaF[],int dificultad);
void fantasmaLento(int matrizJuego[][COL],int vez,int vecPos[],int *pvez,int *pcomida,int *pvidas,int *pDirF2,int pPausaF[],int dificultad);
void reiniciarPacman(int matrizJuego[][COL],int *pVidas);
int morirPacmanF(int matrizJuego[][COL],int posI, int posJ,int *pvez,int tipo,int *pcomida,int *pvidas);
int morirPacman(int matrizJuego[][COL],int posI,int posJ,int *pvez,int *pcomida,int *pQuien);
void reiniciarFantasmas(int matrizJuego[][COL],int *pvez);
void fantasmaContra(int matrizJuego[][COL],int vez,int vecPos[],int *pvez,int *pcomida,int *pvidas,int *pDirF3,int pPausaF[],int dificultad);
void sacarFantasmas(int matrizJuego[][COL],int numReiniciar);
int poderActivo(int *pcomida,int dificultad);
int registrarUsuario(FILE *registroUsuarios, int *pIdentificacion,int *pVidas);
int validarUsuario(FILE *registroUsuarios, char nomUsuario[]);
int secuenciaId(FILE *registroUsuarios);
char **ordenarRank(int *vecRank,FILE *registroUsuarios);
bool verificarContrasena(FILE *registroUsuarios, char contrasena[],char nomUsuario[],int *pIdentificacion,int *pNivel,int *pVidas);
int continuarProgreso(FILE *registroUsuarios,int *pIdentificacion,int *pNivel,int *pVidas);
void guardarNivel(FILE *registroUsuarios,int *pNivel,int *pIdentificacion,bool reiniciar);
void registrarPuntaje(FILE *registroUsuarios,int id,int puntos,int *pVidas);
void init();
void deinit();
int Portada();
int MiniPortada();
void menu(int Opcion);
void mapa1(int matrizJuego[][COL]);
void mapa3(int matrizJuego[][COL]);
void mapa2(int matrizJuego[][COL]);
void mejoresPuntuaciones(char **nombresRank, int *vecRank);
void generarFruta(int matrizJuego[][COL]);
void meterFantasmas(int matrizJuego[][COL],int numMeter);

//Inicio de código
int main(){
	init();
	int Opcion, Salir=4;
	do {
		Opcion=Portada();
		menu(Opcion);
	} while (Opcion!=Salir);
	deinit();
	return 0;
}
END_OF_MAIN();

void init() {
	int depth, res,cancion;
	allegro_init();
	depth = desktop_color_depth();
	if (depth == 0) depth = 32;
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
	if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0) {
       allegro_message("Error: inicializando sistema de sonido\n%s\n", allegro_error);
       return;
    }
	set_volume(55, 55);
	
	MIDI *musica1;  
	srand(time(NULL));
	cancion=rand()%8+1;
	switch(cancion){
		case 1:
			musica1=load_midi("sonora de margarita - que bello.mid");
			break;
		case 2:
			musica1 = load_midi("El_rey_leon_(Hakuna_matata).mid");
			break;
		case 3:
			musica1  = load_midi("Harry_Potter.mid");
			break;
		case 4:
			musica1  = load_midi("angeles_azules_-_amigos_nada_mas.mid");	
			break;
		case 5:
			musica1  = load_midi("Cazafantasmas.mid");	
			break;
		case 6:
			musica1  = load_midi("rata_de_2_patas.mid");
			break;
		case 7:
			musica1  = load_midi("luis miguel - culpable o no.mid");
			break;
		case 8:
			musica1  = load_midi("jefe_de_jefes.mid");
			break;
	}	
	play_midi(musica1,1);
}

void deinit() {
	clear_keybuf();
}

int Portada() {
	int opcion;
	bool salida=false;
	BITMAP *buffer;
	buffer=create_bitmap(960, 620);
	BITMAP *Portada=load_bitmap("Portada.bmp", NULL);
	BITMAP *Portada1=load_bitmap("Portada1.bmp", NULL);
	BITMAP *Portada2=load_bitmap("Portada2.bmp", NULL);
	BITMAP *Portada3=load_bitmap("Portada3.bmp", NULL);
	BITMAP *Portada4=load_bitmap("Portada4.bmp", NULL);
	
	while(!salida) {
		blit(buffer, screen, 0, 0, 0, 0, 960, 620);
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
	return opcion;
}

int MiniPortada() {
	int Opcion1;
	bool salida=false;
	BITMAP *buffer;
	buffer=create_bitmap(960, 620);
	BITMAP *MiniPortada=load_bitmap("MiniPortada.bmp", NULL);
	BITMAP *MiniPortada1=load_bitmap("MiniPortada1.bmp", NULL);
	BITMAP *MiniPortada2=load_bitmap("MiniPortada2.bmp", NULL);
	BITMAP *MiniPortada3=load_bitmap("MiniPortada3.bmp", NULL);
	while(!salida) {
		blit(buffer, screen, 0, 0, 0, 0, 960, 620);
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
	BITMAP *vectorPintarMapa[10],*vectorMotorJuego[15];
	vectorPintarMapa[0]=load_bitmap("Bloques_1.bmp", NULL);
	vectorPintarMapa[1]=load_bitmap("Espacio.bmp", NULL);
	vectorPintarMapa[2]=load_bitmap("PuntosChicos.bmp", NULL);
	vectorPintarMapa[3]=load_bitmap("PuntosGrandes.bmp", NULL);
	vectorPintarMapa[4]=load_bitmap("FantasmaBlinky_D.bmp", NULL);
	vectorPintarMapa[5]=load_bitmap("FantasmaInky_D.bmp", NULL);
	vectorPintarMapa[6]=load_bitmap("FantasmaPinky_D.bmp", NULL);
	vectorPintarMapa[7]=load_bitmap("FantasmaClyde_D.bmp", NULL);
	vectorPintarMapa[8]=load_bitmap("CuerpoPacman_I.bmp", NULL);
	
	vectorMotorJuego[1]=load_bitmap("Vidas1.bmp",NULL);
	vectorMotorJuego[2]=load_bitmap("Vidas2.bmp",NULL);
	vectorMotorJuego[3]=load_bitmap("Vidas3.bmp",NULL);
	vectorMotorJuego[4]=load_bitmap("1Frutas.bmp",NULL);
	vectorMotorJuego[5]=load_bitmap("2Frutas.bmp",NULL);
	vectorMotorJuego[6]=load_bitmap("3Frutas.bmp",NULL);
	vectorMotorJuego[7]=load_bitmap("4Frutas.bmp",NULL);
	vectorMotorJuego[8]=load_bitmap("5Frutas.bmp",NULL);
	vectorMotorJuego[9]=load_bitmap("advertenciasalir.bmp",NULL);
	vectorMotorJuego[10]=load_bitmap("cargando1.bmp",NULL);
	vectorMotorJuego[11]=load_bitmap("cargando2.bmp",NULL);
	vectorMotorJuego[12]=load_bitmap("cargando3.bmp",NULL);
	vectorMotorJuego[13]=load_bitmap("cargando0.bmp", NULL);
	vectorMotorJuego[14]=load_bitmap("perdiste.bmp", NULL);
	vectorMotorJuego[0]=load_bitmap("Felicidades.bmp", NULL);
	
	FILE *registroUsuario;
	Usuario usuario;
	BITMAP *advertencia=load_bitmap("advertencia.bmp",NULL);
	BITMAP *ayuda=load_bitmap("ayuda.bmp",NULL);
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
							op=registrarUsuario(registroUsuario,pIdentificacion,pVidas);
							if(op!=1){
								motorJuego(registroUsuario,pIdentificacion,pNivel,pVidas,vectorPintarMapa,vectorMotorJuego);	
							}
							break;
						case ContinuarJuego:
							op=continuarProgreso(registroUsuario,pIdentificacion,pNivel,pVidas);
							if(op!=1){
								motorJuego(registroUsuario,pIdentificacion,pNivel,pVidas,vectorPintarMapa,vectorMotorJuego);
							}
							break;
					}
				} while (OpcionJuego!=RegresarAlMenuPrincipal);
				break;
			case MejoresPuntuaciones:
				registroUsuario=fopen("Usuarios.dat","rb");
				if(!registroUsuario){//El archivo no se pudo abrir o no existe, por lo tanto no hay rank que mostrar
					do{
						blit(advertencia,screen,0,0,0,0,900,600);
					}while(!key[KEY_ESC]);
				}
				else{
					nombresRank=ordenarRank(vecRank,registroUsuario);
					mejoresPuntuaciones(nombresRank,vecRank);	
				}
				
				break;
			case Ayuda:
					do{
						blit(ayuda,screen,0,0,0,0,900,600);
					}while(!key[KEY_ESC]);
				break;
		}while (Opcion!=Salir);
	}
}

void motorJuego(FILE *registroUsuarios,int *pIdentificacion,int *pNivel,int *pVidas,BITMAP *vectorPintarMapa[],BITMAP *vectorMotorJuego[]){
	//Esta función es la que mueve a todo el juego y la encargada de permitir el flujo del juego
	//Frenamos la música del inicio
	//Iniciamos efecto de sonido de inicio
	play_midi(NULL,NULL);
	SAMPLE *efecto=load_wav("pacman-song.wav");//Tono de inicio de nivel
	MIDI *perderMusic =load_midi("Rosas.mid");
	MIDI *ganarMusic =load_midi("sam dave - soul man.mid");		
	BITMAP *buffer=create_bitmap(960, 660);;	
	FONT *font1=load_font("letritas.pcx", NULL, NULL);
	FONT *font2=load_font("letritas2.pcx", NULL, NULL);
	FONT *font3=load_font("LetritasPoder.pcx",NULL,NULL);
	Usuario usuario;
	int matrizJuego[FIL][COL];//Esta es la matriz principal del juego, con la que movemos todo.
	int l=0,l2=1,l1=1,dificultad=1,*pDificultad,sonidoActivo=0,*pSonidoActivo=&sonidoActivo,puntosScoreFinal=0;//"l","l2","l1" son los que nos ayudan a tener el control de la indicación "listo" en el juego. "*pDificultad" nos ayuda a cambiar la dificultad
	int newkey,newkey2,vezCargar=0,id2,quien=0,*pQuien;//"newkey" nos ayuda a almacenar el valor de lo que ingrese el teclado. "vezCargar" es solo un contador para la pantalla de carga, "quien" nos indica cuál fantasma mató a Pacman.
	int pPausaF[4]; pPausaF[0]=0;pPausaF[1]=0;pPausaF[2]=0;pPausaF[3]=0;//pPausa indica si el fantasma que representa esa posición está o no en pausa
	int vez=1,comida=0,vecPos[2],*pvez,*pcomida;//"Vez" nos ayuda en el ciclo de los fantasmas, "comida" es el que nos ayuda a verificar si el fantasma tiene o no el poder, "vecPos[]" nos ayuda a guardar las posiciones que la función BuscarPosición tenga
	int pTotal=0,*puntuacionTotal,cambioNivel=0,*pCambioNivel,dir=1,*pDir,dirF1=1,dirF2=1,dirF3=1,dirF4=1,*pDirF1,*pDirF2,*pDirF3,*pDirF4,frutas=0,*pFrutas=0,vezFruta=0;//
	char ASCII,ASCII2;
	bool perdedor=false,ganador=false,x=false;//"x" nos sirve para un ciclo interno de pantalla de pausa.
	pvez=&vez;pDir=&dir;pcomida=&comida;puntuacionTotal=&pTotal,pCambioNivel=&cambioNivel;pFrutas=&frutas;pDirF1=&dirF1;pDirF2=&dirF2;pDirF3=&dirF3;pDirF4=&dirF4;pDificultad=&dificultad;pQuien=&quien;
	*pcomida=0;
	do{
		play_sample(efecto,200,150,1000,0);//Efecto de sonido de inicio
		generacionMapa(*pNivel,matrizJuego,pDificultad);//Generamos el mapa con el nivel actual
		pintarMapa(matrizJuego,buffer,pDir,pcomida,pDirF1,pDirF2,pDirF3,pDirF4,pFrutas,*pNivel,pPausaF,dificultad,vectorPintarMapa);//Cargamos todo al mapa
		textprintf(buffer, font1, 400, 0, makecol(255, 255, 255), "Listo? ");//Texto de ¿listo? para antes de comenzar
		do{
			blit(buffer, screen, 0, 0, 0, 0, 960, 660);//Pintamos el mapa dentro de este ciclo para que la palabra "listo" aparezca algunos segundos antes de empezar a jugar
			l++;
		}while(l!=900);
		l=0;
		do{//Ciclo interno  a realizar mientras no haya un cambio de nivel o un perdedor
			movimientoPacman(matrizJuego,vecPos,pvez,pcomida,pVidas,puntuacionTotal,pCambioNivel,pDir,registroUsuarios,pIdentificacion,pFrutas,pQuien,pPausaF,dificultad,sonidoActivo);//Le permitimos al Pacman moverse
			fantasmas(matrizJuego,vez,vecPos,pvez,pcomida,pVidas,pDirF1,pDirF2,pDirF3,pDirF4,pPausaF,dificultad);//Llama a los 4 fantasmas, según corresponda por el valor del "pvez#"
			if(vez%4==0){//Este if controla la animación del Pacman comiendo
				*pDir=5;
			}
			clear(buffer);//Borramos el buffer
			if(keypressed()){//Detectamos si el usuario apretó la tecla de "escape" para pausar el juego
				newkey = readkey();
				ASCII = newkey & 0xff;
				clear_keybuf();
				if(ASCII==27){//Apretó Escape
					do{//Este ciclo se ejecuta mientras el usuario no decida qué hacer en el menú de pausa
						blit(vectorMotorJuego[9], buffer, 0, 0, 0, 0, 960, 620);//Advertencia de salida	
						blit(buffer, screen, 0, 0, 0, 0, 960, 660);
						if(keypressed()){//Confirmación del usuario para proceder
							newkey2=readkey();
							ASCII2 = newkey2 & 0xff;
							if(ASCII2==13){//Apretó enter,se retorna porque el usuario ya no quiso seguir jugando a pesar de la advertencia
								id2=*pIdentificacion;//Detectamos el ID del usuario que se salió, para actualizar las vidas. Esta validación impide al usuario hacer trampas de salirse a la mitad de un nivel para poder reiniciar sus vidas
								registrarPuntaje(registroUsuarios,id2,(-1),pVidas);//Le mandamos un menos 1 para que sepa que no tiene que sumar ni restar nada
								*pNivel=1;
								return;
							}
							else if(ASCII2==27){//apretó Escape, se cancela la operación
								clear(buffer);
								x=true;//Sale de este ciclo para seguir la ejecución normal del motor de juego
							}
							else if(ASCII2==115){//apretó la tecla S para activar/desactivar el sonido
								if(sonidoActivo==0){//Significa que el sonido estaba activado, por lo tanto el usuario quiere desactivarlo
									sonidoActivo=1;//Significa que el usuario no quiere escuchar el sonido de comida
								}
								else{//Significa que el sonido estaba desactivado, por lo tanto el usuario quiere activarlo
									sonidoActivo=0;//Significa que el usuario quiere volver a escuchar el sonido
								}
							}
						}
					}while(x==false);
					x=false;
				}	
			}
				
			switch(frutas){//Con esto, el programa detecta qué fruta(s) va a mostrar en el Score (es la colección de las frutas).
				case 0:
					break;
				case 1:
					draw_sprite(buffer, vectorMotorJuego[4], 495, 0);
					break;
				case 2:
					draw_sprite(buffer, vectorMotorJuego[5], 495, 0);
					break;
				case 3:
					draw_sprite(buffer, vectorMotorJuego[6], 495, 0);
					break;
				case 4:
					draw_sprite(buffer, vectorMotorJuego[7], 495, 0);
					break;
				default:
					draw_sprite(buffer, vectorMotorJuego[8], 495, 0);
					break;
			}	
			if(*pVidas==3){//Aparecen las vidas completas en el Score
				draw_sprite(buffer, vectorMotorJuego[3], 815, 0);
			}
			else if(*pVidas==2){//Aparecen sólo dos vidas completas en el Score
				if(l2==1){//Con esto sabemos que el Pacman ha perdido su primer vida y se le indica de nuevo la palabra "listo" antes de volver a jugar
					do{
						textprintf(screen, font1, 400, 0, makecol(255, 255, 255), "Listo?      ");
						l++;
					}while(l!=900);
					l=0;
					l2=2;
				}
				draw_sprite(buffer, vectorMotorJuego[2], 815, 0);
			}
			else if(*pVidas==1){//Con esto sabemos que el Pacman ha perdido su segunda vida y se le indica de nuevo la palabra "listo" antes de volver a jugar
				if(l1==1){
					do{
						textprintf(screen, font1, 400, 0, makecol(255, 255, 255), "Listo?      ");
						l++;
					}while(l!=900);
					l=0;
					l1=2;
				}
				draw_sprite(buffer, vectorMotorJuego[1],815, 0);
			}
			if(vezFruta==100){//Generemos la fruta si la repetición es la 100
				generarFruta(matrizJuego);
			}
			if(vezFruta==250){//Tenemos que desaparecer la fruta en caso de que no se la haya comido Pacman
				buscarPosicion(matrizJuego,4,vecPos);
				if(vecPos[0]!=-1 && vecPos[1]!=-1){//Si la encontró
					matrizJuego[vecPos[0]][vecPos[1]]=2;//Borramos a la fruta
				}
				//Si no la encuentra quiere decir que el Pacman se la comió
			}
			pintarMapa(matrizJuego,buffer,pDir,pcomida,pDirF1,pDirF2,pDirF3,pDirF4,pFrutas,*pNivel,pPausaF,dificultad,vectorPintarMapa);//Cargamos todo lo visual al mapa
			blit(buffer, screen, 0, 0, 0, 0, 960, 660);//Pintamos lo cargado previamente
			textprintf(screen, font1, 0, 0, makecol(255, 153, 51), "Score: %i", *puntuacionTotal);//Es el encargado de mostrar el Score
			textprintf(screen, font1, 400, 0, makecol(255, 255, 255), "Nivel: %i", *pNivel);//Es el encargado de mostrar el nivel actual
			textprintf(screen, font3, 690, 0, makecol(255, 255, 255), "Poder: %ir", dificultad);//"rep" se refiere a "repeticiones" por lo que 60, por ejemplo, significa que si Pacman toma el poder en ese momento, tardará 60 repeticiones del juego para que termine el poder
			textprintf(screen, font1, 200, 0, makecol(255, 255, 255), "[ESC] Pausa");//"rep" se refiere a "repeticiones" por lo que 60, por ejemplo, significa que si Pacman toma el poder en ese momento, tardará 60 repeticiones del juego para que termine el poder
			vezFruta++;
			vez++;
			rest(VELOCIDAD);//Maneja la velocidad del juego. Entre más alto el parámetro, más lento el juego
			if(*pVidas<=0){//se terminó el juego
				play_midi(perderMusic,1);//Se activa la música de derrota
				perdedor=true;//Permitimos la salida de este ciclo y del siguiente
			}
		} while(perdedor==false && cambioNivel==0);//El ciclo interno, de juego por mapa
		*pcomida=0;//Reiniciamos el contador de comida por si estuviera prendido, que no avance con poder al siguiente nivel
		vezFruta=0;//Reiniciamos este contador para que podamos volver a imprimir la fruta en el siguiente.
		cambioNivel=0;//Reiniciamos a cero esta variable para que el ciclo interno se siga dando
		puntosScoreFinal=*puntuacionTotal;//Por fines prácticos, hacemos un respaldo de la puntuación total del nivel	
		*puntuacionTotal=0;//Reseteamos la puntuación
		*pNivel+=1;//Aumentamos el nivel en 1
		if(perdedor==true){//Reiniciamos el nivel del usuario a 1 y le borramos su puntaje
			id2=*pIdentificacion;
			*pNivel=1;
			registrarPuntaje(registroUsuarios,id2,0,pVidas);
			guardarNivel(registroUsuarios,pNivel,pIdentificacion,false);
		}
		else if(*pNivel==NIVELMAXIMO){//El usuario ganó el juego
			set_volume(70,70);
			play_midi(ganarMusic,1);//Inicia la música de ganador
			ganador=true;//Permitimos la salida de este ciclo
		}
		else{
			guardarNivel(registroUsuarios,pNivel,pIdentificacion,false);//Quiere decir que es cambio de nivel, y hay que guardar
		}
		clear(buffer);
		do{//Este switch es el encargado de la animación de guardado
			switch(vezCargar){
				case 0:
					blit(vectorMotorJuego[12], buffer, 0, 0, 0, 0, 960, 660);
					break;
				case 400:
					blit(vectorMotorJuego[11], buffer, 0, 0, 0, 0, 960, 660);
					break;
				case 800:
					blit(vectorMotorJuego[10], buffer, 0, 0, 0, 0, 960, 660);
					break;
				case 1200:
					blit(vectorMotorJuego[13], buffer, 0, 0, 0, 0, 960, 660);
					break;
			}
			vezCargar++;
			blit(buffer, screen, 0, 0, 0, 0, 960, 660);
		}
		while(vezCargar<1201);
		vezCargar=0;
		vez=0;
		clear(buffer);	
	} while(perdedor==false && ganador==false);//Ganador se activa cuando se termina el nivel 9
	
	if(ganador==true){//El usuario ganó el Juego y, por lo tanto, debemos de mostrarle una pantalla que lo indique
			*pNivel=1;//Le reiniciamos el nivel a 1, para que pueda seguir jugando ahora desde el inicio
			guardarNivel(registroUsuarios,pNivel,pIdentificacion,true);//Se guarda por última vez la información del usuario, pero ahora reiniciando sus vidas a 3, porque es justo porque ya ganó el juego
			clear_keybuf();//Borramos el buffer de entrada del teclado
			clear(buffer);
			do{
				blit(vectorMotorJuego[0], buffer, 0, 0, 0, 0, 900, 660);
				textprintf(buffer, font2, 480, 160, makecol(255, 0, 0), "%i", puntosScoreFinal);
				textprintf(buffer, font2, 570, 235, makecol(255, 0, 0), "%i", *pVidas);
				blit(buffer, screen, 0, 0, 0, 0, 960, 660);			
			}while(!key[KEY_ESC]);
			blit(buffer, screen, 0, 0, 0, 0, 960, 660);																 
		}
	else if(perdedor==true){//El usuario perdió el juego y, por lo tanto, debemos de mostrarle una pantalla que lo indique
			clear_keybuf();//Borramos el buffer de entrada del teclado
				clear(buffer);
				do{
					blit(vectorMotorJuego[14], buffer, 0, 0, 0, 0, 900, 660);
					blit(buffer, screen, 0, 0, 0, 0, 960, 660);
				}while(!key[KEY_ESC]);
	}
	set_volume(45, 45);//Reducimos el nivel de volumen al original
	clear_keybuf();//Borramos el buffer

}

void generacionMapa(int numNivel,int matrizJuego[][COL],int *pDificultad){
	//Esta función se encarga de crear el mapa necesario y llamar a la matriz principal del juego. De aquí, la matriz principal se distribuye a todo el programa.
	switch(numNivel){
		case 1: 
			mapa1(matrizJuego);
			*pDificultad=60;
			break;
		case 2:
			mapa2(matrizJuego);
			*pDificultad=50;
			break;
		case 3:
			mapa3(matrizJuego);
			*pDificultad=40;
			break;
		case 4:
			mapa1(matrizJuego);
			*pDificultad=30;
			break;
		case 5:
			mapa2(matrizJuego);
			*pDificultad=25;
			break;
		case 6:
			mapa3(matrizJuego);
			*pDificultad=20;
			break;	
		case 7:	
			mapa1(matrizJuego);
			*pDificultad=20;
			break;
		case 8:
			mapa2(matrizJuego);
			*pDificultad=20;
			break;
		case 9:
			 mapa3(matrizJuego);
			 *pDificultad=20;
			break;
	}
}

void pintarMapa(int matrizJuego[][COL],BITMAP *buffer,int *pDir,int *pcomida,int *pDirF1,int *pDirF2,int *pDirF3,int *pDirF4,int *pFrutas,int numNivel,int pPausaF[],int dificultad,BITMAP *vectorPintarMapa[]){
	//Esta función se encarga de hacer las impresiones necesarias en pantalla

	switch(numNivel){//Seleccionamos la forma del diseño visual de los mapas
		case 1:
			vectorPintarMapa[0]=load_bitmap("Bloques_1.bmp", NULL);
			break;
		case 2:
			vectorPintarMapa[0]=load_bitmap("Bloques_2.bmp", NULL);
			break;
		case 3:
			vectorPintarMapa[0]=load_bitmap("Bloques_3.bmp", NULL);
			break;
		case 4:
			vectorPintarMapa[0]=load_bitmap("Bloques_4.bmp", NULL);
			break;
		case 5:
			vectorPintarMapa[0]=load_bitmap("Bloques_5.bmp", NULL);
			break;
		case 6:
			vectorPintarMapa[0]=load_bitmap("Bloques_6.bmp", NULL);
			break;
		case 7:
			vectorPintarMapa[0]=load_bitmap("Bloques_7.bmp", NULL);
			break;
		case 8:
			vectorPintarMapa[0]=load_bitmap("Bloques_8.bmp", NULL);
			break;
		case 9:
			vectorPintarMapa[0]=load_bitmap("Bloques_9.bmp", NULL);
			break;
	}
	
	switch(*pDir){//Seleccionamos la dirección en pantalla del Pacman
		case 1://izquierda
			vectorPintarMapa[8]=load_bitmap("CuerpoPacman_I.bmp", NULL);
			break;
		case 2://derecha
			vectorPintarMapa[8]=load_bitmap("CuerpoPacman_II.bmp", NULL);
			break;
		case 3://abajo
			vectorPintarMapa[8]=load_bitmap("CuerpoPacman_III.bmp", NULL);
		break;
		case 4://arriba
			vectorPintarMapa[8]=load_bitmap("CuerpoPacman_IIII.bmp", NULL);
		break;
		case 5://cuerpo completo
			vectorPintarMapa[8]=load_bitmap("CuerpoPacman_V.bmp", NULL);
			break;
	}
	
	switch(*pDirF1){//Seleccionamos la dirección en pantalla del Fantasma 1
		case 1://izquierda
			vectorPintarMapa[4]=load_bitmap("FantasmaBlinky_I.bmp", NULL);
			break;
		case 2://derecha
			vectorPintarMapa[4]=load_bitmap("FantasmaBlinky_D.bmp", NULL);
			break;
		case 3://abajo
			vectorPintarMapa[4]=load_bitmap("FantasmaBlinky_Abajo.bmp", NULL);
		break;
		case 4://arriba
			vectorPintarMapa[4]=load_bitmap("FantasmaBlinky_Arriba.bmp", NULL);
		break;
	}
	
	switch(*pDirF2){//Seleccionamos la dirección en pantalla del Fantasma 2
		case 1://izquierda
			vectorPintarMapa[5]=load_bitmap("FantasmaInky_I.bmp", NULL);
			break;
		case 2://derecha
			vectorPintarMapa[5]=load_bitmap("FantasmaInky_D.bmp", NULL);
			break;
		case 3://abajo
			vectorPintarMapa[5]=load_bitmap("FantasmaInky_Abajo.bmp", NULL);
		break;
		case 4://arriba
			vectorPintarMapa[5]=load_bitmap("FantasmaInky_Arriba.bmp", NULL);
		break;
	}
	
	switch(*pDirF3){//Seleccionamos la dirección en pantalla del Fantasma 3
		case 1://izquierda
			vectorPintarMapa[6]=load_bitmap("FantasmaPinky_I.bmp", NULL);
			break;
		case 2://derecha
			vectorPintarMapa[6]=load_bitmap("FantasmaPinky_D.bmp", NULL);
			break;
		case 3://abajo
			vectorPintarMapa[6]=load_bitmap("FantasmaPinky_Abajo.bmp", NULL);
		break;
		case 4://arriba
			vectorPintarMapa[6]=load_bitmap("FantasmaPinky_Arriba.bmp", NULL);
		break;
	}
	
	switch(*pDirF4){//Seleccionamos la dirección en pantalla del Fantasma 4
		case 1://izquierda
			vectorPintarMapa[7]=load_bitmap("FantasmaClyde_I.bmp", NULL);
			break;
		case 2://derecha
			vectorPintarMapa[7]=load_bitmap("FantasmaClyde_D.bmp", NULL);
			break;
		case 3://abajo
			vectorPintarMapa[7]=load_bitmap("FantasmaClyde_Abajo.bmp", NULL);
		break;
		case 4://arriba
			vectorPintarMapa[7]=load_bitmap("FantasmaClyde_Arriba.bmp", NULL);
		break;
	}
	
	switch(*pFrutas){//Seleccionamos la fruta que corresponda
		case 0:
			vectorPintarMapa[9]=load_bitmap("Fruta 1.bmp",NULL);
			break;
		case 1:
			vectorPintarMapa[9]=load_bitmap("Fruta 2.bmp",NULL);
			break;
		case 2:
			vectorPintarMapa[9]=load_bitmap("Fruta 3.bmp",NULL);
			break;
		case 3:
			vectorPintarMapa[9]=load_bitmap("Fruta 4.bmp",NULL);
			break;
		default:
			vectorPintarMapa[9]=load_bitmap("Fruta 5.bmp",NULL);
			break;
	}

	if(poderActivo(pcomida,dificultad)!=0){//El poder está activado, hay que imprimir a los fantasmas asustados siempre y cuando no estén pausados
		 if(pPausaF[0]==0){
		 	vectorPintarMapa[4]=load_bitmap("Fantasmas_Asustados.bmp", NULL);
		 }
		 if(pPausaF[1]==0){
		 	vectorPintarMapa[5]=load_bitmap("Fantasmas_Asustados.bmp", NULL);
		 }
		 if(pPausaF[2]==0){
		 	vectorPintarMapa[6]=load_bitmap("Fantasmas_Asustados.bmp", NULL);
		 }
		 if(pPausaF[3]==0){
		 	vectorPintarMapa[7]=load_bitmap("Fantasmas_Asustados.bmp", NULL);
		 }
	}
	
	for(int i=0;i<FIL;i++){
		for(int j=0;j<COL;j++){
			if(matrizJuego[i][j]==0){//PacMan
				draw_sprite(buffer, vectorPintarMapa[8], j*30, i*30+35);
			}else if(matrizJuego[i][j]==1 || matrizJuego[i][j]==3){//Bloques
				draw_sprite(buffer, vectorPintarMapa[0], j*30, i*30+35);
			}
			else if(matrizJuego[i][j]==2){//Paso Libre
				draw_sprite(buffer, vectorPintarMapa[1], j*30, i*30+35);
			}
			else if(matrizJuego[i][j]==4){//Frutas
				draw_sprite(buffer, vectorPintarMapa[9], j*30, i*30+35);
			}else if(matrizJuego[i][j]==5){//Puntos  chicos
				draw_sprite(buffer, vectorPintarMapa[2], j*30, i*30+35);
			}else if(matrizJuego[i][j]==6){//Puntos  grandes
				draw_sprite(buffer, vectorPintarMapa[3], j*30, i*30+35);
			}else if(matrizJuego[i][j]==7){//Fantasma Blinky
				draw_sprite(buffer, vectorPintarMapa[4], j*30, i*30+35);
			}else if(matrizJuego[i][j]==8){//Fantasma Inky
				draw_sprite(buffer, vectorPintarMapa[5], j*30, i*30+35);
			}else if(matrizJuego[i][j]==9){//Fantasma Pinky
				draw_sprite(buffer, vectorPintarMapa[6], j*30, i*30+35);
			}else if(matrizJuego[i][j]==10){//Fantasma Clyde
				draw_sprite(buffer, vectorPintarMapa[7], j*30, i*30+35);
			}
		}
	}
		
}

void movimientoPacman(int matrizJuego[][COL],int vecPos[],int *pvez,int *pcomida,int *pVidas,int *puntuacionTotal,int *pCambioNivel,int *pDir,FILE *registroUsuarios, int *pIdentificacion,int *pFrutas,int *pQuien,int pPausaF[],int dificultad,int sonidoActivo){
	//Esta es la función que se encarga de realizar todo lo relacionado con el movimiento del pacman. Es  la segunda función más importante porque de aquí dependen una gran parte de las validaciones
	Usuario usuario;
	int PosI, PosJ, x,newkey,buscar=*pQuien,contadorSonido=0,sonidoChistoso;
	buscarPosicion(matrizJuego,0,vecPos);//Buscamos la posición del Pacman
	PosI=vecPos[0];
	PosJ=vecPos[1];
	SAMPLE *sonido=load_sample("pacman avanza chido.wav");
	SAMPLE *muertePacman=load_sample("pacman-dies.wav");
	SAMPLE *muerteFantasma=load_sample("pacman-eating-ghost.wav");
	queCome(matrizJuego,pcomida,PosI,PosJ,dificultad);
	
	if(key[KEY_UP] || key[KEY_W]) {
		*pDir=4;//Dirección de la imagen de Pacman -> ¿A dónde apunta Pacman?
		PosI--;//Al moverse hacia arriba las filas disminuyen. A partir de este dato, checamos todas las validaciones
		x=verificarTeleport(matrizJuego, PosI, PosJ, 0,0);//Checamos si el Pacman se moverá o no al teleport
		if(x==0) {//Pacman no se movió al teleport, por lo tanto puede hacer el movimiento normal
			if(matrizJuego[PosI][PosJ]==1 || matrizJuego[PosI][PosJ]==3){}//Quiere decir que el PASO NO es libre	
			else {//Pacman si se puede mover, por lo tanto hay que validar que su muerte se de
				puntaje(matrizJuego,PosI,PosJ,pcomida,puntuacionTotal,pCambioNivel,registroUsuarios,pIdentificacion,pFrutas,pVidas,dificultad);//Checamos y acutalizamos puntaje en pantalla
				queCome(matrizJuego,pcomida,PosI,PosJ,dificultad);//Ya sabemos si el puntero de la comida está o no activo
				if(morirPacman(matrizJuego,PosI,PosJ,pvez,pcomida,pQuien)==0){//Pacman no morirá en este movimiento
					if(sonidoActivo==0){//Quiere decir que el usuario está permitiendo el sonido del movimiento del pacman
						play_sample(sonido,200,150,1000,0);
					}
					matrizJuego[PosI][PosJ]=0;//Se hace el movimiento del pacman, cambiándolo de posición
		           	matrizJuego[PosI+1][PosJ]=2;//Borramos la posición anterior, el pacman ya comió	por eso dejamos un 2		        	
				}
				else{//Pacman podría perder una vida, pero hay que validar primero
					buscar=*pQuien;//Ya estamos enterados de qué Fantasma fue el que provocó que Pacman estuviera en esta instancia. El "*pQuien" se modificó en la función morirPacman, pero por practicidad lo metemos a una variable tipo INT
					if(poderActivo(pcomida,dificultad)==0){//El poder está desactivado, por lo tanto Pacman pierde una vida
						play_sample(muertePacman,200,150,1000,0);//Se activa el sonido de la muerte del Pacman
						reiniciarFantasmas(matrizJuego,pvez);//Metemos a todos los fantasmas a su casa para que podamos sacarlos con orden
						reiniciarPacman(matrizJuego,pVidas);//Pacman regresa a su punto de origen
						matrizJuego[PosI+1][PosJ]=2;//Se borra la posición en donde estaba pacman. Pacman ya había comido, por lo tanto se le deja un espacio libre=2	
					}
					else{//El poder está activado, por lo tanto Pacman puede seguir y comerse al fantasma que le indicó el "*pQuien"
						play_sample(muerteFantasma,200,150,1000,0);//Se activa el sonido de la muerte de un fantasma
						meterFantasmas(matrizJuego,buscar);//Metemos a su casa al fantasma "*pQuien" que se le asignó a la variable buscar.
						pPausaF[buscar-7]=1;//Encendemos la pausa al fantasma que corresponda. Buscar tiene valores a partir del 7 al 10, pero el vector pPausaF comienza en 0, por lo tanto a buscar le restamos 7 y ahora tenemos la posición indicada para pausar
						matrizJuego[PosI][PosJ]=0;//Movemos a Pacman
						matrizJuego[PosI+1][PosJ]=2;//Borramos la posición anterior a Pacman
					}	
				}
			}
		}
	} 
	//Procedimientos idénticos al anterior, sólo que ahora validando con las otras 6 teclas disponibles para el juego
	else if(key[KEY_DOWN] || key[KEY_S]) {
		*pDir=3;
		PosI++;
		x=verificarTeleport(matrizJuego, PosI, PosJ, 0,0);
		if(x==0) {
			if(matrizJuego[PosI][PosJ]==1 || matrizJuego[PosI][PosJ]==3){}//Quiere decir que el paso no es libre
			else {
				puntaje(matrizJuego,PosI,PosJ,pcomida,puntuacionTotal,pCambioNivel,registroUsuarios,pIdentificacion,pFrutas,pVidas,dificultad);
				queCome(matrizJuego,pcomida,PosI,PosJ,dificultad);
				if(morirPacman(matrizJuego,PosI,PosJ,pvez,pcomida,pQuien)==0){//Pacman no morirá en este movimiento
					if(sonidoActivo==0){
						play_sample(sonido,200,150,1000,0);
					}
					matrizJuego[PosI][PosJ]=0;
		       		matrizJuego[PosI-1][PosJ]=2;
				}
				else{//Pacman podría perder una vida
					buscar=*pQuien;
					if(poderActivo(pcomida,dificultad)==0){
						play_sample(muertePacman,200,150,1000,0);
						reiniciarFantasmas(matrizJuego,pvez);
						reiniciarPacman(matrizJuego,pVidas);
						matrizJuego[PosI-1][PosJ]=2;//Borramos a Pacman
					}
					else{//El poder está activado
						play_sample(muerteFantasma,200,150,1000,0);
						meterFantasmas(matrizJuego,buscar);
						pPausaF[buscar-7]=1;
						matrizJuego[PosI][PosJ]=0;//Movemos a Pacman
						matrizJuego[PosI-1][PosJ]=2;//Borramos a Pacman
					}
				}		
			}
		}
	}
	else if(key[KEY_LEFT] || key[KEY_A] ) {
		*pDir=1;
		PosJ--;
		x=verificarTeleport(matrizJuego, PosI, PosJ, 0,0);
		if(x==0) {
			if(matrizJuego[PosI][PosJ]==1 || matrizJuego[PosI][PosJ]==3){}//Quiere decir que el paso no es libre
			else {
				queCome(matrizJuego,pcomida,PosI,PosJ,dificultad);
				puntaje(matrizJuego,PosI,PosJ,pcomida,puntuacionTotal,pCambioNivel,registroUsuarios,pIdentificacion,pFrutas,pVidas,dificultad);
				if(morirPacman(matrizJuego,PosI,PosJ,pvez,pcomida,pQuien)==0){
					if(sonidoActivo==0){
						play_sample(sonido,200,150,1000,0);
					}
					matrizJuego[PosI][PosJ]=0;
		            matrizJuego[PosI][PosJ+1]=2;
				}
				else{//Pacman podría perder una vida
					buscar=*pQuien;
					if(poderActivo(pcomida,dificultad)==0){
						play_sample(muertePacman,200,150,1000,0);
						reiniciarFantasmas(matrizJuego,pvez);
						reiniciarPacman(matrizJuego,pVidas);
						matrizJuego[PosI][PosJ+1]=2;//Borramos a Pacman
					}
					else{//El poder está activado
						play_sample(muerteFantasma,200,150,1000,0);
						meterFantasmas(matrizJuego,buscar);
						pPausaF[buscar-7]=1;
						matrizJuego[PosI][PosJ]=0;//Movemos a Pacman
						matrizJuego[PosI][PosJ+1]=2;//Borramos a Pacman
					}
				}		
			}
		}
	} 
	else if(key[KEY_RIGHT] || key[KEY_D]) {
		*pDir=2;
		PosJ++;
		x=verificarTeleport(matrizJuego, PosI, PosJ, 0,0);
		if(x==0) {
			if(matrizJuego[PosI][PosJ]==1 || matrizJuego[PosI][PosJ]==3){}//Quiere decir que el paso no es libre
			else {
				queCome(matrizJuego,pcomida,PosI,PosJ,dificultad);
				puntaje(matrizJuego,PosI,PosJ,pcomida,puntuacionTotal,pCambioNivel,registroUsuarios,pIdentificacion,pFrutas,pVidas,dificultad);
				if(morirPacman(matrizJuego,PosI,PosJ,pvez,pcomida,pQuien)==0){
					if(sonidoActivo==0){
						play_sample(sonido,200,150,1000,0);
					}
					matrizJuego[PosI][PosJ]=0;
			        matrizJuego[PosI][PosJ-1]=2;
				}
				else{//Pacman podría perder una vida		
					buscar=*pQuien;
					if(poderActivo(pcomida,dificultad)==0){
					play_sample(muertePacman,200,150,1000,0);
					reiniciarFantasmas(matrizJuego,pvez);
					reiniciarPacman(matrizJuego,pVidas);
					matrizJuego[PosI][PosJ-1]=2;//Borramos a Pacman	
					}
					else{//El poder está activado
						play_sample(muerteFantasma,200,150,1000,0);
						meterFantasmas(matrizJuego,buscar);
						pPausaF[buscar-7]=1;
						matrizJuego[PosI][PosJ]=0;//Movemos a Pacman
						matrizJuego[PosI][PosJ-1]=2;//Borramos a Pacman
					}
				}
					
			}
		}	
	}			
}

int poderActivo(int *pcomida,int dificultad){
	//Es la función principal encargada de indicar si el poder está activo o no. Regresa un 0 si el poder está desactivado y un 2 si está activado
	if(*pcomida>=1 && *pcomida<=dificultad){//Este es el tiempo máximo del poder Activo
		return 2;
	}
	return 0;//Está desactivado
}

void queCome(int matrizJuego[][COL],int *pcomida,int posI,int posJ,int dificultad){
	/*Esta función es la encargada de indicar cuando el fantasma come un punto grande para poder
	  activar al puntero de comida, o desactivarlo en caso de que ya haya pasado el tiempo*/
	SAMPLE *sonidoFruta=load_sample("pacman-fruta.wav");//Carga el sonido de la fruta
	int x=0;
	if(matrizJuego[posI][posJ]==4){
		play_sample(sonidoFruta,200,150,1000,0);//Activa el sonido de cuando el pacman come una fruta
	}
	
	if(matrizJuego[posI][posJ]==6){//Está comiendo un punto grande, hay que activar el puntero
		*pcomida=1;
	}
	
	if(*pcomida>=1 && *pcomida<=dificultad){//Hay que aumentar el puntero en 1, para que siga contando las repeticiones
		*pcomida=*pcomida+1;
	}
	else{//Se apaga el power up
		*pcomida=0;
	}
}

void meterFantasmas(int matrizJuego[][COL],int numMeter){
	//Esta función se encarga de pausar al fantasma que le llegue, metiéndolo a su casa
	int vecPos[2];
	buscarPosicion(matrizJuego,numMeter,vecPos);//Buscamos al fantasma que queremos meter a su casa
	matrizJuego[vecPos[0]][vecPos[1]]=2;//Posición borrada
	if(numMeter==7){//Se reinicia el fantasma 1 --> Matriz=7
		matrizJuego[10][15]=7;//Lo metemos a su casa
	}
	else if(numMeter==8){
		matrizJuego[9][15]=8;
	}
	else if(numMeter==9){
		matrizJuego[9][14]=9;
	}
	else if(numMeter==10){
		matrizJuego[10][14]=10;
	}
}
void sacarFantasmas(int matrizJuego[][COL],int numReiniciar){
	//En esta función, sacamos a los fantasmas de forma validada para que los ponga en la primer posición disponible
	int i,j,k,band=0,vecPos[2];
	i=7;		
	k=12;
	buscarPosicion(matrizJuego,numReiniciar,vecPos);
	matrizJuego[vecPos[0]][vecPos[1]]=2;//Posición borrada
	for(j=12;j<18;j++){
		if(matrizJuego[i][j]==2 || matrizJuego[i][j]==5){//Posición libre para avanzar			
			matrizJuego[i][j]=numReiniciar;
			band=1;
			return;
		}
		else if(matrizJuego[k][j]==2 || matrizJuego[k][j]==5){
			matrizJuego[k][j]=numReiniciar;
			band=1;
			return;
		}
	}
	j=12;
	k=17;
	if(band==0){
		for(i=7;i<13;i++){
			if(matrizJuego[i][j]==2 || matrizJuego[i][j]==5 ){//Posición libre para avanzar
				matrizJuego[i][j]=numReiniciar;
				return;
			}			
			else if(matrizJuego[i][k]==2 || matrizJuego[i][k]==5){
				matrizJuego[i][k]=numReiniciar;
				return;
			}
		}
	 }
}

void sacarFantasmasMuertos(int matrizJuego[][COL],int *pvez){
	//Se encarga de sacar a los fantasmas muertos de forma ordenada, siempre y cuando les corresponda salir
	int vecPos[2],contador=0,vecFantasmas[4];//contador cuenta el número de fantasmas encerrados, y el vecFantasmas guarda a los fantasmas que están encerrados
	for(int n=7;n<11;n++){//Nos dice cuál fantasma está encerrado
		buscarPosicion(matrizJuego,n,vecPos);//Buscamos la posición del fantasma para después compara
		if(n==7 && vecPos[0]==10 && vecPos[1]==15){//Si el fantasma es el 7 y su posición está dentro de la caja, por lo tanto el fantasma está encerrado
			vecFantasmas[contador]=n;//Fantasma encerrado
			contador++;//Total de fantasmas
		}
		else if(n==8 && vecPos[0]==9 && vecPos[1]==15){
			vecFantasmas[contador]=n;//Fantasma encerrado
			contador++;//Total de fantasmas
		}
		else if(n==9 && vecPos[0]==9 && vecPos[1]==14){
			vecFantasmas[contador]=n;//Fantasma encerrado
			contador++;//Total de fantasmas
		}
		else if(n==10 && vecPos[0]==10 && vecPos[1]==14){
			vecFantasmas[contador]=n;//Fantasma encerrado
			contador++;//Total de fantasmas
		}	
	}
	//Sacamos al fantasma si y sólo si la vez del ciclo principal de los fantasmas es mayor a la primera vez en la que deben de salir los fantasmas
	for(int x=0;x<contador;x++){
		if(vecFantasmas[x]==7 && *pvez>41){
			sacarFantasmas(matrizJuego,7);
		}
		else if(vecFantasmas[x]==8 && *pvez>71){
			sacarFantasmas(matrizJuego,8);
		}
		else if(vecFantasmas[x]==9 && *pvez>100){
			sacarFantasmas(matrizJuego,9);
		}
		else if(vecFantasmas[x]==10 && *pvez>6){
			sacarFantasmas(matrizJuego,10);
		}
	}

}

int morirPacman(int matrizJuego[][COL],int posI,int posJ,int *pvez,int *pcomida,int *pQuien){
	//Esta función ayuda a veriricar si Pacman murió desde la perspectiva de la función MovimientoPacman
	if(matrizJuego[posI][posJ]==7 || matrizJuego[posI][posJ]==8 || matrizJuego[posI][posJ]==9 || matrizJuego[posI][posJ]==10){//murió Pacman y hay que mandar a todos los fantamas, retornamos 1
		switch(matrizJuego[posI][posJ]){
			case 7:
				*pQuien=7;
				break;
			case 8:
				*pQuien=8;
				break;
			case 9:
				*pQuien=9;
				break;
			case 10:
				*pQuien=10;
				break;
		}
		return 1;//Retorna un 1 SI y SOLO SI si el pacman se va a mover a la posición en donde haya un fantasma
	}
	else{
		return 0;	
	}

}

int morirPacmanF(int matrizJuego[][COL],int posI, int posJ,int *pvez,int tipo,int *pcomida,int *pVidas){
	//Retorna un 0 si el fantasma no toca a pacman y un 1 si si
	//Esta función ayuda a veriricar si Pacman murió desde la perspectiva de las funciones de los fantasmas
	if(matrizJuego[posI][posJ]==0){//El fantasma toca a pacman, por lo tanto Pacman podría morir 
		return 1;	
	}
	else{
		return 0;	
	}
}

void reiniciarFantasmas(int matrizJuego[][COL],int *pvez){
	//Reinicia a los fantasmas
	int vecPos[2];
	//Borramos a todos los fantasmas del mapa. Cada que muera Pacman, la posición en donde estaban los fantasmas queda con un punto
	buscarPosicion(matrizJuego,7,vecPos);
	matrizJuego[vecPos[0]][vecPos[1]]=5;
	buscarPosicion(matrizJuego,8,vecPos);
	matrizJuego[vecPos[0]][vecPos[1]]=5;
	buscarPosicion(matrizJuego,9,vecPos);
	matrizJuego[vecPos[0]][vecPos[1]]=5;
	buscarPosicion(matrizJuego,10,vecPos);
	matrizJuego[vecPos[0]][vecPos[1]]=5;
	//Metemos a todos los fantasmas a su casa
	matrizJuego[10][15]=7;
	matrizJuego[9][15]=8;
	matrizJuego[9][14]=9;
	matrizJuego[10][14]=10;
	*pvez=0;//Reiniciamos el contador para que  los fantasmas vuelvan a salir de forma ordenada  
	
}

void fantasmas(int matrizJuego[][COL],int vez,int vecPos[],int *pvez,int *pcomida,int *pvidas,int *pDirF1,int *pDirF2,int *pDirF3,int *pDirF4,int pPausaF[],int dificultad){
	//Va a llamar al movimiento a todos los fantasmas
	//Mecanismo de control de aparición de los fantasmas
	if(vez>5 && vez<40){
		fantasmaRandom(matrizJuego,vez,vecPos,pvez,pcomida,pvidas,pDirF4,pPausaF,dificultad);	
	}
	else if(vez>40 && vez<70){//permitimos un descanso
		fantasmaRandom(matrizJuego,vez,vecPos,pvez,pcomida,pvidas,pDirF4,pPausaF,dificultad);	
		fantasmaFast(matrizJuego,vez,vecPos,pvez,pcomida,pvidas,pDirF1,pPausaF,dificultad);	
	}
	else if(vez>70 && vez<=71){//permitimos un descanso
		fantasmaRandom(matrizJuego,vez,vecPos,pvez,pcomida,pvidas,pDirF4,pPausaF,dificultad);	
		fantasmaFast(matrizJuego,vez,vecPos,pvez,pcomida,pvidas,pDirF1,pPausaF,dificultad);
		fantasmaLento(matrizJuego,vez,vecPos,pvez,pcomida,pvidas,pDirF2,pPausaF,dificultad);
	}
	else if(vez>72 && vez<100){
		fantasmaRandom(matrizJuego,vez,vecPos,pvez,pcomida,pvidas,pDirF4,pPausaF,dificultad);
		fantasmaFast(matrizJuego,vez,vecPos,pvez,pcomida,pvidas,pDirF1,pPausaF,dificultad);
		if(vez%40==0){
			fantasmaLento(matrizJuego,vez,vecPos,pvez,pcomida,pvidas,pDirF2,pPausaF,dificultad);
		}
	}
	else if(vez>=100){
		fantasmaRandom(matrizJuego,vez,vecPos,pvez,pcomida,pvidas,pDirF4,pPausaF,dificultad);
		fantasmaFast(matrizJuego,vez,vecPos,pvez,pcomida,pvidas,pDirF1,pPausaF,dificultad);
		if(vez%40==0){
			fantasmaLento(matrizJuego,vez,vecPos,pvez,pcomida,pvidas,pDirF2,pPausaF,dificultad);
		}	
		if(vez%5==0){
			fantasmaContra(matrizJuego,vez,vecPos,pvez,pcomida,pvidas,pDirF3,pPausaF,dificultad);
		}
	}
}

void fantasmaFast(int matrizJuego[][COL],int vez,int vecPos[],int *pvez,int *pcomida,int *pvidas,int *pDirF1,int pPausaF[],int dificultad){
	//Controla el movimiento del Fantasma tipo 1  --> Matriz=7
	SAMPLE *muertePacman=load_sample("pacman-dies.wav");
	SAMPLE *muerteFantasma=load_sample("pacman-eating-ghost.wav");
	int direccion,direccion2,x=0,posIF,posJF,valorPre,posIP,posJP,decision,auxIF,auxJF;
	if(vez==41){//Hay que sacarlo de la caja por primera vez
		sacarFantasmas(matrizJuego,7);
		matrizJuego[10][15]=2;//Borramos al fantasma de su posición original
		buscarPosicion(matrizJuego,7,vecPos);//Buscamos el lugar en donde está el fantasma con el número7 
		posIF=vecPos[0];
		posJF=vecPos[1];
	}
	else{
		buscarPosicion(matrizJuego,7,vecPos);//Buscamos el lugar en donde está el fantasma con el número7 
		posIF=vecPos[0];
		posJF=vecPos[1];
	}
	
	if(pPausaF[0]==1 && poderActivo(pcomida,dificultad)==0){//Quiere decir que debemos de apagar la pausa, porque el fantasma ya se puede mover, pero hay que sacarlo primero de su casa
		pPausaF[0]=0;
		sacarFantasmasMuertos(matrizJuego,pvez);
		matrizJuego[10][15]=2;//Borramos al fantasma de la posición en donde está
		buscarPosicion(matrizJuego,7,vecPos);//Buscamos el lugar en donde está el fantasma con el número 10 
		posIF=vecPos[0];
		posJF=vecPos[1];
	}
	
	if(pPausaF[0]==0){
		buscarPosicion(matrizJuego,0,vecPos);//Buscamos la posición de pacman
		posIP=vecPos[0];//Fila del Pacman
		posJP=vecPos[1];//Columna del Pacman
		
		if(*pcomida>=1){//El poder está encendido, los fantasmas deben de huir
			posIP=posIP-10;
			posJP=posJP-10;
		}	
	
		if(posIP>posIF){//Quiere decir que el pacman está más abajo
			direccion=4;//Hay que mover hacia abajo el fantasma
		}
		else if(posIP<posIF){//Quiere decir que el pacman está arriba
			direccion=3;//Hay que mover hacia arriba el fantasma
		}
		else{//Están en la misma fila
				direccion=0;//No tiene caso que se mueva de fila
		}
		if(posJP > posJF){//Quiere decir que el pacman  está a la derecha
			direccion2=1;
		}
		else if(posJP < posJF){//Quiere decir que el pacman está a la izquierda
			direccion2=2;
		}
		else{//Están en la misma columna
			direccion2=0;//Hay que dejarlo en la misma columna
		}
			
		//El orden de prioridad del movimiento debe de ser --> derecha, arriba, izquierda, abajo
		if(direccion2==1 && matrizJuego[posIF][posJF+1]!=1){//Se puede y se debe mover a la derecha
			*pDirF1=2;//Indica la dirección en la que la imagen del fantasma se mueve en pantalla
			valorPre=valorPreFantasma(matrizJuego,posIF,posJF+1);//Obtenemos el valor que estaba en la matriz antes de que el fantasma se moviera
			if(morirPacmanF(matrizJuego,posIF,posJF+1,pvez,7,pcomida,pvidas)==0){//Puede pasar sin retornar a pacman, el pacman no muere
				matrizJuego[posIF][posJF+1]=7;//Se hace el movimiento del fantasma
				matrizJuego[posIF][posJF]=valorPre;//Se asigna el valorpre a la posición en donde estaba el fantasma
			}
			else{//El pacman podría perder una vida o el fantasma podría perder una vida
				if(poderActivo(pcomida,dificultad)==0){//El poder está desactivado, por lo tanto Pacman muere
					play_sample(muertePacman,200,150,1000,0);//Sonido de la muerte de pacman activado
					reiniciarFantasmas(matrizJuego,pvez);//Metemos a los fantasmas a su casa
					reiniciarPacman(matrizJuego,pvidas);//Retornamos al pacman a su lugar de origen y lo borramos de su posición actual
				}
				else{//El poder está activo, el fantasma que toca Pacman, muere
					play_sample(muerteFantasma,200,150,1000,0);//Sonido de la muerte de un fantasma activado
					meterFantasmas(matrizJuego,7);//Metemos al fantasma correspondiente a su lugar en la caja
					pPausaF[0]=1;//Pausamos al fantasma correspondiente			
				}
			}
		}
		//Mismo procedimiento pero ahora considerando diferentes direcciones
		else if(direccion==3 && matrizJuego[posIF-1][posJF]!=1){//Se puede y se debe mover hacia arriba
			*pDirF1=4;
			valorPre=valorPreFantasma(matrizJuego,posIF-1,posJF);
			if(morirPacmanF(matrizJuego,posIF-1,posJF,pvez,7,pcomida,pvidas)==0){//Puede pasar sin retornar a pacman
				matrizJuego[posIF-1][posJF]=7;
				matrizJuego[posIF][posJF]=valorPre;
			}	
			else{
				if(poderActivo(pcomida,dificultad)==0){//El poder está desactivado, por lo tanto Pacman muere
					play_sample(muertePacman,200,150,1000,0);
					reiniciarFantasmas(matrizJuego,pvez);
					reiniciarPacman(matrizJuego,pvidas);
				}
				else{//El poder está activo, el fantasma que toca Pacman, muere
					play_sample(muerteFantasma,200,150,1000,0);
					meterFantasmas(matrizJuego,7);
					pPausaF[0]=1;
				}
			}
		}	
		else if(direccion2==2 && matrizJuego[posIF][posJF-1]!=1){//Se debe y se puede mover a la izquierda
			*pDirF1=1;
			valorPre=valorPreFantasma(matrizJuego,posIF,posJF-1);
			if(morirPacmanF(matrizJuego,posIF,posJF-1,pvez,7,pcomida,pvidas)==0){//Puede pasar sin retornar a pacman
				matrizJuego[posIF][posJF-1]=7;
				matrizJuego[posIF][posJF]=valorPre;
			}
			else{
				if(poderActivo(pcomida,dificultad)==0){//El poder está desactivado, por lo tanto Pacman muere
					play_sample(muertePacman,200,150,1000,0);
					reiniciarFantasmas(matrizJuego,pvez);
					reiniciarPacman(matrizJuego,pvidas);
				}
				else{//El poder está activo, el fantasma que toca Pacman, muere
					play_sample(muerteFantasma,200,150,1000,0);
					meterFantasmas(matrizJuego,7);
					pPausaF[0]=1;
				}
			}
		}
		else if(direccion==4 && matrizJuego[posIF+1][posJF]!=1){//Se debe y se puede mover hacia abajo
			*pDirF1=3;
			valorPre=valorPreFantasma(matrizJuego,posIF+1,posJF);
			if(morirPacmanF(matrizJuego,posIF+1,posJF,pvez,7,pcomida,pvidas)==0){//Puede pasar sin retornar a pacman
				matrizJuego[posIF+1][posJF]=7;
				matrizJuego[posIF][posJF]=valorPre;
			}	
			else{
				if(poderActivo(pcomida,dificultad)==0){//El poder está desactivado, por lo tanto Pacman muere
					play_sample(muertePacman,200,150,1000,0);
					reiniciarFantasmas(matrizJuego,pvez);
					reiniciarPacman(matrizJuego,pvidas);
				}
				else{//El poder está activo, el fantasma que toca Pacman, muere
					play_sample(muerteFantasma,200,150,1000,0);
					meterFantasmas(matrizJuego,7);
					pPausaF[0]=1;
				}
			}
		}
		else{//No hay muchas posibilidades
		}
	}
}

void fantasmaLento(int matrizJuego[][COL],int vez,int vecPos[],int *pvez,int *pcomida,int *pvidas,int *pDirF2,int pPausaF[],int dificultad){
	//Controla el movimiento del Fantasma tipo2 --> Matriz=8
	SAMPLE *muerteFantasma=load_sample("pacman-eating-ghost.wav");
	SAMPLE *muertePacman=load_sample("pacman-dies.wav");
	int direccion,x,posIF,posJF,valorPre,posIP,posJP,direccion2;	
	if(vez==71){
		sacarFantasmas(matrizJuego,8);
		matrizJuego[9][15]=2;//Borramos al fantasma de su caja
		buscarPosicion(matrizJuego,8,vecPos);//Buscamos el lugar en donde está el fantasma con el número8 
		posIF=vecPos[0];
		posJF=vecPos[1];
	}
	else{
		buscarPosicion(matrizJuego,8,vecPos);
		posIF=vecPos[0];
		posJF=vecPos[1];
	}
	
	if(pPausaF[1]==1 && poderActivo(pcomida,dificultad)==0){//Quiere decir que debemos de apagar la pausa, porque el fantasma ya se puede mover, pero hay que sacarlo primero de su casa
		pPausaF[1]=0;
		sacarFantasmasMuertos(matrizJuego,pvez);
		matrizJuego[9][15]=2;//Borramos al fantasma de la posición en donde está
		buscarPosicion(matrizJuego,8,vecPos);//Buscamos el lugar en donde está el fantasma con el número 10 
		posIF=vecPos[0];
		posJF=vecPos[1];
	}
	
	if(pPausaF[1]==0){
		buscarPosicion(matrizJuego,0,vecPos);//Buscamos la posición de pacman
		posIP=vecPos[0];//Fila del Pacman
		posJP=vecPos[1];//Columna del Pacman
		
		if(*pcomida>=1){//El poder está encendido, los fantasmas deben de huir
			posIP=posIP+5;
			posJP=posJP+5;
		}	
		
		//Mecanismo de seguimiento
		if(posIP>posIF){//Quiere decir que el pacman está más abajo
			direccion=4;//Hay que mover hacia abajo el fantasma
		}
		else if(posIP<posIF){//Quiere decir que el pacman está arriba
			direccion=3;//Hay que mover hacia arriba el fantasma
		}
		else{//Están en la misma fila
				direccion=0;//No tiene caso que se mueva de fila
		}
		if(posJP > posJF){//Quiere decir que el pacman  está a la derecha
			direccion2=1;
		}
		else if(posJP < posJF){//Quiere decir que el pacman está a la izquierda
			direccion2=2;
		}
		else{//Están en la misma columna
			direccion2=0;//Hay que dejarlo en la misma columna
		}
			
		//El orden de prioridad del movimiento debe de ser --> derecha, arriba, izquierda, abajo
		//Mismo procedimiento que el Fantasma Fast pero ahora considerando al fantasma 8
		if(direccion2==1 && matrizJuego[posIF][posJF+1]!=1){//Se puede y se debe mover a la derecha
				*pDirF2=2;
				valorPre=valorPreFantasma(matrizJuego,posIF,posJF+1);
				if(morirPacmanF(matrizJuego,posIF,posJF+1,pvez,8,pcomida,pvidas)==0){//Pacman no muere, el fantasma se puede mover a su gusto
					matrizJuego[posIF][posJF+1]=8;
					matrizJuego[posIF][posJF]=valorPre;
				}
				else{//Regresa un 1, por lo tanto pacman  podría morir o el fantasma podría morir
					if(poderActivo(pcomida,dificultad)==0){//El poder está desactivado, por lo tanto Pacman muere
						play_sample(muertePacman,200,150,1000,0);
						reiniciarFantasmas(matrizJuego,pvez);
						reiniciarPacman(matrizJuego,pvidas);
					}
					else{//El poder está activo, el fantasma que toca Pacman, muere
					play_sample(muerteFantasma,200,150,1000,0);
					meterFantasmas(matrizJuego,8);
					pPausaF[1]=1;
						matrizJuego[posIF][posJF]=2;				
					}
					
				}	
		}
		
		else if(direccion==3 && matrizJuego[posIF-1][posJF]!=1){//Se puede y se debe mover hacia arriba
				*pDirF2=4;
				valorPre=valorPreFantasma(matrizJuego,posIF-1,posJF);
				if(morirPacmanF(matrizJuego,posIF-1,posJF,pvez,8,pcomida,pvidas)==0){//Puede pasar sin retornar a pacman
					matrizJuego[posIF-1][posJF]=8;
					matrizJuego[posIF][posJF]=valorPre;
				}	
				else{//Regresa un 1, por lo tanto pacman  podría morir
					
					if(poderActivo(pcomida,dificultad)==0){//El poder está desactivado, por lo tanto Pacman muere
						play_sample(muertePacman,200,150,1000,0);	
						reiniciarFantasmas(matrizJuego,pvez);
						reiniciarPacman(matrizJuego,pvidas);
					}
					else{//El poder está activo, el fantasma que toca Pacman, muere
					play_sample(muerteFantasma,200,150,1000,0);
					meterFantasmas(matrizJuego,8);
					pPausaF[1]=1;
						matrizJuego[posIF][posJF]=2;
					}
				}	
		}	
		else if(direccion2==2 && matrizJuego[posIF][posJF-1]!=1){//Se debe y se puede mover a la izquierda
				*pDirF2=1;
				valorPre=valorPreFantasma(matrizJuego,posIF,posJF-1);
				if(morirPacmanF(matrizJuego,posIF,posJF-1,pvez,8,pcomida,pvidas)==0){//Puede pasar sin retornar a pacman
					matrizJuego[posIF][posJF-1]=8;
					matrizJuego[posIF][posJF]=valorPre;
				}	
				else{//Regresa un 1, por lo tanto pacman  podría morir
					
					if(poderActivo(pcomida,dificultad)==0){//El poder está desactivado, por lo tanto Pacman muere
						play_sample(muertePacman,200,150,1000,0);
						reiniciarFantasmas(matrizJuego,pvez);
						reiniciarPacman(matrizJuego,pvidas);
					}
					else{//El poder está activo, el fantasma que toca Pacman, muere
					play_sample(muerteFantasma,200,150,1000,0);
						meterFantasmas(matrizJuego,8);
						pPausaF[1]=1;
						matrizJuego[posIF][posJF]=2;
					}
					
				}	
		}
		else if(direccion==4 && matrizJuego[posIF+1][posJF]!=1){//Se debe y se puede mover hacia abajo
				*pDirF2=3;
				valorPre=valorPreFantasma(matrizJuego,posIF+1,posJF);
				if(morirPacmanF(matrizJuego,posIF+1,posJF,pvez,8,pcomida,pvidas)==0){//Puede pasar sin retornar a pacman
					matrizJuego[posIF+1][posJF]=8;
					matrizJuego[posIF][posJF]=valorPre;
				}	
				
				else{//Regresa un 1, por lo tanto pacman  podría morir
					
					if(poderActivo(pcomida,dificultad)==0){//El poder está desactivado, por lo tanto Pacman muere
						play_sample(muertePacman,200,150,1000,0);
						reiniciarFantasmas(matrizJuego,pvez);
						reiniciarPacman(matrizJuego,pvidas);
					}
					else{//El poder está activo, el fantasma que toca Pacman, muere
					play_sample(muerteFantasma,200,150,1000,0);
						meterFantasmas(matrizJuego,8);
						pPausaF[1]=1;
						matrizJuego[posIF][posJF]=2;
					}
					
				}	
		}
		else{//No hay muchas posibilidades
		
		}
	}	
}

void fantasmaContra(int matrizJuego[][COL],int vez,int vecPos[],int *pvez,int *pcomida,int *pvidas,int *pDirF3,int pPausaF[],int dificultad){
	//Controla el movimiento del Fantasma tipo3  --> Matriz=9 que va con un movimiento distinto al Fast por su orden de prioridad de movimiento
	//Mismo procedimiento que el Fantasma Fast pero ahora considerando al fantasma 9
	SAMPLE *muerteFantasma=load_sample("pacman-eating-ghost.wav");
	SAMPLE *muertePacman=load_sample("pacman-dies.wav");
	int direccion,x,posIF,posJF,valorPre,posIP,posJP,direccion2;
	if(vez == 100){//Primera vez, por lo tanto hay que sacar al fantasma de la caja
		sacarFantasmas(matrizJuego,9);
		matrizJuego[9][14]=2;//Borramos su posición dentro de la caja
		buscarPosicion(matrizJuego,9,vecPos);//Buscamos el lugar en donde está el fantasma con el número7 
		posIF=vecPos[0];
		posJF=vecPos[1];
	}
	else{
		buscarPosicion(matrizJuego,9,vecPos);//Buscamos al fantasma en el mapa
		posIF=vecPos[0];//Le asignamos lo que valga el VecPos a las filas
		posJF=vecPos[1];//Le asignamos lo que valga el VecPos a las columnas
	}
	
	if(pPausaF[2]==1 && poderActivo(pcomida,dificultad)==0){//Quiere decir que debemos de apagar la pausa, porque el fantasma ya se puede mover, pero hay que sacarlo primero de su casa
		pPausaF[2]=0;
		sacarFantasmasMuertos(matrizJuego,pvez);
		matrizJuego[9][14]=2;//Borramos al fantasma de la posición en donde está
		buscarPosicion(matrizJuego,9,vecPos);//Buscamos el lugar en donde está el fantasma con el número 10 
		posIF=vecPos[0];
		posJF=vecPos[1];
	}
	if(pPausaF[2]==0){
		buscarPosicion(matrizJuego,0,vecPos);//Buscamos a Pacman
		posIP=vecPos[0];//Le asignamos lo que valga el VecPos a las filas
		posJP=vecPos[1];//Le asignamos lo que valga el VecPos a las columnas
		if(*pcomida>=1){//El poder está encendido, los fantasmas deben de huir
			posIP=posIP-10;
			posJP=posJP-10;
		}	
		
		//Mecanismo de seguimiento
		if(posJP > posJF){//Quiere decir que el pacman  está a la derecha
			direccion2=1;
		}
		else if(posJP < posJF){//Quiere decir que el pacman está a la izquierda
			direccion2=2;
		}
		else{//Están en la misma columna
			direccion2=0;//Hay que dejarlo en la misma columna
		}
		
		if(posIP>posIF){//Quiere decir que el pacman está más abajo
			direccion=4;//Hay que mover hacia abajo el fantasma
		}
		else if(posIP<posIF){//Quiere decir que el pacman está arriba
			direccion=3;//Hay que mover hacia arriba el fantasma
		}
		else{//Están en la misma fila
			direccion=0;//No tiene caso que se mueva de fila
		}
		//El orden de prioridad del movimiento debe de ser --> Izquierda, abajo, derecha, arriba
		//Mismo procedimiento que el Fantasma Fast pero ahora considerando al fantasma 9
		if(direccion2==2 && matrizJuego[posIF][posJF-1]!=1){//Se debe y se puede mover a la izquierda
				*pDirF3=1;
				valorPre=valorPreFantasma(matrizJuego,posIF,posJF-1);
				if(morirPacmanF(matrizJuego,posIF,posJF-1,pvez,9,pcomida,pvidas)==0){//Puede pasar sin retornar a pacman
					matrizJuego[posIF][posJF-1]=9;
					matrizJuego[posIF][posJF]=valorPre;
				}
				else{//El pacman o el fantasma podrían perder una vida
					if(poderActivo(pcomida,dificultad)==0){//El poder está desactivado, por lo tanto Pacman muere
						play_sample(muertePacman,200,150,1000,0);
						reiniciarFantasmas(matrizJuego,pvez);
						reiniciarPacman(matrizJuego,pvidas);
					}
					else{//El poder está activo, el fantasma que toca Pacman, muere
					play_sample(muerteFantasma,200,150,1000,0);
						meterFantasmas(matrizJuego,9);
						pPausaF[2]=1;
						matrizJuego[posIF][posJF]=2;				
					}
				}
		}
		else if(direccion==4 && matrizJuego[posIF+1][posJF]!=1){//Se debe y se puede mover hacia abajo
				*pDirF3=3;
				valorPre=valorPreFantasma(matrizJuego,posIF+1,posJF);
				if(morirPacmanF(matrizJuego,posIF+1,posJF,pvez,9,pcomida,pvidas)==0){//Puede pasar sin retornar a pacman
					matrizJuego[posIF+1][posJF]=9;
					matrizJuego[posIF][posJF]=valorPre;
				}
				else{
					if(poderActivo(pcomida,dificultad)==0){//El poder está desactivado, por lo tanto Pacman muere
						play_sample(muertePacman,200,150,1000,0);
						reiniciarFantasmas(matrizJuego,pvez);
						reiniciarPacman(matrizJuego,pvidas);
					}
					else{//El poder está activo, el fantasma que toca Pacman, muere
					play_sample(muerteFantasma,200,150,1000,0);
						meterFantasmas(matrizJuego,9);
						pPausaF[2]=1;
						matrizJuego[posIF][posJF]=2;
					}
				}
		}
		else if(direccion2==1 && matrizJuego[posIF][posJF+1]!=1){//Se puede y se debe mover a la derecha
				*pDirF3=2;//Cambia direccion a la derecha
				valorPre=valorPreFantasma(matrizJuego,posIF,posJF+1);
				if(morirPacmanF(matrizJuego,posIF,posJF+1,pvez,9,pcomida,pvidas)==0){//Puede pasar sin retornar a pacman
					matrizJuego[posIF][posJF+1]=9;
					matrizJuego[posIF][posJF]=valorPre;
				}
				else{
					if(poderActivo(pcomida,dificultad)==0){//El poder está desactivado, por lo tanto Pacman muere
						play_sample(muertePacman,200,150,1000,0);
						reiniciarFantasmas(matrizJuego,pvez);
						reiniciarPacman(matrizJuego,pvidas);
					}
					else{//El poder está activo, el fantasma que toca Pacman, muere
					play_sample(muerteFantasma,200,150,1000,0);
						meterFantasmas(matrizJuego,9);
						pPausaF[2]=1;
						matrizJuego[posIF][posJF]=2;
					}
				}	
		}
		else if(direccion==3 && matrizJuego[posIF-1][posJF]!=1){//Se puede y se debe mover hacia arriba
				*pDirF3=1;
				valorPre=valorPreFantasma(matrizJuego,posIF-1,posJF);
				if(morirPacmanF(matrizJuego,posIF-1,posJF,pvez,9,pcomida,pvidas)==0){//Puede pasar sin retornar a pacman
					matrizJuego[posIF-1][posJF]=9;
					matrizJuego[posIF][posJF]=valorPre;
				}	
				else{
					if(poderActivo(pcomida,dificultad)==0){//El poder está desactivado, por lo tanto Pacman muere
						play_sample(muertePacman,200,150,1000,0);
						reiniciarFantasmas(matrizJuego,pvez);
						reiniciarPacman(matrizJuego,pvidas);
					}
					else{//El poder está activo, el fantasma que toca Pacman, muere
					play_sample(muerteFantasma,200,150,1000,0);
						meterFantasmas(matrizJuego,9);
						pPausaF[2]=1;
						matrizJuego[posIF][posJF]=2;
					}
				}
		}
		else{//No hay muchas posibilidades
		}
	}
}

void fantasmaRandom(int matrizJuego[][COL],int vez,int vecPos[],int *pvez,int *pcomida,int *pvidas,int *pDirF4,int pPausaF[],int dificultad){
	//Controla el movimiento del FantasmaTipo4 --> Matriz=10
	SAMPLE *muerteFantasma=load_sample("pacman-eating-ghost.wav");
	SAMPLE *muertePacman=load_sample("pacman-dies.wav");
	int direccion,x,posIF,posJF,valorPre;
	if(vez==6){//Es la primera vez que se generan a este fantasma, hay que sacarlo de su casa en orden
		sacarFantasmas(matrizJuego,10);//Sacamos al fantasma 10, esta función valida que su salida se dé correctamente
		matrizJuego[10][14]=2;//Borramos al fantasma de la posición en donde está
		buscarPosicion(matrizJuego,10,vecPos);//Buscamos el lugar en donde está el fantasma con el número 10 
		posIF=vecPos[0];//Fila del fantasma
		posJF=vecPos[1];//Columna del fantasma
	}
	else{
		buscarPosicion(matrizJuego,10,vecPos);
		posIF=vecPos[0];//Fila del fantasma
		posJF=vecPos[1];//Columna del fantasma
	}
	
	if(pPausaF[3]==1 && poderActivo(pcomida,dificultad)==0){//Quiere decir que debemos de apagar la pausa, porque el fantasma ya se puede mover, pero hay que sacarlo primero de su casa
		pPausaF[3]=0;
		sacarFantasmasMuertos(matrizJuego,pvez);
		matrizJuego[10][14]=2;//Borramos al fantasma de la posición en donde está
		buscarPosicion(matrizJuego,10,vecPos);//Buscamos el lugar en donde está el fantasma con el número 10 
		posIF=vecPos[0];
		posJF=vecPos[1];
	}
	
	if(pPausaF[3]==0){//El  fantasma se puede mover libremente
		srand(time(NULL));
		direccion=rand()%4+1;//Generamos random un número para que se mueva 1=Derecha 2=Izquierda 3=Arriba 4=Abajo
		switch(direccion){
		case 1://Derecha
			*pDirF4=2;//Apunta a la derecha en pintar mapa
			posJF++;
			valorPre=valorPreFantasma(matrizJuego,posIF,posJF);//Guarda el valor que tiene la matriz ANTES de que el fantasma pase, para evitar que el fantasma también coma puntos o frutas
			x=verificarTeleport(matrizJuego,posIF,posJF,10,valorPre);
	    	if(x==0){//No hay teleport
		        if(matrizJuego[posIF][posJF]==1 || matrizJuego[posIF][posJF]==3){//Quiere decir que el paso no es libre
				}
				else{//El fantasma se pudo mover, hay que validar sus movimientos
					if(morirPacmanF(matrizJuego,posIF,posJF,pvez,10,pcomida,pvidas)==0){//Puede pasar sin retornar a Pacman
					    matrizJuego[posIF][posJF]=10;//Movemos al fantasma
		          		matrizJuego[posIF][posJF-1]=valorPre;//Reiniciamos el valor que tenía antes de que el fantasma pasara			
					}
					else{
						if(poderActivo(pcomida,dificultad)==0){//El poder está desactivado, por lo tanto Pacman muere
							play_sample(muertePacman,200,150,1000,0);//Sonido de muerte de pacman activado
							reiniciarFantasmas(matrizJuego,pvez);//Metemos a los fantasmas a su casa
							reiniciarPacman(matrizJuego,pvidas);//Reiniciamos al pacman y borramos su posición
						}
						else{//El poder está activo, el fantasma que toca Pacman, el fantasma muere y hay que pausarlo
							play_sample(muerteFantasma,200,150,1000,0);
							pPausaF[3]=1;//Pausamos al fantasma número 4
							meterFantasmas(matrizJuego,10);//Metemos al fantasma a su posición en casa
							matrizJuego[posIF][posJF-1]=2;//Borramos la posición donde estaba el fantasma
						}
					}
				}
			}
	        break;
	    //Mismo proceso pero ahora evaluando otras direcciones
	    case 2://Izquierda
	    	posJF--;
	    	*pDirF4=1;//Apunta a la derecha en pintar mapa
	    	valorPre=valorPreFantasma(matrizJuego,posIF,posJF);
	  	 	x=verificarTeleport(matrizJuego,posIF,posJF,10,valorPre);
	    	if(x==0){//No hay teleport
		        if(matrizJuego[posIF][posJF]==1 || matrizJuego[posIF][posJF]==3){//Quiere decir que el paso es libre
				}
				else{//Quiere decir que llegó al marco, hay que cambiar la dirección de forma automática hacia otro lado
					if(morirPacmanF(matrizJuego,posIF,posJF,pvez,10,pcomida,pvidas)==0){//Puede pasar sin retornar a Pacman
					    matrizJuego[posIF][posJF]=10;
		            	matrizJuego[posIF][posJF+1]=valorPre;//Reiniciamos el valor que tenía antes de que el fantasma pasara			
					}
					else{
						if(poderActivo(pcomida,dificultad)==0){//El poder está desactivado, por lo tanto Pacman muere
							play_sample(muertePacman,200,150,1000,0);
							reiniciarFantasmas(matrizJuego,pvez);
							reiniciarPacman(matrizJuego,pvidas);
						}
						else{//El poder está activo, el fantasma que toca Pacman, muere
							play_sample(muerteFantasma,200,150,1000,0);
							pPausaF[3]=1;//Pausamos al fantasma número 4
							meterFantasmas(matrizJuego,10);
							matrizJuego[posIF][posJF+1]=2;	
						}
					}
				}
			}
			break;
		case 3://Arriba
	    	posIF--;
	    	*pDirF4=4;//Apunta a arriba en pintar mapa
	    	valorPre=valorPreFantasma(matrizJuego,posIF,posJF);
			x=verificarTeleport(matrizJuego,posIF,posJF,10,valorPre);
	    	if(x==0){//No hay teleport
		        if(matrizJuego[posIF][posJF]==1 || matrizJuego[posIF][posJF]==3){//Quiere decir que el paso no es libre
				}
				else{
					if(morirPacmanF(matrizJuego,posIF,posJF,pvez,10,pcomida,pvidas)==0){//Puede pasar sin retornar a Pacman
					    matrizJuego[posIF][posJF]=10;
		         	    matrizJuego[posIF+1][posJF]=valorPre;//Reiniciamos el valor que tenía antes de que el fantasma pasara		
					}
					else{
						if(poderActivo(pcomida,dificultad)==0){//El poder está desactivado, por lo tanto Pacman muere
							play_sample(muertePacman,200,150,1000,0);
							reiniciarFantasmas(matrizJuego,pvez);
							reiniciarPacman(matrizJuego,pvidas);
						}
						else{//El poder está activo, el fantasma que toca Pacman, muere
							play_sample(muerteFantasma,200,150,1000,0);
							pPausaF[3]=1;//Pausamos al fantasma número 4
							meterFantasmas(matrizJuego,10);
							matrizJuego[posIF+1][posJF]=2;	
						}
					}
				}
			}
			break;
		case 4://Abajo
	    	posIF++;
	    	*pDirF4=3;//Apunta a abajo en pintar mapa
	    	valorPre=valorPreFantasma(matrizJuego,posIF,posJF);
			x=verificarTeleport(matrizJuego,posIF,posJF,10,valorPre);
	    	if(x==0){//No hay teleport
		        if(matrizJuego[posIF][posJF]==1 || matrizJuego[posIF][posJF]==3){//Quiere decir que el paso no es libre
				}
				else{ //Quiere decir que el paso es libre
					//Verificamos que el fantasma toque a Pacman
					if(morirPacmanF(matrizJuego,posIF,posJF,pvez,10,pcomida,pvidas)==0){//Puede pasar sin retornar a Pacman
						matrizJuego[posIF][posJF]=10;
		           		matrizJuego[posIF-1][posJF]=valorPre;//Reiniciamos el valor que tenía antes de que el fantasma pasara
					}
					else{
						if(poderActivo(pcomida,dificultad)==0){//El poder está desactivado, por lo tanto Pacman muere
							play_sample(muertePacman,200,150,1000,0);
							reiniciarFantasmas(matrizJuego,pvez);
							reiniciarPacman(matrizJuego,pvidas);
						}
						else{//El poder está activo, el fantasma que toca Pacman, muere
							play_sample(muerteFantasma,200,150,1000,0);
							pPausaF[3]=1;//Pausamos al fantasma número 4
							meterFantasmas(matrizJuego,10);
							matrizJuego[posIF-1][posJF]=2;	
						}
					}
				}
			}
			break;
	}
	}
	
	
}

void mapa1(int matrizJuego[][COL]){
	//Se carga el mapa tipo 1
	int matrizJuegoAux[20][30]{
	//   0 1 2 3 4 5 6 7 8 9 10        15        20        25      29
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},//0
		{1,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,1},//1
		{1,2,1,2,1,1,2,1,1,1,1,2,1,2,1,1,2,1,2,1,1,1,1,2,1,1,2,1,2,1},//2									
		{1,2,1,2,1,1,2,1,1,2,2,2,1,2,2,2,2,1,2,2,2,1,1,2,1,1,2,1,2,1},//3
		{1,2,1,2,1,1,2,2,2,2,1,2,1,2,1,1,2,1,2,1,2,2,2,2,1,1,2,1,2,1},//4										
		{1,2,2,2,2,2,2,1,2,1,1,2,1,2,2,2,2,1,2,1,1,2,1,2,2,2,2,2,2,1},//5											
		{1,2,1,1,2,1,1,1,2,2,1,2,1,2,1,1,2,1,2,1,2,2,1,1,1,2,1,1,2,1},//6
		{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},//7
		{1,2,1,1,1,2,1,2,1,1,1,1,2,1,1,1,1,2,1,1,1,1,2,1,2,1,1,1,2,1},//8
		{1,2,1,1,1,2,1,2,1,1,1,1,2,1,9,8,1,2,1,1,1,1,2,1,2,1,1,1,2,1},//9
		{2,2,2,2,2,2,1,2,2,1,1,1,2,1,10,7,1,2,1,1,1,2,2,1,2,2,2,2,2,2},//10
		{1,2,1,1,1,2,1,1,2,2,2,2,2,1,1,1,1,2,2,2,2,2,1,1,2,1,1,1,2,1},//11
		{1,2,1,2,2,2,2,2,2,1,1,1,2,2,2,2,2,2,1,1,1,2,2,2,2,2,2,1,2,1},//12
		{1,2,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,2,1},//13
		{1,2,1,2,1,2,2,2,1,2,2,2,2,2,0,2,2,2,2,2,2,1,2,2,2,1,2,1,2,1},//14
		{1,2,1,2,2,2,1,2,2,2,1,1,2,1,1,1,1,2,1,1,2,2,2,1,2,2,2,1,2,1},//15
		{1,2,1,2,1,2,2,2,1,2,1,1,2,2,2,2,2,2,1,1,2,1,2,2,2,1,2,1,2,1},//16
		{1,2,1,2,1,1,2,1,1,2,1,1,2,1,1,1,1,2,1,1,2,1,1,2,1,1,2,1,2,1},//17
		{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},//18
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},//19
	};	
	
	for(int i=0;i<20;i++){//Este FOR reemplaza los 2 por 5 y 6 para comenzar el mapa lleno de puntos.
		for(int j=0;j<30;j++){
			if( (i==1 && j==1) || (i==1 && j==28) || (i==18 && j==1) || (i==18 && j==28)){
				matrizJuegoAux[i][j]=6;//Asigna los puntos grandes
			}
			else if(matrizJuegoAux[i][j]==2){//Asigna los puntos chicos
				matrizJuegoAux[i][j]=5;
			}
		}
	}
	
	for(int i=0;i<20;i++){//Hacemos una copia directa hacia la matriz de Juego
		for(int j=0;j<30;j++){
			matrizJuego[i][j]=matrizJuegoAux[i][j];
		}
	}
	
}

void mapa2(int matrizJuego[][COL]) {
	//Se carga el mapa tipo 2
	int matrizJuegoAux[FIL][COL]={
   //1 2 3 4 5 6 7 8 9 1 1 2 3 4 5 6 7 8 9 2 1 2 3 4 5 6 7 8 9 3
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},//1
	{1,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,1},//2
	{1,2,1,1,1,1,2,1,2,1,2,1,1,2,2,2,2,1,1,2,1,2,1,2,1,1,1,1,2,1},//3									
	{1,2,1,2,2,2,2,1,2,1,2,1,1,2,1,1,2,1,1,2,1,2,1,2,2,2,2,1,2,1},//4
	{1,2,1,2,1,1,2,1,2,2,2,1,2,2,2,2,2,2,1,2,2,2,1,2,1,1,2,1,2,1},//5										
	{1,2,1,2,1,1,2,1,2,1,2,1,1,2,1,1,2,1,1,2,1,2,1,2,1,1,2,1,2,1},//6											
	{1,2,2,2,2,2,2,2,2,2,2,1,1,2,1,1,2,1,1,2,2,2,2,2,2,2,2,2,2,1},//7
	{1,2,1,1,1,1,1,2,1,1,2,2,2,2,2,2,2,2,2,2,1,1,2,1,1,1,1,1,2,1},//8
	{1,2,2,2,2,2,2,2,2,2,2,1,2,1,1,1,1,2,1,2,2,2,2,2,2,2,2,2,2,1},//9
	{1,2,1,2,1,1,1,1,1,1,2,1,2,1,9,8,1,2,1,2,1,1,1,1,1,1,2,1,2,1},//10
	{2,2,2,2,2,2,2,2,2,2,2,1,2,1,10,7,1,2,1,2,2,2,2,2,2,2,2,2,2,2},//11
	{1,2,1,1,1,1,1,1,1,1,2,2,2,1,1,1,1,2,2,2,1,1,1,1,1,1,1,1,2,1},//12
	{1,2,1,2,2,2,2,2,2,1,2,1,2,2,2,2,2,2,1,2,1,2,2,2,2,2,2,1,2,1},//13
	{1,2,2,2,1,1,1,1,2,1,2,1,2,1,1,1,1,2,1,2,1,2,1,1,1,1,2,2,2,1},//14
	{1,2,1,2,2,2,2,2,2,2,2,2,2,2,0,2,2,2,2,2,2,2,2,2,2,2,2,1,2,1},//15
	{1,2,1,2,1,1,1,2,1,2,1,2,1,1,1,1,1,1,2,1,2,1,2,1,1,1,2,1,2,1},//16
	{1,2,1,2,2,2,2,2,1,2,2,2,1,2,2,2,2,1,2,2,2,1,2,2,2,2,2,1,2,1},//17
	{1,2,1,1,1,1,1,2,1,2,1,2,1,2,1,1,2,1,2,1,2,1,2,1,1,1,1,1,2,1},//18
	{1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},//19
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},//20
	};
	for(int i=0;i<20;i++){//Este FOR reemplaza los 2 por 5 y 6 para comenzar el mapa lleno de puntos.
		for(int j=0;j<30;j++){
			if( (i==1 && j==1) || (i==1 && j==28) || (i==18 && j==1) || (i==18 && j==28)){
				matrizJuegoAux[i][j]=6;//ASIGNACIÓN DE LOS PUNTOS GRANDES
			}
			else if(matrizJuegoAux[i][j]==2){
				matrizJuegoAux[i][j]=5;
			}
		}
	}
	
	for(int i=0;i<20;i++){//Hacemos una copia directa
		for(int j=0;j<30;j++){
			matrizJuego[i][j]=matrizJuegoAux[i][j];
		}
	}
}

void mapa3(int matrizJuego[][COL]) {
	//Se carga el mapa tipo 3
	int matrizJuegoAux[FIL][COL]={
   //0 1 2 3 4 5 6 7 8 9 1 1 2 3 4 5 6 7 8 9 2 1 2 3 4 5 6 7 8 9 
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},//0
	{1,1,1,2,2,1,1,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,1,1,2,2,1,1,1},//1
	{1,1,2,2,2,1,2,2,1,2,1,1,1,2,2,2,2,1,1,1,2,1,2,2,1,2,2,2,1,1},//2									
	{1,2,2,1,2,1,2,1,1,2,2,1,1,2,1,1,2,1,1,2,2,1,1,2,1,2,1,2,2,1},//4
	{1,2,1,1,2,1,2,1,1,2,1,2,1,2,1,1,2,1,2,1,2,1,1,2,1,2,1,1,2,1},//5										
	{1,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,1},//6											
	{1,2,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,2,1},//7
	{1,2,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,1},//8
	{1,2,1,2,1,1,1,2,1,2,1,1,2,1,1,1,1,2,1,1,2,1,2,1,1,1,2,1,2,1},//9
	{1,2,1,2,1,1,1,2,1,2,2,2,2,1,9,10,1,2,2,2,2,1,2,1,1,1,2,1,2,1},//10
	{2,2,2,2,1,1,1,2,1,1,1,1,2,1,8,7,1,2,1,1,1,1,2,1,1,1,2,2,2,2},//11
	{1,2,1,2,1,1,1,2,1,1,1,1,2,1,1,1,1,2,1,1,1,1,2,1,1,1,2,1,2,1},//12
	{1,2,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,1},//13
	{1,2,1,2,1,2,1,2,1,2,1,1,1,1,1,1,1,1,1,1,2,1,2,1,2,1,2,1,2,1},//14
	{1,2,1,2,2,2,2,2,1,2,2,2,2,2,0,2,2,2,2,2,2,1,2,2,2,2,2,1,2,1},//15
	{1,2,2,2,1,2,1,2,1,2,2,1,2,1,1,1,1,2,1,2,2,1,2,1,2,1,2,2,2,1},//15
	{1,1,1,1,2,2,2,2,2,2,1,1,2,1,2,2,1,2,1,1,2,2,2,2,2,2,1,1,1,1},//16
	{1,2,2,1,2,1,1,1,1,2,1,1,2,2,2,2,2,2,1,1,2,1,1,1,1,2,1,2,2,1},//17
	{1,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,1},//18
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},//19
	};
	for(int i=0;i<20;i++){//Este FOR reemplaza los 2 por 5 y 6 para comenzar el mapa lleno de puntos.
		for(int j=0;j<30;j++){
			if( (i==1 && j==3) || (i==1 && j==26) || (i==17 && j==2) || (i==17 && j==28)){
				matrizJuegoAux[i][j]=6;//ASIGNACIÓN DE LOS PUNTOS GRANDES
			}
			else if(matrizJuegoAux[i][j]==2){
				matrizJuegoAux[i][j]=5;
			}
		}
	}
	
	for(int i=0;i<20;i++){//Hacemos una copia directa
		for(int j=0;j<30;j++){
			matrizJuego[i][j]=matrizJuegoAux[i][j];
		}
	}	
}

void reiniciarPacman(int matrizJuego[][COL],int *pVidas){
	//Regresa a Pacman a su posición original porque murió
	int vecPos[2];
	buscarPosicion(matrizJuego,0,vecPos);
	matrizJuego[vecPos[0]][vecPos[1]]=2;//Borramos a Pacman
	matrizJuego[14][14]=0;//Reseteamos a pacman	
	*pVidas=*pVidas-1;//Le restamos una vida al jugador
}

int verificarTeleport(int matrizJuego[][COL],int posI,int posJ,int tipo,int valorPre){
	//Se encarga de verificar si alguien está entrando a un teleport
	//Regresará un 1 si hay que hacer un teleport hacia la derecha
	//Regresará un 2 si hay que  hacer un teleport hacia la izquierda
	//Regresará un 0 si no hay que hacer nadita
	int x=0;
	if(tipo==0){//Entró Pacman
		if(posI==10 && posJ==0){//De izquierda a derecha
			matrizJuego[10][29]=0;//Ponemos a Pacman en su nueva posición
			matrizJuego[10][0]=2;//Borra la posición anterior
			matrizJuego[10][1]=2;//Borra la posición ANTE Anterior
			x=1;
		}
		else if(posI==10 && posJ==29){//De derecha a izquierda
			matrizJuego[10][0]=0;//Ponemos a Pacman en su nueva posición
			matrizJuego[10][29]=2;//Borra la posición anterior
			matrizJuego[10][28]=2;//Borra la posición ANTE Anterior
			x=2;
		}
		else{
			x=0;
		}
	}
	else{//Entró un fantasma
	
		if(posI==10 && posJ==0){//De izquierda a derecha
			matrizJuego[10][29]=tipo;//Ponemos al fantasma correspondiente en su nueva posición
			matrizJuego[10][0]=valorPre;//Borra la posición anterior
			matrizJuego[10][1]=valorPre;//Borra la posición ANTE Anterior POSIBLE BUG, PORQUE TENDRÍA QUE TENER 2 VALORES PRE
			x=1;
		}
		else if(posI==10 && posJ==29){//De derecha a izquierda
			matrizJuego[10][0]=tipo;//Ponemos al fantasma correspondiente en su nueva posición
			matrizJuego[10][29]=valorPre;//Borra la posición anterior
			matrizJuego[10][28]=valorPre;//Borra la posición ANTE Anterior POSIBLE BUG, PORQUE TENDRÍA QUE TENER 2 VALORES PRE
			x=2;
		}
		else{
			x=0;
		}
		
	}
	
	return x;
}

void buscarPosicion(int matrizJuego[][COL],int tipoBusqueda,int vecPos[]){
	//Se encarga de buscar la posición de lo que se requiera
	vecPos[0]=-1;
	vecPos[1]=-1;//Esto nos ayuda a tener un control respecto a las frutas. De cualquier forma, si encontrara una posición en el ciclo, los valores se cambiarían sin problema y no tronaría el programa.

	for(int i=0;i<20;i++){//Buscamos en qué posición está lo que se necesita
			for(int j=0;j<30;j++){
				if(matrizJuego[i][j]==tipoBusqueda){
					vecPos[0]=i;//Posición Fila
					vecPos[1]=j;//Posición Columna
					return;
				}
			}
	}
}

int valorPreFantasma(int matrizJuego[][COL],int posIF, int posJF){
	//Se encarga de retornar el valor numérico que teníala matriz antes de que el fantasma pase
	if(matrizJuego[posIF][posJF]==1 || matrizJuego[posIF][posJF]==3 || matrizJuego[posIF][posJF]==0){//Evitamos que retorne estos valores para que no ponga cajas de más en caso de algún "bug" en un movimiento
		return 2;		
	}
	else if(matrizJuego[posIF][posJF]!=0){
		return matrizJuego[posIF][posJF];
	}
}

void puntaje(int matrizJuego[][COL],int posI,int posJ,int *pcomida,int *puntuacionTotal,int *pCambioNivel,FILE *registroUsuarios,int *pIdentificacion,int *pFrutas,int *pVidas,int dificultad){
	//Es la función que hacas las cuentas numéricas de los puntos y que además realiza el cambio de nivel cuando es necesario
	Usuario usuario;
	int cont=0, id=*pIdentificacion,contadorRegistro=0,puntos=0;
	
	for(int i=0;i<20;i++){//Este FOR cuenta el número total de puntos activos en el mapa. Son los puntos reales que existen. Es necesario esto para tener un control en tiempo real de los puntos existentes en el mapa, recordando que en algunos casos se agregan (por mecanismos propios del juego) puntos al mapa o se quitan
		for(int j=0;j<30;j++){
			if(matrizJuego[i][j]==5 || matrizJuego[i][j]==6 ){//Cuenta cuántos puntos hay 
				cont++;
			}
		}
	}
    if(matrizJuego[posI][posJ]==7 || matrizJuego[posI][posJ]==8 || matrizJuego[posI][posJ]== 9 || matrizJuego[posI][posJ]==10){//Si pacman se ha comido una pastilla y hay un fantasma...
	 	if(poderActivo(pcomida,dificultad)==2){//El poder está activo
	 		*puntuacionTotal+=20;	
		}
    }
	else if(matrizJuego[posI][posJ] == 4){//Si es igual a donde hay una fruta...
		*pFrutas=*pFrutas+1; //Si pFrutas=1 -> Fruta 1   pFrutas=3 -> Fruta 2   pFrutas=3 -> Fruta 3
    	*puntuacionTotal+=15;
	} else if(matrizJuego[posI][posJ] == 5){//Si pacman se ha comido un puntito...
		*puntuacionTotal+=5;
    }
    else if (matrizJuego[posI][posJ]==6){//Si pacman se ha comido un punto grande...
    	*puntuacionTotal+=10;
    }	
	if(cont<=PUNTOS){//Se tiene que activar un cambio de mapa y se guardan los puntos en el archivo
		puntos=*puntuacionTotal;
		registrarPuntaje(registroUsuarios,id,puntos,pVidas);//Aplica el guardado con los puntos actuales + los puntos que tiene registrados
		*pCambioNivel=1;//Se activa el cambio de nivel
	}
}

void registrarPuntaje(FILE *registroUsuarios,int id,int puntos,int *pVidas){
	//Se encarga de hacer una actualización de la información del jugador cada que hay un cambio de nivel.
	Usuario usuario;
	int contadorRegistro=0;
	registroUsuarios=fopen("Usuarios.dat","rb+");
	fseek(registroUsuarios,(id-1)*sizeof(Usuario),SEEK_SET);
	fread(&usuario,sizeof(Usuario),1,registroUsuarios);
	
	if(puntos>0){//Llega a la función con puntos válidos porque pasó el nivel
		contadorRegistro=usuario.info.puntos+puntos;	
		usuario.info.puntos=contadorRegistro;
	}
	else if(puntos==-1){//No hace ningún cambio
		usuario.info.puntos=usuario.info.puntos;
	}
	else{//Tenemos que borrar el puntaje del usuario
		usuario.info.puntos=0;
	}
	if(*pVidas==0){//Validación que sirve para que si el usuario perdió, pueda volver a jugar con su mismo usario reiniciando a 3 vidas
		usuario.info.vidas=3;
	}
	else{//Quiere decir que se pueden guardar las vidas que tiene hasta el momento
		usuario.info.vidas=*pVidas;
	}
	fseek(registroUsuarios,(id-1)*sizeof(Usuario),SEEK_SET);
	fwrite(&usuario,sizeof(Usuario),1,registroUsuarios);
	
	fclose(registroUsuarios);
}

int registrarUsuario(FILE *registroUsuarios, int *pIdentificacion,int *pVidas) {
	//Valida un registro de usuarios de buena manera
	Usuario usuario;
	char ASCII;
	int newkey,indice=0;
	BITMAP *registro=create_bitmap(900, 600);
	registro=load_bitmap("FotoRegistroInicio.bmp", NULL);
	BITMAP *continuar=create_bitmap(900, 600);
	continuar=load_bitmap("FotoRegistro.bmp", NULL);
	bool salida=false,salida2=false,usuarioLibre=false;
	strcpy(usuario.nombreUsuario,"                           ");
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
						usuario.nombreUsuario[indice]=ASCII;//Metemos el caracter ingresado al vector
						indice++;
						usuario.nombreUsuario[indice]='\0';//Asignamos el fin de línea
					
					}
				}
				else if(ASCII==8){//Está retrocediendo
					usuario.nombreUsuario[indice]=' ';//Se borra la posición actual
					if(indice!=0){
						indice--;//Cambiamos de índice
						usuario.nombreUsuario[indice]=' ';//Se borra la posición actual
					}
				}
				else if(ASCII==27){//Apretó Escape, hay que retornar
					return 1;
				}
			}
			if(ASCII==13){//Verificamos si ya está completo el nombre de usuario
				if(validarUsuario(registroUsuarios,usuario.nombreUsuario)==1){//El usuario está repetido, hay que indicarle al usuario
					textout(screen, font, "¡Ups! nombre ya existente, intente con uno diferente", 335, 139, AZUL);
				}	
				else{
					textout(screen, font, "                                                    ", 335, 139, AZUL);//Borramos la leyenda del "Ups"
					usuarioLibre=true;
				}
			}
			textout(screen, font, usuario.nombreUsuario, 335, 99, AZUL);
		} while(usuarioLibre==false);//Sale cuando se le da un enter y el usuario es válido
		
		indice=0;
		strcpy(usuario.contrasena,"        ");
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
							usuario.contrasena[indice]=ASCII;
							indice++;
							usuario.contrasena[indice]='\0';
						}
				 	}
					 else if(ASCII==8){//Está retrocediendo
						usuario.contrasena[indice-1]=' ';//Se borra la posición actual
						if(indice!=0){
							indice--;//Cambiamos de índice
						}
					}
					else if(ASCII==27){//Apretó Escape, hay que retornar
						return 1;
					}	
				}
			}
			textout(screen, font, usuario.contrasena, 335, 210, AZUL);
		} 
		while(salida2==false);
		*pIdentificacion=usuario.id=secuenciaId(registroUsuarios);//Verifica cuál fue el último ID 
		*pVidas=3;//Inicializamos al usuario con 3 vidas
		
		registroUsuarios = fopen("Usuarios.dat","rb+");
		if(!registroUsuarios){
			registroUsuarios=fopen("Usuarios.dat","a+");
		}
		fseek(registroUsuarios, 0, SEEK_END);//Colocamos el puntero en su última posición
		fwrite(&usuario,sizeof(Usuario),1,registroUsuarios);
		rewind(registroUsuarios);
		fread(&usuario,sizeof(Usuario),1,registroUsuarios);
		do{
			fread(&usuario,sizeof(Usuario),1,registroUsuarios);
		}while(!(feof(registroUsuarios)));
		fclose(registroUsuarios);//Se recorre el archivo por precaución
		
		if(strcmp(usuario.nombreUsuario, "NULL")!=0 && strcmp(usuario.contrasena, "NULL")!=0) {
			salida=true;
		}//Salimos del ciclo que controla la función
	}
}
int validarUsuario(FILE *registroUsuarios, char nomUsuario[]){
	//Esta función se encarga de validar que el nombre de usuario que alguien ingrese no esté ya siendo usado
	//Regresará un 1 si el nombre ya existe o un 0 si el nombre está libre
	Usuario usuario2;
	registroUsuarios=fopen("Usuarios.dat","rb");//Abrimos el archivo en modo lectura
	if(!registroUsuarios){//Hay un error y no se podrá continuar
		registroUsuarios=fopen("Usuarios.dat","wb+");
	}
	rewind(registroUsuarios);//Posicionamos el cursor al inicio del archivo
	do{//Recorremos el archivo para buscar si el nombre ya está siendo usado o no.
		fread(&usuario2,sizeof(Usuario),1,registroUsuarios);
		if(strcmp(nomUsuario,usuario2.nombreUsuario)==0){//Los nombres son iguales, por lo tanto hay que retornar un 1 y terminar la función
			return 1;
		}
	}
	while(!(feof(registroUsuarios)));
	fclose(registroUsuarios);
	//Como para este punto ya debió de haber retornado un 1 si el nombre ya estuviera ocupado pero no sucedió, entonces regresamos un 0
	return 0;
}
int secuenciaId(FILE *registroUsuarios){
	//Auxilia a la función registrarUsuario para poder obtener el último ID y tener la información de forma ordenada
	Usuario usuario;
	int x=0;
	Usuario ids;//Aquí guardaremos toda la estructura del último renglón del archivo
	registroUsuarios=fopen("Usuarios.dat","rb");
	
	if(!registroUsuarios){//No se encontró el archivo, por lo tanto hay que crearlo
		printf("Apertura incorrecta para el guardado. Verifique la existencia del archivo Usuarios.dat\n");
	}
	fseek(registroUsuarios, 0, SEEK_END);//Coloco el cursor al final del archivo para hacer la siguiente operación
	x=ftell(registroUsuarios)/sizeof(Usuario);	//Verifico cuántos registros hay
	
	return x+1;//Le sumamos un 1 al valor porque los archivos comienzan en 0, pero nuestros ID´s comienzan en 1	
}

char **ordenarRank(int *vecRank,FILE *registroUsuarios){
	//Esta función ordena a los puntajes, permitiendo también saber quiénes tienen esos puntajes.
	Usuario usuario;
	int *vecAux,x,i=0,tamano=0,n=0,id=-1,aux;
	char **nombresRank;
	nombresRank=new char *[3];
	for(int j=0;j<3;j++){
		nombresRank[j]=new char [20];//Creamos nuestra matriz dinámica
	}
	strcpy(nombresRank[0]," ");//Inicializamos los valores de los nombres, para evitar basura en pantalla
	strcpy(nombresRank[1]," ");
	strcpy(nombresRank[2]," ");
	registroUsuarios=fopen("Usuarios.dat","rb");
	fseek(registroUsuarios, 0, SEEK_END);//Colocamos el cursor al final del archivo para hacer la siguiente operación
	x=ftell(registroUsuarios)/sizeof(Usuario);	//Verifico cuántos registros hay
	vecAux = new int [x];//Se crea un vector del número de registros en el archivo
	rewind(registroUsuarios);
	fread(&usuario,sizeof(Usuario),1,registroUsuarios);
	do{
		vecAux[i]=usuario.info.puntos;//Asignamos a un vector  dinámico todos los puntajes existentes, de forma como aparezcan en el archivo
		i++;
		fread(&usuario,sizeof(Usuario),1,registroUsuarios);
	}while(!(feof(registroUsuarios)));
	
	for(int k=1;k<x;k++){//Ordenamos al vector que llenamos previamente. Método burbuja
		for(int i=1;i<x;i++){
			if(vecAux[i]>=vecAux[i-1]){
				aux=vecAux[i];
				vecAux[i]=vecAux[i-1];
				vecAux[i-1]=aux;
			}
		}	
	}
	vecRank[0]=vecAux[0];//Colocamos las primeras posiciones del vector auxiliar a nuestro vector Rank, que tendrá la información ya ordenada
	vecRank[1]=vecAux[1];
	vecRank[2]=vecAux[2];
	fseek(registroUsuarios,0,SEEK_END);//Posicionamos el cursor al final
	tamano=ftell(registroUsuarios)/sizeof(usuario);//Calculamos el tamaño del archivo
	switch(tamano){
		case 1://Hay sólo un registro, por lo tanto que hay evitar que muestre basura
			vecRank[1]=0;
			vecRank[2]=0;
			strcpy(nombresRank[1]," ");
			strcpy(nombresRank[2]," ");
			break;
		case 2://Hay sólo dos registros, evitamos se muestre basura
			vecRank[2]=0;
			strcpy(nombresRank[2]," ");
			break;
	}
	int cont=1;
	
	for(int k=0;k<3;k++){//Obtenemos ahora los nombres de los que tienen los puntos en el rank.
		rewind(registroUsuarios);
		do{
			fread(&usuario,sizeof(Usuario),1,registroUsuarios);
			if(usuario.info.puntos==vecAux[k] && id!=usuario.id){
				id=usuario.id;//Nos ayuda a evitar errores en caso de que existan dos personas con el mismo puntaje
				strcpy(nombresRank[k],usuario.nombreUsuario);//Asignamos el nombre de la persona que tiene el puntaje comparado
				break;
			}
		}while(!(feof(registroUsuarios)));
	}
	return nombresRank;	
}

int continuarProgreso(FILE *registroUsuarios,int *pIdentificacion,int *pNivel,int *pVidas){
	//Se encarga de pedirle al usuario su información de inicio de sesión y de obtener todos su datos anteriores para que pueda seguir jugando en el nivel y con los puntos que tenía
	Usuario usuario;
	char ASCII;
	int newkey,indice=0;
	BITMAP *registro=create_bitmap(900, 600);
	registro=load_bitmap("FotoRegistroInicio.bmp", NULL);
	BITMAP *continuar=create_bitmap(900, 600);
	continuar=load_bitmap("FotoRegistro.bmp", NULL);
	bool salida=false,salida2=false,usuarioLibre=false;
	strcpy(usuario.nombreUsuario,"                           ");
	blit(registro, screen, 0, 0, 0, 0, 900, 600);
	clear_keybuf();//Borramos el buffer de entrada del teclado
	while(!salida) {
		do {
			vline(screen, (indice*8)+335, 99, 107, AZUL);//Aparece el cursor
			if(keypressed()) {
				 newkey = readkey();//Detectamos la letra
				 ASCII = newkey & 0xff;//Convertimos
				if(ASCII>=32 && ASCII <=126) {//Verificamos que sólo ingrese estos caracteres
					if(indice<20-1) {//Validamos que no pueda ingresar más caracteres de lo establecido
						usuario.nombreUsuario[indice]=ASCII;//Metemos al vector lo que el usuario puso
						indice++;
						usuario.nombreUsuario[indice]='\0';//Asignamos el fin de línea
					}
				}
				else if(ASCII==8){//Está retrocediendo
					usuario.nombreUsuario[indice]=' ';//Se borra la posición actual
					if(indice!=0){
						indice--;//Cambiamos de índice
						usuario.nombreUsuario[indice]=' ';
					}
				}
				else if(ASCII==27){//Apretó Escape, hay que retornar
					return 1;
				}
			}
			if(ASCII==13){//Verificamos si ya está completo el nombre de usuario
				if(validarUsuario(registroUsuarios,usuario.nombreUsuario)==1){//El usuario Existe y puede proseguir a poner la contraseña
					textout(screen, font, "                                                    ", 335, 139, AZUL);//Borramos la leyenda del "Ups"
					usuarioLibre=true;
				}	
				else{//El usuario no existe y no puede proseguir a poner la contraseña
					textout(screen, font, "¡Ups! Usuario inexistente, intente de nuevo", 335, 139, AZUL);
				}
			}
			textout(screen, font, usuario.nombreUsuario, 335, 99, AZUL);
		} while(usuarioLibre==false);//Sale cuando se le da un enter y el usuario es válido
		indice=0;
		strcpy(usuario.contrasena,"        ");
		clear_keybuf();
		do {
			vline(screen, indice*8+335, 210, 218, AZUL);
			if(keypressed()) {
				 newkey = readkey();
				 ASCII =newkey & 0xff;
				 if(ASCII==13){
				 	if(verificarContrasena(registroUsuarios,usuario.contrasena,usuario.nombreUsuario,pIdentificacion,pNivel,pVidas)==true){//La contraseña es correcta y puede salir del ciclo
				 		salida2=true;
					}
					else{//La contraseña es incorrecta y se le tiene que avisar al usuario
						textout(screen, font, "¡Ups! Clave incorrecta, intente de nuevo", 335, 245, AZUL);
						salida2=false;
					}
				 
			 	 }
				 else{
				 	if(ASCII>=32 && ASCII <=126) {
						if(indice<25-1) {
							usuario.contrasena[indice]=ASCII;
							indice++;
							usuario.contrasena[indice]='\0';
						}
				 	}
					 else if(ASCII==8){//Está retrocediendo
						usuario.contrasena[indice]=' ';//Se borra la posición actual
						if(indice!=0){
							indice--;//Cambiamos de índice
							usuario.contrasena[indice]=' ';
						}
					}
					else if(ASCII==27){//Apretó Escape, hay que retornar
						return 1;
					}	
				}
			}
			textout(screen, font, usuario.contrasena, 335, 210, AZUL);
		} 
		while(salida2==false);
		//Cuando verificamos la contraseña, el puntero identificacion cambia su valor para que sea el valor que está activo y rescata también el nivel en el que el usuario se había quedado
		if(strcmp(usuario.nombreUsuario, "NULL")!=0 && strcmp(usuario.contrasena, "NULL")!=0) {
			salida=true;
		}//Salimos del ciclo 
	}
}

bool verificarContrasena(FILE *registroUsuarios, char contrasena[],char nomUsuario[],int *pIdentificacion,int *pNivel,int *pVidas){
	//Esta función nos ayuda al inicio de sesión
	//La función regresará un TRUE si las contraseñas coinciden, y regresará un FALSE si las contraseñas no coinciden
	Usuario usuario;
	registroUsuarios=fopen("Usuarios.dat","rb");//Abrimos el archivo en modo lectura
	if(!registroUsuarios){//¡ERROR! No existe ningún usuario en el archivo, tiene que crear uno. NOTA: Para este punto el archivo ya debe de estar creado, por lo que nunca va a entrar a este if.
		
	}
	registroUsuarios=fopen("Usuarios.dat","rb");
	rewind(registroUsuarios);//Posicionamos el cursor al inicio del archivo
	fread(&usuario,sizeof(Usuario),1,registroUsuarios);
	do{//Recorremos el archivo para buscar si la contrasena es correcta o no
		if(strcmp(nomUsuario,usuario.nombreUsuario)==0){//Los nombres son iguales, por lo tanto hay que retornar un 1 y terminar la función
			if(strcmp(contrasena,usuario.contrasena)==0){//las contrasenas coinicidieron, hay que dar luz verde al inicio de sesión
				*pIdentificacion=usuario.id;//Activamos el puntero de identificación para saber en todo momento quién está jugando
				*pNivel=usuario.info.nivel;//Activamos el puntero de nivel para saber a partir de qué nivel empezar a jugar
				*pVidas=usuario.info.vidas;//Activamos el puntero de vidas para saber cuántas vidas tiene el usuario guardado
				fclose(registroUsuarios);
				return true;
			}
			else{
				fclose(registroUsuarios);
				return false;
			}
		}
		fread(&usuario,sizeof(Usuario),1,registroUsuarios);
	}
	while(!(feof(registroUsuarios)));
	fclose(registroUsuarios);
	//Como para este punto ya debió de haber retornado un true si la contaseña fuera correcta pero no sucedió, entonces regresamos un false
	return false;
}

void guardarNivel(FILE *registroUsuarios,int *pNivel,int *pIdentificacion,bool reiniciar){
	//Esta función hace un guardado especial de toda la información del usuario
	Usuario usuario;
	int id=*pIdentificacion;//*pIdentificación contiene el ID del jugador actual, por lo tanto hay que modificar en ese renglón
	registroUsuarios=fopen("Usuarios.dat","rb+");
	fseek(registroUsuarios,(id-1)*sizeof(Usuario),SEEK_SET);
	fread(&usuario,sizeof(Usuario),1,registroUsuarios);
	usuario.info.nivel=*pNivel;
	if(reiniciar==true){//Quiere decir que al llegar a esta función ya ganó el juego y es justo que el usuario pueda volver a empezar con su mismo nombre pero ahora con 3 vidas
		usuario.info.vidas=3;//Como al llegar a esta función significa que ya ganó el juego, es 
	}
	fseek(registroUsuarios,(id-1)*sizeof(Usuario),SEEK_SET);
	fwrite(&usuario,sizeof(Usuario),1,registroUsuarios);
	fclose(registroUsuarios);
}

void mejoresPuntuaciones(char **nombresRank, int *vecRank) {
	//Mostramos en pantalla al mejor rank
    BITMAP *buffer=create_bitmap(900, 600);
    BITMAP *mejoresPuntuaciones=load_bitmap("MejoresPuntuaciones.bmp", NULL);
    BITMAP *regresar=load_bitmap("MejoresPuntuacionesRegreso.bmp", NULL);
    bool salir=false;
    FONT *font1=load_font("letritas.pcx", NULL, NULL);
    while (!salir) {
        blit(buffer, screen, 0, 0, 0, 0, 900, 600);
        if(mouse_x>=17 && mouse_x<=257 && mouse_y>=495 && mouse_y<=581) {
            blit(regresar, buffer, 0, 0, 0, 0, 900, 600);
            textprintf(buffer, font1, 308, 200, makecol(255, 0, 0), "%s", nombresRank[0]);
            textprintf(buffer, font1, 520, 200, makecol(255, 0, 0), "%i", vecRank[0]);
            textprintf(buffer, font1, 308, 240, makecol(0, 255, 0), "%s", nombresRank[1]);
            textprintf(buffer, font1, 520, 240, makecol(0, 255, 0), "%i", vecRank[1]);
            textprintf(buffer, font1, 308, 280, makecol(0, 0, 255), "%s", nombresRank[2]);
            textprintf(buffer, font1, 520, 280, makecol(0, 0, 255), "%i", vecRank[2]);
            if(mouse_b & 1) {
                salir=true;
            }
        } else {
            blit(mejoresPuntuaciones, buffer, 0, 0, 0, 0, 900, 600);
            textprintf(buffer, font1, 308, 200, makecol(255, 0, 0), "%s", nombresRank[0]);
            textprintf(buffer, font1, 520, 200, makecol(255, 0, 0), "%i", vecRank[0]);
            textprintf(buffer, font1, 308, 240, makecol(0, 255, 0), "%s", nombresRank[1]);
            textprintf(buffer, font1, 520, 240, makecol(0, 255, 0), "%i", vecRank[1]);
            textprintf(buffer, font1, 308, 280, makecol(0, 0, 255), "%s", nombresRank[2]);
            textprintf(buffer, font1, 520, 280, makecol(0, 0, 255), "%i", vecRank[2]);
        }
    }
    destroy_bitmap(buffer);
}

void generarFruta(int matrizJuego[][COL]){
	//Esta función genera la fruta de forma aleatoria
	int i,j;
	srand(time(NULL));
	do{
		i=rand()%17+1;
		j=rand()%27+1;
	}
	while(matrizJuego[i][j]!=5);
	matrizJuego[i][j]=4;
	
}
