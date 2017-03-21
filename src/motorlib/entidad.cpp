#include "motorlib/entidad.hpp"
#include <stdlib.h>

int aleatorio2(int tope){
  double a = rand(), c = RAND_MAX;
  a = a / (c+1);
  int b = a*(tope+1);
  return b;
}


void Entidad::resetEntidad(){

  hitbox = false;
  if(tipo == jugador){
    desactivado = 0;
  }
  else{
    desactivado = aleatorio2(7)+3;
  }

  std::queue <pair <int , unsigned char > > aux;
  mochila.swap(aux);

  colision = false;

  reset = true;

  //Mapa
  //MapaEnt

  objActivo.first = -1;
  objActivo.second = -1;

  vida = 1000;


}

Action Entidad::think(std::vector <std::vector< unsigned char> > vision){


    Action accion;
    Sensores sensor;
    if(desactivado == 0 and tipo != objeto){
      vida--;
      hitbox = true;

      sensor.vida = vida;

      if(objActivo.first == -1){
        sensor.objetoActivo = '_';
      }
      else{
        sensor.objetoActivo = objActivo.second;
      }

      if(mochila.size() > 0){
        sensor.mochila = mochila.front().second;
      }
      else{
        sensor.mochila = '_';
      }

      sensor.colision = colision;
      sensor.reset = reset;

      if(mensaje){
        sensor.mensajeF = x;
        sensor.mensajeC = y;
      }else{
        sensor.mensajeF = -1;
        sensor.mensajeC = -1;
      }

      sensor.superficie = vision[1];
      sensor.terreno = vision[0];


      visionAux = vision;


      accion = comportamiento -> think(sensor);


      colision = false;
      reset = false;
      mensaje = false;

      return accion;
    }
    else{
      desactivado--;
      return actIDLE;
    }

  }


bool Entidad::interact(Action accion, int valor){

  if(accion == actPUSH){
    if(mochila.size() < 4){
      mochila.push(objActivo);
      quitarObjActivo();
      return true;
    }
    else{
      return false;
    }


  }

  if(accion == actPOP){
    if(manoVacia()){
      objActivo = mochila.front();
      mochila.pop();
      return true;
    }
    else{
      return false;
    }

  }

  int retorno  =  comportamiento -> interact(accion, valor);

  if( retorno == 1){
    resetEntidad();
    return true;
  }

  if( retorno == 2){
    hitbox = false;
    desactivado = -1;
    return true;
  }

  if( retorno == 3){
    hitbox = true;
    desactivado = 0;
    return true;
  }

  return false;
}


unsigned char Entidad::getSubTipoChar(){
  switch (subtipo) {
  case hueso:
    return '0';
  break;

  case biquini:
    return '1';
  break;

  case zapatillas:
    return '2';
  break;

  case llaves:
    return '3';
  break;

  case jugador_:
    return 'j';
  break;

  case lobo:
    return 'l';
  break;

  case aldeano:
    return 'a';
  break;

  case puerta:
    return 'd';
  break;
  }
}

string strAccion(int accion){
  switch (accion){
    case 0: return "actFORWARD";
    case 1: return "actTURN_L";
    case 2: return "actTURN_R";
    case 3: return "actIDLE";
    case 4: return "actPICKUP";
    case 5: return "actPUTDOWN";
    case 6: return "actPUSH";
    case 7: return "actPOP";
    case 8: return "actGIVE";
    case 9: return "actTHROW";
  }
}



std::string Entidad::toString(){

  std::string str;

  str += "Vida restante: ";
  str += std::to_string(vida);
  str += "\n";


  str +=  "Objeto en uso ";

  if(objActivo.first == -1){
    str += '_';
  }
  else{
    str += objActivo.second;
  }

  str += "\n";


  str += "Primer objeto de la mochila ";

  if(mochila.size() > 0){
      str += mochila.front().second;
  }
  else{
      str += '_';
  }

  str += "\n";

  str += "He colisionado? ";
  str += to_string(colision);
  str += "\n";


  str += "Acabo de morir? ";
  str += to_string(reset);
  str += "\n";


  str += "Fila y columna del PK (";
  if(mensaje){
    str += to_string(x);
    str += " , ";
    str += to_string(y);
    str += ")";
    str += "\n";
  }else{
    str += to_string(-1);
    str += " , ";
    str += to_string(-1);
    str += ")";
    str += "\n";
  }

  str +="Ultima Accion: " + strAccion(last_action) + "\n";


  if(visionAux.size() > 0){
    str += "Superficie:\n";

    for(unsigned int i = 0; i<visionAux[1].size(); i++){
      str += visionAux[1][i];
      str += " ";
    }

    str += "\n";

    str += "Terreno:\n";

    for(unsigned int i = 0; i<visionAux[0].size(); i++){
      str += visionAux[0][i];
      str += " ";
    }

    str += "\n";
  }

      return str;

}
