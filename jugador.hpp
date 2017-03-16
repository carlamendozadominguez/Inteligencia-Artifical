#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include <vector>


#include "comportamientos/comportamiento.hpp"
using namespace std;

class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
   

    	fil=99;
    	col=99;
    	brujula=0; //  0->Norte, 1-> Este , 2->Sur , 3->Oeste
    	ultimaAccion=actIDLE;
    	girar_derecha=false;
    	bien_situado=false;


      std::vector< unsigned char> aux(size, '?');
      
      for(unsigned int i = 0; i<size; i++)
          mapaDescubierto.push_back(aux);


    

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);

    int interact(Action accion, int valor);

    int objeto_mochila(Sensores sensores,unsigned char objeto);

    void RellenaMapa(int brujula);


    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}


  private:

  	int fil,col,brujula;  //donde estoy? y hacia donde voy?
  	bool bien_situado;
  	Action ultimaAccion;
  	bool girar_derecha;
    std::vector< std::vector< unsigned char> > mapaDescubierto;


};


#endif
