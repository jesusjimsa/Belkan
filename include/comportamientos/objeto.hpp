#ifndef COMPORTAMIENTOOBJETO_H
#define COMPORTAMIENTOOBJETO_H

#include "comportamientos/comportamiento.hpp"

class ComportamientoObjeto : public Comportamiento{

  public:
    ComportamientoObjeto(unsigned int size = 0) : Comportamiento(size){}
    ComportamientoObjeto(const ComportamientoObjeto & comport) : Comportamiento(comport){}
    ~ComportamientoObjeto(){}

    Action think(Sensores sensores);

    int interact(Action accion, int valor);


    ComportamientoObjeto * clone(){return new ComportamientoObjeto(*this);}
};


#endif
