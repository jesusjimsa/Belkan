#include "motorlib.hpp"
#include "obj3dlib.hpp"
#include "motorlib/monitor.hpp"

#include "modelos_belkan.hpp"

#include <GL/glut.h>
#include <glui/glui.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>

#include <string>
#include <boost/algorithm/string.hpp>


MonitorJuego monitor;
//extern MonitorJuego monitor;

//Variables que controlan las ventanas y vistas
GLuint ventanaPrincipal, vistaPrincipal, vistaMiniMapa, vistaIU, idIU;
GLUI * panelIU, * panelSelecMapa;
GLUI_Panel * panelInfo;
GLUI_Button * botonElegirMapa, * botonJuegoRemoto, * botonPaso, * botonEjecucion, * botonEjecutar, * botonSalir, * botonAceptarNuevoMapa, * botonCancelarNuevoMapa;
GLUI_EditText * editTextPasos, * editTextRetardo;
GLUI_Checkbox * drawMM;
GLUI_StaticText * lineaVacia1, * lineaVacia2, * lineaVacia3, * lineaVacia4, * rellenarPanel;
GLUI_StaticText * info0, * info1, * info2, * info3,* info4, * info5, * info6, * info7, * info8, * info9, *info10;
GLUI_FileBrowser *map_fb;

// variables que determninan la posicion y tamaño de la ventana X
int UI_window_pos_x=50,UI_window_pos_y=50,IU_ancho_ventana=1024,IU_alto_ventana=768;

int nPasos = 10, tRetardo = 10, MMmode = 0;

float valor = 0.0, ratio = 1.0;

void irAlJuego(int value);

//***************************************************************************
// Funcion llamada cuando se produce un cambio en el tamaño de la ventana
//
// el evento manda a la funcion:
// nuevo ancho
// nuevo alto
//***************************************************************************

void reshape(int ancho,  int alto){

  if(ancho > 1 and alto > 1){
    //parametros de la ventana principal

    ratio = ((float) ancho * 0.7) / (float) alto;

    glMatrixMode(GL_PROJECTION);
    glViewport(0, 0, ancho, alto);
    glLoadIdentity();
    gluOrtho2D(0, ancho, alto, 0);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    //Display vista principal
    glutSetWindow(vistaPrincipal);
    glutPositionWindow(0, 0);
    glutReshapeWindow(ancho*0.7, alto);
    glutPostRedisplay();

    //Display vista mini-mapa

    glutSetWindow(vistaMiniMapa);
    glutPositionWindow(ancho*0.7, 0);
    glutReshapeWindow( (ancho*0.3) + 1, alto*0.3);
    glutPostRedisplay();

    //Display interfaz
    glutSetWindow(vistaIU);
    glutPositionWindow(ancho*0.7, alto*0.3);
    glutReshapeWindow((ancho*0.3) + 1, (alto*0.7) + 1);
    lineaVacia1 -> set_w((ancho*0.3) + 1);
    rellenarPanel -> set_h((alto*0.7) + 1);
    glutPostRedisplay();
  }
}

  void ResetViewport(){
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glFrustum(-10.0, 10.0, -10.0, 10.0, 0.0, 10.0);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
  }

  void display_ventPrincipal(void){
      //Background Color
      glClearColor(0.8, 0.8, 0.8, 0.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      //last
      glutSwapBuffers();
  }

  //View1Display
  void display_vistPrincipal(){
    //viewport rest;
    ResetViewport();

    glMatrixMode(GL_PROJECTION);

    if(monitor.getMapa() != 0){
      //gluPerspective(45,ratio,1,6.0*monitor.getMapDepth());
      gluPerspective(45,ratio,1,1000);
      glClearColor(60.0/256.0, 154.0/256.0, 201.0/256.0, 1);
    }
    else{
      glClearColor(0.0, 0.0, 0.0, 1);
    }


    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glDisable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);


    glPushMatrix();
      if(monitor.jugar()){
        if(monitor.getMapa() != 0){
          monitor.getMapa() -> drawFirstPerson();
        }
      }
    glPopMatrix();
    glFlush();
    glutSwapBuffers();
  }


  //View4Display
  void display_vistMiniMapa(){

      //viewport rest;
      ResetViewport();


      glMatrixMode(GL_MODELVIEW);
      glClear(GL_COLOR_BUFFER_BIT);
      glLoadIdentity();

      glDisable(GL_LIGHTING);
      glColor3f(1.0, 1.0, 1.0);
      glPushMatrix();
        if(monitor.jugar() or monitor.resultadosMostrados()){
          if(MMmode == 1){
            monitor.getMapa() -> drawMM1();
          }
          else{
              monitor.getMapa() -> drawMM2();
          }

        }
      glPopMatrix();
      glFlush();
      glutSwapBuffers();


  }

  void display_vistIU(){
    ResetViewport();
    GLUI_Master.auto_set_viewport();
    std::vector<std::string> strs;
    string str;

    if(monitor.jugar() and monitor.numero_entidades() > 0){
      str = monitor.get_entidad(0) -> toString();
      boost::split(strs, str, boost::is_any_of("\n"));

      info0 -> set_text(strs[0].c_str());
      info1 -> set_text(strs[1].c_str());
      info2 -> set_text(strs[2].c_str());
      info3 -> set_text(strs[3].c_str());
      info4 -> set_text(strs[4].c_str());
      info5 -> set_text(strs[5].c_str());


      if(strs.size() > 7){
        info6 -> set_text(strs[6].c_str());
        info7 -> set_text(strs[7].c_str());
        info8 -> set_text(strs[8].c_str());
        info9 -> set_text(strs[9].c_str());
        info10-> set_text(strs[10].c_str());

      }



    }

    glutSwapBuffers();
  }

  void update(int valor){
    glutSetWindow(vistaPrincipal);
    glutPostRedisplay();

    glutSetWindow(vistaMiniMapa);
    glutPostRedisplay();

    glutSetWindow(vistaIU);
    glutPostRedisplay();

    glutTimerFunc(1,irAlJuego,0);
  }

  void irAlJuego(int valor){
    lanzar_motor_juego();
    glutTimerFunc(1,update,0);
  }

  void botonCancelarNuevoMapaCB(int valor){
    panelSelecMapa -> close();
    botonElegirMapa -> enable();
    //botonJuegoRemoto -> enable();
    botonPaso -> enable();
    botonEjecucion -> enable();
    botonEjecutar -> enable();
    botonSalir -> enable();
  }

  void botonAceptarNuevoMapaCB(int valor){
    panelSelecMapa -> close();
    botonElegirMapa -> enable();
    //botonJuegoRemoto -> enable();
    botonPaso -> enable();
    botonEjecucion -> enable();
    botonEjecutar -> enable();
    botonSalir -> enable();
    if(strcmp(map_fb -> get_file(), "") != 0){
      char path[255];
      strcpy(path, "./mapas/");
      const char * file = map_fb -> get_file();
      monitor.setMapa(strcat(path,file));
      monitor.startGame();
      if(monitor.inicializarJuego()){
        monitor.inicializar();
        monitor.juegoInicializado();
        monitor.setResultadosMostrados(false);
      }
    }
  }

  void botonElegirMapaCB(int valor){
	  panelSelecMapa = GLUI_Master.create_glui("Nuevo Juego");

    botonElegirMapa -> disable();
    //botonJuegoRemoto -> disable();
    botonPaso -> disable();
    botonEjecucion -> disable();
    botonEjecutar -> disable();
    botonSalir -> disable();


    map_fb = new GLUI_FileBrowser(panelSelecMapa,"Mapas",GLUI_PANEL_EMBOSSED, 0, botonAceptarNuevoMapaCB);
    map_fb -> set_allow_change_dir(0);
    map_fb -> fbreaddir("mapas/");

    botonCancelarNuevoMapa=new GLUI_Button(panelSelecMapa,"Cancelar",0,botonCancelarNuevoMapaCB);

  }

  void botonJuegoRemotoCB(int valor){
  }

  void botonPasoCB(int valor){
    monitor.setPasos(1);
  }

  void botonEjecucionCB(int valor){
    monitor.setPasos(-1);
  }

  void botonEjecutarCB(int valor){
    monitor.setPasos(nPasos);
  }

  void setRetardo(int valor){
    monitor.setRetardo(tRetardo);
  }

  void botonSalirCB(int valor){
    monitor.cerrarBelkan();
    exit(0);
  }

void lanzar_motor_grafico(int argc, char **argv){
    glutInit(&argc, argv);
    //Mode Setting
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    //window size (+gap size)
    glutInitWindowSize(IU_ancho_ventana, IU_alto_ventana);
    ratio = ((float) IU_ancho_ventana * 0.7) / (float) IU_alto_ventana;
    //Initial position
    glutInitWindowPosition(0, 0);

    //Main Window
    ventanaPrincipal = glutCreateWindow("UI Test");
    //Main Window callback function
    glutReshapeFunc(reshape);
    glutDisplayFunc(display_ventPrincipal);

    //World Window and Display
    vistaPrincipal = glutCreateSubWindow(ventanaPrincipal, 0, 0, IU_ancho_ventana*0.7, IU_alto_ventana);
    glutDisplayFunc(display_vistMiniMapa);
    //glutDisplayFunc(display_vistPrincipal);


    //screen Window and Display
    vistaMiniMapa = glutCreateSubWindow(ventanaPrincipal, IU_ancho_ventana*0.7, 0, (IU_ancho_ventana*0.3) + 1, IU_alto_ventana*0.3);

    //glutDisplayFunc(display_vistMiniMapa);
    glutDisplayFunc(display_vistPrincipal);

    vistaIU = glutCreateSubWindow(ventanaPrincipal, IU_ancho_ventana*0.7, IU_alto_ventana*0.3, (IU_ancho_ventana*0.3) + 1, (IU_alto_ventana*0.7) + 1);
    glutDisplayFunc(display_vistIU);

    glutInitWindowPosition(IU_ancho_ventana*0.7,50);
    glutInitWindowSize(50, 50);


    panelIU = GLUI_Master.create_glui_subwindow(vistaIU, GLUI_SUBWINDOW_TOP);
    panelIU -> set_main_gfx_window(vistaIU);

    botonElegirMapa = panelIU -> add_button("Elegir Mapa", 0, botonElegirMapaCB);
    botonElegirMapa -> set_alignment(GLUI_ALIGN_CENTER);
    /*botonJuegoRemoto = panelIU -> add_button("juego Remoto", 0, botonJuegoRemotoCB);
    botonJuegoRemoto -> set_alignment(GLUI_ALIGN_CENTER); */

    drawMM =   panelIU -> add_checkbox( "Mostrar mapa", &MMmode);
    drawMM -> set_alignment(GLUI_ALIGN_CENTER);

    lineaVacia1 = panelIU -> add_statictext("");

    botonPaso = panelIU -> add_button("Paso", 0, botonPasoCB);
    botonPaso -> set_alignment(GLUI_ALIGN_CENTER);
    botonEjecucion = panelIU -> add_button("Ejecucion", 0, botonEjecucionCB);
    botonEjecucion -> set_alignment(GLUI_ALIGN_CENTER);

    lineaVacia2 = panelIU -> add_statictext("");

    editTextPasos = panelIU -> add_edittext("Pasos", GLUI_EDITTEXT_INT, &nPasos);
    editTextPasos -> set_int_val(nPasos);
    editTextPasos -> set_int_limits(1, 10000000, GLUI_LIMIT_CLAMP);
    editTextPasos -> set_alignment(GLUI_ALIGN_CENTER);

    editTextRetardo = panelIU -> add_edittext("Retardo", GLUI_EDITTEXT_INT, &tRetardo, -1, setRetardo);
    editTextRetardo -> set_int_val(tRetardo);
    editTextRetardo -> set_int_limits(1, 10000000, GLUI_LIMIT_CLAMP);
    editTextRetardo -> set_alignment(GLUI_ALIGN_CENTER);

    lineaVacia3 = panelIU -> add_statictext("");

    botonEjecutar = panelIU -> add_button("Ejecutar", 0, botonEjecutarCB);
    botonEjecutar -> set_alignment(GLUI_ALIGN_CENTER);

    lineaVacia4 = panelIU -> add_statictext("");

    botonSalir = panelIU -> add_button("Salir", 0, botonSalirCB);
    botonSalir -> set_alignment(GLUI_ALIGN_CENTER);

    panelInfo = panelIU -> add_panel("", GLUI_PANEL_NONE);
    panelInfo-> set_alignment(GLUI_ALIGN_CENTER);

    panelIU -> add_separator_to_panel(panelInfo);

    info0 = panelIU -> add_statictext_to_panel(panelInfo, "Informacion");
    info1 = panelIU -> add_statictext_to_panel(panelInfo, "del juego");
    info2 = panelIU -> add_statictext_to_panel(panelInfo, "");
    info3 = panelIU -> add_statictext_to_panel(panelInfo, "");
    info4 = panelIU -> add_statictext_to_panel(panelInfo, "");
    info5 = panelIU -> add_statictext_to_panel(panelInfo, "");
    info6 = panelIU -> add_statictext_to_panel(panelInfo, "");
    info7 = panelIU -> add_statictext_to_panel(panelInfo, "");
    info8 = panelIU -> add_statictext_to_panel(panelInfo, "");
    info9 = panelIU -> add_statictext_to_panel(panelInfo, "");
    info10 = panelIU -> add_statictext_to_panel(panelInfo, "");

    panelIU -> add_separator_to_panel(panelInfo);

    rellenarPanel = panelIU -> add_statictext("");

    glutTimerFunc(100,update,0);



    glutMainLoop();
}
