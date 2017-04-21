#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include "stdlib.h"
#include <stdlib.h>     /* abs */
#include <limits>
using namespace std;


/*************************************************************************************************************
 * Think --> método para controlar la lógica del agente reactivo, nos devolvera que accion llevaremos a cabo *
 *																											 *
 *************************************************************************************************************/

Action ComportamientoJugador::think(Sensores sensores){

	Action accion = actIDLE;

		
//Cuando morimos se resetean algunas variables
	if(sensores.reset){
		
		brujula=0; //  0->Norte, 1-> Este , 2->Sur , 3->Oeste
    	ultimaAccion=actIDLE;
    	girar_derecha=false;
    	bien_situado=false;
    	fil=99;
    	col=99;
    	x=99;
    	y = 99;


	
	//Inicializamos la maleta vacia
		for(int i=0;i<maleta.size();i++)
			maleta.pop_back();


	// inicialiar la matrix mapaPulgarcitoMax a cero, mapaPrueba a ?, y mapaObjetoCiego a _
		for(int i = 0 ; i < mapaPrueba.size() ; i++){
    		for(int j = 0 ; j < mapaPrueba.at(i).size() ;j++){
    			mapaPrueba[i][j]='?';
    			mapaPulgarcitoMax[i][j]=0;
    			mapaObjetoCiego[i][j]='_';
    			}
			}

} //Fin de resetar valores de muerte


// Nos servira para movernos por los mapas
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





	//Si no estamos bien situados, es decir, no hemos encontrado un punto PK,solo se rellena el mapaPrueba
	if(!bien_situado){  
		RellenaMapa(sensores,brujula,false);
	}


	//Si estamos bien situados se guardara el mapaPRueba en mapaResultado, el mapa PulgarcitoMax en PulgarcitoMin y	 el mapaObjetosCiego en mapaObjetos
	else {  
		RellenaMapa(sensores, brujula,true);
			for (int i = 0; i < mapaPrueba.size(); ++i){
				for(int j = 0; j < mapaPrueba.at(i).size(); ++j){
					if(mapaPrueba[i][j]!='?') {
						mapaResultado[i-(x-fil)][j-(y-col)]=mapaPrueba[i][j];
						mapaPulgarcitoMin.at(i-(x-fil)).at(j-(y-col))+=mapaPulgarcitoMax.at(i).at(j);
					}

					if(mapaObjetoCiego[i][j]!='_'){
						mapaObjetos[i-(x-fil)][j-(y-col)]=mapaObjetoCiego[i][j];

					}
				}
			}	
	}

	//Hemos llegado a un punto PK, y guardamos la fila y col
	if((sensores.terreno[0]=='K') && (!bien_situado)){
		fil=sensores.mensajeF;
		col=sensores.mensajeC;
		bien_situado=true;
	}



	//Cuando cogemos un objeto lo metemos en la mochila, siempre y cuando no este ya metido

	if(ultimaAccion==actPICKUP){
		if(sensores.objetoActivo=='1' || sensores.objetoActivo=='2' || sensores.objetoActivo=='3' || sensores.objetoActivo=='4'){
			cout<<"pickup"<<endl;
			if(!esta_en_mochila(sensores.objetoActivo)){
				maleta.push_back(sensores.objetoActivo);
				accion=actPUSH;
			}

		}
	}


	//Cogemos el objeto que nos hemos encontrado, si no esta en la mochila y si no estamos en un terreno que no nos permita dejar el objeto en uso
	else if((sensores.superficie[2]=='1' || sensores.superficie[2]=='2' || sensores.superficie[2]=='3' || sensores.superficie[2]=='4') && !esta_en_mochila(sensores.superficie[2]) && (sensores.terreno[0]!='B' && sensores.terreno[0]!='A')){
		accion=actPICKUP;
	}



	//Si nos encontramos con un bosque entonces:
	else if(sensores.terreno[2]=='B' && (esta_en_mochila('2') || sensores.objetoActivo=='2') && sensores.terreno[0]!='A'){

			if(sensores.objetoActivo=='_'){
				maleta.pop_front();
				accion=actPOP;

			}

			else if(sensores.objetoActivo!='2'){

				maleta.push_back(sensores.objetoActivo);
				accion=actPUSH;
			}

			else if(sensores.objetoActivo=='2'){
				accion=actFORWARD;
			}


			
			}

		//Si nos encontramos con agua, entonces:
		else if(sensores.terreno[2]=='A' && (esta_en_mochila('1') || sensores.objetoActivo=='1') && sensores.terreno[0]!='B' ){
		
			if(sensores.objetoActivo=='_'){
				maleta.pop_front();
				accion=actPOP;

			 }

			 else if(sensores.objetoActivo!='1'){

				maleta.push_back(sensores.objetoActivo);
			 	accion=actPUSH;
		 	}

		 	else if(sensores.objetoActivo=='1'){
			 	accion=actFORWARD;
		 	}


		
		}


		//Si estamos ante una puerta:
		else if(sensores.terreno[2]=='D' && (esta_en_mochila('3') || sensores.objetoActivo=='3' )){

			if(sensores.objetoActivo=='_'){
				maleta.pop_front();
				accion=actPOP;

			 }

			 else if(sensores.objetoActivo!='3'){

				 maleta.push_back(sensores.objetoActivo);
				 accion=actPUSH;
			 }

			 else if(sensores.objetoActivo=='3'){
				 accion=actGIVE;
			 }

		
		}

		//Si nos encontramos por un rey:
		else if(sensores.superficie[2]=='r' && (esta_en_mochila('4') || sensores.objetoActivo=='4' )){

			if(sensores.objetoActivo=='_'){
				maleta.pop_front();
				accion=actPOP;

			 }

			 else if(sensores.objetoActivo!='4'){

				 maleta.push_back(sensores.objetoActivo);
				 accion=actPUSH;
			 }

			 else if(sensores.objetoActivo=='4'){
				 accion=actGIVE;
			 }

		

		
		}


	//Si estamos por un espacio de terreno arenoso o pedregoso, o no tenemos los objetos necesarios, nos moveremos con Pulgarcito		
		else if(bien_situado ) {

			accion = dondeVasPulgarcito(fil,col,mapaPulgarcitoMin,sensores);

		}

		else{ // no hay más ciego que el que no quiere ver 
			accion=dondeVasPulgarcito(x,y,mapaPulgarcitoMax,sensores);
		}


	ultimaAccion=accion;

	return accion;
}



/*************************************************************************************************************
 * esta_en_mochila --> método para controlar lo que hay en la mochila. Le pasamos un objeto y nos devolvera  *
 * 						true si este ya se encuentra dentro. 												 *
 *																											 *
 *************************************************************************************************************/

bool ComportamientoJugador::esta_en_mochila(char x){

	list<char> aux;
	aux=maleta;
	bool esta=false;

	for(int i=0; i<maleta.size();i++){
		
		 if(aux.front()==x){
			return true;
		}

		else{
			aux.pop_front();
		}
	}

	return false;
}


int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}

/*************************************************************************************************************
 * mochila_llena--> método para controlar si la mochila esta al tope de su capacidad (4 objetos)			 *
 *																											 *
 *************************************************************************************************************/

bool ComportamientoJugador::mochila_llena(){

	if (maleta.size()==4)
		return true;

	else
		return false;
}


/************************************************************************************************************
 * RellenaMapa --> Segun vayamos descubriendo mundo lo guardaremos en una matriz,dependiendo de si estamos	*
 *				   bien situados o no.Se contempla la vision que tiene nuestro jugador y se ira guardando  *
 *				   lo que observe en esas 15 casillas segun hacia donde este mirando.						*
 *																											*
 ************************************************************************************************************/
void ComportamientoJugador::RellenaMapa(Sensores sensores,int brujula,bool bien_situado){

int k=0;
	
		
			if(bien_situado){
			mapaPulgarcitoMin[fil][col]=cont;
			cont++;
		} else {
			mapaPulgarcitoMax[x][y]=cont;
			cont++;
		}

	//}


	 if(brujula==0){   //Si nuestro jugador mira al NORTE
	
		for(int i=0;i>-4;i--){
			for(int j=i;j<=abs(i);j++){
				mapaPrueba[x+i][y+j]=sensores.terreno[k];
				
				if(sensores.superficie[k]!='l')
					mapaObjetoCiego[x+i][y+j]=sensores.superficie[k];

				k++;
				}
			}
	}

	if(brujula == 1){  // jugador mira al este  
		for(int i=0;i<=3;i++){
			for(int j=-i;j<=i;j++){
				mapaPrueba[x+j][y+i]= sensores.terreno[k];

				if(sensores.superficie[k]!='l')
					mapaObjetoCiego[x+j][y+i]=sensores.superficie[k];

				k++;
			}	
		}
	}
	else if(brujula==2){  //Si nuestro jugador mira al SUR
				
		for(int i=0;i<=3;i++){
			for(int j=i;abs(j)<=i;j--){
				mapaPrueba[x+i][y+j]=sensores.terreno[k];

				if(sensores.superficie[k]!='l')
					mapaObjetoCiego[x+i][y+j]=sensores.superficie[k];
				k++;
			}
		}
	}
	else if(brujula==3){  // jugador mira al oeste     
		for(int i=0;i<=3;i++){
			for(int j=i;abs(j)<=i;j--){
				mapaPrueba[x+j][y-i]=sensores.terreno[k];

				if(sensores.superficie[k]!='l')
					mapaObjetoCiego[x+j][y-i]=sensores.superficie[k];

				k++;
			}
		}	
	}		
}

/*************************************************************************************************************
 * NoPuedesPasar --> Devuelve true si no podemos pasar po la posicion (fila,col)							 *
 *																											 *
 *************************************************************************************************************/

bool ComportamientoJugador::NoPuedesPasar(int fila, int col, Sensores sensor){
	
	bool no_puedes_pasar = false; // por defecto puedes pasar 

	//Si sabemos por donde estamos caminando
	if(bien_situado){
		cout<<endl<<mapaObjetos[fila][col]<<endl;
		if (mapaObjetos[fila][col]=='0' ||mapaObjetos[fila][col]=='4'||mapaObjetos[fila][col]=='3' || mapaObjetos[fila][col]=='a' || mapaObjetos[fila][col]=='r' &&  sensor.objetoActivo!='4' || esta_en_mochila(mapaObjetos[fila][col]) || mochila_llena()||
		 mapaResultado[fila][col] == 'B' && sensor.objetoActivo!='2' || mapaResultado[fila][col] == 'M' || mapaResultado[fila][col] == 'A' && sensor.objetoActivo!='1'|| mapaResultado[fila][col]=='P'  
		 || mapaResultado[fila][col]=='D' && sensor.objetoActivo!='3'){
			no_puedes_pasar = true; 
		}
		

	}

	//No hay mas ciego que el que no quiere ver
	else{ 
		if ( mapaObjetoCiego[fila][col]=='0' || mapaObjetoCiego[fila][col]=='4'|| mapaObjetoCiego[fila][col]=='3' || mapaObjetoCiego[fila][col]=='a' || mapaObjetoCiego[fila][col]=='r' &&  sensor.objetoActivo!='4' || esta_en_mochila(mapaObjetoCiego[fila][col]) || mochila_llena() ||
			mapaPrueba[fila][col] == 'B' && sensor.objetoActivo!='2' || mapaPrueba[fila][col] == 'M' || mapaPrueba[fila][col] == 'A' && sensor.objetoActivo!='1'|| mapaPrueba[fila][col]=='P'|| mapaPrueba[fila][col]=='D' && sensor.objetoActivo!='3'){
			no_puedes_pasar = true; 
		}
	}

	return no_puedes_pasar; 
}


// controla el comportamiento del método pulgarcito 
Action ComportamientoJugador::dondeVasPulgarcito(int fila, int columna, std::vector< std::vector< int> > mapaPulgarcito, Sensores sensor){	


 cout<<"DELANTE:"<<tengoDelante(fila,columna,mapaPulgarcito, sensor)<<endl;
 cout<<"DERECHA:"<<tengoDerecha(fila,columna,mapaPulgarcito, sensor)<<endl;
 cout<<"IZQUIERDA:"<<tengoIzquierda(fila,columna,mapaPulgarcito, sensor)<<endl;

	Action siguiente_accion; 	// accion a devolver 
	if ( (tengoDelante(fila,columna,mapaPulgarcito, sensor) < tengoDerecha(fila,columna,mapaPulgarcito, sensor)) && 
		 (tengoDelante(fila,columna,mapaPulgarcito, sensor) < tengoIzquierda(fila,columna,mapaPulgarcito, sensor)))
	{
		// la posición que tengo delante tiene un valor más bajo 
		siguiente_accion = actFORWARD; 	
	}else if ( (tengoDerecha(fila,columna,mapaPulgarcito, sensor) < tengoDelante(fila,columna,mapaPulgarcito, sensor)) and (tengoDerecha(fila,columna,mapaPulgarcito, sensor) < tengoIzquierda(fila,columna,mapaPulgarcito, sensor) ) ){
			siguiente_accion = actTURN_R;

	}else if ((tengoIzquierda(fila,columna,mapaPulgarcito, sensor) < tengoDerecha(fila,columna,mapaPulgarcito, sensor)) and (tengoIzquierda(fila,columna,mapaPulgarcito, sensor) < tengoDelante(fila,columna,mapaPulgarcito, sensor) ) ){
			siguiente_accion = actTURN_L;

	}else if (tengoDelante(fila,columna,mapaPulgarcito, sensor)== 0 ) {
		siguiente_accion = actFORWARD;

	}else if(tengoIzquierda(fila,columna,mapaPulgarcito, sensor)== 0){
		siguiente_accion = actTURN_L;
	}else if(tengoDerecha(fila,columna,mapaPulgarcito, sensor)== 0){
		siguiente_accion = actTURN_R;
	}else{
			siguiente_accion = actTURN_R ;
	}
	return siguiente_accion;
}



// ============================================================================================

// Funciones auxiliares para el comportameinto de pulgarcito
// Las siguientes funciones devuelve un número muy grande si no se puede pasar por la posición 
// deseada (hacia delante, derecha, izquierda) y devolverán la siguiente posisión a recorrer si 
// se puede pasar por ella. 
// Para estas comprobaciones se hace uso de la función NoPuedesPasar(int,int)
int ComportamientoJugador::tengoDelante(int fila, int columna, std::vector< std::vector< int> > mapaPulgarcito ,Sensores sensor)
{
	int max = numeric_limits<int>::max(); 	// ... el numero muy grande
	switch(brujula){
		case 0: 
			if(NoPuedesPasar(fila-1,columna, sensor)) return max; 
			else return mapaPulgarcito[fila-1][columna]; 
			break; 
		case 1: 
			if(NoPuedesPasar(fila,columna+1, sensor)) return max; 
			else return mapaPulgarcito[fila][columna+1];
			break;
		case 2: 
			if(NoPuedesPasar(fila+1,columna, sensor)) return max; 
			else return mapaPulgarcito[fila+1][columna];
			break;
		case 3: 
			if(NoPuedesPasar(fila,columna-1, sensor)) return max; 
			else return mapaPulgarcito[fila][columna-1];
			break;
	}
}

int ComportamientoJugador::tengoIzquierda(int fila, int columna, std::vector< std::vector< int> > mapaPulgarcito, Sensores sensor)
{
	int max = numeric_limits<int>::max(); 	// ... el numero muy grande
	switch(brujula){
		case 0: 
			if(NoPuedesPasar(fila,columna-1, sensor)) return max; 
			else return mapaPulgarcito[fila][columna-1]; 
			break; 
		case 1: 
			if(NoPuedesPasar(fila-1,columna, sensor)) return max; 
			else return mapaPulgarcito[fila-1][columna];
			break;
		case 2: 
			if(NoPuedesPasar(fila,columna+1, sensor)) return max; 
			else return mapaPulgarcito[fila][columna+1];
			break;
		case 3: 
			if(NoPuedesPasar(fila+1,columna, sensor)) return max; 
			else return mapaPulgarcito[fila+1][columna];
			break;
	}
}



int ComportamientoJugador::tengoDerecha(int fila, int columna, std::vector< std::vector< int> > mapaPulgarcito, Sensores sensor)
{
	int max = numeric_limits<int>::max(); 	// ... el numero muy grande
	switch(brujula){
		case 0: 
			if(NoPuedesPasar(fila,columna+1, sensor)) return max; 
			else return mapaPulgarcito[fila][columna+1]; 
			break; 
		case 1: 
			if(NoPuedesPasar(fila+1,columna, sensor)) return max; 
			else return mapaPulgarcito[fila+1][columna];
			break;
		case 2: 
			if(NoPuedesPasar(fila,columna-1, sensor)) return max; 
			else return mapaPulgarcito[fila][columna-1];
			break;
		case 3: 
			if(NoPuedesPasar(fila-1,columna, sensor)) return max; 
			else return mapaPulgarcito[fila-1][columna];
			break;
	}
}

// ==================== Fin funciones auxiliares para pulgarcito ========================================


