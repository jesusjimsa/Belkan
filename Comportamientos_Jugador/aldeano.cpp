#include "../Comportamientos_Jugador/aldeano.hpp"


Action ComportamientoAldeano::think(Sensores sensores){
  Action accion;

  switch (paso){
      case 0: accion = actFORWARD;
              break;
      case 1: accion = actTURN_R;
  }
  paso = (paso+1)%2;
  return accion;
}

int ComportamientoAldeano::interact(Action accion, int valor){
    return 0;
}
