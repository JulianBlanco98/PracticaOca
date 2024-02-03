//============================================================================
// Name        : EPF23_24julianbg_v2.cpp
// Author      : Julian Blanco Gonzalez
// Version     : 2
// Copyright   : Your copyright notice
// Description : Práctica Final de Fundamentos de Programación
//============================================================================

#include <iostream>
#include <conio2.h>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

//Constantes
#define MAX_CASILLAS 63 //numero de casillas
#define FILAS 7 //numero de filas
#define COLUMNAS 9 //numero de columnas
#define MAXIMO_CARACTERES 10 //numero maximo de caracteres del nombre del jugador
#define MIN_JUG 2 //Minimo de jugadores
#define MAX_JUG 5 //Maximo de jugadores

//Tipos de datos
typedef char tablero[MAX_CASILLAS]; //tablero de juego
typedef char cadena[MAXIMO_CARACTERES + 1]; //String del nombre de jugador
typedef struct{
	int posicion;
	char casillaTablero;
	int color;
	bool mover;
	int castigo;
	bool pozoSalir;
	bool xcasilla;
}casilla; //casilla donde esta el jugador
typedef struct {
	cadena nombreJugador;
	int numeroPartidas;
	int victorias;
	int dado;
	int turno;
	casilla casillaT;
}jugador; //jugador que juega la partida
typedef jugador v_jugadores [MAX_JUG]; //vector de jugadores

//Vector variable
typedef struct{
	int cont;
	v_jugadores vj;
}jugadores;

//Funciones
char check_respuesta (char resp);
jugador introducirJugador(int cont);
jugadores leerJugadores(int numJ, const char *nombreArchivo);
int lanzarDado (int inferior, int superior);
char getCasilla (tablero tableroOca, int casilla);

//Procedimientos
void check_longitudNombre (cadena nombreJugador);
void convertiMinusculas (cadena nombre);
void check_Jugador (const char *nombreArchivo, bool &encontrado, jugador &jugadorABuscar);
void turnoJugador (jugadores &jug);
void ordenarPorTurno(jugadores &jug);
void getTablero (const char *nombreArchivo, tablero &tableroOca);
void mostrarTablero (tablero tableroOca);
void inicializarCasilla (jugadores &jug);

void jugada (int turno, jugadores &jug, tablero tableroOca);
void accionTablero (int &total, int castigo, int dado, bool &xc);
void actualizarArchivoJugadores (const char *nombreArchivo, bool victoria, jugador &jugadorActualizado);
void pintarCasilla (int casilla, tablero tableroOca, int turno, jugadores jug);
void limpiarPantalla (int fila, int columna);

int main() {

	system("cls");
	textcolor(WHITE);

	//Variables
	char resp;
	int numJugadores;
	jugadores jug;
	tablero tablero_Oca;
	const char *nombreArchivoJugadores = "Jugadores.txt";
	//const char *nombreArchivoTableroOca = "TableroOca.txt";
	const char *nombreArchivoTableroOcaEnero = "TableroOca_Ene.txt";

	srand(time(NULL));
	cout << endl << "Desea jugar al juego de la OCA (s/n,S/N): ";
	cin >> resp;
	resp = check_respuesta(resp);
	while (resp == 'S' || resp == 's'){

		numJugadores = 0; //Reset a 0

		textcolor(BROWN);
		cout << "COMENZAMOS A JUGAR..." << endl;
		cout << "PASO 1. NUMERO DE JUGADORES Y SU NOMBRE"<< endl;
		textcolor(WHITE);
		cout << "Numero de jugadores para jugar(2-5): ";
		cin >> numJugadores;
		while(numJugadores < MIN_JUG || numJugadores > MAX_JUG){
			limpiarPantalla(1, 5);
			textcolor(RED);
			cout << "Error.";
			textcolor(WHITE);
			cout << " Introduce un numero correcto de jugadores(2-5): ";
			cin >> numJugadores;
		}
		jug = leerJugadores(numJugadores, nombreArchivoJugadores);
		textcolor(BROWN);
		cout << "PASO 2. LANZAR LOS DADOS PARA VER QUIEN EMPIEZA." << endl;
		textcolor(WHITE);
		turnoJugador(jug);
		inicializarCasilla(jug);
		ordenarPorTurno(jug);
		cout << "Orden de los jugadores" << endl;
		for(int i=0;i<jug.cont;i++){
			textcolor(jug.vj[i].casillaT.color);
			cout << "| Turno: " << jug.vj[i].turno << ", Nombre: " << jug.vj[i].nombreJugador <<", Color: " << jug.vj[i].casillaT.color << " |" <<endl;
		}
		textcolor(WHITE);

		//Recupero el tablero de la Oca del archivo txt
		//getTablero(nombreArchivoTableroOca, tablero_Oca);
		getTablero(nombreArchivoTableroOcaEnero, tablero_Oca);
		mostrarTablero(tablero_Oca);

		gotoxy(1, 38);
		textcolor(BROWN);
		cout << "PASO 3. EMPEZAR A JUGAR AL JUEGO DE LA OCA." << endl;
		textcolor(WHITE);
		int indice;
		bool ganador;
		do{
			limpiarPantalla(1, 40);
			indice = 0;
			ganador = false;
			while (indice < jug.cont && !ganador){
				limpiarPantalla(1, 40);
				textcolor(jug.vj[indice].casillaT.color);
				cout << "Turno de: " << jug.vj[indice].nombreJugador << endl;
				//Llamada a la funcion jugada
				jugada(indice, jug, tablero_Oca);
				if(jug.vj[indice].casillaT.posicion == MAX_CASILLAS){ //Si llega al jardin de la OCA
					ganador = true;
				}
				else{
					indice++;
					textcolor(BROWN);
					cout << "PULSA CUALQUIER TECLA PARA EL SIGUIENTE TURNO..." << endl;
					textcolor(WHITE);
					getch();
					mostrarTablero(tablero_Oca);
				}
			}
			if(!ganador){
				indice = 0; //Resetear el turno
			}
		}
		while(!ganador);

		gotoxy(1, 52);
		for(int i=0;i<jug.cont;i++){
			if(jug.vj[i].casillaT.posicion != MAX_CASILLAS){
				actualizarArchivoJugadores(nombreArchivoJugadores, false, jug.vj[i]);
			}
			else{
				cout << "-----------------------------------HAS GANADO " << jug.vj[i].nombreJugador << "-----------------------------------" << endl;
				actualizarArchivoJugadores(nombreArchivoJugadores, true, jug.vj[i]);
			}
		}

		cout << "FIN DE LA PARTIDA. PULSA CUALQUIER BOTON PARA CONTINUAR..." << endl;
		getch();
		system("cls");
		gotoxy(1, 1);
		cout << endl << "Desea volver a jugar al juego de la OCA (s/n,S/N): ";
		cin >> resp;
		resp = check_respuesta(resp);
	}

	cout << "Hasta luego" << endl;
	return 0;
}
/**
 * Procedimiento que simula el turno de cada jugador
 * @param turno: entero del turno de cada jugador
 * @param jug: referencia al vector de jugadores
 * @param tableroOca: tablero de la OCA
 */
void jugada (int turno, jugadores &jug, tablero tableroOca){

	int contador = 1;
	jugador jugTurno;
	jugTurno = jug.vj[turno]; //varible jugador que representa al jugador que juega
	textcolor(WHITE);
	cout << "********************************************" << endl; //fila 41
	gotoxy(1, 51);
	textcolor(WHITE);
	cout << "********************************************" << endl; //fila 51
	for (int i = 42; i < 51; i++) {
		limpiarPantalla(1, i);
	}
	limpiarPantalla(1, 52);
	gotoxy(1, 42);
	textcolor(WHITE);
	cout << "Turno: " << contador;
	textcolor(LIGHTCYAN);
	cout << ". Casilla Inicial: ( " << jugTurno.casillaT.posicion << " )" << endl; //Fila 42
	do{



		if(contador > 1 && jugTurno.casillaT.mover && jugTurno.casillaT.castigo == 0 && jugTurno.casillaT.posicion != MAX_CASILLAS){
			for (int i = 42; i < 49; i++) { //Limpio pantalla de la 42 a la 48
				limpiarPantalla(1, i);
			}
			gotoxy(1, 42);
			textcolor(WHITE);
			cout << "Turno: " << contador;
			textcolor(LIGHTCYAN);
			cout << " | Casilla: ( " << jugTurno.casillaT.posicion << " ) | Tipo casilla: ( " << jugTurno.casillaT.casillaTablero << " )		          " << endl; //Fila 42
			limpiarPantalla(1, 43);
			textcolor(WHITE);
			cout << "Tirada adicional. PULSA CUALQUIER TECLA PARA LANZAR LOS DADOS... | ";
			getch();
			jugTurno.dado = lanzarDado(1, 6);
			cout << " --> Ha sacado un: " << jugTurno.dado << endl; //Fila 43
			jugTurno.casillaT.posicion = jugTurno.casillaT.posicion + jugTurno.dado;
			contador++;
		}

		if(contador == 1 && jugTurno.casillaT.mover && jugTurno.casillaT.castigo == 0 && jugTurno.casillaT.posicion != MAX_CASILLAS || jugTurno.casillaT.xcasilla){
			if(jugTurno.casillaT.posicion){
				//pintar casilla
				pintarCasilla(jugTurno.casillaT.posicion, tableroOca, turno, jug);
			}
			limpiarPantalla(1, 43);
			gotoxy(1, 43);
			textcolor(WHITE);
			cout << "PULSA CUALQUIER TECLA PARA LANZAR LOS DADOS...| ";
			getch();
			jugTurno.dado = lanzarDado(1, 6);
			//cin >> jugTurno.dado; //probar
			cout << " --> Ha sacado un: " << jugTurno.dado << endl; //Fila 43
			jugTurno.casillaT.posicion = jugTurno.casillaT.posicion + jugTurno.dado; //casilla en la que cae el jugador
			contador++;
		}

		if(jugTurno.casillaT.posicion >= MAX_CASILLAS){
			jugTurno.casillaT.mover = false;
		}
		else{
			//1. Recupero la casilla donde pone la ficha el jugadors
			jugTurno.casillaT.casillaTablero = getCasilla(tableroOca, jugTurno.casillaT.posicion);
			//2. Dependiendo de la casilla, muevo la ficha en el tablero
			accionTablero(jugTurno.casillaT.posicion, jugTurno.casillaT.castigo, jugTurno.dado, jugTurno.casillaT.xcasilla); //Dependiendo de la casilla, cambio de sitio o no la ficha | Fila 44
			//3. Pinto la nueva posición de la ficha
			textcolor(WHITE);
			cout << "Turno: " << contador;
			textcolor(LIGHTCYAN);
			cout << " | Casilla: ( " << jugTurno.casillaT.posicion << " ) | Tipo casilla: ( " << jugTurno.casillaT.casillaTablero << " )		          " << endl; //Fila 45
			mostrarTablero(tableroOca); //Reset de tablero
			pintarCasilla(jugTurno.casillaT.posicion, tableroOca, turno, jug); //Pintar la nueva posicion de la ficha


			//4. Dependiendo de la nueva posición de la ficha, ver si pierde turno o no
			switch (jugTurno.casillaT.casillaTablero) {
			case 'O':
				jugTurno.casillaT.mover = true;
				break;
			case 'P':
				jugTurno.casillaT.mover = true;
				break;
			case 'S':
				if (jugTurno.casillaT.castigo == 0) {
					jugTurno.casillaT.castigo = 1;
				} else {
					jugTurno.casillaT.castigo--;
				}
				jugTurno.casillaT.mover = false;
				break;
			case 'X':
				jugTurno.casillaT.mover = false;
				break;
			case 'Z':
				int i, jugSalePozo;
				bool enc;
				enc = false;
				i=0;
				while(!enc && i < jug.cont){
					//buscar a un jugador que esté en el pozo
					if(jug.vj[i].casillaT.casillaTablero == 'Z' && i != turno){//Buscar entre todos los jugadores, excepto el propio jugador
						enc = true;
						jugSalePozo = i;
					}
					else{
						i++;
					}
				}

				//Si existe un jugador en el pozo
				if(enc){
					gotoxy(1, 46);
					cout << "El jugador " << jug.vj[jugSalePozo].nombreJugador << " esta en el pozo. Sale. ";
					//Entra en el pozo este jugador, y el juagdor del pozo pierde su castigo
					jug.vj[jugSalePozo].casillaT.castigo = 0; //castigo del jugador anterior a 0
					jug.vj[jugSalePozo].casillaT.pozoSalir = true;
					jug.vj[jugSalePozo].casillaT.mover = true; //Puede volver a mover
					jugTurno.casillaT.castigo = 3; //Castigo de 3 al jugadorTurno
					jugTurno.casillaT.pozoSalir = false;
					jugTurno.casillaT.mover = false; //El jugador del turno cae en el pozo y no puede mover
					cout << "castigo de " << jug.vj[jugSalePozo].nombreJugador << ": " << jug.vj[jugSalePozo].casillaT.castigo << endl;
				}
				//Si no hay nadie en el pozo, castigo de 3 turnos
				else{
					if (jugTurno.casillaT.castigo == 0 && !jugTurno.casillaT.pozoSalir) {
						jugTurno.casillaT.castigo = 3;
						jugTurno.casillaT.mover = false;
					} else {
			            if (jugTurno.casillaT.castigo > 0) {
			                jugTurno.casillaT.castigo--;
			                jugTurno.casillaT.mover = false;
			            }
			            if (jugTurno.casillaT.castigo == 0) {
			                jugTurno.casillaT.pozoSalir = true;

			            }
			        }
				}
				getch();
				break;
			case 'L':
				jugTurno.casillaT.mover = false;
				break;
			case 'C':
				if (jugTurno.casillaT.castigo == 0) {
					jugTurno.casillaT.castigo = 2;
				} else {
					jugTurno.casillaT.castigo--;
				}
				jugTurno.casillaT.mover = false;
				break;
			case 'D':
				jugTurno.casillaT.mover = true;
				break;
			case 'V':
				jugTurno.casillaT.mover = false;
				break;
			default:
				jugTurno.casillaT.mover = false;
				break;
			}
		} //Fin else

		if (jugTurno.casillaT.mover && jugTurno.casillaT.castigo == 0 && jugTurno.casillaT.posicion != MAX_CASILLAS){
			gotoxy(1, 47);
			cout<< "\nPULSA CUALQUIER TECLA PARA IR AL SIGUIENE TURNO DE ESTE JUGADOR..."<< endl; //Fila 47
			getch();
		}

	}
	while(jugTurno.casillaT.posicion != MAX_CASILLAS && jugTurno.casillaT.mover);

	if (jugTurno.casillaT.casillaTablero == 'N' || jugTurno.casillaT.casillaTablero == 'V' || jugTurno.casillaT.casillaTablero == 'L') {
		jugTurno.casillaT.mover = true;
	}
	if ((jugTurno.casillaT.casillaTablero == 'C' || jugTurno.casillaT.casillaTablero == 'S' || jugTurno.casillaT.casillaTablero == 'Z') && jugTurno.casillaT.castigo == 0) {
	    jugTurno.casillaT.mover = true;
	}

	if(jugTurno.casillaT.posicion > MAX_CASILLAS){
		int aux = 0;
		mostrarTablero(tableroOca);
		gotoxy(1, 47);
		textcolor(RED);
		cout << jugTurno.nombreJugador << " no puede mover " << jugTurno.dado << ", se ha pasado.";
		aux = jugTurno.casillaT.posicion - MAX_CASILLAS; //exceso de casillas
		jugTurno.casillaT.posicion = MAX_CASILLAS - aux; //restar ese exceso a 63
		jugTurno.casillaT.casillaTablero = getCasilla(tableroOca, jugTurno.casillaT.posicion);
		cout << " Retrocedes hasta la casilla: [" << jugTurno.casillaT.casillaTablero << "]													" << endl; //Fila 47
		textcolor(LIGHTCYAN);
		cout << "Casilla numero: ( " << jugTurno.casillaT.posicion << " )																		" << endl; //Fila 48
		pintarCasilla(jugTurno.casillaT.posicion, tableroOca, turno, jug);
		gotoxy(1, 49);
		//3 posibilidades una vez que vuelve hacia atras
		if(jugTurno.casillaT.casillaTablero == 'N'){
			jugTurno.casillaT.mover = true;
			textcolor(LIGHTGRAY);
			cout << "Has caido en una casilla normal" << endl;
			//pintarCasilla(jugTurno.casillaT.posicion, tableroOca, turno, jug);
		}
		if(jugTurno.casillaT.casillaTablero == 'V'){
			textcolor(RED);
			cout << "Has caido en la calavera. Vuelves al inicio" << endl;
			jugTurno.casillaT.mover = true;
			jugTurno.casillaT.posicion = 0; //Reset a 0
		}
		if(jugTurno.casillaT.casillaTablero == 'O'){
			textcolor(LIGHTGREEN);
			cout << "De oca a oca y tiro porque me toca." << endl;
			jugTurno.casillaT.posicion = MAX_CASILLAS;
		}
	}

	if (jugTurno.casillaT.posicion == MAX_CASILLAS) {
		mostrarTablero(tableroOca);
		pintarCasilla(jugTurno.casillaT.posicion, tableroOca, turno, jug);
		textcolor(GREEN);
		limpiarPantalla(1, 49);
		gotoxy(1, 49);
		cout << "HAS CAIDO EN LA CASILLA DEL JARDIN DE LA OCA" << endl;
	}

	gotoxy(1, 50);
	cout << "Finaliza el turno de este jugador.";
	textcolor(WHITE);
	cout << endl << "********************************************" << endl;

	//asignar los nuevos valores del jugador. la variable la hag por comodidad
	jug.vj[turno] = jugTurno;
}
/**
 * Función que devuelve un char de la casilla donde tiene el jugador la ficha
 * @param tableroOca: tablero de la partida de la OCA
 * @param casilla: entero de la posicion de la ficha en el tablero
 */
char getCasilla (tablero tableroOca, int casilla){
	int i=0;
	char casillaTablero;
	bool encontrado = false;
	for(int f=0;f < FILAS;f++){
		for(int c=0;c<COLUMNAS;c++){
			if(casilla == i && !encontrado){ //Si la casilla coincide con la posición
				casillaTablero = tableroOca[i-1];
				encontrado = true;
			}
			i++;
		}
	}
	return casillaTablero;
}
/**
 * Procedimiento donde se trata los cambios de posición en el tablero con sus mensajes
 * @param total: entero de referencia de la posicion del jugador en el tablero
 * @param castigo: entero del número de turnos que no puede tirar el jugador
 */
void accionTablero (int &total, int castigo, int dado, bool &xc){
	textcolor(WHITE);
	cout << "--> ";
	switch (total){
	//Casilla nueva
	case 3:
		total = total+ (dado * 2);
		xc = true;
		break;

	//Casillas de los puentes
	case 6:
		textcolor(YELLOW);
		total = 12;
		cout << "De puente a puente y tiro porque me lleva la corriente. Vuelves a tirar                                                     " << endl;
		break;
	case 12:
		textcolor(YELLOW);
		total = 6;
		cout << "De puente a puente y tiro porque me lleva la corriente. Vuelves a tirar                                                     " << endl;
		break;
	//Casilla de Posada
	case 19:
		textcolor(LIGHTMAGENTA);
		if (castigo !=0){
			cout << "Sales en el siguiente turno de la posada" << endl;
		}
		else{
			cout << "Has caido en la posada, pierdes un turno                                                                                    " << endl;
		}
		break;
	//Casilla de Pozo
	case 31:
		textcolor(BROWN);
		if (castigo == 1){
			cout << "Sales en el siguiente turno del pozo" << endl;
		}
		else if (castigo > 1){
			cout << "Sigues en el pozo. Te quedan: " << castigo << " turnos" << endl;
		}
		else{
			cout << "Has caido en el pozo. Pierdes 3 turnos                                                                                                        " << endl;
		}
		break;
	//Casilla de laberinto
	case 42:
		total = 30;
		textcolor(LIGHTBLUE);
		cout << "Has caido en el laberinto. Vuelves a la casilla 30                                                                          " << endl;
		break;
	//Casilla de carcel
	case 56:
		textcolor(DARKGRAY);
		if (castigo == 1){
			cout << "Sales en el siguiente turno de la carcel" << endl;
		}
		else if (castigo == 2){
			cout << "Sigues en la carcel. Te quedan: " << castigo << " turnos" << endl;
		}
		else{
			cout << "Has caido en la carcel. Pierdes 2 turnos para volver a jugar                                                     " << endl;
		}
		break;
	//Casillas de dados
	case 26:
		total = 53;
		textcolor(BLUE);
		cout << "Has caido en la casilla de dados. Avanzas hasta la casilla 53. Vuelves a tirar                                              " << endl;
		break;
	case 53:
		total = 26;
		textcolor(BLUE);
		cout << "Has caido en la casilla de dados. Retrocedes hasta la casilla 26. Vuelves a tirar                                           " << endl;
		break;
	//Casilla de Calavera
	case 58:
		total = 0;
		textcolor(RED);
		cout << "Has caido en la calavera. Vuelves al inicio                                                                                 " << endl;
		break;
	//Casillas de oca
	case 5:
	case 9:
	case 14:
	case 18:
	case 23:
	case 27:
	case 32:
	case 36:
	case 41:
	case 45:
	case 50:
	case 54:
	case 59:
		if (total != 63 && total % 9 == 0){ //Segunda columna de la oca
			total = total + 5;
		}
		else{ //Primera columna de la oca
			total = total + 4;
		}
		textcolor(LIGHTGREEN);
		cout << "De oca a oca y tiro porque me toca. Vuelves a tirar                                                                         " << endl;
		break;
	case 63:
		textcolor(GREEN);
		cout << "Has llegado al jardín de la OCA                                                                                             " << endl;
		break;
	default:
		textcolor(LIGHTGRAY);
		cout << "Has caido en una casilla normal                                                                                             " << endl;
		break;
	}
}
/**
 * Función que devuelve el char correcto para poder ejecutar o no el programa.
 * @param resp: char introducido por el usuario
 */
char check_respuesta (char resp){
	while (resp != 'n' && resp != 'N' && resp != 's' && resp != 'S') {
		limpiarPantalla(1, 2);
		textcolor(RED);
		gotoxy(1, 2);
		cout << "Ha introducido una respuesta incorrecta.";
		textcolor(WHITE);
		cout <<	" Responda (s/n,S/N): ";
		cin >> resp;
	}
	textcolor(WHITE);
	return resp;
}
/**
 * Procedimiento que certifica que el nombre del jugador tiene 20 caracteres
 * @param nombreJugador: nombre introducido por el usuario.
 */
void check_longitudNombre (cadena nombreJugador) {
	while (strlen(nombreJugador) > MAXIMO_CARACTERES) {
		textcolor(RED);
		cout << "El nombre del jugador es muy largo.";
		cout << "Tiene que tener " << MAXIMO_CARACTERES << " caracteres." << endl;
		textcolor(WHITE);
		cout << "Introduce el nombre: ";
		cin >> nombreJugador;
	}
	//Convertir a minusculas el nombre del Jugador una vez esté correcto
	//strlen devuelve este tipo de datos que es unsigned
	for (size_t i = 0; i < strlen(nombreJugador); i++) {
		nombreJugador[i] = tolower(nombreJugador[i]);
	}
	textcolor(WHITE);
}
/**
 * Procedimiento para conventir una cadena a minusculas
 * @param nombre: cadena a convertir
 */
void convertiMinusculas (cadena nombre){
	for (size_t i = 0; i < strlen(nombre); i++) {
		nombre[i] = tolower(nombre[i]);
	}
}
/**
 * Funcion para introducir un jugador
 * @param cont: entero para ayudar al usuario
 */
jugador introducirJugador(int cont){
	jugador j;
	cout << "Introducir el nombre del jugador " << cont+1 <<" : ";
	cin >> j.nombreJugador;
	check_longitudNombre(j.nombreJugador);
	j.turno=0;
	return j;
}
/**
 * Funcion para introducir los jugadores en el vector juagdores.
 * numJug: entero del numero de jugadores
 * @param jug: referencia al vector de jugadores
 * @param nombreArchivo: puntero al nombre del fichero Jugadores.txt
 */
jugadores leerJugadores(int numJ, const char *nombreArchivo){
	jugadores jg;
	jg.cont = 0; //Inicializar el contador del vector variable
	bool encontrado;
	while(jg.cont < numJ){
		encontrado = false;
		jg.vj[jg.cont] = introducirJugador(jg.cont);
		if (jg.cont > 0) { //Una vez introducido un nombre de Jugador, comprobar que no haya 2 nombres iguales
			for (int i = 0; i < jg.cont; i++) { //Bucle del resto de jugadores
				if (jg.cont != i) {
					//Comparo el nombre del jugador introducido con el resto
					while (strcmp(jg.vj[jg.cont].nombreJugador, jg.vj[i].nombreJugador) == 0) {
						switch (jg.cont) {
						case 1:
							limpiarPantalla(1, 8);
							break;
						case 2:
							limpiarPantalla(1, 10);
							break;
						case 3:
							limpiarPantalla(1, 12);
							break;
						default:
							limpiarPantalla(1, 14);
							break;
						}
						textcolor(RED);
						cout << "No puede haber 2 jugadores con el mismo nombre.";
						textcolor(WHITE);
						jg.vj[jg.cont] = introducirJugador(jg.cont);
					}

				}
			}
		}
		check_Jugador(nombreArchivo, encontrado, jg.vj[jg.cont]);
		if(encontrado){
			cout << "Hola de nuevo " << jg.vj[jg.cont].nombreJugador << endl;
		}
		else{
			cout << "Bienvenido " << jg.vj[jg.cont].nombreJugador << ". Este juego te va a encantar" << endl;
		}
		jg.cont++; //actualizar el contador con el nuevo jugador introducido
	}
	return jg;
}
/**
 * Procedimiento que busca por nombre si el jugador está en el archivo Jugadores.txt
 * @param nombreArchivo: puntero al nombre del fichero Jugadores.txt
 * @param encontrado: referencia por booleano que certifica si el jugador se ha encontrado o no
 * @param jugadorEncontrado: referencia al jugador. Si no está, se inicializan sus valores a 0.
 */
void check_Jugador (const char *nombreArchivo, bool &encontrado, jugador &jugadorEncontrado){
	ifstream archivoJugadores(nombreArchivo);
	    if (!archivoJugadores) {
	        cout << "Se ha producido un error en la apertura del fichero Jugadores.txt" << endl;
	    } else {
	        cadena nombreAux;
	        int valor1, valor2;
	        while (archivoJugadores >> nombreAux >> valor1 >> valor2) {
	            if (strcmp(nombreAux, jugadorEncontrado.nombreJugador) == 0 && !encontrado) {
	                // Si se encuentra el nombre, guardar los valores en jugadorEncontrado
	                encontrado = true;
	                jugadorEncontrado.numeroPartidas = valor1;
	                jugadorEncontrado.victorias = valor2;
	            }
	        }

	        archivoJugadores.close(); // cerrar archivo Jugadores.txt

	        if (!encontrado) {
	            // Si no se encuentra el nombre, inicializar el jugador con los parámetros a 0
	            //strcpy(jugadorEncontrado.nombreJugador, nombreAux);
	            jugadorEncontrado.numeroPartidas = 0;
	            jugadorEncontrado.victorias = 0;
	        }
	    }
}
/**
 * Procedimiento para asignar el turno de cada jugador dependiendo del resultado de los dados
 * @param jug: referencia al vector de jugadores
 */
void turnoJugador (jugadores &jug){

	//int numJug = jug.cont;
	int turno = 1;
	int contadorEmpate = 1;
	bool empate;
	do{
		contadorEmpate = 1;
		empate = false;
		for (int i=0; i < jug.cont; i++){
			if(jug.vj[i].turno == 0){ //Solo tiran dados aquellos que no se les haya asignado el turno
				jug.vj[i].dado = lanzarDado(1, 6);
				cout << jug.vj[i].nombreJugador << "-->Dado: " << jug.vj[i].dado << "|";
			}
		}
		int indiceMax = -1;
		for (int i = 0; i < jug.cont; ++i) {
			if (jug.vj[i].turno == 0) {
				if (indiceMax == -1 || jug.vj[i].dado > jug.vj[indiceMax].dado) {
					indiceMax = i;
					contadorEmpate = 1;
				} else if (jug.vj[i].dado == jug.vj[indiceMax].dado) {
					contadorEmpate++;
				}
			}
		}
		if(contadorEmpate > 1){
			textcolor(LIGHTRED);
			cout << "Empate.";
			textcolor(WHITE);
			empate = true;
		}
		if(empate){
			cout << "Dados Again" << endl;
		}
		else{
			jug.vj[indiceMax].turno = turno;
			cout << endl << jug.vj[indiceMax].nombreJugador << " va el " << turno << endl;
			turno++; //Siguiente turno
//			numJug--;
		}
	}
	while (turno < jug.cont);
	//Ultimo jugador
	int i=0;
	bool encontrado = false;
	while (!encontrado && i < jug.cont) {
		if (jug.vj[i].turno == 0) {
			jug.vj[i].turno = turno;
			encontrado = true;
		} else {
			i++;
		}
	}
}
/**
 * Procedimiento para inicializar el atributo casilla de cada jugador
 * @param jug: referencia al vector de jugadores
 */
void inicializarCasilla (jugadores &jug){
	for(int i=0;i<jug.cont;i++){
		jug.vj[i].casillaT.posicion = 0;
		jug.vj[i].casillaT.castigo = 0;
		jug.vj[i].casillaT.mover = true;
		jug.vj[i].casillaT.pozoSalir = false;
		jug.vj[i].casillaT.xcasilla = false;
		switch (jug.vj[i].turno){
		case 1:
			jug.vj[i].casillaT.color = YELLOW;
			break;
		case 2:
			jug.vj[i].casillaT.color = LIGHTGREEN;
			break;
		case 3:
			jug.vj[i].casillaT.color = LIGHTMAGENTA;
			break;
		case 4:
			jug.vj[i].casillaT.color = BROWN;
			break;
		default:
			jug.vj[i].casillaT.color = CYAN;
			break;
		}
	}
}
/**
 * Procedimiento que recupera el tablero de la OCA del TableroOca.txt
 * @param nombreArchivo: puntero al nombre del fichero TableroOca.txt
 * @param tableroOca: referencia al struct para almacenar el tablero
 */
void getTablero (const char *nombreArchivo, tablero &tableroOca){

	ifstream archivoTablero(nombreArchivo);
	if (!archivoTablero){
		cout << "Se ha producido un error en la apertura del fichero TableroOca.txt" << endl;
	}
	else {
		int i;
		char letra;
		i = 0;
		archivoTablero >> letra;
		while (!archivoTablero.eof()) {
			tableroOca[i] = letra;
			i++;
			archivoTablero >> letra;
		}
		archivoTablero.close();
	}
}
/**
 * Procedimiento para mostrar el tablero de la OCA por pantalla
 * @param tableroOca: struct de tablero de la OCA
 */
void mostrarTablero (tablero tableroOca){

	textcolor(WHITE);
	int i = 0;
	for (int f = 0; f < FILAS; f++) {
		gotoxy(90, 20 + f* 2); //Coge cada fila de arriba
		cout << "+";
		for (int c = 0; c < COLUMNAS; c++) {
			cout << "-----+";
		}
		cout << endl;

		gotoxy(90, 21 + f * 2); //fila del tablero de la oca
		cout << "|";
		for (int c = 0; c < COLUMNAS; c++) {
			printf("%3c  |", tableroOca[i]);
			i++;
		}
		cout << endl;
	}
	gotoxy(90, 20 + FILAS * 2); //ultima fila
	cout << "+";
	for (int c = 0; c < COLUMNAS; c++) {
		cout << "-----+";
	}
	cout << endl;
}
/**
 * Función que devuelve un entero de manera aleatoria. Simula un lanzamiento de un dado
 * @param inferior: int limite inferior = 1
 * @param superior: int limite supeior = 6
 */
int lanzarDado (int inferior, int superior) {
	int alea = inferior + rand() % (superior + 1 - inferior);
	return alea;
}
/**
 * Metodo de la burbuja para ordenar el vector dependiendo del campo turno
 * @param &jug: referencia al vector de juagdores
 */
void ordenarPorTurno(jugadores &jug) {
    int n = jug.cont;
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (jug.vj[j].turno > jug.vj[j + 1].turno) {
                // Intercambiar jugadores
                jugador temp = jug.vj[j];
                jug.vj[j] = jug.vj[j + 1];
                jug.vj[j + 1] = temp;
            }
        }
    }
}
void limpiarPantalla (int fila, int columna){
	gotoxy(fila, columna);
	cout << "\r";
	cout << "                                                                                                    ";
	cout << "\r";
	gotoxy(fila, columna);
}
/**
 * Procedimiento para escribir o actualizar el resultado de los jugadores en el archivo Jugadores.txt
 * @param *nombreArchivo: puntero con el nombre: Jugadores.txt
 * @param victoria: bool para saber si el jugador ha ganado o ha perdido
 * @param jugadorActualizado: referencia al jugador
 */
void actualizarArchivoJugadores (const char *nombreArchivo, bool victoria, jugador &jugadorActualizado){
	fstream archivoJugadores(nombreArchivo, ios::in | ios::out);
	ofstream tempFile("tempFile.txt", ios::out);

	if (!archivoJugadores || !tempFile) {
		cout << "Se ha producido un error en la apertura de los archivos" << endl;
	} else {
		bool encontrado = false;
		cadena nombre;
		int num_Partidas, victorias;

		while (archivoJugadores >> nombre >> num_Partidas >> victorias) {
			if (strcmp(nombre, jugadorActualizado.nombreJugador) == 0) { // Buscar por nombre
				encontrado = true;
				if (victoria) {
					jugadorActualizado.victorias++;
				}
				jugadorActualizado.numeroPartidas++;
				cout << " | Nombre: " << jugadorActualizado.nombreJugador << ", Numero Partidas: " << jugadorActualizado.numeroPartidas << ", Victorias: " << jugadorActualizado.victorias << endl;
				tempFile << jugadorActualizado.nombreJugador << " " << jugadorActualizado.numeroPartidas << " " << jugadorActualizado.victorias << endl;
			}
			else{
				tempFile << nombre << " " << num_Partidas << " " << victorias << endl;
			}
		}

		if (!encontrado) {
			archivoJugadores.clear();
			archivoJugadores.seekp(0, ios::end);
			if (victoria) {
				jugadorActualizado.victorias = 1;
			}
			jugadorActualizado.numeroPartidas = 1;
			cout << " | Nombre: " << jugadorActualizado.nombreJugador << ", Numero Partidas: " << jugadorActualizado.numeroPartidas << ", Victorias: " << jugadorActualizado.victorias << endl;
			tempFile << jugadorActualizado.nombreJugador << " " << jugadorActualizado.numeroPartidas << " " << jugadorActualizado.victorias << endl;
		}

		archivoJugadores.close();
		tempFile.close();

		remove(nombreArchivo);
		rename("tempFile.txt", nombreArchivo);
	}
}
/**
 * Procedimiento para pintar la casilla del jugador en el tablero
 * @param casilla: entero de la posicion del jugador en el tablero
 * @param tableroOca: tablero de la OCA
 * @param turno: entero para saber el jugador que juega
 */
void pintarCasilla (int casilla, tablero tableroOca, int turno, jugadores jug){
	int i = 1;
	bool encontrado = false;
	char letra;
	int color;
	color = jug.vj[turno].casillaT.color;
	textcolor(WHITE);
	for (int f = 0; f < FILAS; f++) {
		for (int c = 0; c < COLUMNAS; c++) {
			if (casilla == i && !encontrado) { //Si la casilla coincide con la posición
				gotoxy(90 + c * 6, 21 + f * 2);
				cout << "|";
				textcolor(color);
				letra = tableroOca[i - 1]; //Posicion - 1, ya que la posicion [0,0] es la casilla 1
				printf("%3c  ", letra);

				encontrado = true;
			}
			i++;
		}
	}
	textcolor(WHITE);
}
