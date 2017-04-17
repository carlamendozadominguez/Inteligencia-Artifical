#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include "stdlib.h"
#include <stdlib.h>     /* abs */
#include <limits>
using namespace std;


// método para controlar la lógica del agente reactivo 
Action ComportamientoJugador::think(Sensores sensores){

	Action accion = actIDLE;

	
	if(sensores.reset){
		
		brujula=0; //  0->Norte, 1-> Este , 2->Sur , 3->Oeste
    	ultimaAccion=actIDLE;
    	girar_derecha=false;
    	bien_situado=false;
    	fil=99;
    	col=99;
    	x=99;
    	y = 99;


		// inicialiar la matrix mapaPulgarcitoMax a cero
		// inicializar la matrix mapaPrueba a ?
		for(int i = 0 ; i < mapaPrueba.size() ; i++){
    		for(int j = 0 ; j < mapaPrueba.at(i).size() ;j++){
    			mapaPrueba[i][j]='?';
    			mapaPulgarcitoMax[i][j]=0;
    		}
	}
}

if(!sensores.colision)
	switch(ultimaAccion){

		case actFORWARD:
			switch(brujula){

				case 0:		//NORTE
				  fil--;
				  x--;
				  ;
				 break;

				 case 1:	//ESTE
				   col++;
				   y++;
				  
				 break;

				 case 2:	//SUR
				 	fil++;
				 	x++;
				 	
				 break;

				 case 3:	//OESTE
				 	col--;
				 	y--;
				
				 break;

			}

		break;

//La brujula solo cambia cuando se realiza un giro

		case actTURN_R:
			brujula=(brujula+1)%4;

			if(rand()%2==0)
			girar_derecha=true;
		else
			girar_derecha=false;

		break;



		case actTURN_L:
			brujula=(brujula+3)%4;

			if(rand()%2==0)
			girar_derecha=true;
		else
			girar_derecha=false;

		break;
	}





	
	if(!bien_situado){  //Si no estamos bien situados, es decir, no hemos encontrado un punto PK,solo se rellena el mapaPrueba
		RellenaMapa(sensores,brujula,false);
	}

	
	else {  //Si estamos bien situados se guardara el mapaPRueba en mapaResultado
		RellenaMapa(sensores, brujula,true);
			for (int i = 0; i < mapaPrueba.size(); ++i){
				for(int j = 0; j < mapaPrueba.at(i).size(); ++j){
					//cerr << "mapaPrueba.size(): " << mapaPrueba.size() << "mapaPrueba.at(i).size(): "<< mapaPrueba.at(i).size() << endl;
					if(mapaPrueba[i][j]!='?') {
						mapaResultado[i-(x-fil)][j-(y-col)]=mapaPrueba[i][j];
						mapaPulgarcitoMin.at(i-(x-fil)).at(j-(y-col))+=mapaPulgarcitoMax.at(i).at(j);
					}
				}
			}	
}

	
	if((sensores.terreno[0]=='K') && (!bien_situado)){
		fil=sensores.mensajeF;
		col=sensores.mensajeC;
		bien_situado=true;
	}


	if(ultimaAccion==actPICKUP){
		if((sensores.objetoActivo=='2' || sensores.objetoActivo=='1') && !(sensores.superficie[2]=='0' ||sensores.superficie[2]=='1' || sensores.superficie[2]=='2' || sensores.superficie[2]=='3')){
			//accion=actTURN_R;
			if((sensores.objetoActivo=='2' && maleta.front()=='1') || (sensores.objetoActivo=='1' && maleta.front()=='2') || maleta.empty() )
				maleta.push(sensores.objetoActivo);
		}


		else{
			accion=actTHROW;
		}
	}


//CUANDO ENCONTREMOS UN OBSTACULO IREMOS A DECISIONOBJETO

	else if(sensores.superficie[2]=='0' ||sensores.superficie[2]=='1' || sensores.superficie[2]=='2' || sensores.superficie[2]=='3'){
			cout<<"OBSTACULO!!!!"<<endl;
			accion=actPICKUP;

	}

//avanzaremos si estamos en terreno arenoso o pedregoso y no hay ningun objeto 
//o personaje en esa casilla
	/*else if(sensores.terreno[2]=='B' && ultimaAccion!=actPOP && maleta.front()=='1'){
			accion=actPOP;
			maleta.pop();
		}

	else if(sensores.terreno[2]=='A' && ultimaAccion!=actPOP && maleta.front()=='2'){
			accion=actPOP;
			maleta.pop();
		}
*/

	else if(sensores.terreno[2]=='B' && maleta.front()=='2'){

		accion=actPOP;
			maleta.pop();
		}


	else if(sensores.terreno[2]=='A' && maleta.front()=='1'){

		accion=actPOP;
			maleta.pop();
		} 

	else if(sensores.terreno[2]=='B' && maleta.front()!='2' && maleta.size()>=2){

		accion=actPOP;
		maleta.pop();
	}
	//else if(sensores.terreno[2]=='B' && ultimaAccion!=actPOP && maleta.front()!='1')
	else if(ultimaAccion==actPOP)
		accion=actPUTDOWN;


	
	else if(bien_situado /*&& !sensores.colision*/) {

		accion = dondeVasPulgarcito(fil,col,mapaPulgarcitoMin,sensores,sensores.superficie[2]);

	}

	else{ // no hay más ciego que el que no quiere ver 
		accion=dondeVasPulgarcito(x,y,mapaPulgarcitoMax,sensores,sensores.superficie[2]);
	}


	ultimaAccion=accion;

	return accion;
	
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}




/************************************************************************************************************
 * RellenaMapa --> Segun vayamos descubriendo mundo lo guardaremos en una matriz mapaDescubierto, en esta   *
 * 				   funcion se contempla la vision que tiene nuestro jugador y se ira guardando lo que 		*
 *				   observe en esas 15 casillas segun hacia donde este mirando.								*
 *																											*
 ************************************************************************************************************/
void ComportamientoJugador::RellenaMapa(Sensores sensores,int brujula,bool bien_situado){


	int k=0;	// posiciones de los sensores 

	if(bien_situado){
		mapaPulgarcitoMin[fil][col]=cont;
		cont++;
	} else {
		mapaPulgarcitoMax[x][y]=cont;
		cont++;
	}


	 if(brujula==0){   //Si nuestro jugador mira al NORTE
	
		for(int i=0;i>-4;i--){
			for(int j=i;j<=abs(i);j++){
				mapaPrueba[x+i][y+j]=sensores.terreno[k];
					k++;
				}
			}
	}

	if(brujula == 1){  // jugador mira al este  
		for(int i=0;i<=3;i++){
			for(int j=-i;j<=i;j++){
				mapaPrueba[x+j][y+i]= sensores.terreno[k];
				k++;
			}	
		}
	}
	else if(brujula==2){  //Si nuestro jugador mira al SUR
				
		for(int i=0;i<=3;i++){
			for(int j=i;abs(j)<=i;j--){
				mapaPrueba[x+i][y+j]=sensores.terreno[k];
				k++;
			}
		}
	}
	else if(brujula==3){  // jugador mira al oeste     
		for(int i=0;i<=3;i++){
			for(int j=i;abs(j)<=i;j--){
				mapaPrueba[x+j][y-i]=sensores.terreno[k];

				k++;
			}
		}	
	}		
}



// Devuelve true si no podemos pasar po la posicion (fila,col)
bool ComportamientoJugador::NoPuedesPasar(int fila, int col, Sensores sensor,char personaje)
{
	bool no_puedes_pasar = false; // por defecto puedes pasar 

	cout<<personaje<<endl;
	
	if(bien_situado){
		if (personaje == 'l' || personaje == 'a' || mapaResultado[fila][col] == 'B' && sensor.objetoActivo!='2' || mapaResultado[fila][col] == 'M' || mapaResultado[fila][col] == 'A' && sensor.objetoActivo!='1' || mapaResultado[fila][col]=='P' || mapaResultado[fila][col]=='D' ){
			no_puedes_pasar = true; 
		}

	}
	else{ // no está bien situado 
		if (personaje == 'l' ||personaje == 'a' || mapaPrueba[fila][col] == 'B' && sensor.objetoActivo!='2' || mapaPrueba[fila][col] == 'M' || mapaPrueba[fila][col] == 'A' && sensor.objetoActivo!='1'|| mapaPrueba[fila][col]=='P' || mapaPrueba[fila][col]=='D' ){
			no_puedes_pasar = true; 
		}

	}

cout<<"no puedes pasar"<<no_puedes_pasar<<endl;
	return no_puedes_pasar; 
}


// controla el comportamiento del método pulgarcito 
Action ComportamientoJugador::dondeVasPulgarcito(int fila, int columna, std::vector< std::vector< int> > mapaPulgarcito, Sensores sensor,char personaje){	


	Action siguiente_accion; 	// accion a devolver 
	if ( (tengoDelante(fila,columna,mapaPulgarcito, sensor,personaje) < tengoDerecha(fila,columna,mapaPulgarcito, sensor,personaje)) && 
		 (tengoDelante(fila,columna,mapaPulgarcito, sensor,personaje) < tengoIzquierda(fila,columna,mapaPulgarcito, sensor,personaje)))
	{
		// la posición que tengo delante tiene un valor más bajo 
		siguiente_accion = actFORWARD; 	
	}else if ( (tengoDerecha(fila,columna,mapaPulgarcito, sensor,personaje) < tengoDelante(fila,columna,mapaPulgarcito, sensor,personaje)) and (tengoDerecha(fila,columna,mapaPulgarcito, sensor,personaje) < tengoIzquierda(fila,columna,mapaPulgarcito, sensor,personaje) ) ){
			siguiente_accion = actTURN_R;
			cout<<"voy derecha"<<endl;

	}else if ((tengoIzquierda(fila,columna,mapaPulgarcito, sensor,personaje) < tengoDerecha(fila,columna,mapaPulgarcito, sensor,personaje)) and (tengoIzquierda(fila,columna,mapaPulgarcito, sensor,personaje) < tengoDelante(fila,columna,mapaPulgarcito, sensor,personaje) ) ){
			siguiente_accion = actTURN_L;
			cout<<"voy izquierdax"<<endl;

	}else if (tengoDelante(fila,columna,mapaPulgarcito, sensor,personaje)== 0 ) {
		siguiente_accion = actFORWARD;
		cout<<"voy recto"<<endl;

	}else if(tengoIzquierda(fila,columna,mapaPulgarcito, sensor,personaje)== 0){
		siguiente_accion = actTURN_L;
		cout<<"voy izquierday"<<endl;
	}else if(tengoDerecha(fila,columna,mapaPulgarcito, sensor,personaje)== 0){
		siguiente_accion = actTURN_R;
		cout<<"voy derecha"<<endl;
	}else{
			siguiente_accion = actTURN_R ;
			cout<<"voy derecha"<<endl;
	}
	return siguiente_accion;
}



// ============================================================================================

// Funciones auxiliares para el comportameinto de pulgarcito
// Las siguientes funciones devuelve un número muy grande si no se puede pasar por la posición 
// deseada (hacia delante, derecha, izquierda) y devolverán la siguiente posisión a recorrer si 
// se puede pasar por ella. 
// Para estas comprobaciones se hace uso de la función NoPuedesPasar(int,int)
int ComportamientoJugador::tengoDelante(int fila, int columna, std::vector< std::vector< int> > mapaPulgarcito ,Sensores sensor,char personaje)
{
	int max = numeric_limits<int>::max(); 	// ... el numero muy grande
	switch(brujula){
		case 0: 
			if(NoPuedesPasar(fila-1,columna, sensor,personaje)) return max; 
			else return mapaPulgarcito[fila-1][columna]; 
			break; 
		case 1: 
			if(NoPuedesPasar(fila,columna+1, sensor,personaje)) return max; 
			else return mapaPulgarcito[fila][columna+1];
			break;
		case 2: 
			if(NoPuedesPasar(fila+1,columna, sensor,personaje)) return max; 
			else return mapaPulgarcito[fila+1][columna];
			break;
		case 3: 
			if(NoPuedesPasar(fila,columna-1, sensor,personaje)) return max; 
			else return mapaPulgarcito[fila][columna-1];
			break;
	}
}

int ComportamientoJugador::tengoIzquierda(int fila, int columna, std::vector< std::vector< int> > mapaPulgarcito, Sensores sensor,char personaje)
{
	int max = numeric_limits<int>::max(); 	// ... el numero muy grande
	switch(brujula){
		case 0: 
			if(NoPuedesPasar(fila,columna-1, sensor,personaje)) return max; 
			else return mapaPulgarcito[fila][columna-1]; 
			break; 
		case 1: 
			if(NoPuedesPasar(fila-1,columna, sensor,personaje)) return max; 
			else return mapaPulgarcito[fila-1][columna];
			break;
		case 2: 
			if(NoPuedesPasar(fila,columna+1, sensor,personaje)) return max; 
			else return mapaPulgarcito[fila][columna+1];
			break;
		case 3: 
			if(NoPuedesPasar(fila+1,columna, sensor,personaje)) return max; 
			else return mapaPulgarcito[fila+1][columna];
			break;
	}
}



int ComportamientoJugador::tengoDerecha(int fila, int columna, std::vector< std::vector< int> > mapaPulgarcito, Sensores sensor,char personaje)
{
	int max = numeric_limits<int>::max(); 	// ... el numero muy grande
	switch(brujula){
		case 0: 
			if(NoPuedesPasar(fila,columna+1, sensor,personaje)) return max; 
			else return mapaPulgarcito[fila][columna+1]; 
			break; 
		case 1: 
			if(NoPuedesPasar(fila+1,columna, sensor,personaje)) return max; 
			else return mapaPulgarcito[fila+1][columna];
			break;
		case 2: 
			if(NoPuedesPasar(fila,columna-1, sensor,personaje)) return max; 
			else return mapaPulgarcito[fila][columna-1];
			break;
		case 3: 
			if(NoPuedesPasar(fila-1,columna, sensor,personaje)) return max; 
			else return mapaPulgarcito[fila-1][columna];
			break;
	}
}

// ==================== Fin funciones auxiliares para pulgarcito ========================================
