#include "modelos/leo/leo.hpp"


Leo3D::Leo3D() : Objeto3D(){
  glTranslatef(posicion.x, posicion.y, posicion.z);

  setHijo(new TorsoLeo3D("leo.jpg"));
  hijos[0] -> setTranformacion(_vertex4<float>(Translacion_, 0.0, 2.05, 0.0));
}
