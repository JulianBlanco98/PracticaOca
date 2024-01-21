//============================================================================
// Name        : PF23_24julianbg.cpp
// Author      : Julián Blanco González
// Version     : 1
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

//Tipos de datos
typedef char tablero[MAX_CASILLAS]; //tablero de juego
typedef char cadena[MAXIMO_CARACTERES + 1]; //String del nombre de jugador
typedef struct {
	cadena nombreJugador;
	int numeroPartidas;
	int victorias;
} jugador;

//Funciones
char check_respuesta (char resp);
int lanzarDado (int inferior, int superior);
char getCasilla (tablero tableroOca, int casilla);

//Procedimientos
void check_longitudNombre (cadena nombreJugador);
void convertiMinusculas (cadena nombre);
void check_Jugador (const char *nombreArchivo, bool &encontrado, jugador &jugadorABuscar);
void getTablero (const char *nombreArchivo, tablero &tableroOca);
void mostrarTablero (tablero tableroOca);
void jugada (int &turno, int &total, bool &mover, jugador jugadorTurno, int dado, tablero tableroOca, int &castigo, int puntuacionPozo, int &castigoPozo);
void accionTablero (int &total, int castigo);
void pintarCasilla (int casilla, tablero tableroOca, int turno);
void actualizarArchivoJugadores (const char *nombreArchivo, bool victoria, jugador &actualizarJugador);
void limpiarPantalla (int fila, int columna);

int main() {

	system("cls");

	//Variables
	tablero tablero_Oca;
	ofstream out; //out será para el fichero de salida
	ifstream archivo_Tablero, archivo_Jugadores;
	cadena nombreJugador1, nombreJugador2;
	bool edadCorrecta = true;
	bool encontrado;
	char resp;
	int dado_jugador1, dado_jugador2;
	int turno; //0 --> jugador1, 1 --> jugador2
	const char *nombreArchivoJugadores = "Jugadores.txt";
	const char *nombreArchivoTableroOca = "TableroOca.txt";
	int puntuacionTotal_jugador1, puntuacionTotal_jugador2;
	bool movimiento1 = false, movimiento2 = false;
	int castigo1, castigo2;

	srand(time(NULL));
	textcolor(WHITE);
	cout << endl << "Desea jugar al juego de la OCA (s/n,S/N): ";
	cin >> resp;
	resp = check_respuesta(resp);
	while ((resp == 'S' || resp == 's') && edadCorrecta) {

		jugador jugador1, jugador2;
		encontrado = false; //reset de la variable
		textcolor(BROWN);
		cout << "COMENZAMOS A JUGAR..." << endl;
		cout << "PASO 1. INTRODUCIR EL NOMBRE DE LOS 2 JUGADORES" << endl;
		textcolor(WHITE);
		cout << "-->Introduce el nombre del jugador 1: ";
		cin >> nombreJugador1;
		check_longitudNombre(nombreJugador1);
		strcpy(jugador1.nombreJugador, nombreJugador1);
		check_Jugador(nombreArchivoJugadores, encontrado, jugador1);
		if (encontrado){
			cout << "Hola de nuevo " << jugador1.nombreJugador << endl;
		}
		else{
			cout << "Bienvenido " << jugador1.nombreJugador << ". Este juego te va a encantar" << endl;
		}
		encontrado = false;
		cout << "-->Introduce el nombre del jugador 2: ";
		cin >> nombreJugador2;
		convertiMinusculas(nombreJugador2);
		while (strcmp(jugador1.nombreJugador, nombreJugador2) == 0){
			limpiarPantalla(1, 7);
			gotoxy(1, 7);
			textcolor(RED);
			cout << "No puede haber 2 jugadores con el mismo nombre.";
			textcolor(WHITE);
			cout << " Introduce el nombre del jugador 2: ";
			cin >> nombreJugador2;
			convertiMinusculas(nombreJugador2);
		}
		check_longitudNombre(nombreJugador2);
		strcpy(jugador2.nombreJugador, nombreJugador2);
		check_Jugador(nombreArchivoJugadores, encontrado, jugador2);
		if (encontrado){
			cout << "Hola de nuevo " << jugador2.nombreJugador << endl;
		}
		else{
			cout << "Bienvenido " << jugador2.nombreJugador << ". Este juego te va a encantar" << endl;
		}
		if (!edadCorrecta){
			cout << "Lo siento. No tienes la edad correcta para poder jugar." << endl;
		}
		else{

			textcolor(BROWN);
			cout << "PASO 2. LANZAR LOS DADOS PARA VER QUIEN EMPIEZA." << endl;
			textcolor(WHITE);
			do{
				dado_jugador1 = lanzarDado(1, 6);
				dado_jugador2 = lanzarDado(1, 6);
				cout << jugador1.nombreJugador << " ha sacado un " << dado_jugador1 << endl;
				cout << jugador2.nombreJugador << " ha sacado un " << dado_jugador2 << endl;
				if (dado_jugador1 == dado_jugador2) {
					textcolor(YELLOW);
					cout << "Se ha producido un empate, hay que volver a tirar los dados" << endl;
				}
				textcolor(WHITE);
			}
			while (dado_jugador1 == dado_jugador2);
			if (dado_jugador1 > dado_jugador2){
				cout << "Comienza la partida " << jugador1.nombreJugador << " que ha obtenido un " << dado_jugador1 << endl;
				turno = 0;
			}
			else{
				cout << "Comienza la partida " << jugador2.nombreJugador << " que ha obtenido un " << dado_jugador2 << endl;
				turno = 1;
			}
			textcolor(LIGHTMAGENTA);
			cout << "\n\n\n( " << jugador1.nombreJugador << " ) va a usar el color LIGHMAGENTA" << endl;
			textcolor(LIGHTGREEN);
			cout << "( " << jugador2.nombreJugador << " ) va a usar el color LIGHTGREEN" << endl;
			textcolor(WHITE);
			//Recupero el tablero de la Oca del archivo txt
			getTablero(nombreArchivoTableroOca, tablero_Oca);
			mostrarTablero(tablero_Oca);

			puntuacionTotal_jugador1 = 0;//Reset de las puntuaciones de cada jugador
			puntuacionTotal_jugador2 = 0;
			castigo1 = 0;
			castigo2 = 0;
			movimiento1 = true;
			movimiento2 = true;
			textcolor(BROWN);
			cout << "PASO 3. EMPEZAR A JUGAR AL JUEGO DE LA OCA." << endl;
			textcolor(WHITE);

			do{
				limpiarPantalla(1, 40);
				if (turno == 0){ //jugador1
					textcolor(LIGHTMAGENTA);
					cout << "Turno de: " << jugador1.nombreJugador << ". Castigo: " << castigo1 << endl;
					jugada(turno, puntuacionTotal_jugador1, movimiento1, jugador1, dado_jugador1, tablero_Oca, castigo1, puntuacionTotal_jugador2, castigo2);
				}
				else{ //jugador2
					textcolor(LIGHTGREEN);
					cout << "Turno de: " << jugador2.nombreJugador << ". Castigo: " << castigo2 << endl;
					jugada(turno, puntuacionTotal_jugador2, movimiento2, jugador2, dado_jugador2, tablero_Oca, castigo2, puntuacionTotal_jugador1, castigo1);
				}
				textcolor(BROWN);
				cout << "PULSA CUALQUIER TECLA PARA EL SIGUIENTE TURNO..." << endl;
				textcolor(WHITE);
				getch();
				mostrarTablero(tablero_Oca);

			}
			while (puntuacionTotal_jugador1 != MAX_CASILLAS && puntuacionTotal_jugador2 != MAX_CASILLAS);

			gotoxy(1, 52);
			if (puntuacionTotal_jugador1 == MAX_CASILLAS){
				cout << "-----------------------------------HAS GANADO " << jugador1.nombreJugador << "-----------------------------------" << endl;
				actualizarArchivoJugadores(nombreArchivoJugadores, true, jugador1);
				actualizarArchivoJugadores(nombreArchivoJugadores, false, jugador2);
			}
			else{
				cout << "-----------------------------------HAS GANADO " << jugador2.nombreJugador << "-----------------------------------" << endl;
				actualizarArchivoJugadores(nombreArchivoJugadores, true, jugador2);
				actualizarArchivoJugadores(nombreArchivoJugadores, false, jugador1);
			}

			cout << "FIN DE LA PARTIDA. PULSA CUALQUIER BOTON PARA CONTINUAR..." << endl;
			getch();
			system("cls");
			gotoxy(1, 1);

			cout << endl << "Desea volver a jugar al juego de la OCA (s/n,S/N): ";
			cin >> resp;
			resp = check_respuesta(resp);
		}
	}
	if (resp == 'N' || resp == 'n'){
		cout << "Has pulsado " << resp << endl;
	}

	cout << "!!!Puede jugar cuando quiera!!" << endl;
	cout << "!!!Vuelva pronto!!" << endl;
	textcolor(WHITE); //Restaurar color
	system("PAUSE");
	return 0;
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
 * Función que devuelve un entero de manera aleatoria. Simula un lanzamiento de un dado
 * @param inferior: int limite inferior = 1
 * @param superior: int limite supeior = 6
 */
int lanzarDado (int inferior, int superior) {
	int alea = inferior + rand() % (superior + 1 - inferior);
	return alea;
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
void convertiMinusculas (cadena nombre){
	for (size_t i = 0; i < strlen(nombre); i++) {
		nombre[i] = tolower(nombre[i]);
	}
}
/**
 * Procedimiento para todas las posibles jugadas del jugador a la hora de tirar en el tablero
 * @param &turno: referencia de entero para saber el jugador que le toca jugar
 * @param &total: referencia de entero del total del puntos del jugador
 * @param &mover: referencia de bool para saber si el jugador puede mover ficha o no
 * @param jugadorTurno: jugador que le toca el turno
 * @param dado: dado del jugador
 * @param tableroOca: tablero de la partida de la OCA
 * @param &castigo: referencia de entero del número de turnos que no puede tirar el jugador
 * @param puntuacionPozo: entero de la posicion del otro jugador para comprobar la casusitica del pozo
 * @param &castigoPozo: referencia al entero del castigo del otro jugador para que pueda salir del pozo si el jugador que está jugando cae en el pozo
 */
void jugada (int &turno, int &total, bool &mover, jugador jugadorTurno, int dado, tablero tableroOca, int &castigo, int puntuacionPozo, int &castigoPozo) {

	//Fila 40
	int turnoAux;
	turnoAux = turno;
	char casillaTablero;
	int contador = 1;
	textcolor(WHITE);
	cout << "********************************************" << endl; //fila 41
	gotoxy(1,51);
	textcolor(WHITE);
	cout << "********************************************" << endl; //fila 51
	for (int i=42; i< 51;i++){
		limpiarPantalla(1, i);
	}
	limpiarPantalla(1, 52);
	gotoxy(1, 42);
	textcolor(WHITE);
	cout << "Turno: " << contador;
	textcolor(LIGHTCYAN);
	cout << ". Casilla Inicial: ( " << total << " )" << endl; //Fila 42
	do{

		//Tiradas adicionales siempre que se haya caído en una casilla de tirada adicional
		if (contador > 1 && castigo == 0 && mover && total != MAX_CASILLAS){
			//mostrarTablero(tableroOca);
			for (int i = 42; i < 49; i++) { //Limpio pantalla de la 42 a la 48
				limpiarPantalla(1, i);
			}
			gotoxy(1, 42);
			textcolor(WHITE);
			cout << "Turno: " << contador;
			textcolor(LIGHTCYAN);
			cout << " | Casilla: ( " << total << " ) | Tipo casilla: ( " << casillaTablero << " )		          " << endl; //Fila 42
			limpiarPantalla(1, 43);
			textcolor(WHITE);
			cout << "Tirada adicional. PULSA CUALQUIER TECLA PARA LANZAR LOS DADOS... | ";
			getch();
			dado = lanzarDado(1, 6);
			cout << " --> Ha sacado un: " << dado << endl; //Fila 43
			total = total + dado;
			contador ++;

		}
		//Primer lanzamiento de dados si se puede
		if (contador == 1 && mover && castigo == 0 && total != MAX_CASILLAS){
			if (total != 0){
				pintarCasilla(total, tableroOca, turno);
			}
			limpiarPantalla(1, 43);
			gotoxy(1, 43);
			textcolor(WHITE);
			cout << "PULSA CUALQUIER TECLA PARA LANZAR LOS DADOS...| ";
			getch();
			dado = lanzarDado(1, 6);
			cout << " --> Ha sacado un: " << dado << endl; //Fila 43
			total = total + dado; //casilla en la que cae el jugador
			contador ++;
		}

		if (total >= MAX_CASILLAS){
			mover = false;
		}
		else{

		//1. Recupero la casilla donde pone la ficha el jugadors
			casillaTablero = getCasilla(tableroOca, total);
			//2. Dependiendo de la casilla, muevo la ficha en el tablero
			accionTablero(total, castigo); //Dependiendo de la casilla, cambio de sitio o no la ficha | Fila 44
			//3. Pinto la nueva posición de la ficha
			textcolor(WHITE);
			cout << "Turno: " << contador;
			textcolor(LIGHTCYAN);
			cout << " | Casilla: ( " << total << " ) | Tipo casilla: ( " << casillaTablero << " )		          " << endl; //Fila 45
			mostrarTablero(tableroOca); //Reset de tablero
			pintarCasilla(total, tableroOca, turno); //Pintar la nueva posicion de la ficha
			//getch();
			//4. Dependiendo de la nueva posición de la ficha, ver si pierde turno o no
			switch (casillaTablero) {
			case 'O':
				mover = true;
				break;
			case 'P':
				mover = true;
				break;
			case 'S':
				if (castigo == 0) {
					castigo = 1;
				} else {
					castigo--;
				}
				mover = false;
				break;
			case 'Z':
				if (castigo == 0) {
					mover = false;
					castigo = 3;
					if (puntuacionPozo == 31){
						castigoPozo = 1;
					}
				} else {
					castigo--;
					mover = false;
					if (puntuacionPozo == 31) {
						castigoPozo = 1;
					}
				}
				break;
			case 'L':
				mover = false;
				break;
			case 'C':
				if (castigo == 0) {
					castigo = 2;
				} else {
					castigo--;
				}
				mover = false;
				break;
			case 'D':
				mover = true;
				break;
			case 'V':
				mover = false;
				break;
			default:
				mover = false;
				break;
			}
		} //Fin else


		if (mover && castigo == 0 && total != MAX_CASILLAS){
			gotoxy(1, 47);
			cout << "\nPULSA CUALQUIER TECLA PARA IR AL SIGUIENE TURNO DE ESTE JUGADOR..." << endl; //Fila 47
			getch();
		}
	} //fin del do
	while (total != MAX_CASILLAS && mover); //se sale del bucle cuando caiga en una casilla que se pierda turno, normal o no se pase del total

	if (casillaTablero == 'N' || casillaTablero == 'V' || casillaTablero == 'L'){
		mover = true;
	}
	if (casillaTablero == 'C' && castigo == 0){
		mover = true;
	}
	if (casillaTablero == 'S' && castigo == 0){
		mover = true;
	}
	if (casillaTablero == 'Z' && castigo == 0){
		mover = true;
	}
	//Si supera 63,
	if (total > MAX_CASILLAS) {
		//mover = false;
		int aux = 0;
		//limpiarPantalla(1, 43);
		mostrarTablero(tableroOca);
		gotoxy(1, 47);
		textcolor(RED);
		cout << jugadorTurno.nombreJugador << " no puede mover " << dado << ", se ha pasado.";
		aux = total - MAX_CASILLAS; //exceso de casillas
		total = MAX_CASILLAS - aux; //restar ese exceso a 63
		casillaTablero = getCasilla(tableroOca, total);
		cout << " Retrocedes hasta la casilla: [" << casillaTablero << "]													" << endl; //Fila 47
		textcolor(CYAN);
		cout << "Casilla numero: ( " << total << " )																		" << endl; //Fila 48
		pintarCasilla(total, tableroOca, turno);
		gotoxy(1, 49);
		if (casillaTablero == 'V'){
			textcolor(RED);
			cout << "Has caido en la calavera. Vuelves al inicio" << endl;
			mover = true;
			total = 0;
		}
		if (casillaTablero == 'N'){
			mover = true;
			textcolor(LIGHTGRAY);
			cout << "Has caido en una casilla normal" << endl;
			pintarCasilla(total, tableroOca, turno);
		}
		if (casillaTablero == 'O'){
			textcolor(LIGHTGREEN);
			cout << "De oca a oca y tiro porque me toca." << endl;
			total = 63;
		}
	}

	if (total == MAX_CASILLAS){
		mostrarTablero(tableroOca);
		pintarCasilla(total, tableroOca, turno);
		textcolor(GREEN);
		limpiarPantalla(1, 49);
		gotoxy(1, 49);
		cout << "HAS CAIDO EN LA CASILLA DEL JARDIN DE LA OCA" << endl;
	}

	gotoxy(1, 50);
	textcolor(WHITE);
	if (turnoAux == turno){
		turno = 1 - turno; //cambiar el turno de jugador
	}
	cout << "Finaliza el turno de este jugador.";
	textcolor(WHITE);
	cout << endl << "********************************************" << endl;
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
void accionTablero (int &total, int castigo){
	textcolor(WHITE);
	cout << "--> ";
	switch (total){
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
 * Procedimiento para pintar la casilla del jugador en el tablero
 * @param casilla: entero de la posicion del jugador en el tablero
 * @param tableroOca: tablero de la OCA
 * @param turno: entero para saber el jugador que juega
 */
void pintarCasilla (int casilla, tablero tableroOca, int turno){
	int i = 1;
	bool encontrado = false;
	char letra;
	int color;
	if (turno == 0) {
		color = LIGHTMAGENTA;
	} else {
		color = LIGHTGREEN;
	}
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
 * Procedimiento para limpiar la consola
 * @param fila: entero de la fila
 * @param columna: entero de la columna
 */
void limpiarPantalla (int fila, int columna){
	gotoxy(fila, columna);
	cout << "\r";
	cout << "                                                                                                    ";
	cout << "\r";
	gotoxy(fila, columna);
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
