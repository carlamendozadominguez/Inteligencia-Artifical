#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include "stdlib.h"
#include <stdlib.h>     /* abs */
using namespace std;





Action ComportamientoJugador::think(Sensores sensores){

	Action accion = actIDLE;

	//if(sensores.superficie[2]==)

	//	accion=actTURN_R;

	// En esta matriz de tamano 100x100 hay que escribir el mapa solucion
	// mapaResultado[fila][columna] = lo que hay en fila columna

	switch(ultimaAccion){

		case actFORWARD:
			switch(brujula){

				case 0:		//NORTE
				  fil--;
				 break;

				 case 1:	//ESTE
				   col++;
				 break;

				 case 2:	//SUR
				 	fil++;
				 break;

				 case 3:	//OESTE
				 	col--;
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


	// Comprobacion para no salirme del rango del mapa
	bool algo_va_mal=false;

	if (col<0){
		col=0;
		algo_va_mal=true;
	}

	else if (col>199){
		col=199;
		algo_va_mal=true;
	}

	if (fil<0){
		fil=0;
		algo_va_mal=true;
	}

	else if (fil>199){
		fil=199;
		algo_va_mal=true;
	}

	if (algo_va_mal){
		cout << "CUIDADO: NO ESTAS CONTROLANDO BIEN LA UBICACION DE TU AGENTE\n";
	}



	if(bien_situado){


		int k=0;

		if(brujula==1){   //Si mira al este


			for(int i=0;i<=3;i++){
				for(int j=-i;j<=i;j++){

					mapaResultado[fil+j][col+i]= sensores.terreno[k];

					k++;

				}	
			}
		}

		else if(brujula==0){  //Si mira al norte
				for(int i=0;i>-4;i--){
					for(int j=i;j<=abs(i);j++){

						mapaResultado[fil+i][col+j]= sensores.terreno[k];

						k++;
					}
				}
		}

		else if(brujula==3){  //Si mira al oeste    
			for(int i=0;i<=3;i++){
					for(int j=i;abs(j)<=i;j--){

						mapaResultado[fil+j][col-i]= sensores.terreno[k];

						k++;
					}
				}

		}

		else if(brujula==2){  //Si mira al sur
				for(int i=0;i<=3;i++){
					for(int j=i;abs(j)<=i;j--){

						mapaResultado[fil+i][col+j]= sensores.terreno[k];

						k++;
					}
				}

		}


	}

	if((sensores.terreno[0]=='K') && (!bien_situado)){
		fil=sensores.mensajeF;
		col=sensores.mensajeC;
		bien_situado=true;
	}


int pos=-1;

	if(sensores.superficie[2]!='_'){

		switch(sensores.superficie[2]){

			case 'd':
				//Comprobar si tenemos llave en la mochila
				pos=objeto_mochila(sensores,3);
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


//avanzaremos si estamos en terreno arenoso o pedregoso y no hay ningun objeto 
//o personaje en esa casilla


	if(((sensores.terreno[2]=='T')||(sensores.terreno[2]=='S')||(sensores.terreno[2]=='K')) && (sensores.superficie[2]=='_')) {
		accion=actFORWARD;
	}


	else if(!girar_derecha){
		accion=actTURN_L;  //en caso de que no podamos seguir recto,giramos
	}

	else{
		accion=actTURN_R;
	}

	//Recordar la ultima accion
	
	ultimaAccion=accion;


	cout << "Terreno: ";
	for (int i=0; i<sensores.terreno.size(); i++)
		cout << sensores.terreno[i];
	cout << endl;

	cout << "Superficie: ";
	for (int i=0; i<sensores.superficie.size(); i++)
		cout << sensores.superficie[i];
	cout << endl;

	cout << "Mochila: " << sensores.mochila << endl;
	cout << "Reset: " << sensores.reset << endl;
	cout << "Vida: " << sensores.vida << endl;
	cout<<"colision:"<<sensores.colision<<endl;
	cout << "objetoActivo: " << sensores.objetoActivo << endl;
	cout<<"fila:"<<fil<<endl;
	cout<<"Col:"<<col<<endl;
	cout << endl;

	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}


//funcion que comprobara si el objeto esta en la mochila

int ComportamientoJugador::objeto_mochila(Sensores sensores,int objeto){

	int pos_objeto=-1;

		if(sensores.mochila==objeto)
			pos_objeto=0;

	return pos_objeto;
	
}