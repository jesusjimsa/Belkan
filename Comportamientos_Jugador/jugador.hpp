#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
#include <queue>
#include <stdlib.h>	//Biblioteca para usar la función rand

using namespace std;

class ComportamientoJugador : public Comportamiento{
private:
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
public:
	ComportamientoJugador(unsigned int size) : Comportamiento(size){
		fil = 99;
		col = 99;
		brujula = 0;    //0 indica orientaci�n Norte, 1 orientaci�n Este, 2 orientaci�n Sur y 3 orientaci�n Oeste
		ultimaAccion = actIDLE; //Al principio, no hay acci�n anterior, as� que la �ltima acci�n fue no hacer nada
		girar_derecha = false;
		bien_situado = false;
		estaAbierta = false;
		girarEnIzUno = girarEnIzDos = girarEnIzTres = girarYaIz = false;
		girarEnDerUno = girarEnDerDos = girarEnDerTres = girarYaDer = false;
		objetoMano = '-';   //Comienza no teniendo nada en la mano
		//La mochila no hay que inicializarla porque comienza vacía
		pasosGirar = (rand() % 50) + 1; //El máximo de pasos aleatorios será 50, y no podrá ser 0

		for(int i = 0; i < 100; i++){
			for(int j = 0; j < 100; j++){
				mapaResultado[i][j] = 'T';	//Se comienza suponiendo que el mapa está completamente compuesto de tierra
			}
		}
	}

	ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}

	~ComportamientoJugador(){}

	ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}

	Action think(Sensores sensores);

	Action Girar(Sensores sensores);

	int interact(Action accion, int valor);

	void reinicio(Sensores sensores);

	void guardarVisitado(Sensores sensores);

	void Buscar(Sensores sensores, char QueBuscas);

	bool HayObjeto(Sensores sensores);

	bool PuedeAvanzar(Sensores sensores);

	bool BuscarEnMochila(Sensores sensores);

	bool SituacionIdonea(Sensores sensores);

};

#endif
