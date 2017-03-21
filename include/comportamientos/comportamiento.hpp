#ifndef COMPORTAMIENTO_H
#define COMPORTAMIENTO_H

#include <vector>

enum Action {
	actFORWARD, //Caminar hacia delante
	actTURN_L,  //Girar a la izquierda
	actTURN_R,  //Girar a la derecha
	actIDLE,  //No hacer nada
	actPICKUP,  //Coger algo
	actPUTDOWN, //Soltar algo
	actPUSH,  //Empujar
	actPOP, //Tirar
	actGIVE,  //Dar
	actTHROW  //Lanzar
};

struct Sensores{
	int vida;
	unsigned char objetoActivo;
	unsigned char mochila;
	bool colision;
	bool reset;
	int mensajeF;
	int mensajeC;
	std::vector< unsigned char> terreno;
	std::vector< unsigned char> superficie;
};


class Comportamiento{
  private:

    bool manoVacia = true;
    std::pair <unsigned int, int> objActivo;

    std::vector<std::pair <unsigned int, int> > mochila;

  public:
    std::vector< std::vector< unsigned char> > mapaResultado;
    std::vector< std::vector< unsigned char> > mapaEntidades;

    Comportamiento(unsigned int size);
    Comportamiento(const Comportamiento & comport) : mapaEntidades(comport.mapaEntidades), mapaResultado(comport.mapaResultado), manoVacia(comport.manoVacia), objActivo(comport.objActivo), mochila(comport.mochila){}
    ~Comportamiento(){}

    virtual Action think(Sensores sensores);

    virtual int interact(Action accion, int valor);


    Comportamiento * clone(){return new Comportamiento(*this);}

    std::vector< std::vector< unsigned char> > getMapaResultado(){return mapaResultado;}
    std::vector< std::vector< unsigned char> > getMapaEntidades(){return mapaEntidades;}

};


#endif
