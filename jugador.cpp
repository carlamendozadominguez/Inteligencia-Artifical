#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include "stdlib.h"
#include <stdlib.h>     /* abs */
#include <limits>
using namespace std;



Action ComportamientoJugador::think(Sensores sensores){

	Action accion = actIDLE;

	sensor=sensores;


	/*cout << "Superficie: ";
	for (int i=0; i<sensores.superficie.size(); i++)
		cout << sensores.superficie[i];
	cout << endl;
*/
	if(sensores.reset){
		
		brujula=0; //  0->Norte, 1-> Este , 2->Sur , 3->Oeste
    	ultimaAccion=actIDLE;
    	girar_derecha=false;
    	bien_situado=false;
    	fil=99;
    	col=99;

		/*for(std::vector<unsigned char> v: mapaResultado){
			for (unsigned char c : v)
			{
				cout << c;
			}
		}*/


		for(int i = 0 ; i < mapaPrueba.size() ; i++){
    		for(int j = 0 ; j < mapaPrueba.at(i).size() ;j++){
    			mapaPrueba[i][j]='?';
    		}
	}
}

		


	// En esta matriz de tamano 100x100 hay que escribir el mapa solucion
	// mapaResultado[fila][columna] = lo que hay en fila columna

if(!sensores.colision)
	switch(ultimaAccion){

		case actFORWARD:
			switch(brujula){

				case 0:		//NORTE
				  fil--;
				  x--;
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



	//Cuando hayamos muerto se nos copiara lo descubierto en la matriz original (mapaResultado)

	/*if(sensores.vida==0){

	}*/

	//Si no estamos bien situados, es decir, no hemos encontrado un punto PK solo se rellena el mapaPrueba

	if(!bien_situado){
		RellenaMapa(sensores,brujula,false);
		
	}

//Si estamos bien situados se guardara el mapaPRueba en mapaResultado
	else {

		RellenaMapa(sensores, brujula,true);

			for (int i = 0; i < mapaPrueba.size(); ++i){
				for(int j = 0; j < mapaPrueba.at(i).size(); ++j)

					if(mapaPrueba[i][j]!='?')
						mapaResultado[i-(x-fil)][j-(y-col)]=mapaPrueba[i][j];
			}	

		
}


	if((sensores.terreno[0]=='K') && (!bien_situado)){
		fil=sensores.mensajeF;
		col=sensores.mensajeC;
		bien_situado=true;
	}


	if(ultimaAccion==actPICKUP){
		if(sensores.objetoActivo=='2' && !(sensores.superficie[2]=='0' ||sensores.superficie[2]=='1' || sensores.superficie[2]=='2' || sensores.superficie[2]=='3')){
			//accion=actTURN_R;
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
	else if(sensores.terreno[2]=='B' && ultimaAccion!=actPOP && maleta.front()=='2'){
			accion=actPOP;
			maleta.pop();
		}

	else if(ultimaAccion==actPOP)
		accion=actFORWARD;


	
	else if(bien_situado && !sensores.colision) {

		/* if(sensores.vida % 5 == 0)
		 	for (int i = 0; i < mapaPulgarcito.size(); ++i)
		 	{
		 		for (int j = 0; j < mapaPulgarcito.size(); ++j)
		 		{
		 			cerr << " " << mapaPulgarcito[i][j];
		 		}
		 		cerr << endl;
		 	}*/

		accion = dondeVasPulgarcito(fil,col);
		//cout << "accion"<<accion; 
	}

	else{ // funcionamiento por a ciegas

	if(sensores.colision)
		accion=actTURN_R;

	else 
		if((sensores.terreno[2]=='T'||sensores.terreno[2]=='S'||sensores.terreno[2]=='K') && sensores.superficie[2]=='_') {
			accion=actFORWARD;
		}


		else if(!girar_derecha){
			accion=actTURN_L;  //en caso de que no podamos seguir recto,giramos
		}

		else{
			accion=actTURN_R;
		}
		
	}


	//Recordar la ultima accion
	
	ultimaAccion=accion;

/*
	cout << "Terreno: ";
	for (int i=0; i<sensores.terreno.size(); i++)
		cout << sensores.terreno[i];
	cout << endl;

	

	cout << "Mochila: " << sensores.mochila << endl;
	cout << "Reset: " << sensores.reset << endl;
	cout << "Vida: " << sensores.vida << endl;
	cout<<"colision:"<<sensores.colision<<endl;
	cout << "objetoActivo: " << sensores.objetoActivo << endl;
	cout<<"fila:"<<fil<<endl;
	cout<<"Col:"<<col<<endl;
	cout << endl;
*/
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

//cout<<"Bien situado:"<<bien_situado<<endl;
	int k=0;	// posiciones de los sensores 

	if(bien_situado){
		mapaPulgarcito[fil][col]=cont;
		cont++;
}
//Si nuestro jugador mira al NORTE

	 if(brujula==0){  
	
		for(int i=0;i>-4;i--){
			for(int j=i;j<=abs(i);j++){

				//cout<<"x0:"<<x+i<<" "<<y+j<<endl;

				mapaPrueba[x+i][y+j]=sensores.terreno[k];



				/*if(sensores.terreno[k]=='M' || sensores.terreno[k]=='A' || sensores.terreno[k]=='P')
					mapaPulgarcito[fil+i][col+j]=numeric_limits<int>::max();
*/
					k++;
				}
			}

		
	}


//Si nuestro jugador mira al ESTE

	if(brujula==1){   

		for(int i=0;i<=3;i++){
			for(int j=-i;j<=i;j++){

				//cout<<"x1:"<<x+j<<" "<<y+i<<endl;

				mapaPrueba[x+j][y+i]= sensores.terreno[k];
				

				/*if(sensores.terreno[k]=='M' || sensores.terreno[k]=='A' || sensores.terreno[k]=='P')
					mapaPulgarcito[fil+j][col+i]=numeric_limits<int>::max();
*/
				k++;

			}	
		}

	}


//Si nuestro jugador mira al SUR

	else if(brujula==2){  
				
		for(int i=0;i<=3;i++){
			for(int j=i;abs(j)<=i;j--){

				//cout<<"x2:"<<x+i<<" "<<y+j<<endl;

				mapaPrueba[x+i][y+j]=sensores.terreno[k];

/*				if(sensores.terreno[k]=='M' || sensores.terreno[k]=='A' || sensores.terreno[k]=='P')
					mapaPulgarcito[fil+i][col+j]=numeric_limits<int>::max();
*/
				k++;
			}
		}

		
	}


//Si nuestro jugador mira al OESTE

	else if(brujula==3){     
		for(int i=0;i<=3;i++){
			for(int j=i;abs(j)<=i;j--){

				//cout<<"x3:"<<x+j<<" "<<y-i<<endl;

				mapaPrueba[x+j][y-i]=sensores.terreno[k];

/*				if(sensores.terreno[k]=='M' || sensores.terreno[k]=='A' || sensores.terreno[k]=='P')
					mapaPulgarcito[fil+j][col-i]=numeric_limits<int>::max();
*/
				k++;
			}
		}


		
	}

		


}

/************************************************************************************************************
 * DecisionObjeto --> Le pasaremos el obstaculo, dependiendo del obstaculo del que se trate tomaremos una   *
 * 					  decision diferente.																	*
 *																											*
 ************************************************************************************************************/
/*
void ComportamientoJugador::DecisionObjeto(unsigned char obstaculo){


int pos=-1;

		switch(obstaculo){

			case 'd':
				//Comprobar si tenemos llave en la mochila
				pos=Objeto_mochila(3); //El numero 3 es la llave!
				//hacer funcion que compruebe lo que tenemos en la mochila y ponga en la mano lo que queramos
			break;

			case 'l':
				//tirar hueso, mirar en la funcion
			cout<<"hola";
			break;

			case '0':
				//meter en la mochila si no esta
				cout<<"ssdf";
			break;

			case '1':
				//meter en la mochila si no esta
			break;

			case '2':
				//meter en la mochila si no esta
			break;

			case '3':
				//meter en la mochila si no esta
			break;

		}

	}


*/
/************************************************************************************************************
 * Objeto_mochila --> comprobara si el objeto se encuentra en la mochila y en que posicion				   	*
 * 					  																						*																											*
 ************************************************************************************************************/

int ComportamientoJugador::Objeto_mochila(unsigned char objeto){

	int pos_objeto=-1;
	Sensores sensores;

		if(sensores.mochila==objeto) //DUDO QUE ESTE BIEN!!!
			pos_objeto=0;

	return pos_objeto;
	
}

// Devuelve true si no podemos pasar po la posicion (fila,col)
bool ComportamientoJugador::NoPuedesPasar(int fila, int col)
{
	bool no_puedes_pasar = false; // por defecto puedes pasar 
	if (mapaResultado[fila][col] == 'B' && sensor.objetoActivo!='2' || mapaResultado[fila][col] == 'M' || mapaResultado[fila][col] == 'A' || mapaResultado[fila][col]=='P' || mapaResultado[fila][col]=='D' )
	{
		no_puedes_pasar = true; 
	}
	return no_puedes_pasar; 
}



Action ComportamientoJugador::dondeVasPulgarcito(int fila, int columna)
{	

	Action siguiente_accion; 	// accion a devolver 
	if ( (tengoDelante(fila,columna) < tengoDerecha(fila,columna)) && 
		 (tengoDelante(fila,columna) < tengoIzquierda(fila,columna)))
	{
		// la posición que tengo delante tiene un valor más bajo 
		siguiente_accion = actFORWARD; 	
	}else if ( (tengoDerecha(fila,columna) < tengoDelante(fila,columna)) and (tengoDerecha(fila,columna) < tengoIzquierda(fila,columna) ) ){
			siguiente_accion = actTURN_R;

	}else if ((tengoIzquierda(fila,columna) < tengoDerecha(fila,columna)) and (tengoIzquierda(fila,columna) < tengoDelante(fila,columna) ) ){
			siguiente_accion = actTURN_L;

	}else if (tengoDelante(fila,columna)== 0 ) {
		siguiente_accion = actFORWARD;

	}else if(tengoIzquierda(fila,columna)== 0){
		siguiente_accion = actTURN_L;
	}else if(tengoDerecha(fila,columna)== 0){
		siguiente_accion = actTURN_R;
	}else{
		//if(rand()%2==0){	
			siguiente_accion = actTURN_L ;
		//}else{
			siguiente_accion = actTURN_R ;
		//}
	}
	return siguiente_accion;
}

// devuelve qué tengo delante de la posición pasada
// si no puedo pasar por esa posición, devuelve un número
// muy grande 
int ComportamientoJugador::tengoDelante(int fila, int columna)
{
	int max = numeric_limits<int>::max(); 	// ... el numero muy grande
	switch(brujula){
		case 0: 
			if(NoPuedesPasar(fila-1,columna)) return max; 
			else return mapaPulgarcito[fila-1][columna]; 
			break; 
		case 1: 
			if(NoPuedesPasar(fila,columna+1)) return max; 
			else return mapaPulgarcito[fila][columna+1];
			break;
		case 2: 
			if(NoPuedesPasar(fila+1,columna)) return max; 
			else return mapaPulgarcito[fila+1][columna];
			break;
		case 3: 
			if(NoPuedesPasar(fila,columna-1)) return max; 
			else return mapaPulgarcito[fila][columna-1];
			break;
	}
}

// devuelve qué debo poner en el mapa pulgarcito a la izquierda
int ComportamientoJugador::tengoIzquierda(int fila, int columna)
{
	int max = numeric_limits<int>::max(); 	// ... el numero muy grande
	switch(brujula){
		case 0: 
			if(NoPuedesPasar(fila,columna-1)) return max; 
			else return mapaPulgarcito[fila][columna-1]; 
			break; 
		case 1: 
			if(NoPuedesPasar(fila-1,columna)) return max; 
			else return mapaPulgarcito[fila-1][columna];
			break;
		case 2: 
			if(NoPuedesPasar(fila,columna+1)) return max; 
			else return mapaPulgarcito[fila][columna+1];
			break;
		case 3: 
			if(NoPuedesPasar(fila+1,columna)) return max; 
			else return mapaPulgarcito[fila+1][columna];
			break;
	}
}



int ComportamientoJugador::tengoDerecha(int fila, int columna)
{
	int max = numeric_limits<int>::max(); 	// ... el numero muy grande
	switch(brujula){
		case 0: 
			if(NoPuedesPasar(fila,columna+1)) return max; 
			else return mapaPulgarcito[fila][columna+1]; 
			break; 
		case 1: 
			if(NoPuedesPasar(fila+1,columna)) return max; 
			else return mapaPulgarcito[fila+1][columna];
			break;
		case 2: 
			if(NoPuedesPasar(fila,columna-1)) return max; 
			else return mapaPulgarcito[fila][columna-1];
			break;
		case 3: 
			if(NoPuedesPasar(fila-1,columna)) return max; 
			else return mapaPulgarcito[fila-1][columna];
			break;
	}
}

// // aplica el metodo pulgarcito 
// void ComportamientoJugador::Pulgarcito(){
// 	Action accion_pulgarcito = dondeVasPulgarcito(); 
// 	return accion_pulgarcito; 
	
// }




