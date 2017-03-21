#ifndef COMPORTAMIENTOPUERTA_H
#define COMPORTAMIENTOPUERTA_H

#include "comportamientos/comportamiento.hpp"

class ComportamientoPuerta : public Comportamiento{

  public:
    ComportamientoPuerta(unsigned int size = 0) : Comportamiento(size){}
    ComportamientoPuerta(const ComportamientoPuerta & comport) : Comportamiento(comport){}
    ~ComportamientoPuerta(){}

    Action think(Sensores sensores);

    int interact(Action accion, int valor);


    ComportamientoPuerta * clone(){return new ComportamientoPuerta(*this);}
};


#endif
