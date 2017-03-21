#include "motorlib.hpp"
#include "modelos_belkan.hpp"

#include <iostream>

#include <chrono>
#include <thread>

//extern MonitorJuego monitor;

extern MonitorJuego monitor;

int aleatorio3(int tope){
  double a = rand(), c = RAND_MAX;
  a = a / (c+1);
  int b = a*(tope+1);
  return b;
}


bool actuacionJugador(unsigned char celda, Action accion, unsigned int x, unsigned int y){
  unsigned int xRand, yRand;
  unsigned char celdaRand;
  bool salida = false;

  int auxEnt;
  switch (accion) {
    case actFORWARD:
      if(monitor.getMapa() -> casillaOcupada(0) == -1){
        if(celda != 'P' and celda != 'M'){
          if(celda == 'B' and monitor.get_entidad(0) -> getObjActivo().second == '2'){
            monitor.get_entidad(0) -> setPosicion(x,y);
            salida = true;
          }
          if(celda == 'B' and monitor.get_entidad(0) -> getObjActivo().second != '2'){
            monitor.get_entidad(0) -> seAostio();
          }

          if(celda == 'A' and monitor.get_entidad(0) -> getObjActivo().second == '1'){
            monitor.get_entidad(0) -> setPosicion(x,y);
            salida = true;
          }
          if(celda == 'A' and monitor.get_entidad(0) -> getObjActivo().second != '1'){

            do {
              xRand = aleatorio3(monitor.getMapa()->getNFils()-1);
              yRand = aleatorio3(monitor.getMapa()->getNCols()-1);

              celdaRand = monitor.getMapa() -> getCelda(xRand, yRand);
            }
            while(not(((celdaRand == 'T') and monitor.getMapa() -> entidadEnCelda(xRand, yRand) == '_')) );



            monitor.get_entidad(0) -> setPosicion(xRand,yRand);
            monitor.get_entidad(0) -> setOrientacion(norte);
            monitor.get_entidad(0) -> resetEntidad();

          }

          if(celda == 'K' or celda == 'S' or celda == 'T' or celda == 'D'){
            if(celda == 'K'){
                monitor.get_entidad(0) -> notify();
            }
            monitor.get_entidad(0) -> setPosicion(x,y);
            salida = true;
          }
        }
        else{
          if(celda == 'P'){
            do {
              xRand = aleatorio3(monitor.getMapa()->getNFils()-1);
              yRand = aleatorio3(monitor.getMapa()->getNCols()-1);

              celdaRand = monitor.getMapa() -> getCelda(xRand, yRand);
            }
            while(not(((celdaRand == 'T') and monitor.getMapa() -> entidadEnCelda(xRand, yRand) == '_')) );


              monitor.get_entidad(0) -> setPosicion(xRand,yRand);
              monitor.get_entidad(0) -> setOrientacion(norte);
              monitor.get_entidad(0) -> resetEntidad();

            }
          else{
              monitor.get_entidad(0) -> seAostio();
          }

          salida = false;
        }
      }
      else{
        monitor.get_entidad(0) -> seAostio();
        if (monitor.get_entidad(monitor.getMapa() -> casillaOcupada(0)) -> getSubTipo() == aldeano){
            monitor.get_entidad(0) -> perderPV(1);
        }
        else if (monitor.get_entidad(monitor.getMapa() -> casillaOcupada(0)) -> getSubTipo() == lobo){
            monitor.get_entidad(0) -> perderPV(10);
        }
        salida = false;
      }
    break;

    case actTURN_R:
      switch (monitor.get_entidad(0) -> getOrientacion()) {
        case norte:
          monitor.get_entidad(0) -> setOrientacion(este);
        break;

        case este:
          monitor.get_entidad(0) -> setOrientacion(sur);
        break;

        case sur:
          monitor.get_entidad(0) -> setOrientacion(oeste);
        break;

        case oeste:
          monitor.get_entidad(0) -> setOrientacion(norte);
        break;
      }
      monitor.girarJugadorDerecha();
      salida = true;
    break;

    case actTURN_L:
      switch (monitor.get_entidad(0) -> getOrientacion()) {
        case norte:
          monitor.get_entidad(0) -> setOrientacion(oeste);
        break;

        case este:
          monitor.get_entidad(0) -> setOrientacion(norte);
        break;

        case sur:
          monitor.get_entidad(0) -> setOrientacion(este);
        break;

        case oeste:
          monitor.get_entidad(0) -> setOrientacion(sur);
        break;
        }
        monitor.girarJugadorIzquierda();
        salida = true;
    break;

    case actGIVE:
      auxEnt = monitor.getMapa() -> casillaOcupada(0);
      if(auxEnt != -1){
        if(monitor.get_entidad(auxEnt) -> interact(actGIVE,monitor.get_entidad(0) -> getObjActivo().second)){
          if(monitor.get_entidad(0) -> getObjActivo().second != '3'){
            monitor.get_entidad(0) -> quitarObjActivo();
          }

          if(monitor.get_entidad(0) -> getObjActivo().second == '0'){
            int xRand, yRand;
            do {
              xRand = aleatorio3(monitor.getMapa()->getNFils()-1);
              yRand = aleatorio3(monitor.getMapa()->getNCols()-1);

              celdaRand = monitor.getMapa() -> getCelda(xRand, yRand);
            }
            while(not(((celdaRand == 'T' or celdaRand=='S') and monitor.getMapa() -> entidadEnCelda(xRand, yRand) == '_')) );

            monitor.nueva_entidad(new Entidad(objeto, hueso, norte, xRand, yRand, new Item3D(), new ComportamientoObjeto()));
          }

          salida = true;
        }
        else{
          salida = false;
        }

      }
      else{
        salida = false;
      }

    break;

    case actPICKUP:
      auxEnt = monitor.getMapa() -> casillaOcupada(0);
      if(auxEnt != -1 and monitor.get_entidad(0) -> manoVacia()){
        if(monitor.get_entidad(auxEnt) -> interact(actPICKUP,0)){
          monitor.get_entidad(0) -> anadirObjActivo(pair<unsigned int, unsigned char> (auxEnt, monitor.get_entidad(auxEnt) -> getSubTipoChar()));
          salida = true;
        }
        else{
          salida = false;
        }

      }
      else{
        salida = false;
      }
    break;

    case actPUTDOWN:
      auxEnt = monitor.get_entidad(0) -> getObjActivo().first;
      if(auxEnt != -1 and
         (monitor.getMapa()->getCelda(x,y)=='T' or  monitor.getMapa()->getCelda(x,y)=='S') and
          monitor.getMapa()->casillaOcupada(0)==-1){
        monitor.get_entidad(auxEnt) -> interact(actPUTDOWN,0);
        monitor.get_entidad(auxEnt) -> setPosicion(x,y);
        monitor.get_entidad(0) -> quitarObjActivo();
        salida = true;
      }
      else{
        salida = false;
      }
    break;

    case actPUSH:
      salida=monitor.get_entidad(0) -> interact(actPUSH,0);
    break;

    case actPOP:
      salida=monitor.get_entidad(0) -> interact(actPOP,0);
    break;

    case actTHROW:
      auxEnt = monitor.getMapa() -> casillaOcupadaThrow(0);
      monitor.get_entidad(0) -> quitarObjActivo();
      if(auxEnt != -1){
          monitor.get_entidad(auxEnt) -> interact(actTHROW,0);
      }
      else{
        salida = false;
      }
      salida = true;
    break;

    case actIDLE:
      salida = true;
    break;
  }

  int f = monitor.get_entidad(0)->getX();
  int c = monitor.get_entidad(0)->getY();
  int fe,ce;
  for(unsigned int i = 1; i < monitor.numero_entidades(); i++){
    if (monitor.get_entidad(i)->getSubTipo() == lobo){
      fe = monitor.get_entidad(i)->getX();
      ce = monitor.get_entidad(i)->getY();
      int distancia = fabs(f-fe)+fabs(c-ce);
      if (distancia<4){
        monitor.get_entidad(0) -> perderPV(4-distancia);
      }
    }
  }


  return salida;
}

bool actuacionNPC(unsigned int entidad, unsigned char celda, Action accion, unsigned int x, unsigned int y){
  switch (monitor.get_entidad(entidad) -> getSubTipo()) {
    case lobo:
      //Acciones para el lobo


      switch (accion) {
        case actFORWARD:
         if((celda == 'B' or celda == 'S' or celda == 'T') and monitor.getMapa() -> casillaOcupada(entidad) == -1){
           monitor.get_entidad(entidad) -> setPosicion(x,y);
           return true;
         }
         else{
           return false;
         }
        break;

        case actTURN_R:
          switch (monitor.get_entidad(entidad) -> getOrientacion()) {
            case norte:
              monitor.get_entidad(entidad) -> setOrientacion(este);
            break;

            case este:
              monitor.get_entidad(entidad) -> setOrientacion(sur);
            break;

            case sur:
              monitor.get_entidad(entidad) -> setOrientacion(oeste);
            break;

            case oeste:
              monitor.get_entidad(entidad) -> setOrientacion(norte);
            break;
          }
        return true;
        break;

        case actTURN_L:
          switch (monitor.get_entidad(entidad) -> getOrientacion()) {
            case norte:
              monitor.get_entidad(entidad) -> setOrientacion(oeste);
            break;

            case este:
              monitor.get_entidad(entidad) -> setOrientacion(norte);
            break;

            case sur:
              monitor.get_entidad(entidad) -> setOrientacion(este);
            break;

            case oeste:
              monitor.get_entidad(entidad) -> setOrientacion(sur);
            break;
          }
        return true;
        break;
      }
      break;
      //Fin Acciones para el lobo
      case aldeano:
        //Acciones para el lobo


        switch (accion) {
          case actFORWARD:
           if((celda == 'S' or celda == 'T') and monitor.getMapa() -> casillaOcupada(entidad) == -1){
             monitor.get_entidad(entidad) -> setPosicion(x,y);
             return true;
           }
           else{
             return false;
           }
          break;

          case actTURN_R:
            switch (monitor.get_entidad(entidad) -> getOrientacion()) {
              case norte:
                monitor.get_entidad(entidad) -> setOrientacion(este);
              break;

              case este:
                monitor.get_entidad(entidad) -> setOrientacion(sur);
              break;

              case sur:
                monitor.get_entidad(entidad) -> setOrientacion(oeste);
              break;

              case oeste:
                monitor.get_entidad(entidad) -> setOrientacion(norte);
              break;
            }
          return true;
          break;

          case actTURN_L:
            switch (monitor.get_entidad(entidad) -> getOrientacion()) {
              case norte:
                monitor.get_entidad(entidad) -> setOrientacion(oeste);
              break;

              case este:
                monitor.get_entidad(entidad) -> setOrientacion(norte);
              break;

              case sur:
                monitor.get_entidad(entidad) -> setOrientacion(este);
              break;

              case oeste:
                monitor.get_entidad(entidad) -> setOrientacion(sur);
              break;
            }
          return true;
          break;
        }
        break;
        //Fin Acciones para el aldeano
  }
  return false;
}

bool actuacion(unsigned int entidad, Action accion){
  unsigned char celda, objetivo;
  unsigned int x,y;

  //Calculamos cual es la celda justo frente a la entidad
  switch (monitor.get_entidad(entidad) -> getOrientacion()) {
    case norte:
      x = monitor.get_entidad(entidad) -> getX() - 1;
      y = monitor.get_entidad(entidad) -> getY();
    break;

    case sur:
      x = monitor.get_entidad(entidad) -> getX() + 1;
      y = monitor.get_entidad(entidad) -> getY();
    break;

    case este:
      x = monitor.get_entidad(entidad) -> getX();
      y = monitor.get_entidad(entidad) -> getY() + 1;
    break;

    case oeste:
      x = monitor.get_entidad(entidad) -> getX();
      y = monitor.get_entidad(entidad) -> getY() - 1;
    break;
  }

  celda = monitor.getMapa() -> getCelda(x,y);

  //Dependiendo el tipo de la entidad actuamos de una forma u otra
  switch (monitor.get_entidad(entidad) -> getTipo()) {
    case jugador:
      return actuacionJugador(celda, accion,  x, y);
    break;

    case npc:
      return actuacionNPC(entidad, celda, accion, x, y);
    break;

    case objeto:
      return false;
    break;
  }

}

void lanzar_motor_juego(){

  Action accion;
  unsigned int xRand;
  unsigned int yRand;
  unsigned char celdaRand;

  vector < vector < vector <unsigned char> > > estado;


  if (monitor.continuarBelkan()) {


    if(monitor.jugar()){


      if(monitor.getPasos() != 0){

        estado.clear();
        for(unsigned int i = 0; i < monitor.numero_entidades(); i++){
          estado.push_back(monitor.getMapa() -> vision(i));
        }


        for(unsigned int i = 1; i < monitor.numero_entidades(); i++){
          accion = monitor.get_entidad(i) -> think(estado[i]);
          actuacion(i, accion);
          monitor.get_entidad(i) -> setVision(monitor.getMapa() -> vision(i));
        }


        accion = monitor.get_entidad(0) -> think(estado[0]);
        monitor.get_entidad(0)->getLastAction(accion);
        actuacion(0, accion);
        monitor.get_entidad(0) -> setVision(monitor.getMapa() -> vision(0));

        monitor.decPasos();


        //std::cout <<  monitor.get_entidad(0) ->toString();

        if(monitor.numero_entidades() > 0){
          if(not(monitor.get_entidad(0) -> vivo())
            or (monitor.getMapa() -> getCelda(monitor.get_entidad(0) -> getX(),monitor.get_entidad(0) -> getY()) == 'A' and monitor.get_entidad(0) -> getObjActivo().second != '1')
              or (monitor.getMapa() -> getCelda(monitor.get_entidad(0) -> getX(),monitor.get_entidad(0) -> getY()) == 'B' and monitor.get_entidad(0) -> getObjActivo().second != '2')){
            do {
              xRand = aleatorio3(monitor.getMapa()->getNFils()-1);
              yRand = aleatorio3(monitor.getMapa()->getNCols()-1);

              celdaRand = monitor.getMapa() -> getCelda(xRand,yRand);
            }
            while(not(((celdaRand == 'T') and monitor.getMapa() -> entidadEnCelda(xRand, yRand) == '_')) );


            monitor.get_entidad(0) -> setPosicion(xRand,yRand);
            monitor.get_entidad(0) -> setOrientacion(norte);
            monitor.get_entidad(0) -> resetEntidad();

          }
        }
      }
    }

    if(monitor.mostrarResultados()){
      unsigned int aciertos = 0, totalCasillas = 0;

      for(unsigned int i = 0; i < monitor.getMapa() -> getNCols(); i++){
        for(unsigned int j = 0; j < monitor.getMapa() -> getNCols(); j++){
          if(monitor.getMapa() -> getCelda(i, j) == monitor.get_entidad(0) -> getMapaResultado()[i][j]){
            aciertos++;
          }
          totalCasillas++;
        }
      }
      std::cout<< "Porcentaje de aciertos: " << ((float)aciertos/(float)totalCasillas) * 100.0 << std::endl;
      monitor.setMostrarResultados(false);
      monitor.setResultadosMostrados(true);
    }

    //std::this_thread::sleep_for(std::chrono::milliseconds(/*monitor.getRetardo()*/ 50));
  }
}


void lanzar_motor_juego2(MonitorJuego &monitor){

  Action accion;
  unsigned int xRand;
  unsigned int yRand;
  unsigned char celdaRand;

  vector < vector < vector <unsigned char> > > estado;


  while (!monitor.finJuego()) {

    if(monitor.jugar()){
      if(true){
        estado.clear();
        for(unsigned int i = 0; i < monitor.numero_entidades(); i++){
          estado.push_back(monitor.getMapa() -> vision(i));
        }


        for(unsigned int i = 1; i < monitor.numero_entidades(); i++){
          accion = monitor.get_entidad(i) -> think(estado[i]);
          actuacion(i, accion);
          monitor.get_entidad(i) -> setVision(monitor.getMapa() -> vision(i));
        }


        accion = monitor.get_entidad(0) -> think(estado[0]);
        monitor.get_entidad(0)->getLastAction(accion);
        actuacion(0, accion);
        monitor.get_entidad(0) -> setVision(monitor.getMapa() -> vision(0));

        monitor.decPasos();


        //std::cout <<  monitor.get_entidad(0) ->toString();

        if(monitor.numero_entidades() > 0){
          if(not(monitor.get_entidad(0) -> vivo())
            or (monitor.getMapa() -> getCelda(monitor.get_entidad(0) -> getX(),monitor.get_entidad(0) -> getY()) == 'A' and monitor.get_entidad(0) -> getObjActivo().second != '1')
              or (monitor.getMapa() -> getCelda(monitor.get_entidad(0) -> getX(),monitor.get_entidad(0) -> getY()) == 'B' and monitor.get_entidad(0) -> getObjActivo().second != '2')){
            do {
              xRand = aleatorio3(monitor.getMapa()->getNFils()-1);
              yRand = aleatorio3(monitor.getMapa()->getNCols()-1);

              celdaRand = monitor.getMapa() -> getCelda(xRand,yRand);
            }
            while(not(((celdaRand == 'T') and monitor.getMapa() -> entidadEnCelda(xRand, yRand) == '_')) );


            monitor.get_entidad(0) -> setPosicion(xRand,yRand);
            monitor.get_entidad(0) -> setOrientacion(norte);
            monitor.get_entidad(0) -> resetEntidad();

          }
        }
      }

    }
  }

    if(monitor.mostrarResultados()){
      unsigned int aciertos = 0, totalCasillas = 0;

      for(unsigned int i = 0; i < monitor.getMapa() -> getNCols(); i++){
        for(unsigned int j = 0; j < monitor.getMapa() -> getNCols(); j++){
          if(monitor.getMapa() -> getCelda(i, j) == monitor.get_entidad(0) -> getMapaResultado()[i][j]){
            aciertos++;
          }
          totalCasillas++;
        }
      }
      std::cout<< "Porcentaje de aciertos: " << ((float)aciertos/(float)totalCasillas) * 100.0 << std::endl;
      monitor.setMostrarResultados(false);
      monitor.setResultadosMostrados(true);
    }
}
