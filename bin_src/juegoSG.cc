#include "motorlib.hpp"
#include <stdlib.h>


MonitorJuego monitor;


int main(int argc, char ** argv){
  if (argc!=2){
    cout << "Falta incluir como argumento el camino al mapa del juego\n";
  }
  else {
    srand(1);
    monitor.setMapa(argv[1]);
    monitor.inicializar();
    monitor.startGame();
    monitor.setPasos(20000);
    monitor.setRetardo(100);
    monitor.juegoInicializado();

    //cout << "lanzando el juego...\n";
    lanzar_motor_juego2(monitor);
  }
  exit(EXIT_SUCCESS);
}
