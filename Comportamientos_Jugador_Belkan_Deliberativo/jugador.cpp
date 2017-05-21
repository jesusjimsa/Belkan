#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include <vector>
#include <queue>
#include <stdlib.h>	//Biblioteca para usar la función rand y la abs
#include <cmath>
#include <list>
#include <algorithm>	//find
#include <set>

using namespace std;

/*
	Enlaces de interés:
		Explicación A*  http://www.policyalmanac.org/games/articulo1.htm
		Explicación A*  https://www.youtube.com/watch?v=-L-WgKMFuhE
		Código A*	    https://www.youtube.com/watch?v=mZfyt03LDH4
		A* más rápido	https://www.youtube.com/watch?v=3Dw5d7PlcTM
		Explicación A*  https://www.youtube.com/watch?v=ySN5Wnu88nE
*/

/*
	- Primer estado: dónde está
	- Segundo estado: dónde está su padre
	- Enteros: costes, que se calculan así:
		· costeG: coste de movimiento desde la casilla inicial hasta
		la casilla examinada
			- 10 -> recto
			- 20 -> girar
			- 30 -> agua o bosque
			- 5000 -> obstáculo
		· costeH: coste de movimiento hasta destino
			- Se calcula con el método distanciaNodos
		· costeF: suma de costeG y costeH
*/


bool ComportamientoJugador::pathFinding(const estado &origen, const estado &destino, list<Action> &plan){
	list< Nodo > abiertosLista, cerrados;
	priority_queue< Nodo, vector<Nodo>, functorNodo> abiertos;
	list< Nodo > vecinos;	//izquierda, delante y derecha
	list< Action > chachiGuay;
	Nodo insercion;
	Nodo current;
	Nodo original, target;
	estado moverDer, moverDel, moverIzq;
	char queHay[3];		//0 -> izquierda; 1 -> delante; 2 -> derecha
	int costeG, costeH;

	//Borro la lista
	plan.clear();

	//Insertar nodo incial en abiertos
	original.setEstado(origen);
	original.setCosteF(0);
	original.setCosteG(0);
	original.setCosteH(0);

	abiertos.push(original);
	abiertosLista.push_back(original);

	//Crear un nodo para el destino
	target.setEstado(destino);

	//Comienza el algoritmo A*
	while(!abiertos.empty()){
		current.setEstado(abiertos.top().getEstado());
		current.setCosteF(abiertos.top().getCosteF());
		current.setCosteH(abiertos.top().getCosteH());
		current.setCosteG(abiertos.top().getCosteG());
		
		list<Action> caminoNuevoCerrado = abiertos.top().getCamino();

		abiertosLista.erase(find(abiertosLista.begin(), abiertosLista.end(), abiertos.top()));

		abiertos.pop();

		//Guardamos su camino en la lista chachiGuay
		for(int i = 0; !caminoNuevoCerrado.empty(); i++){
			chachiGuay.push_back(caminoNuevoCerrado.front());
			caminoNuevoCerrado.pop_front();
		}

		//Guardamos current en cerrados y lo borramos de abiertos
		cerrados.push_back(current);

		if(current.getFila() == destino.fila && current.getColumna() == destino.columna){
			/*
				Como en el bucle las acciones se hacen para colocarse encima de la
				casilla de destino, después de darle la vuelta al plan, le borramos
				la última acción, que siempre será actFORWARD (aunque esto no tiene
				nada que ver)
			*/

			plan = chachiGuay;

			if(!plan.empty()){
				plan.pop_back();
			}

			return true;		//Si hemos llegado a destino, salimos del bucle, es decir, hay un camino
		}

		//Comprueba qué hay en cada nodo visitable para asignarle el coste más adelante
		switch(current.getBrujula()){
			case 0:
				queHay[0] = mapaResultado[current.getFila()][current.getColumna() - 1];
				queHay[1] = mapaResultado[current.getFila() - 1][current.getColumna()];
				queHay[2] = mapaResultado[current.getFila()][current.getColumna() + 1];

				moverIzq.fila = current.getFila();
				moverIzq.columna = current.getColumna() - 1;
				moverIzq.orientacion = 3;
				moverDel.fila = current.getFila() - 1;
				moverDel.columna = current.getColumna();
				moverDel.orientacion = 0;
				moverDer.fila = current.getFila();
				moverDer.columna = current.getColumna() + 1;
				moverDer.orientacion = 1;
				
				break;
			case 1:
				queHay[0] = mapaResultado[current.getFila() - 1][current.getColumna()];
				queHay[1] = mapaResultado[current.getFila()][current.getColumna() + 1];
				queHay[2] = mapaResultado[current.getFila() + 1][current.getColumna()];

				moverIzq.fila = current.getFila() - 1;
				moverIzq.columna = current.getColumna();
				moverIzq.orientacion = 0;
				moverDel.fila = current.getFila();
				moverDel.columna = current.getColumna() + 1;
				moverDel.orientacion = 1;
				moverDer.fila = current.getFila() + 1;
				moverDer.columna = current.getColumna();
				moverDer.orientacion = 2;
				
				break;
			case 2:
				queHay[0] = mapaResultado[current.getFila()][current.getColumna() + 1];
				queHay[1] = mapaResultado[current.getFila() + 1][current.getColumna()];
				queHay[2] = mapaResultado[current.getFila()][current.getColumna() - 1];

				moverIzq.fila = current.getFila();
				moverIzq.columna = current.getColumna() + 1;
				moverIzq.orientacion = 1;
				moverDel.fila = current.getFila() + 1;
				moverDel.columna = current.getColumna();
				moverDel.orientacion = 2;
				moverDer.fila = current.getFila();
				moverDer.columna = current.getColumna() - 1;
				moverDer.orientacion = 3;

				break;
			case 3:
				queHay[0] = mapaResultado[current.getFila() + 1][current.getColumna()];
				queHay[1] = mapaResultado[current.getFila()][current.getColumna() - 1];
				queHay[2] = mapaResultado[current.getFila() - 1][current.getColumna()];

				moverIzq.fila = current.getFila() + 1;
				moverIzq.columna = current.getColumna();
				moverIzq.orientacion = 2;
				moverDel.fila = current.getFila();
				moverDel.columna = current.getColumna() - 1;
				moverDel.orientacion = 3;
				moverDer.fila = current.getFila() - 1;
				moverDer.columna = current.getColumna();
				moverDer.orientacion = 0;

				break;
		}

		//Asigno los costes
		vecinos.clear();
		for(int i = 0; i < 3; i++){
			switch(i){
				case 0:
					insercion.setEstado(moverIzq);

					insercion.addAccion(actTURN_L);
					insercion.addAccion(actFORWARD);

					costeG = 20;

					break;
				case 1:
					insercion.setEstado(moverDel);

					insercion.addAccion(actFORWARD);

					costeG = 10;

					break;
				case 2:
					insercion.setEstado(moverDer);

					insercion.addAccion(actTURN_R);
					insercion.addAccion(actFORWARD);

					costeG = 20;

					break;
			}

			switch(queHay[i]){
				case 'A':	//Agua
				case 'B':	//Bosque
				case 'P':	//Precipicio
				case 'M':	//Muro
				case 'D':	//Puerta
				case '?':	//Inexplorado
					costeG = 5000;
					
					break;
				//El resto de casos (cuando es sencillo caminar por el terreno)
				//se ha hecho ya en el switch anterior
			}			


			costeH = distanciaNodos(original, insercion);

			insercion.setCosteG(costeG);
			insercion.setCosteH(costeH);
			insercion.setCosteF(costeG + costeH);
			
			vecinos.push_back(insercion);

		}

		//Aquí viene la chicha del algoritmo
		for(auto it = vecinos.begin(); it != vecinos.end(); ++it){
				//La casilla es transitable							//La casilla no está en cerrados
			if((*it).getCosteG() < 5000 && find(cerrados.begin(), cerrados.end(), (*it)) == cerrados.end()){
				int nuevoMovimientoVecino = current.getCosteG() + distanciaNodos(current, (*it));
				auto estaEnAb = find(abiertosLista.begin(), abiertosLista.end(), (*it));	//Para comprobar si está ya en abiertosLista

						//Cuesta menos llegar así					//No está en abiertos
				if(nuevoMovimientoVecino < (*it).getCosteG() || estaEnAb == abiertosLista.end()){
					// (*it).setCosteG(nuevoMovimientoVecino);
					// (*it).setCosteH(distanciaNodos((*it), target));

					if(estaEnAb == abiertosLista.end()){
						abiertos.push(*it);
						abiertosLista.push_back(*it);
					}
				}
			}
		}
	}

	return false;	//No ha conseguido encontrar un camino al rey
}

bool ComportamientoJugador::Nodo::operator==(Nodo &otro) const{
	return propio.fila == otro.getFila() && propio.columna == otro.getColumna() && propio.orientacion == otro.getBrujula();
}

bool ComportamientoJugador::Nodo::operator!=(Nodo &otro) const{
	return propio.fila != otro.getFila() && propio.columna != otro.getColumna() && propio.orientacion != otro.getBrujula();
}

bool ComportamientoJugador::Nodo::operator==(const Nodo &otro) const{
	return propio.fila == otro.getFila() && propio.columna == otro.getColumna() && propio.orientacion == otro.getBrujula();
}

bool ComportamientoJugador::Nodo::operator!=(const Nodo &otro) const{
	return propio.fila != otro.getFila() && propio.columna != otro.getColumna() && propio.orientacion != otro.getBrujula();
}

int ComportamientoJugador::distanciaNodos(const Nodo &uno, const Nodo &otro){
	int distX = abs(uno.getColumna() - otro.getColumna());
	int distY = abs(uno.getFila() - otro.getFila());
	int distancia = distX + distY;

	return distancia;
}

void ComportamientoJugador::PintaPlan(list<Action> plan){
	auto it = plan.begin();
	
	while (it != plan.end()){
		if (*it == actFORWARD){
			cout << "A ";
		}
		else{
			if (*it == actTURN_R){
				cout << "D ";
			}
			else{
				if (*it == actTURN_L){
					cout << "I ";
				}
				else {
					cout << "- ";
				}
			}
		}

		it++;
	}

	cout << endl;
}


Action ComportamientoJugador::think(Sensores sensores){
	bool continuarNormal = true;
	bool saltarPensar = false;
	
	Action accion = actIDLE;

	if(sensores.reset){
		reiniciar(sensores);
	}

	// Actualización de la información del mundo
	switch (ultimaAccion) {
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
		case actPICKUP:
			if (sensores.objetoActivo == '4'){
				tengo_regalo = true;
				estoy_ejecutando_plan = false;
			}

			break;
		case actGIVE:
			if (sensores.objetoActivo == '_'){
				tengo_regalo = false;
			}

			//break;
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

	if (error_plan){
		estoy_ejecutando_plan = false;
	}
	
	if (estoy_ejecutando_plan && plan.front() == actFORWARD && !PuedeAvanzar(sensores)){
		if (sensores.superficie[2] == 'a' || sensores.superficie[2] == 'l'){
			plan.push_front(actIDLE);
			error_plan = false;
		}
		else {
			if(HayObjeto(sensores) && mochila.size() != 4 && objetoMano == '-'){
				plan.push_front(actPICKUP);
				error_plan = false;
			}
			else{
				if(objetoMano != '-'){
					plan.push_front(actPUSH);
					plan.push_front(actPICKUP);
					error_plan = false;
				}
				else{
					error_plan = true;
				}
			}
		}
	}
	else {
		error_plan = false;
	}

	if(sensores.terreno[0] == 'K' && !bien_situado){
		fil = sensores.mensajeF;
		col = sensores.mensajeC;
		bien_situado = true;
	}

	if (bien_situado){
		//mapaResultado[fil][col] = sensores.terreno[0];
		guardarVisitado(sensores);
		GuardaReyes(sensores, fil, col, brujula);
	}

	if(bien_situado && TengoRegalo() && !mapaDeReyes.empty() && !estoy_ejecutando_plan && !saltarPensar && sensores.tiempo <= 270){
		auto ite = mapaDeReyes.begin();
		auto cercano = ite;

		if(mapaDeReyes.size() != 1){
			int distanciaMenor = 999999;
			Nodo salida(fil, col);
			Nodo meta;

			for(int i = 0; i < mapaDeReyes.size(); i++){
				meta.setFila((*ite).first);
				meta.setColumna((*ite).second);

				if(distanciaNodos(salida, meta) < distanciaMenor){
					cercano = ite;
				}

				++ite;
			}
		}

		estado origen;
		origen.fila = fil;
		origen.columna = col;
		origen.orientacion = brujula;

		estado destino;
		destino.fila = (*cercano).first;
		destino.columna = (*cercano).second;

		estoy_ejecutando_plan = pathFinding(origen, destino, plan);
	}

	if(!bien_situado){
		Buscar(sensores, 'K');
	}

	if (bien_situado && !tengo_regalo && !estoy_ejecutando_plan && sensores.regalos.size() > 0 && !saltarPensar && sensores.tiempo <= 270){
		estado origen;
		origen.fila = fil;
		origen.columna = col;
		origen.orientacion = brujula;

		estado destino;
		destino.fila = sensores.regalos[0].first;
		destino.columna = sensores.regalos[0].second;

		estoy_ejecutando_plan = pathFinding(origen, destino, plan);
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

		if (sensores.superficie[2] == '4' && sensores.objetoActivo == '_'){
			accion = actPICKUP;
		}
		else{
			if (sensores.superficie[2] == 'r' && TengoRegalo()){
				if(sensores.objetoActivo == '4'){
					accion = actGIVE;
				}
			}
			else{
				if (estoy_ejecutando_plan && !error_plan){
					if(!plan.empty()){
						accion = plan.front();
						plan.erase(plan.begin());
					}

					//PintaPlan(plan);
					
					if (plan.size() == 0){
						estoy_ejecutando_plan = false;
					}
				}
				else{
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
			}
		}
	}

	// Recordar la ultima accion
	ultimaAccion = accion;

	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}

void ComportamientoJugador::reiniciar(Sensores sensores){
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

	//Belkan 2: La venganza

	estoy_ejecutando_plan = false;
	error_plan = false;
	pintaTodo--;
	

	/*
		Cuando pintaTodo, que es decrementado con cada muerte,
		llegue a 0 (al final de la vida de Belkan) se pintará
		todo el mapa que no haya sido explorado al mismo color
		que el elemento más común de dicho mapa.
		El elemento más común debe superar el 75% de ocupación
		para poder pintar el resto con su color.
	*/
	if(pintaTodo == 0){
		int terreno, bosque, agua, piedras, mas_extenso;
		const int tres_cuartos = 7500;

		terreno = bosque = agua = piedras = 0;

		for(int i = 0; i < 99; i += 2){
			for(int j = 0; j < 99; j += 2){
				//Se hacen cuatro switch para reducir las iteraciones a la mitad, de O(n^2) a O(n)
				switch(mapaResultado[i][j]){
					case 'T':
						terreno++;
						break;
					case 'S':
						piedras++;
						break;
					case 'B':
						bosque++;
						break;
					case 'A':
						agua++;
						break;
				}

				switch(mapaResultado[i][j + 1]){
					case 'T':
						terreno++;
						break;
					case 'S':
						piedras++;
						break;
					case 'B':
						bosque++;
						break;
					case 'A':
						agua++;
						break;
				}

				switch(mapaResultado[i + 1][j]){
					case 'T':
						terreno++;
						break;
					case 'S':
						piedras++;
						break;
					case 'B':
						bosque++;
						break;
					case 'A':
						agua++;
						break;
				}

				switch(mapaResultado[i][j + 1]){
					case 'T':
						terreno++;
						break;
					case 'S':
						piedras++;
						break;
					case 'B':
						bosque++;
						break;
					case 'A':
						agua++;
						break;
				}
			}
		}

		mas_extenso = Max4(terreno, piedras, bosque, agua);

		if(mas_extenso == terreno && terreno >= tres_cuartos){
			for(int i = 0; i < 100; i++){
				for(int j = 0; j < 100; j++){
					if(mapaResultado[i][j] == '?'){
						mapaResultado[i][j] = 'T';
					}
				}
			}
		}
		else{
			if(mas_extenso == piedras && piedras >= tres_cuartos){
				for(int i = 0; i < 100; i++){
					for(int j = 0; j < 100; j++){
						if(mapaResultado[i][j] == '?'){
							mapaResultado[i][j] = 'S';
						}
					}
				}
			}
			else{
				if(mas_extenso == bosque && bosque >= tres_cuartos){
					for(int i = 0; i < 100; i++){
						for(int j = 0; j < 100; j++){
							if(mapaResultado[i][j] == '?'){
								mapaResultado[i][j] = 'B';
							}
						}
					}
				}
				else{
					if(mas_extenso == agua && agua >= tres_cuartos){
						for(int i = 0; i < 100; i++){
							for(int j = 0; j < 100; j++){
								if(mapaResultado[i][j] == '?'){
									mapaResultado[i][j] = 'A';
								}
							}
						}
					}
				}
			}
		}
	}
}

bool ComportamientoJugador::HayObjeto(Sensores sensores){
	return (sensores.superficie[2] == '0' || sensores.superficie[2] == '1' || sensores.superficie[2] == '2' || sensores.superficie[2] == '3' || sensores.superficie[2] == '4');
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
				else{
					if(sensores.terreno[2] == 'r' && objetoMano != '4'){
						for(int i = 0; !mochilaExtra.empty() && !tirarObjetos; i++){
							if(mochilaExtra.front() == '4'){
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
	}

	return tirarObjetos;
}

bool ComportamientoJugador::SituacionIdonea(Sensores sensores){
								//Agua				//Bikini						//Bosque			//Zapatillas					//Puerta				//Llave							//Rey			  //Regalo
	return (sensores.terreno[2] == 'A' && sensores.objetoActivo == '1') || (sensores.terreno[2] == 'B' && sensores.objetoActivo == '2') || (sensores.terreno[2] == 'D' && sensores.objetoActivo == '3' || (sensores.terreno[2] == 'r' && sensores.objetoActivo == '4'));
}

bool ComportamientoJugador::TengoRegalo(){
	queue<char> mochilaExtra = mochila;
	bool lo_tengo = false;

	if(objetoMano == '4'){
		lo_tengo = true;
	}

	for(int i = 0; !mochilaExtra.empty() && !lo_tengo; i++){
		if(mochilaExtra.front() == '4'){
			lo_tengo = true;
		}
		else{
			mochilaExtra.pop();
		}
	}

	return lo_tengo;
}

void ComportamientoJugador::Buscar(Sensores sensores, char QueBuscas){
	bool aqui[16];
	int posicionBusqueda = -1;

	for(int i = 0; i < 16; i++){
		aqui[i] = false;
	}

	switch(QueBuscas){	//Se añaden también las minúsculas al switch por si acaso
		case 'k':
		case 'K':	//GPS
			for(int i = 1; i < 16 && !estoy_ejecutando_plan; i++){
				if(i != 2){
					estoy_ejecutando_plan = aqui[i] = sensores.terreno[i] == 'K';
				}
			}
			break;
		case 'o':
		case 'O':	//Objeto
			for(int i = 1; i < 16 && !estoy_ejecutando_plan; i++){
				if(i != 2){
					estoy_ejecutando_plan = aqui[i] = sensores.terreno[i] == '0' || sensores.terreno[i] == '1' || sensores.terreno[i] == '2' || sensores.terreno[i] == '3';
				}
			}
			break;
		case 'd':
		case 'D':	//Puerta
			for(int i = 1; i < 16 && !estoy_ejecutando_plan; i++){
				if(i != 2){
					estoy_ejecutando_plan = aqui[i] = sensores.terreno[i] == 'D';
				}
			}
			break;
	}

	if(estoy_ejecutando_plan){
		for(int i = 1; i < 16 && !aqui[i - 1]; i++){
			if(aqui[i]){
				posicionBusqueda = i;
				plan.clear();
			}
		}

		//Según dónde esté el elemento buscado seguirá un camino distinto
		switch(posicionBusqueda){
			case 1:
				plan.push_back(actFORWARD);
				plan.push_back(actTURN_L);
				break;
			case 3:
				plan.push_back(actFORWARD);
				plan.push_back(actTURN_R);
				break;
			case 4:
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actTURN_L);
				plan.push_back(actFORWARD);
				break;
			case 5:
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actTURN_R);
				break;
			case 6:
				plan.push_back(actFORWARD);
				break;
			case 7:
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actTURN_L);
				break;
			case 8:
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actTURN_R);
				plan.push_back(actFORWARD);
				break;
			case 9:
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actTURN_L);
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				break;
			case 10:
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actTURN_L);
				plan.push_back(actFORWARD);
				break;
			case 11:
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actTURN_L);
				break;
			case 12:
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
			case 13:
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actTURN_R);
				break;
			case 14:
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actTURN_R);
				plan.push_back(actFORWARD);
				break;
			case 15:
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				plan.push_back(actTURN_R);
				plan.push_back(actFORWARD);
				plan.push_back(actFORWARD);
				break;
		}

		if(QueBuscas == 'K'){
			plan.push_back(actFORWARD);
		}
	}
}

void ComportamientoJugador::GuardaReyes(Sensores sensores, int fila, int columna, int brujula){
	/*
		Comprueba los sensores y busca reyes en ellos. Si los hay
		guarda su posición en un set para que no se repitan.
	*/
	
	switch(brujula){	//0N 1E 2S 3O
		case 0:
			if(sensores.superficie[1] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila - 1, columna - 1));
			}
			if(sensores.superficie[2] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila - 1, columna));
			}
			if(sensores.superficie[3] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila - 1, columna + 1));
			}
			if(sensores.superficie[4] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila - 2, columna - 2));
			}
			if(sensores.superficie[5] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila - 2, columna - 1));
			}
			if(sensores.superficie[6] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila - 2, columna));
			}
			if(sensores.superficie[7] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila - 2, columna + 1));
			}
			if(sensores.superficie[8] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila - 2, columna + 2));
			}
			if(sensores.superficie[9] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila - 3, columna - 3));
			}
			if(sensores.superficie[10] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila - 3, columna - 2));
			}
			if(sensores.superficie[11] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila - 3, columna - 1));
			}
			if(sensores.superficie[12] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila - 3, columna));
			}
			if(sensores.superficie[13] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila - 3, columna + 1));
			}
			if(sensores.superficie[14] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila - 3, columna + 2));
			}
			if(sensores.superficie[15] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila - 3, columna + 3));
			}
			
			break;
		case 1:
			if(sensores.superficie[1] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila - 1, columna + 1));
			}
			if(sensores.superficie[2] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila, columna + 1));
			}
			if(sensores.superficie[3] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila + 1, columna + 1));
			}
			if(sensores.superficie[4] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila - 2, columna + 2));
			}
			if(sensores.superficie[5] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila - 1, columna + 2));
			}
			if(sensores.superficie[6] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila, columna + 2));
			}
			if(sensores.superficie[7] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila + 1, columna + 2));
			}
			if(sensores.superficie[8] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila + 2, columna + 2));
			}
			if(sensores.superficie[9] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila - 3, columna + 3));
			}
			if(sensores.superficie[10] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila - 2, columna + 3));
			}
			if(sensores.superficie[11] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila - 1, columna + 3));
			}
			if(sensores.superficie[12] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila, columna + 3));
			}
			if(sensores.superficie[13] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila + 1, columna + 3));
			}
			if(sensores.superficie[14] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila + 2, columna + 3));
			}
			if(sensores.superficie[15] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila + 3, columna + 3));
			}

			break;
		case 2:
			if(sensores.superficie[1] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila + 1, columna + 1));
			}
			if(sensores.superficie[2] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila + 1, columna));
			}
			if(sensores.superficie[3] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila + 1, columna - 1));
			}
			if(sensores.superficie[4] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila + 2, columna + 2));
			}
			if(sensores.superficie[5] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila + 2, columna + 1));
			}
			if(sensores.superficie[6] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila + 2, columna));
			}
			if(sensores.superficie[7] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila + 2, columna - 1));
			}
			if(sensores.superficie[8] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila + 2, columna - 2));
			}
			if(sensores.superficie[9] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila + 3, columna + 3));
			}
			if(sensores.superficie[10] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila + 3, columna + 2));
			}
			if(sensores.superficie[11] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila + 3, columna + 1));
			}
			if(sensores.superficie[12] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila + 3, columna));
			}
			if(sensores.superficie[13] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila + 3, columna - 1));
			}
			if(sensores.superficie[14] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila + 3, columna - 2));
			}
			if(sensores.superficie[15] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila + 3, columna - 3));
			}

			break;
		case 3:
			if(sensores.superficie[1] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila + 1, columna - 1));
			}
			if(sensores.superficie[2] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila, columna - 1));
			}
			if(sensores.superficie[3] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila - 1, columna - 1));
			}
			if(sensores.superficie[4] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila + 2, columna - 2));
			}
			if(sensores.superficie[5] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila + 1, columna - 2));
			}
			if(sensores.superficie[6] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila, columna - 2));
			}
			if(sensores.superficie[7] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila - 1, columna - 2));
			}
			if(sensores.superficie[8] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila - 2, columna - 2));
			}
			if(sensores.superficie[9] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila + 3, columna - 3));
			}
			if(sensores.superficie[10] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila + 2, columna - 3));
			}
			if(sensores.superficie[11] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila + 1, columna - 3));
			}
			if(sensores.superficie[12] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila, columna - 3));
			}
			if(sensores.superficie[13] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila - 1, columna - 3));
			}
			if(sensores.superficie[14] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila - 2, columna - 3));
			}
			if(sensores.superficie[15] == 'r'){
				mapaDeReyes.insert(pair<int, int>(fila - 3, columna - 3));
			}

			break;
	}
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

int ComportamientoJugador::Max4(const int &uno, const int &dos, const int &tres, const int &cuatro){
	int max = uno;

	if(dos > max){
		max = dos;
	}

	if(tres > max){
		max = tres;
	}

	if(cuatro > max){
		max = cuatro;
	}

	return max;
}

