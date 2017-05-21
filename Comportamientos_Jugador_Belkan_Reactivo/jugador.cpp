#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include <stdlib.h>	//Biblioteca para usar la función rand
#include <queue>

using namespace std;

Action ComportamientoJugador::think(Sensores sensores){
	bool continuarNormal = true;
	bool saltarPensar = false;
	
	//Decisión de la siguiente acción
	Action accion = actIDLE;

	//Actualización de la información del mundo
	switch (ultimaAccion){
		case actFORWARD:
			if(!sensores.colision){
				switch (brujula){
					case 0: // Norte
						fil--;
						break;
					case 1: // Este
						col++;
						break;
					case 2: // Sur
						fil++;
						break;
					case 3: // Oeste
						col--;
						break;
				}
			}
			break;
		case actTURN_L:
			brujula = (brujula + 3) % 4;
			if(rand() % 2 == 0){
				girar_derecha = true;
			}
			else{
				girar_derecha = false;
			}
			break;
		case actTURN_R:
			brujula = (brujula + 1) % 4;
			if(rand() % 2 == 0){
				girar_derecha = true;
			}
			else{
				girar_derecha = false;
			}
			break;
		//En cualquiera de estos dos casos:
		case actGIVE:
		case actTHROW:
		case actPUTDOWN:
			if(!mochila.empty()){
				objetoMano = mochila.front();
				mochila.pop();
			}
			saltarPensar = true;
			accion = actPOP;
			break;
	}

	if(sensores.reset){
		reinicio(sensores);
	}

	if(sensores.terreno[0] == 'K' && !bien_situado){
		fil = sensores.mensajeF;
		col = sensores.mensajeC;
		bien_situado = true;
	}

	if(bien_situado){
		//mapaResultado[fil][col] = sensores.terreno[0];
		guardarVisitado(sensores);
	}

	if(girarEnDerTres){
		accion = actFORWARD;
		saltarPensar = true;
		girarEnDerTres = false;
	}

	if(girarEnDerDos){
		accion = actFORWARD;
		saltarPensar = true;
		girarEnDerDos = false;
	}

	if(girarEnDerUno){
		accion = actFORWARD;
		saltarPensar = true;
		girarEnDerUno = false;
		girarYaDer = true;
	}

	if(girarYaDer){
		accion = actTURN_R;
		saltarPensar = true;
		girarYaDer = false;
	}

	if(girarEnIzTres){
		accion = actFORWARD;
		saltarPensar = true;
		girarEnIzTres = false;
	}

	if(girarEnIzDos){
		accion = actFORWARD;
		saltarPensar = true;
		girarEnIzDos = false;
	}

	if(girarEnIzUno){
		accion = actFORWARD;
		saltarPensar = true;
		girarEnIzUno = false;
		girarYaIz = true;
	}

	if(girarYaIz){
		accion = actTURN_L;
		saltarPensar = true;
		girarYaIz = false;
	}

	if(mochila.size() < 4 && bien_situado){
		Buscar(sensores, 'O');
		//Se buscarán objetos solo si ya ha encontrado el GPS y tiene espacio en la mochila
	}

	if(objetoMano == '3'){
		Buscar(sensores, 'D');
		//Solo buscará la puerta cuando tenga la llave activa
		//En este casono hará falta el GPS, ya que puede que esté encerrado
	}

	if(!bien_situado){
		Buscar(sensores, 'K');
	}

	if(!saltarPensar){
		/*
			Cuando pasosGirar llega a 0, no dejará seguir recto a Belkan y estará obligado a girar.
			Además, pasosGirar será actualizado a un valor aleatorio otra vez.
		*/
		if(pasosGirar == 0){
			continuarNormal = false;
			pasosGirar = (rand() % 50) + 1;

			if(sensores.vida < 100){
				pasosGirar = (rand() % 10) + 1;
			}
		}

		if (PuedeAvanzar(sensores) && continuarNormal){
			accion = actFORWARD;
			pasosGirar--;	//Cada vez que avance recto, pasosGirar se disminuirá en uno
			estaAbierta = false;	//Si ha avanzado es que no se ha encontrado una puerta, y en ese caso la puerta ya se ha cerrado
		}
		else{
			/*
				Si pasosGirar ha llegado a 0, ya toca girar.
				Con este bloque se if se evita que Belkan intente
				girar hacia el precipicio, lo que sería una pérdida
				de tiempo y de vida innecesarios.
			*/
			if((sensores.terreno[1] == 'P' || sensores.terreno[3] == 'P') && !continuarNormal){
				if(sensores.terreno[1] == 'P' && sensores.terreno[3] != 'P'){
					accion = actTURN_R;
				}
				else{
					if(sensores.terreno[1] != 'P' && sensores.terreno[3] == 'P'){
						accion = actTURN_L;
					}
				}
			}
			else{
				if(sensores.terreno[2] == 'l' && objetoMano == '0'){	//Si se encuentra con un lobo y tiene un hueso en la mano
					accion = actGIVE;									//Le dará el hueso
					objetoMano = '-';
				}
				else{
					/*
						Si se encuentra con un bosque y tiene objetos en la mochila
						deberá buscar en su mochila, soltando los objetos, un
						bikini o unas zapatillas.
						Esto lo hará hasta que la mochila se quede vacía, si la
						deja en este estado y no encuentra el bikini o las zapatillas,
						deberá girar.

						Ahora también con las llaves.
					*/
					if(BuscarEnMochila(sensores)){
						accion = actTHROW;
					}
					else{
						if(SituacionIdonea(sensores)){	//Se da cuando tiene un objeto que sirve para lo que tiene delante
							accion = actFORWARD;

							if(sensores.terreno[2] == 'D' && !estaAbierta){
								accion = actGIVE;
								estaAbierta = true;
							}

							pasosGirar--;
						}
						else{
							if(HayObjeto(sensores)){
								if(objetoMano == '-'){
									accion  = actPICKUP;

									objetoMano = sensores.superficie[2];
								}
								else{   //Guarda el objeto en la mochila y se queda con las manos libres
									if(mochila.size() < 4){
										accion = actPUSH;
										mochila.push(objetoMano);
										objetoMano = '-';
									}
									else{
										accion = Girar(sensores);
									}
								}
							}
							else{
								accion = Girar(sensores);	//En esta función se determinará el sentido del giro
							}
						}
					}
				}
			}
		}
	}

	/*
		cout << "Terreno: ";
		for (int i = 0; i < sensores.terreno.size(); i++)
			cout << sensores.terreno[i];
		cout << endl;

		cout << "Superficie: ";
		for (int i = 0; i < sensores.superficie.size(); i++)
			cout << sensores.superficie[i];
		cout << endl;

		cout << "Colisión: " << sensores.colision << endl;
		cout << "Mochila: " << sensores.mochila << endl;
		cout << "Reset: " << sensores.reset << endl;
		cout << "Vida: " << sensores.vida << endl;
		cout << "Fila: " << sensores.mensajeF << endl;
		cout << "Columna: " << sensores.mensajeC << endl;
		cout << "objetoActivo: " << sensores.objetoActivo << endl;
		cout << endl;
	*/

	ultimaAccion = accion;

	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor){
	return false;
}

void ComportamientoJugador::reinicio(Sensores sensores){
	brujula = 0;
	fil = 99;
	col = 99;
	pasosGirar = (rand() % 50) + 1;	//Se le da un nuevo valor
	objetoMano = '-';	//Se vacían las manos
	bien_situado = false;
	girarEnIzUno = girarEnIzDos = girarEnIzTres = girarYaIz = false;
	girarEnDerUno = girarEnDerDos = girarEnDerTres = girarYaDer = false;
	girar_derecha = false;
	estaAbierta = false;
	ultimaAccion = actIDLE;

	for(int i = 0; !mochila.empty(); i++){	//Se vacía la mochila
		mochila.pop();
	}
}

bool ComportamientoJugador::HayObjeto(Sensores sensores){
	return (sensores.superficie[2] == '0' || sensores.superficie[2] == '1' || sensores.superficie[2] == '2' || sensores.superficie[2] == '3');
}

bool ComportamientoJugador::PuedeAvanzar(Sensores sensores){
	return ((sensores.terreno[2] == 'T' || sensores.terreno[2] == 'S' || sensores.terreno[2] == 'K') && (sensores.superficie[2] == '_'));
}

bool ComportamientoJugador::BuscarEnMochila(Sensores sensores){
	bool tirarObjetos = false;
	queue<char> mochilaExtra = mochila;

	if(!mochila.empty()){
		if(sensores.terreno[2] == 'A' && objetoMano != '1'){
			for(int i = 0; !mochilaExtra.empty() && !tirarObjetos; i++){
				if(mochilaExtra.front() == '1'){
					tirarObjetos = true;
				}
				else{
					mochilaExtra.pop();
				}
			}
		}
		else{
			if(sensores.terreno[2] == 'B' && objetoMano != '2'){
				for(int i = 0; !mochilaExtra.empty() && !tirarObjetos; i++){
					if(mochilaExtra.front() == '2'){
						tirarObjetos = true;
					}
					else{
						mochilaExtra.pop();
					}
				}
			}
			else{
				if(sensores.terreno[2] == 'D' && objetoMano != '3'){
					for(int i = 0; !mochilaExtra.empty() && !tirarObjetos; i++){
						if(mochilaExtra.front() == '3'){
							tirarObjetos = true;
						}
						else{
							mochilaExtra.pop();
						}
					}
				}
			}
		}
	}

	return tirarObjetos;
}

bool ComportamientoJugador::SituacionIdonea(Sensores sensores){
								//Agua				//Bikini						//Bosque			//Zapatillas					//Puerta				//Llave
	return (sensores.terreno[2] == 'A' && objetoMano == '1') || (sensores.terreno[2] == 'B' && objetoMano == '2') || (sensores.terreno[2] == 'D' && objetoMano == '3');
}

void ComportamientoJugador::Buscar(Sensores sensores, char QueBuscas){
	switch(QueBuscas){
		case 'k':
		case 'K':	//GPS
			if(sensores.terreno[1] == 'K'){
				girarEnIzUno = true;
			}
			else{
				if(sensores.terreno[3] == 'K'){
					girarEnDerUno = true;
				}
				else{
					if(sensores.terreno[5] == 'K'){
						girarEnIzDos = true;
					}
					else{
						if(sensores.terreno[7] == 'K'){
							girarEnDerDos = true;
						}
						else{
							if(sensores.terreno[11] == 'K'){
								girarEnIzTres = true;
							}
							else{
								if(sensores.terreno[13] == 'K'){
									girarEnDerTres = true;
								}
							}
						}
					}
				}
			}
			break;
		case 'o':
		case 'O':	//Objeto
			if(sensores.terreno[1] == '0' || sensores.terreno[1] == '1' || sensores.terreno[1] == '2' || sensores.terreno[1] == '3'){
				girarEnIzUno = true;
			}
			else{
				if(sensores.terreno[3] == '0' || sensores.terreno[3] == '1' || sensores.terreno[3] == '2' || sensores.terreno[3] == '3'){
					girarEnDerUno = true;
				}
				else{
					if(sensores.terreno[5] == '0' || sensores.terreno[5] == '1' || sensores.terreno[5] == '2' || sensores.terreno[5] == '3'){
						girarEnIzDos = true;
					}
					else{
						if(sensores.terreno[7] == '0' || sensores.terreno[7] == '1' || sensores.terreno[7] == '2' || sensores.terreno[7] == '3'){
							girarEnDerDos = true;
						}
						else{
							if(sensores.terreno[11] == '0' || sensores.terreno[11] == '1' || sensores.terreno[11] == '2' || sensores.terreno[11] == '3'){
								girarEnIzTres = true;
							}
							else{
								if(sensores.terreno[13] == '0' || sensores.terreno[13] == '1' || sensores.terreno[13] == '2' || sensores.terreno[13] == '3'){
									girarEnDerTres = true;
								}
							}
						}
					}
				}
			}
			break;
		case 'd':
		case 'D':	//Puerta
			if(sensores.terreno[1] == 'D'){
				girarEnIzUno = true;
			}
			else{
				if(sensores.terreno[3] == 'D'){
					girarEnDerUno = true;
				}
				else{
					if(sensores.terreno[5] == 'D'){
						girarEnIzDos = true;
					}
					else{
						if(sensores.terreno[7] == 'D'){
							girarEnDerDos = true;
						}
						else{
							if(sensores.terreno[11] == 'D'){
								girarEnIzTres = true;
							}
							else{
								if(sensores.terreno[13] == 'D'){
									girarEnDerTres = true;
								}
							}
						}
					}
				}
			}
			break;
	}

	//Se añaden también las minúsculas al switch por si acaso
}

Action ComportamientoJugador::Girar(Sensores sensores){
	Action accion;

	if(!girar_derecha){
		accion = actTURN_L;

		girar_derecha = (rand() % 2 == 0);
		pasosGirar = (rand() % 50) + 1;
		/*
			Si gira, ya sea a la izquierda o a la derecha, pasosGirar tendrá que
			volver a tomar un valor nuevo
		*/
	}
	else{
		accion = actTURN_R;

		girar_derecha = (rand() % 2 == 0);
		pasosGirar = (rand() % 50) + 1;
	}

	if(sensores.vida < 100){	//Cuando le queden menos de 100 puntos de vida empezará a girar más
		pasosGirar = (rand() % 10) + 1;
	}

	return accion;
}

void ComportamientoJugador::guardarVisitado(Sensores sensores){
	mapaResultado[fil][col] = sensores.terreno[0];

	if (brujula == 0){
		mapaResultado[fil-1][col-1] = sensores.terreno[1];
		mapaResultado[fil-1][col] = sensores.terreno[2];
		mapaResultado[fil-1][col+1] = sensores.terreno[3];

		mapaResultado[fil-2][col-2] = sensores.terreno[4];
		mapaResultado[fil-2][col-1] = sensores.terreno[5];
		mapaResultado[fil-2][col] = sensores.terreno[6];
		mapaResultado[fil-2][col+1] = sensores.terreno[7];
		mapaResultado[fil-2][col+2] = sensores.terreno[8];

		mapaResultado[fil-3][col-3] = sensores.terreno[9];
		mapaResultado[fil-3][col-2] = sensores.terreno[10];
		mapaResultado[fil-3][col-1] = sensores.terreno[11];
		mapaResultado[fil-3][col] = sensores.terreno[12];
		mapaResultado[fil-3][col+1] = sensores.terreno[13];
		mapaResultado[fil-3][col+2] = sensores.terreno[14];
		mapaResultado[fil-3][col+3] = sensores.terreno[15];
	}
	else
		if (brujula == 1){
			mapaResultado[fil-1][col+1] = sensores.terreno[1];
			mapaResultado[fil][col+1] = sensores.terreno[2];
			mapaResultado[fil+1][col+1] = sensores.terreno[3];

			mapaResultado[fil-2][col+2] = sensores.terreno[4];
			mapaResultado[fil-1][col+2] = sensores.terreno[5];
			mapaResultado[fil][col+2] = sensores.terreno[6];
			mapaResultado[fil+1][col+2] = sensores.terreno[7];
			mapaResultado[fil+2][col+2] = sensores.terreno[8];

			mapaResultado[fil-3][col+3] = sensores.terreno[9];
			mapaResultado[fil-2][col+3] = sensores.terreno[10];
			mapaResultado[fil-1][col+3] = sensores.terreno[11];
			mapaResultado[fil][col+3] = sensores.terreno[12];
			mapaResultado[fil+1][col+3] = sensores.terreno[13];
			mapaResultado[fil+2][col+3] = sensores.terreno[14];
			mapaResultado[fil+3][col+3] = sensores.terreno[15];
		}
		else
			if (brujula == 2){
				mapaResultado[fil+1][col+1] = sensores.terreno[1];
				mapaResultado[fil+1][col] = sensores.terreno[2];
				mapaResultado[fil+1][col-1] = sensores.terreno[3];

				mapaResultado[fil+2][col+2] = sensores.terreno[4];
				mapaResultado[fil+2][col+1] = sensores.terreno[5];
				mapaResultado[fil+2][col] = sensores.terreno[6];
				mapaResultado[fil+2][col-1] = sensores.terreno[7];
				mapaResultado[fil+2][col-2] = sensores.terreno[8];

				mapaResultado[fil+3][col+3] = sensores.terreno[9];
				mapaResultado[fil+3][col+2] = sensores.terreno[10];
				mapaResultado[fil+3][col+1] = sensores.terreno[11];
				mapaResultado[fil+3][col] = sensores.terreno[12];
				mapaResultado[fil+3][col-1] = sensores.terreno[13];
				mapaResultado[fil+3][col-2] = sensores.terreno[14];
				mapaResultado[fil+3][col-3] = sensores.terreno[15];
			}
			else
				if (brujula == 3){
					mapaResultado[fil+1][col-1] = sensores.terreno[1];
					mapaResultado[fil][col-1] = sensores.terreno[2];
					mapaResultado[fil-1][col-1] = sensores.terreno[3];

					mapaResultado[fil+2][col-2] = sensores.terreno[4];
					mapaResultado[fil+1][col-2] = sensores.terreno[5];
					mapaResultado[fil][col-2] = sensores.terreno[6];
					mapaResultado[fil-1][col-2] = sensores.terreno[7];
					mapaResultado[fil-2][col-2] = sensores.terreno[8];

					mapaResultado[fil+3][col-3] = sensores.terreno[9];
					mapaResultado[fil+2][col-3] = sensores.terreno[10];
					mapaResultado[fil+1][col-3] = sensores.terreno[11];
					mapaResultado[fil][col-3] = sensores.terreno[12];
					mapaResultado[fil-1][col-3] = sensores.terreno[13];
					mapaResultado[fil-2][col-3] = sensores.terreno[14];
					mapaResultado[fil-3][col-3] = sensores.terreno[15];
				}
}
