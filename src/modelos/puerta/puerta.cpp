#include "modelos/puerta/puerta.hpp"

Puerta3D::Puerta3D() : Objeto3D(){
  glTranslatef(posicion.x, posicion.y, posicion.z);

  setTranformacion(_vertex4<float>(Translacion_, 0.0, 1.3, 0.0));
  //Ponemos el tronco
  setHijo(new CuboObj3D());
  hijos[0] -> setTranformacion(_vertex4<float>(Escalado_, 5.0, 5.0, 5.0));
  hijos[0] -> setTranformacion(_vertex4<float>(Translacion_, 0.0, 0.2, 0.0));
  hijos[0] -> setColor(_vertex3<float>(66.0/256.0, 18.0/256.0, 7.0/256.0));

}

Puerta3D::~Puerta3D(){
  delete hijos[0];
}
