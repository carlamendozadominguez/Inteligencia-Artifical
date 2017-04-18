#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include <vector>
#include <list>

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
       cont=1;


       
       std::vector< unsigned char> aux1(200, '?');
      
      for(unsigned int i = 0; i<200; i++)
          mapaPrueba.push_back(aux1);


        std::vector<int> aux2(mapaPrueba.size(),0);
      
     for(unsigned int i = 0; i<mapaPrueba.size(); i++)
          mapaPulgarcitoMax.push_back(aux2);

        std::vector<int> aux3(mapaResultado.size(),0);
      
     for(unsigned int i = 0; i<mapaResultado.size(); i++)
          mapaPulgarcitoMin.push_back(aux3);
        
}
    

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);

    bool esta_en_mochila(char);

    int interact(Action accion, int valor);

    // rellena el mapa 
    void RellenaMapa(Sensores sensores,int brujula,bool bien_situado);


    // define la accion a realizar siguiento el metodo Pulgarcito 
    Action dondeVasPulgarcito(int, int, std::vector< std::vector< int> >,Sensores sensores ); 

    // devuelve true si no podemos pasar por una posicion dada
    bool NoPuedesPasar(int,int,Sensores sensores); 


    // devuelven los valroes para orientar a Pulgarcito 
    int tengoDelante(int,int, std::vector< std::vector< int> >,Sensores sensores); 
    int tengoDerecha(int,int, std::vector< std::vector< int> >,Sensores sensores); 
    int tengoIzquierda(int,int, std::vector< std::vector< int> >,Sensores sensores); 




    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}


  private:

  	int fil,col,brujula,cont;  //donde estoy? y hacia donde voy?
  	bool bien_situado;        // he encontrado un método PK 
  	Action ultimaAccion;
  	bool girar_derecha;
    std::vector< std::vector< unsigned char> > mapaDescubierto;
    std::vector< std::vector< unsigned char> > mapaPrueba;

    std::vector< std::vector< int> > mapaPulgarcitoMax;
      // aplicación del método pulgarcito si no conocemos el punto PK aún o si no estás
      // bien situado
    std::vector< std::vector< int> > mapaPulgarcitoMin;
      // aplicación del método pulgarcito si conocemos el método PK y estamos ubicados en
      // el mapa  

    Sensores sensor;

    std::list<char> maleta;

    // controlan el movimiento cuando no conocemos la posición PK aún 
    int x, 
        y;  

};


#endif
