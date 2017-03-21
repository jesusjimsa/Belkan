#include "comportamientos/comportamiento.hpp"

#include <iostream>

Comportamiento::Comportamiento(unsigned int size){
	objActivo.first = -1;
	objActivo.second = -1;

	std::vector< unsigned char> aux(size, '?');
	for(unsigned int i = 0; i<size; i++){
		mapaResultado.push_back(aux);
		mapaEntidades.push_back(aux);
	}
}



Action Comportamiento::think(Sensores sensores){
  	return actIDLE;
}

int Comportamiento::interact(Action accion, int valor){
 	return 0;
}
