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

//Inicializamos a T todo el mapa resultado para que haya mayor porcentaje de coincidencia

       for(int i = 0 ; i < mapaResultado.size() ; i++){
    			for(int j = 0 ; j < mapaResultado.at(i).size() ;j++){
						mapaResultado[i][j]='T';
				}
			}

    
      
   //Mapa donde se guardara los distintos terrenos encontrados       
    std::vector< unsigned char> aux1(200, '?');
      
    for(unsigned int i = 0; i<200; i++)
       mapaPrueba.push_back(aux1);

    //Mapa pulgarcito , para cuando vamos a ciegas por el mapa
     std::vector<int> aux2(mapaPrueba.size(),0);
      
     for(unsigned int i = 0; i<mapaPrueba.size(); i++)
          mapaPulgarcitoMax.push_back(aux2);

    //Mapa pulgarcito para cuando llegamos a un punto PK y ya estamos bien ubicados
    std::vector<int> aux3(mapaResultado.size(),0);
      
     for(unsigned int i = 0; i<mapaResultado.size(); i++)
          mapaPulgarcitoMin.push_back(aux3);

    //Mapa que nos guarda los objetos encontrados para luego tratarlos como obstaculos
     std::vector< unsigned char> aux4(200, '_');
      
      for(unsigned int i = 0; i<200; i++)
          mapaObjetoCiego.push_back(aux4);

       
    //Mapa que nos guarda los objetos cuando ya estamos ubicados
      std::vector< unsigned char> aux5(mapaResultado.size(), '_');
      
      for(unsigned int i = 0; i<mapaResultado.size(); i++)
          mapaObjetos.push_back(aux5);
        
}
    

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);

    bool esta_en_mochila(char);  //Se le manda un objeto y nos dice si se encuntra ya en la mochila

    int interact(Action accion, int valor);

    // Rellena el mapa con los terrenos y objetos encontrados
    void RellenaMapa(Sensores sensores,int brujula,bool bien_situado);


    // define la accion a realizar siguiento el metodo Pulgarcito 
    Action dondeVasPulgarcito(int, int, std::vector< std::vector< int> >,Sensores sensores ); 

    // devuelve true si no podemos pasar por una posicion dada
    bool NoPuedesPasar(int,int,Sensores sensores); 


    // devuelven los valroes para orientar a Pulgarcito 
    int tengoDelante(int,int, std::vector< std::vector< int> >,Sensores sensores); 
    int tengoDerecha(int,int, std::vector< std::vector< int> >,Sensores sensores); 
    int tengoIzquierda(int,int, std::vector< std::vector< int> >,Sensores sensores); 


    bool mochila_llena(void);

    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}

    //AGENTE DELIBERATIVO
    bool hayObstaculoDelante(const vector<unsigned char> & terreno, const vector<unsigned char> & superficie, Sensores sensor);


private:

  	int fil,col,brujula,cont;  //donde estoy? y hacia donde voy?
  	bool bien_situado;        // he encontrado un método PK 
  	Action ultimaAccion;
  	bool girar_derecha;


    //Declaramos los diferentes mapas para poder movernos por el mapa

    std::vector< std::vector< unsigned char> > mapaPrueba;
    std::vector< std::vector< unsigned char> > mapaObjetos;
    std::vector< std::vector< unsigned char> > mapaObjetoCiego;
    std::vector< std::vector< int> > mapaPulgarcitoMax;
    std::vector< std::vector< int> > mapaPulgarcitoMin;

    Sensores sensor;

    std::list<char> maleta; //La maleta aunque es de tipo lista funciona como una cola FIFO

    // controlan el movimiento cuando no conocemos la posición PK aún 
    int x,y;  



    //AGENTE DELIBERATIVO
    struct estado{
      int fila;
      int columna;
      int orientacion;
    };


   bool tengo_regalo;  //Variable de estado

    // Para el plan
    bool estoy_ejecutando_plan;
    bool error_plan;
    list <Action> plan;

    bool pathFinding(const estado &origen, const estado &destino, list<Action> &plan);

    
   

};


#endif
