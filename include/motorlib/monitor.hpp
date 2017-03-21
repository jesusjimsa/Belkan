#ifndef MONITOR_H
#define MONITOR_H

#include <omp.h>
#include <vector>

#include "motorlib/entidad.hpp"
#include "motorlib/mapa.hpp"

/*Clase para monitorizar y controlar la ejecucion de todo el juego
Es necesaria si se quiere independizar el dibujado de los modelo del
comportamiento de los agentes.

*/

class MonitorJuego{
private:
  std::vector<Entidad *> entidades;
  Mapa * mapa;

  bool apagarBelkan = false; //Notificar al motor de juego que sigua corriendo
  bool jugando = false; //Notificar al motor de juego que haga los thinks
  bool empezarJuego = false; //Notificar al motor de juego que inicie un nuevo juego

  bool resultados = false;
  bool resMostrados = false;


  int pasos = 0;
  int retardo = 1000;
  int iteracion = 0;



public:
  MonitorJuego(){}
  ~MonitorJuego(){}

  void nueva_entidad(Entidad * entidad);
  unsigned int numero_entidades();

  void clear();
  Entidad * get_entidad(unsigned entidad);
  std::vector<Entidad *> * get_entidades();
  void cambiarPosicion(unsigned char entidad, unsigned char x, unsigned char y);
  void cambiarOrientacion(unsigned char entidad, unsigned char orientacion);
  long unsigned int getIteracion(){return iteracion;}

  float getMapDepth(){return mapa -> getMapDepth();}


  Mapa * getMapa();
  void setMapa(char * file);

  void startGame(); //Indica que hay un nuevo juego que inicializar
  bool inicializarJuego(); //Mira si tiene que inicializar un juego
  bool juegoInicializado();

  bool continuarBelkan();
  void cerrarBelkan();

  bool mostrarResultados();
  void setMostrarResultados(bool);

  bool resultadosMostrados();
  void setResultadosMostrados(bool valor);

  bool jugar();


  void girarJugadorIzquierda();
  void girarJugadorDerecha();

  void setPasos(int Npasos);
  void decPasos();
  int getPasos();

  bool finJuego();

  void setRetardo(int tRetardo);
  int getRetardo();

  void inicializar();


};
#endif
