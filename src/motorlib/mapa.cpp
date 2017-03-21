#include "motorlib/mapa.hpp"

#include <iostream>


void Mapa::colorCeldaMM(unsigned char celda){
  switch (celda) {
    case 'P': glColor3f(0.15,0.15,0.15);
        break;
    case 'B': glColor3f(0.0,1.0,0.0);
        break;
    case 'A': glColor3f(0.0,0.0,1.0);
        break;
    case 'S': glColor3f(0.6,0.6,0.6);
        break;
    case 'M': glColor3f(0.6,0.0,0.0);
        break;
    case 'T': glColor3f(0.3,0.25,0.2);
        break;
    case 'K': glColor3f(1.0,1.0,0.0);
        break;
    case 'Z': glColor3f(0.3,0.3,0.3);
        break;
    case 'D': glColor3f(0.0,0.0,0.0);
        break;
    default:
      glColor3f(0.5, 0.5, 0.5);
      break;
    }
}

void Mapa::OrientacionEntidadMM(Orientacion orienParam){
  switch (orienParam) {
    case norte:
      glRotatef(90.0,0,0,1);
      break;

    case este:
      glRotatef(0.0,0,0,1);
      break;

    case sur:
      glRotatef(270.0,0,0,1);
      break;

    case oeste:
    glRotatef(180.0,0,0,1);
    break;
  }
}

void Mapa::OrientacionEntidadFP(Orientacion orienParam){
  switch (orienParam) {
    case norte:
      glRotatef(0.0,0,1,0);
      break;

    case este:
      glRotatef(270.0,0,1,0);
      break;

    case sur:
      glRotatef(180.0,0,1,0);
      break;

    case oeste:
    glRotatef(90.0,0,1,0);
    break;
  }

}


void Mapa::complementosCelda(unsigned char celda){

  Arbol3D *arbol = new Arbol3D();

  switch (celda) {
    case 'P':
      glColor3f(0.15,0.15,0.15);

      glTranslatef(0.0, 7.5, 0.0);
      glScalef(5.0, 15.0, 5.0);
      glutSolidCube(1.0);
      break;
    case 'B':
      //glColor3f(0.0,1.0,0.0);
      glScalef(5.0, 15.0, 5.0);
      arbol->draw(1);
      break;
    case 'A':
      //glColor3f(0.0,0.0,1.0);
      break;
    case 'S':
      //glColor3f(0.6,0.6,0.6);
      break;
    case 'M':
      glColor3f(0.6,0.0,0.0);

      glTranslatef(0.0, 7.5, 0.0);
      glScalef(5.0, 15.0, 5.0);
      glutSolidCube(1.0);
      break;
    case 'T':
      //glColor3f(0.3,0.25,0.2);
      break;
    case 'K':
      //glColor3f(1.0,1.0,0.0);
      break;
    case 'Z':
      //glColor3f(0.3,0.3,0.3);
      break;
    case 'D':
      //glColor3f(0.0,0.0,0.0);
      break;
    default:
      //glColor3f(0.5, 0.5, 0.5);
      break;
  }
  delete arbol;
}


void Mapa::formaEntidad(unsigned char tipoParam){

  RevolucionObj3D * jug = new RevolucionObj3D("ply/cilindro.ply", 3);
  switch (tipoParam) {
    case 'j':
      glRotatef(90.0,1,0,0);
      glScalef(1.0, 1.0, 0.75);
      jug -> setColor( _vertex3<float> (1.0,0.0,0.0));
      jug -> draw(1);
      break;

    case 'l':
    case 'a':
    case 'd':
      //std::cout << "NPC??" << std::endl;
      glColor3f(255.0/255.0, 159.0/255.0, 5.0/255.0);
      glutSolidCube(0.6);
      break;


    case '0':
    case '1':
    case '2':
    case '3':
      glColor3f(1.0, 5.0/255.0, 238/255.0);
      glutSolidSphere(0.5, 6, 2);
      break;

  }
}

void  Mapa::drawMM1() {


  unsigned int colMed, filaMed;

  colMed = (mapaCompleto.size()/2);
  filaMed = (mapaCompleto[0].size()/2);

  glLoadIdentity();
  glOrtho(-10.0,10.0,-10.0,10.0,-1.0,1.0);
  glMatrixMode(GL_MODELVIEW);


  float ratio = 1.0;
  if(mapaCompleto.size() > 15){
    ratio = 15.0 / (float) mapaCompleto.size();
  }

 //std:: cout << mapaCompleto[98].size() << " " << mapaVisible[0].size() << std::endl;


  for (unsigned int i = 0; i < mapaCompleto.size(); i++) {
    for (unsigned int j = 0; j < mapaCompleto[i].size(); j++) {
      glPushMatrix();

        glTranslatef(((GLfloat) i - (GLfloat) filaMed ) * ratio, ((GLfloat) colMed - (GLfloat) j) * ratio, 0.0);
        glScalef(ratio, ratio, ratio);

        colorCeldaMM(mapaCompleto[j][i]);

        glBegin(GL_QUADS);
          glVertex3f(-0.5, -0.5, 0.0);
          glVertex3f(-0.5, 0.5, 0.0);
          glVertex3f( 0.5, 0.5, 0.0);
          glVertex3f( 0.5, -0.5, 0.0);
        glEnd();

      glPopMatrix();

    }


  }
  for (unsigned int i = 0; i < entidades->size(); i++) {
    if((*entidades)[i]-> getHitbox()){
      glPushMatrix();

      glTranslatef(((GLfloat) (*entidades)[i]->getY() - (GLfloat) colMed) * ratio, ((GLfloat) filaMed - (GLfloat) (*entidades)[i]->getX()) * ratio, 0.0);

      OrientacionEntidadMM((*entidades)[i]->getOrientacion());

      glScalef(ratio, ratio, ratio);

      formaEntidad((*entidades)[i]->getSubTipoChar());

      glPopMatrix();
    }
  }



  //glMatrixMode(GL_PROJECTION);
}



void  Mapa::drawMM2() {

  std::vector< std::vector< unsigned char> > mapaSuperficie = (*entidades)[0] -> getMapaResultado();
  std::vector< std::vector< unsigned char> > mapaEntidades = (*entidades)[0] -> getMapaEntidades();

  unsigned int colMed, filaMed;

  /*colMed = (mapaSuperficie.size()/2);
  filaMed = (mapaSuperficie[0].size()/2);*/

  colMed = (mapaCompleto.size()/2);
  filaMed = (mapaCompleto[0].size()/2);

  glLoadIdentity();
  glOrtho(-10.0,10.0,-10.0,10.0,-1.0,1.0);
  glMatrixMode(GL_MODELVIEW);


//  float ratio = 15.0 / (float) mapaSuperficie.size();


   float ratio = 1.0;
   if(mapaCompleto.size() > 15){
     ratio = 15.0 / (float) mapaCompleto.size();
   }



 /*

  for (unsigned int i = 0; i < mapaSuperficie.size(); i++) {
    for (unsigned int j = 0; j < mapaSuperficie[i].size(); j++) { */

      for (unsigned int i = 0; i < mapaCompleto.size(); i++) {
        for (unsigned int j = 0; j < mapaCompleto[i].size(); j++) {

      glPushMatrix();

      glTranslatef(((GLfloat) i - (GLfloat) filaMed ) * ratio, ((GLfloat) colMed - (GLfloat) j) * ratio, 0.0);
        glScalef(ratio, ratio, ratio);

        colorCeldaMM(mapaSuperficie[j][i]);

        glBegin(GL_QUADS);
          glVertex3f(-0.5, -0.5, 0.0);
          glVertex3f(-0.5, 0.5, 0.0);
          glVertex3f( 0.5, 0.5, 0.0);
          glVertex3f( 0.5, -0.5, 0.0);
        glEnd();

      glPopMatrix();

    }
  }

  for (unsigned int i = 0; i < entidades->size(); i++) {
    if((*entidades)[i]-> getHitbox()){
      glPushMatrix();

      glTranslatef(((GLfloat) (*entidades)[i]->getY() - (GLfloat) colMed) * ratio, ((GLfloat) filaMed - (GLfloat) (*entidades)[i]->getX()) * ratio, 0.0);

      OrientacionEntidadMM((*entidades)[i]->getOrientacion());

      glScalef(ratio, ratio, ratio);

      formaEntidad((*entidades)[i]->getSubTipoChar());

      glPopMatrix();
    }
  }
}


void Mapa::drawFirstPerson(){

  unsigned int colMed, filaMed;

  colMed = (mapaCompleto.size()/2);
  filaMed = (mapaCompleto[0].size()/2);

  if(entidades->size() > 0){
    //X = COLUMNA
    x = (*entidades)[0]->getY() + 1;
    z = (*entidades)[0]->getX() + 1;

    switch ((*entidades)[0]->getOrientacion()) {
      case norte:
      angulo = 180.0;
      break;
      case sur:
      angulo = 0.0;
      break;
      case este:
      angulo = 270.0;
      break;
      case oeste:
      angulo = 90.0;
      break;
    }
  }

  //Camara


    gluLookAt(	((GLfloat) colMed - (GLfloat) x) * 5.0,
                  3.75,
                  ((GLfloat) filaMed - (GLfloat) z) * 5.0,
                      (((GLfloat) colMed - (GLfloat) x) * 5.0 ) + sin((angulo * 2.0 * M_PI)/360.0),
                      3.5,
                        (((GLfloat) filaMed - (GLfloat) z) * 5.0) - cos((angulo * 2.0 * M_PI)/360.0) ,
                            0.0,
                              1.0,
                                0.0);


  int fs = x-15; if (fs<0) fs=0;
  int fi = x+15; if (fi>=mapaCompleto.size()) fi=mapaCompleto.size()-1;

  int ci = z-15; if (ci<0) ci=0;
  int cd = z+15; if (cd>=mapaCompleto.size()) cd=mapaCompleto.size()-1;

  for (unsigned int i = fs; i < fi; i++) {
    for (unsigned int j = ci; j < cd; j++) {
      //Suelo
      glPushMatrix();


      //glTranslatef(((GLfloat) i - (GLfloat) filaMed ) * ratio, ((GLfloat) colMed - (GLfloat) j) * ratio, 0.0);

      if(mapaCompleto[j][i] == 'K'){
        glTranslatef(((GLfloat) filaMed  - (GLfloat) i - 1 ) * 5.0, 0.25, ((GLfloat) colMed - (GLfloat) j  - 1) * 5.0);

        colorCeldaMM(mapaCompleto[j][i]);

        glScalef(5.0, 0.4, 5.0);
        glutSolidCube(1.0);

      }
      else if(mapaCompleto[j][i] == 'A'){
        glTranslatef(((GLfloat) filaMed  - (GLfloat) i - 1 ) * 5.0, -0.3, ((GLfloat) colMed - (GLfloat) j  - 1) * 5.0);

        colorCeldaMM(mapaCompleto[j][i]);

        glScalef(5.0, 0.4, 5.0);
        glutSolidCube(1.0);

      }
      else{
      //else if(mapaCompleto[j][i] == 'T' or mapaCompleto[j][i] == 'S'){
        glTranslatef(((GLfloat) filaMed  - (GLfloat) i - 1 ) * 5.0, 0.0, ((GLfloat) colMed - (GLfloat) j  - 1) * 5.0);

        colorCeldaMM(mapaCompleto[j][i]);

        glScalef(5.0, 0.4, 5.0);
        glutSolidCube(1.0);
      }

      glPopMatrix();

        /*if(entidadEnCelda(i,j) == '_'){*/
          //Complementos
          glPushMatrix();

          glTranslatef(((GLfloat) filaMed  - (GLfloat) i - 1 ) * 5.0, 0.0, ((GLfloat) colMed - (GLfloat) j  - 1) * 5.0);

          complementosCelda(mapaCompleto[j][i]);

          glPopMatrix();

    //}*/
  }
}


  Luz luz(GL_LIGHT0);
  luz.setPosicion(_vertex3<float>(0.0, 20.0, 0.0));
  luz.encender();
  luz.draw();


  //Dibujamos las entidades
  for (unsigned int i = 1; i < entidades->size(); i++) {
    x = (*entidades)[i]->getY() + 1;
    z = (*entidades)[i]->getX() + 1;

    glPushMatrix();
      glTranslatef(((GLfloat) colMed - (GLfloat) x) * 5.0, 0.4 ,   ((GLfloat) filaMed - (GLfloat) z) * 5.0);

      OrientacionEntidadFP((*entidades)[i]->getOrientacion());
      (*entidades)[i]->draw(2);

    glPopMatrix();
  }

}

int Mapa::casillaOcupada(unsigned int entidad){

  unsigned int x = (*entidades)[entidad] ->getX();
  unsigned int y = (*entidades)[entidad] ->getY();


  switch ((*entidades)[entidad] -> getOrientacion()) {
    case norte:
      x--;
    break;

    case este:
      y++;
    break;

    case sur:
      x++;
    break;
    case oeste:
      y--;
    break;
  }


  for (unsigned int i = 0; i < entidades->size(); i++) {

    if(((*entidades)[i]->getX() == x and (*entidades)[i]->getY() == y) and (entidad != i) and ((*entidades)[i]->getHitbox())){
      return i;
    }
  }
    return -1;
}

  int Mapa::casillaOcupadaThrow(unsigned int entidad){

    unsigned int x1 = (*entidades)[entidad] ->getX();
    unsigned int x2 = (*entidades)[entidad] ->getX();
    unsigned int x3 = (*entidades)[entidad] ->getX();

    unsigned int y1 = (*entidades)[entidad] ->getY();
    unsigned int y2 = (*entidades)[entidad] ->getY();
    unsigned int y3 = (*entidades)[entidad] ->getY();


    switch ((*entidades)[entidad] -> getOrientacion()) {
      case norte:
        x1--;
        x2-=2;
        x3-=3;
      break;

      case este:
        y1++;
        y2+=2;
        y3+=3;
      break;

      case sur:
        x1++;
        x2+=2;
        x3+=3;
      break;
      case oeste:
        y1--;
        y2-=2;
        y3-=3;
      break;
    }

    for (unsigned int i = 0; i < entidades->size(); i++) {
      if((*entidades)[i]->getX() == x1 and (*entidades)[i]->getY() == y1 and entidad != i){
        return i;
      }

      if((*entidades)[i]->getX() == x2 and (*entidades)[i]->getY() == y2 and entidad != i){
        return i;
      }

      if((*entidades)[i]->getX() == x3 and (*entidades)[i]->getY() == y3 and entidad != i){
        return i;
      }
    }
    return -1;
}

void Mapa::girarCamaraIzquierda(){
  angulo -= 90.0;
  if (angulo < 0.0){
    angulo = 270.0;
  }
}
void Mapa::girarCamaraDerecha(){
  angulo += 90.0;
  if (angulo >= 360.0){
    angulo = 0.0;
  }
}


unsigned char Mapa::entidadEnCelda(unsigned int x, unsigned int y){

  bool encontrado = false;
  unsigned int aux = 0;


  for(unsigned int i = 0; i< entidades -> size(); i++){
    if((*entidades)[i] -> getX() == x and (*entidades)[i] -> getY() == y){
      encontrado = true;
      aux = i;
      break;
    }
  }

  if(encontrado and (*entidades)[aux] -> getHitbox()){
    switch ((*entidades)[aux] -> getSubTipo()) {
      case jugador_:
        return 'j';
      break;

      case lobo:
        return 'l';
      break;

      case puerta:
        return 'd';
      break;

      case aldeano:
        return 'a';
      break;

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

    }
  }
  else{
    return '_';
  }

}

std::vector <std::vector< unsigned char> > Mapa::vision(unsigned int Entidad){

  unsigned int x = (*entidades)[Entidad] -> getX();
  unsigned int y = (*entidades)[Entidad] -> getY();
  Orientacion orient = (*entidades)[Entidad] -> getOrientacion();
  bool _jugador = (*entidades)[Entidad] -> getTipo() == jugador;


  std::vector <std::vector< unsigned char> > fov;
  std::vector< unsigned char> aux(16);
  fov.push_back(aux);
  fov.push_back(aux);
  switch (orient) {
    case norte:
    fov[0][0] = getCelda(x, y);
    fov[0][1] = getCelda(x-1, y-1);
    fov[0][2] = getCelda(x-1, y);
    fov[0][3] = getCelda(x-1, y+1);
    fov[0][4] = getCelda(x-2, y-2);
    fov[0][5] = getCelda(x-2, y-1);
    fov[0][6] = getCelda(x-2, y);
    fov[0][7] = getCelda(x-2, y+1);
    fov[0][8] = getCelda(x-2, y+2);
    fov[0][9] = getCelda(x-3, y-3);
    fov[0][10] = getCelda(x-3, y-2);
    fov[0][11] = getCelda(x-3, y-1);
    fov[0][12] = getCelda(x-3, y);
    fov[0][13] = getCelda(x-3, y+1);
    fov[0][14] = getCelda(x-3, y+2);
    fov[0][15] = getCelda(x-3, y+3);

    fov[1][0] = entidadEnCelda(x, y);
    fov[1][1] = entidadEnCelda(x-1, y-1);
    fov[1][2] = entidadEnCelda(x-1, y);
    fov[1][3] = entidadEnCelda(x-1, y+1);
    fov[1][4] = entidadEnCelda(x-2, y-2);
    fov[1][5] = entidadEnCelda(x-2, y-1);
    fov[1][6] = entidadEnCelda(x-2, y);
    fov[1][7] = entidadEnCelda(x-2, y+1);
    fov[1][8] = entidadEnCelda(x-2, y+2);
    fov[1][9] = entidadEnCelda(x-3, y-3);
    fov[1][10] = entidadEnCelda(x-3, y-2);
    fov[1][11] = entidadEnCelda(x-3, y-1);
    fov[1][12] = entidadEnCelda(x-3, y);
    fov[1][13] = entidadEnCelda(x-3, y+1);
    fov[1][14] = entidadEnCelda(x-3, y+2);
    fov[1][15] = entidadEnCelda(x-3, y+3);

    break;

    case este:
    fov[0][0] = getCelda(x, y);
    fov[0][1] = getCelda(x-1, y+1);
    fov[0][2] = getCelda(x, y+1);
    fov[0][3] = getCelda(x+1, y+1);
    fov[0][4] = getCelda(x-2, y+2);
    fov[0][5] = getCelda(x-1, y+2);
    fov[0][6] = getCelda(x, y+2);
    fov[0][7] = getCelda(x+1, y+2);
    fov[0][8] = getCelda(x+2, y+2);
    fov[0][9] = getCelda(x-3, y+3);
    fov[0][10] = getCelda(x-2, y+3);
    fov[0][11] = getCelda(x-1, y+3);
    fov[0][12] = getCelda(x, y+3);
    fov[0][13] = getCelda(x+1, y+3);
    fov[0][14] = getCelda(x+2, y+3);
    fov[0][15] = getCelda(x+3, y+3);


    fov[1][0] = entidadEnCelda(x, y);
    fov[1][1] = entidadEnCelda(x-1, y+1);
    fov[1][2] = entidadEnCelda(x, y+1);
    fov[1][3] = entidadEnCelda(x+1, y+1);
    fov[1][4] = entidadEnCelda(x-2, y+2);
    fov[1][5] = entidadEnCelda(x-1, y+2);
    fov[1][6] = entidadEnCelda(x, y+2);
    fov[1][7] = entidadEnCelda(x+1, y+2);
    fov[1][8] = entidadEnCelda(x+2, y+2);
    fov[1][9] = entidadEnCelda(x-3, y+3);
    fov[1][10] = entidadEnCelda(x-2, y+3);
    fov[1][11] = entidadEnCelda(x-1, y+3);
    fov[1][12] = entidadEnCelda(x, y+3);
    fov[1][13] = entidadEnCelda(x+1, y+3);
    fov[1][14] = entidadEnCelda(x+2, y+3);
    fov[1][15] = entidadEnCelda(x+3, y+3);




    break;

    case sur:
    fov[0][0] = getCelda(x, y);
    fov[0][1] = getCelda(x+1, y+1);
    fov[0][2] = getCelda(x+1, y);
    fov[0][3] = getCelda(x+1, y-1);
    fov[0][4] = getCelda(x+2, y+2);
    fov[0][5] = getCelda(x+2, y+1);
    fov[0][6] = getCelda(x+2, y);
    fov[0][7] = getCelda(x+2, y-1);
    fov[0][8] = getCelda(x+2, y-2);
    fov[0][9] = getCelda(x+3, y+3);
    fov[0][10] = getCelda(x+3, y+2);
    fov[0][11] = getCelda(x+3, y+1);
    fov[0][12] = getCelda(x+3, y);
    fov[0][13] = getCelda(x+3, y-1);
    fov[0][14] = getCelda(x+3, y-2);
    fov[0][15] = getCelda(x+3, y-3);

    fov[1][0] = entidadEnCelda(x, y);
    fov[1][1] = entidadEnCelda(x+1, y+1);
    fov[1][2] = entidadEnCelda(x+1, y);
    fov[1][3] = entidadEnCelda(x+1, y-1);
    fov[1][4] = entidadEnCelda(x+2, y+2);
    fov[1][5] = entidadEnCelda(x+2, y+1);
    fov[1][6] = entidadEnCelda(x+2, y);
    fov[1][7] = entidadEnCelda(x+2, y-1);
    fov[1][8] = entidadEnCelda(x+2, y-2);
    fov[1][9] = entidadEnCelda(x+3, y+3);
    fov[1][10] = entidadEnCelda(x+3, y+2);
    fov[1][11] = entidadEnCelda(x+3, y+1);
    fov[1][12] = entidadEnCelda(x+3, y);
    fov[1][13] = entidadEnCelda(x+3, y-1);
    fov[1][14] = entidadEnCelda(x+3, y-2);
    fov[1][15] = entidadEnCelda(x+3, y-3);



    break;
    case oeste:
      fov[0][0] = getCelda(x, y);
      fov[0][1] = getCelda(x+1, y-1);
      fov[0][2] = getCelda(x, y-1);
      fov[0][3] = getCelda(x-1, y-1);
      fov[0][4] = getCelda(x+2, y-2);
      fov[0][5] = getCelda(x+1, y-2);
      fov[0][6] = getCelda(x, y-2);
      fov[0][7] = getCelda(x-1, y-2);
      fov[0][8] = getCelda(x-2, y-2);
      fov[0][9] = getCelda(x+3, y-3);
      fov[0][10] = getCelda(x+2, y-3);
      fov[0][11] = getCelda(x+1, y-3);
      fov[0][12] = getCelda(x, y-3);
      fov[0][13] = getCelda(x-1, y-3);
      fov[0][14] = getCelda(x-2, y-3);
      fov[0][15] = getCelda(x-3, y-3);


      fov[1][0] = entidadEnCelda(x, y);
      fov[1][1] = entidadEnCelda(x+1, y-1);
      fov[1][2] = entidadEnCelda(x, y-1);
      fov[1][3] = entidadEnCelda(x-1, y-1);
      fov[1][4] = entidadEnCelda(x+2, y-2);
      fov[1][5] = entidadEnCelda(x+1, y-2);
      fov[1][6] = entidadEnCelda(x, y-2);
      fov[1][7] = entidadEnCelda(x-1, y-2);
      fov[1][8] = entidadEnCelda(x-2, y-2);
      fov[1][9] = entidadEnCelda(x+3, y-3);
      fov[1][10] = entidadEnCelda(x+2, y-3);
      fov[1][11] = entidadEnCelda(x+1, y-3);
      fov[1][12] = entidadEnCelda(x, y-3);
      fov[1][13] = entidadEnCelda(x-1, y-3);
      fov[1][14] = entidadEnCelda(x-2, y-3);
      fov[1][15] = entidadEnCelda(x-3, y-3);
    break;
  }

  return fov;
}
