#ifndef ENTIDAD_H
#define ENTIDAD_H

#include "obj3dlib.hpp"
#include "comportamientolib.hpp"


#include <queue>
#include <utility>
#include <string>
//#include "agent.h"

/*Macro clase para englobar entidades de juego
Contiene la informacion para pintar y ejecutar en el juego las entidades

Las entidades son el jugador, los npcs y los objetos.
*/
enum Orientacion {norte, este, sur, oeste};
enum Tipo {jugador, npc, objeto};
enum SubTipo{hueso, biquini, zapatillas, llaves, jugador_, lobo, aldeano, puerta, __NONE__};



class Entidad{
private:
  Tipo tipo;
  SubTipo subtipo;
  Orientacion orient;
  unsigned int x,y;

  Objeto3D * modelo;
  Comportamiento* comportamiento;


  //Mochila
  std::queue <pair <int , unsigned char > > mochila;

  //Colision
  bool colision = false;
  //reset
  bool reset = false;

  bool mensaje = false;
  //Mapa
  //MapaEnt

  //ObjActivo
  //First = Indice de la entidad en el motor
  //Second = Tipo de la entidad
  pair <int , unsigned char > objActivo;
  //vida
  int vida = 1000;



  bool hitbox = true;
  int desactivado = 0;

  int last_action = 3;

 std::vector <std::vector< unsigned char> > visionAux;





public:
  Entidad(Tipo paramTipo, SubTipo paramSubTipo, Orientacion paramOrient, unsigned int paramX, unsigned int paramY, Objeto3D * mod, Comportamiento * comp)
      : tipo(paramTipo), subtipo(paramSubTipo), orient(paramOrient), x(paramX), y(paramY), modelo(mod), comportamiento(comp){
              objActivo.first = -1;
              objActivo.second = -1;

  }

  ~Entidad(){delete modelo; delete comportamiento;}

  void draw(int modo){if(hitbox){modelo -> draw(modo);}}
  Action think(std::vector <std::vector< unsigned char> > vision);

  //Programar funciones para la interacción con el entorno
  void setPosicion(unsigned int paramX, unsigned int paramY){x = paramX; y = paramY;}
  void setOrientacion(Orientacion paramOrient){orient = paramOrient;}

  Tipo getTipo(){return tipo;}
  SubTipo getSubTipo(){return subtipo;}

  unsigned char getSubTipoChar();

  Orientacion getOrientacion(){return orient;}
  unsigned int getX(){return x;}
  unsigned int getY(){return y;}
  bool getHitbox(){return hitbox;}

  bool interact(Action accion, int valor);

  pair <int , unsigned char > getObjActivo(){return objActivo;}
  void quitarObjActivo(){objActivo.first = -1; objActivo.second = -1;}
  bool manoVacia(){return objActivo.first == -1;}
  void anadirObjActivo(pair<unsigned int, unsigned char> elemento){objActivo = elemento;}

  void resetEntidad();
  void seAostio(){colision = true;}
  void notify(){mensaje = true;}

  void getLastAction(int accion){last_action=accion;}

  std::vector< std::vector< unsigned char> > getMapaResultado(){return comportamiento -> getMapaResultado();}
  std::vector< std::vector< unsigned char> > getMapaEntidades(){return comportamiento -> getMapaEntidades();}

  bool vivo(){return vida > 0;}

  std::string toString();

  void setVision(std::vector <std::vector< unsigned char> > vision){visionAux = vision;}

  void perderPV(int valor){vida -= valor;}

};
#endif
