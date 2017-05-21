#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include <list>
#include "comportamientos/comportamiento.hpp"
#include <queue>
#include <stdlib.h>	//Biblioteca para usar la función rand
#include <iostream>
#include <set>

using namespace std;

class ComportamientoJugador : public Comportamiento{
private:
	//Belkan: Los mundos maravillosos

	// Declarar Variables de Estado
	int fil, col, brujula;  //fil y col determinan dónde estoy y brujula determina hacia dónde miro
	int pasosGirar; //Variable aleatoria que se actualizará cuando llegue a 0 y que determinará cuando debe girar Belkan
					//Siempre y cuando no se encuentre ningún obstáculo
	bool girar_derecha, bien_situado;
	bool girarEnIzUno, girarEnIzDos, girarEnIzTres, girarYaIz;
	bool girarEnDerUno, girarEnDerDos, girarEnDerTres, girarYaDer;
	bool estaAbierta;
	char objetoMano;
	queue<char> mochila;
	Action ultimaAccion;

	//Belkan 2: La venganza

	struct estado{
		int fila;
		int columna;
		int orientacion;
	};

	class Nodo{
	private:
		estado propio;
		list<Action> camino;

		int costeG, costeH;
		int costeF;
	public:
		Nodo(int fila, int columna){
			propio.fila = fila;
			propio.columna = columna;
		}

		Nodo(estado uno){
			propio = uno;
		}

		Nodo(){
			//Constructor vacío
		}

		void setFila(int fila){
			propio.fila = fila;
		}

		void setColumna(int columna){
			propio.columna = columna;
		}

		void setBrujula(int orientacion){
			propio.orientacion = orientacion;
		}

		void setEstado(estado uno){
			propio = uno;
		}

		void setCosteG(int costeG){
			this->costeG = costeG;
		}

		void setCosteH(int costeH){
			this->costeH = costeH;
		}

		void setCosteF(int costeF){
			this->costeF = costeF;
		}

		estado getEstado() const{
			return propio;
		}

		int getCosteH() const{
			return costeH;
		}

		int getCosteG() const{
			return costeG;
		}

		int getCosteF() const{
			return costeF;
		}

		int getFila() const{
			return propio.fila;
		}

		int getColumna() const{
			return propio.columna;
		}

		int getBrujula() const{
			return propio.orientacion;
		}

		void addAccion(Action una){
			camino.push_back(una);
		}

		list<Action> getCamino() const{
			return camino;
		}

		bool operator==(Nodo &otro) const;

		bool operator!=(Nodo &otro) const;

		bool operator==(const Nodo &otro) const;

		bool operator!=(const Nodo &otro) const;
	};

	class functorNodo{
	public:
		bool operator()(Nodo &uno, Nodo &otro){
			return uno.getCosteF() > otro.getCosteF() || (uno.getCosteF() == otro.getCosteF() && uno.getCosteH() > otro.getCosteH());
		}
	};

	int pintaTodo;
	bool tengo_regalo;

	set< pair<int, int> > mapaDeReyes;

	// Para el plan
	bool estoy_ejecutando_plan;
	bool error_plan;
	list <Action> plan;

	void Reiniciar();
	bool pathFinding(const estado &origen, const estado &destino, list<Action> &plan);


public:
	ComportamientoJugador(unsigned int size) : Comportamiento(size){
		//Belkan 1: Los mundos maravillosos

		fil = 99;
		col = 99;
		brujula = 0;    //0 indica orientación Norte, 1 orientación Este, 2 orientación Sur y 3 orientación Oeste
		ultimaAccion = actIDLE; //Al principio, no hay acción anterior, así que la última acción fue no hacer nada
		girar_derecha = false;
		bien_situado = false;
		estaAbierta = false;
		girarEnIzUno = girarEnIzDos = girarEnIzTres = girarYaIz = false;
		girarEnDerUno = girarEnDerDos = girarEnDerTres = girarYaDer = false;
		objetoMano = '-';   //Comienza no teniendo nada en la mano
		//La mochila no hay que inicializarla porque comienza vacía
		pasosGirar = (rand() % 50) + 1; //El máximo de pasos aleatorios será 50, y no podrá ser 0

		//Belkan 2: La venganza

		pintaTodo = 20;
		tengo_regalo = false;
		estoy_ejecutando_plan = false;
		error_plan = false;
	}

	ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
	~ComportamientoJugador(){}

	Action think(Sensores sensores);

	int interact(Action accion, int valor);


	ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}

	Action Girar(Sensores sensores);

	void reiniciar(Sensores sensores);

	void guardarVisitado(Sensores sensores);

	void Buscar(Sensores sensores, char QueBuscas);

	void GuardaReyes(Sensores sensores, int fila, int columna, int brujula);

	void PintaPlan(list<Action> plan);

	bool HayObjeto(Sensores sensores);

	bool PuedeAvanzar(Sensores sensores);

	bool BuscarEnMochila(Sensores sensores);

	bool SituacionIdonea(Sensores sensores);

	bool TengoRegalo();

	int Max4(const int &uno, const int &dos, const int &tres, const int &cuatro);

	int distanciaNodos(const Nodo &uno, const Nodo &otro);
};


#endif
