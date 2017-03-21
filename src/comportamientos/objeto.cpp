#include "comportamientos/objeto.hpp"

#include <random>


#include <iostream>



Action ComportamientoObjeto::think(Sensores sensores){
  	return actIDLE;
}


int ComportamientoObjeto::interact(Action accion, int valor){

	if(accion == actPICKUP){
		return 2;
	}
	else{
		if(accion == actPUTDOWN){
			return 3;
		}
		else{
			return 0;
		}
	}
}
