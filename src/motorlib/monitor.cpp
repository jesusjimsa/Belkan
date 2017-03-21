#include "motorlib/monitor.hpp"

#include <fstream>
#include <stdlib.h>

int aleatorio(int tope){
  double a = rand(), c = RAND_MAX;
  a = a / (c+1);
  int b = a*(tope+1);
  return b;
}

void MonitorJuego::nueva_entidad(Entidad * entidad){

    entidades.push_back(entidad);
}

unsigned int MonitorJuego::numero_entidades(){

  unsigned int ents;
    ents = entidades.size();

  return ents;
}

void MonitorJuego::clear(){
    entidades.clear();
    //pasos = 0;
    iteracion = 0;

}

Entidad * MonitorJuego::get_entidad(unsigned entidad){

  Entidad * ent;
    ent = entidades[entidad];

  return ent;
}

std::vector<Entidad *> * MonitorJuego::get_entidades(){
  std::vector<Entidad *> * listEnt;

    listEnt = &entidades;

  return listEnt;
}

void MonitorJuego::cambiarPosicion(unsigned char entidad, unsigned char x, unsigned char y){
    entidades[entidad]-> setPosicion(x,y);
}

void MonitorJuego::cambiarOrientacion(unsigned char entidad, unsigned char orientacion){
  Orientacion aux;
  switch (orientacion) {
    case 0:
    aux = norte;
    break;

    case 1:
    aux = este;
    break;

    case 2:
    aux = sur;
    break;

    case 3:
    aux=oeste;
    break;
  }
    entidades[entidad] -> setOrientacion(aux);
}


Mapa * MonitorJuego::getMapa(){
  Mapa * map;
    map = mapa;

  return map;
}


void MonitorJuego::setMapa(char * file){
  std::ifstream ifile;
  ifile.open(file, ios::in);
  unsigned int colSize, filSize;
  ifile >> colSize;
  ifile >> filSize;


  std::vector<std::vector<unsigned char> > mapAux;
  std::vector<unsigned char> colAux(colSize);

  for (unsigned int i = 0; i < filSize; i++) {
    mapAux.push_back(colAux);
  }

  for (unsigned int i = 0; i < filSize; i++) {
    for (unsigned int j = 0; j < colSize; j++) {
      ifile >> mapAux[i][j];
    }
  }
  ifile.close();


  mapa = new Mapa(mapAux, &entidades);

}

void MonitorJuego::startGame(){
    empezarJuego = true;
}


void MonitorJuego::cerrarBelkan(){
    apagarBelkan = true;
}

bool MonitorJuego::continuarBelkan(){
  bool fin;
    fin = apagarBelkan;
  return !fin;
}

bool MonitorJuego::inicializarJuego(){
  bool aux;
    aux = empezarJuego;
    if(empezarJuego){
      empezarJuego = false;
      iteracion = 0;
      resultados = false;
    }
  return aux;
}

bool MonitorJuego::juegoInicializado(){
      jugando = true;
}

bool MonitorJuego::jugar(){
  bool aux;
    aux = jugando;
  return aux;
}

bool MonitorJuego::mostrarResultados(){
  bool aux;
    aux = resultados;
  return aux;
}

void MonitorJuego::setMostrarResultados(bool valor){
    resultados = valor;
}


void MonitorJuego::girarJugadorIzquierda(){
    mapa -> girarCamaraIzquierda();
}

void MonitorJuego::girarJugadorDerecha(){
      mapa -> girarCamaraDerecha();
}


void MonitorJuego::setPasos(int Npasos){
    pasos = Npasos;
}

void MonitorJuego::decPasos(){
    if(pasos > 0){
      pasos--;
    }
    iteracion++;
    if(iteracion >= 20000){
      jugando = false;
      resultados = true;
    }
}

int MonitorJuego::getPasos(){
  int aux;
    aux = pasos;

  return aux;
}

bool MonitorJuego::finJuego(){
  return (iteracion>=20000);
}

void MonitorJuego::setRetardo(int tRetardo){
    retardo = tRetardo;
}

int MonitorJuego::getRetardo(){
  int aux;
    aux = retardo;

  return aux;
}


bool MonitorJuego::resultadosMostrados(){
  bool aux;
    aux = resMostrados;

  return aux;

}

void MonitorJuego::setResultadosMostrados(bool valor){
    resMostrados = valor;
}

void MonitorJuego::inicializar(){

  clear();
  setPasos(0);
  int tama= getMapa()->getNCols();
  int nhuesos = tama / 10;
  int nbiquinis = tama / 10;
  int nzapatillas = tama /10;
  int naldeanos = tama /10;
  int nlobos = tama /10;
  int npuertas = 0;
  for (unsigned int i = 0; i < getMapa() -> getNFils(); i++) {
    for (unsigned int j = 0; j <getMapa() -> getNCols(); j++) {
      if(getMapa() -> getCelda(i,j) == 'D'){
        npuertas++;
      }
    }
  }
  int nllaves = npuertas+1;


  unsigned int xRand, yRand;
  unsigned char celdaRand;

  //Primero SIEMPRE se coloca al jugador. SIEMPRE.
  do {

    xRand = aleatorio(getMapa()->getNFils()-1);
    yRand = aleatorio(getMapa()->getNCols()-1);

    celdaRand = getMapa() -> getCelda(xRand, yRand);
  }
  while(not(((celdaRand == 'T' ) and getMapa() -> entidadEnCelda(xRand, yRand) == '_')) );

  nueva_entidad(new Entidad(jugador, jugador_, norte, xRand, yRand, new Jugador3D("perrete.jpg"), new ComportamientoJugador(100)));

  /*do {
    xRand = distribution(random);
    yRand = distribution(random);

    celdaRand = monitor.getMapa() -> getCelda(xRand, yRand);
  }
  while(not(((celdaRand == 'T' ) and monitor.getMapa() -> entidadEnCelda(xRand, yRand) == '_')) );

  monitor.nueva_entidad(new Entidad(jugador, jugador_, norte, xRand, yRand, new Jugador3D("perrete.jpg"), new ComportamientoJugador(100)));*/


  for (int i=0; i<nhuesos; i++){
    do {
      xRand = aleatorio(getMapa()->getNFils()-1);
      yRand = aleatorio(getMapa()->getNCols()-1);

      celdaRand = getMapa() -> getCelda(xRand, yRand);
    }
    while(not(((celdaRand == 'T' or celdaRand=='S') and getMapa() -> entidadEnCelda(xRand, yRand) == '_')) );

    nueva_entidad(new Entidad(objeto, hueso, norte, xRand, yRand, new Item3D(), new ComportamientoObjeto()));
  }


  for (int i=0; i<nbiquinis; i++){
    do {
      xRand = aleatorio(getMapa()->getNFils()-1);
      yRand = aleatorio(getMapa()->getNCols()-1);

      celdaRand = getMapa() -> getCelda(xRand, yRand);
    }
    while(not(((celdaRand == 'T' or celdaRand=='S') and getMapa() -> entidadEnCelda(xRand, yRand) == '_')) );

    nueva_entidad(new Entidad(objeto, biquini, norte, xRand, yRand, new Item3D(), new ComportamientoObjeto()));
  }


  for (int i=0; i<nzapatillas; i++){
    do {
      xRand = aleatorio(getMapa()->getNFils()-1);
      yRand = aleatorio(getMapa()->getNCols()-1);

      celdaRand = getMapa() -> getCelda(xRand, yRand);
    }
    while(not(((celdaRand == 'T' or celdaRand=='S') and getMapa() -> entidadEnCelda(xRand, yRand) == '_')) );

    nueva_entidad(new Entidad(objeto, zapatillas, norte, xRand, yRand, new Item3D(), new ComportamientoObjeto()));
  }


  for (int i=0; i<nllaves; i++){
    do {
      xRand = aleatorio(getMapa()->getNFils()-1);
      yRand = aleatorio(getMapa()->getNCols()-1);

      celdaRand = getMapa() -> getCelda(xRand, yRand);
    }
    while(not(((celdaRand == 'T') and getMapa() -> entidadEnCelda(xRand, yRand) == '_')) );

    nueva_entidad(new Entidad(objeto, llaves, norte, xRand, yRand, new Item3D(), new ComportamientoObjeto()));
  }

  for (unsigned int i = 0; i < getMapa() -> getNFils(); i++) {
    for (unsigned int j = 0; j < getMapa() -> getNCols(); j++) {
      if(getMapa() -> getCelda(i,j) == 'D'){
        nueva_entidad(new Entidad(npc, puerta, norte, i, j, new Puerta3D(), new ComportamientoPuerta()));
      }
    }
  }


  for (int i=0; i<nlobos; i++){
    do {
      xRand = aleatorio(getMapa()->getNFils()-1);
      yRand = aleatorio(getMapa()->getNCols()-1);

      celdaRand = getMapa() -> getCelda(xRand, yRand);
    }
    while(not(((celdaRand == 'T' or celdaRand=='S' or celdaRand=='B') and getMapa() -> entidadEnCelda(xRand, yRand) == '_')) );

    nueva_entidad(new Entidad(npc, lobo, norte, xRand, yRand, new Perro3D(), new ComportamientoPerro()));
  }


  for (int i=0; i<naldeanos; i++){
    do {
      xRand = aleatorio(getMapa()->getNFils()-1);
      yRand = aleatorio(getMapa()->getNCols()-1);

      celdaRand = getMapa() -> getCelda(xRand, yRand);
    }
    while(not(((celdaRand == 'T' or celdaRand=='S' ) and getMapa() -> entidadEnCelda(xRand, yRand) == '_')) );

    nueva_entidad(new Entidad(npc, aldeano, norte, xRand, yRand, new Aldeano3D("cara1.jpg"), new ComportamientoAldeano()));
  }


  get_entidad(0) -> setVision(getMapa() -> vision(0));

}
