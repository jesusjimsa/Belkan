#include "comportamientos/puerta.hpp"

#include <random>


#include <iostream>



Action ComportamientoPuerta::think(Sensores sensores){
  return actIDLE;
}


int ComportamientoPuerta::interact(Action accion, int valor){
  if(accion == actGIVE){
    if(valor == '3'){

      return 1;
    }
    else{
      return 0;
    }
  }
  else{
    return 0;
  }
}
