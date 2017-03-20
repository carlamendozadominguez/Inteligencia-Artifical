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

       x=99;
       y=99;

      std::vector< unsigned char> aux(mapaResultado.size(), '?');
      
      for(unsigned int i = 0; i<mapaResultado.size(); i++)
          mapaDescubierto.push_back(aux);

       
       std::vector< unsigned char> aux1(200, '?');
      
      for(unsigned int i = 0; i<200; i++)
          mapaPrueba.push_back(aux1);


        std::vector<int> aux2(mapaResultado.size(),0);
      
     /for(unsigned int i = 0; i<mapaResultado.size(); i++)
          mapaPulgarcito.push_back(aux2);





}
    

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);

    int interact(Action accion, int valor);

    int Objeto_mochila(unsigned char objeto);

    void RellenaMapa(Sensores sensores,int brujula,bool situado);

    void DecisionObjeto(unsigned char obstaculo);




    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}


  private:

  	int fil,col,brujula;  //donde estoy? y hacia donde voy?
  	bool bien_situado;
  	Action ultimaAccion;
  	bool girar_derecha;
    std::vector< std::vector< unsigned char> > mapaDescubierto;
        std::vector< std::vector< unsigned char> > mapaPrueba;

    int x,y;

};


#endif
