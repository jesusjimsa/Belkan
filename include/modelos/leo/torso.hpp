#ifndef TORSOLEO3D_HPP
#define TORSOLEO3D_HPP

#include "obj3dlib.hpp"
#include "modelos/leo/cabeza.hpp"
#include "modelos/leo/brazo.hpp"
#include "modelos/leo/cintura.hpp"
#include <string.h>


class TorsoLeo3D : public Objeto3D{
private:

public:
  TorsoLeo3D(const std::string & filename);
  ~TorsoLeo3D(){}

};

#endif
