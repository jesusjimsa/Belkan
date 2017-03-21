#ifndef CABEZALEO3D_HPP
#define CABEZALEO3D_HPP

#include "obj3dlib.hpp"
#include "modelos/retrato.hpp"
#include <string.h>


class CabezaLeo3D : public Objeto3D{
private:

public:
  CabezaLeo3D(const std::string & filename);
  ~CabezaLeo3D(){}

};

#endif
