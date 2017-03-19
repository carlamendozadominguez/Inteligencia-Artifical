#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include "stdlib.h"
#include <stdlib.h>     /* abs */
using namespace std;



Action ComportamientoJugador::think(Sensores sensores){

	Action accion = actIDLE;



	if(sensores.vida == 0 || sensores.vida == 1000){
		
		brujula=0; //  0->Norte, 1-> Este , 2->Sur , 3->Oeste
    	ultimaAccion=actIDLE;
    	girar_derecha=false;
    	bien_situado=false;
        x=200;
        y=200;
		fil=99;
		col=99;
		

		cout<<"PK:"<<sensores.mensajeF<<","<<sensores.mensajeC<<endl;


        for(int i = 0 ; i < mapaPrueba.size() ; i++){
    		for(int j = 0 ; j < mapaPrueba.at(i).size() ;j++){
    			mapaPrueba[i][j]='?';
    		}
    	}

	  /*for(int i = 0 ; i < mapaDescubierto.size() ; i++){
    		for(int j = 0 ; j < mapaDescubierto.at(i).size() ;j++){
    			mapaDescubierto[i][j]='?';

    			cout<<mapaDescubierto[i][j];
    		}
    	}*/
}
		/*for(std::vector<unsigned char> v: mapaResultado){
			for (unsigned char c : v)
			{
				cout << c;
			}
		}*/





		/*while(getchar() != '\n');


		for (int i = 0; i < mapaResultado.size(); ++i){
			for(int j = 0; j < mapaResultado.at(i).size(); ++j)
				//mapaResultado[i][j]=mapaDescubierto[i][j];
				//if(mapaDescubierto[i][j]!='?')
					mapaResultado[i][j]=mapaDescubierto[i][j];

		}
	}*/

	// En esta matriz de tamano 100x100 hay que escribir el mapa solucion
	// mapaResultado[fila][columna] = lo que hay en fila columna

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

if(!bien_situado){


	RellenaMapa(sensores,brujula,false);
							cout<<"PK copy:"<<sensores.mensajeF<<","<<sensores.mensajeC<<endl;

	for (int i = 0; i < mapaResultado.size(); ++i){
			for(int j = 0; j < mapaResultado.at(i).size(); ++j){
				//mapaResultado[i][j]=mapaDescubierto[i][j];
				//if(mapaDescubierto[i][j]=='?')

					mapaDescubierto[i][j]=mapaPrueba[x-sensores.mensajeF+i][y-sensores.mensajeC+j];
}
				cout<<"X:"<<x<<endl;
				cout<<"Y:"<<y<<endl;

	}
}

	else {

		cout<<"PK:"<<sensores.mensajeF<<","<<sensores.mensajeC<<endl;


		RellenaMapa(sensores, brujula,true);

		for (int i = 0; i < mapaResultado.size(); ++i){
			for(int j = 0; j < mapaResultado.at(i).size(); ++j)
				if(mapaResultado[i][j]=='?')				
				mapaResultado[i][j]=mapaDescubierto[i][j];
				//if(mapaDescubierto[i][j]!='?')
					//mapaResultado[i][j]=mapaPrueba[x-sensores.mensajeF+i][y-sensores.mensajeC+j];
		
	}
}



	if((sensores.terreno[0]=='K') && (!bien_situado)){
		fil=sensores.mensajeF;
		col=sensores.mensajeC;
		bien_situado=true;
	}



//CUANDO ENCONTREMOS UN OBSTACULO IREMOS A DECISIONOBJETO

	if(sensores.superficie[2]!='_'){
			cout<<"OBSTACULO!!!!"<<endl;
			//accion=actPICKUP;
		DecisionObjeto(sensores.superficie[2]);
	}

	//sensores.objetoActivo='1';

//avanzaremos si estamos en terreno arenoso o pedregoso y no hay ningun objeto 
//o personaje en esa casilla


	if((sensores.terreno[2]=='T'||sensores.terreno[2]=='S'||sensores.terreno[2]=='K') && sensores.superficie[2]=='_') {
		accion=actFORWARD;
	} 

	//else if(sensores.objetoActivo=='1' && sensores.terreno[2]=='A')
	//		accion=actFORWARD;

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




/************************************************************************************************************
 * RellenaMapa --> Segun vayamos descubriendo mundo lo guardaremos en una matriz mapaDescubierto, en esta   *
 * 				   funcion se contempla la vision que tiene nuestro jugador y se ira guardando lo que 		*
 *				   observe en esas 15 casillas segun hacia donde este mirando.								*
 *																											*
 ************************************************************************************************************/



void ComportamientoJugador::RellenaMapa(Sensores sensores,int brujula,bool situado){

	int k=0;


//Si nuestro jugador mira al NORTE

	 if(brujula==0){  
	
		for(int i=0;i>-4;i--){
			for(int j=i;j<=abs(i);j++){
				if(situado==true){
					mapaResultado[fil+i][col+j]= sensores.terreno[k];
					mapaDescubierto[fil+i][col+j]= sensores.terreno[k];
				}

				else
					mapaPrueba[x+i][y+j]=sensores.terreno[k];

					k++;
				}
			}
	}


//Si nuestro jugador mira al ESTE

	if(brujula==1){   

		for(int i=0;i<=3;i++){
			for(int j=-i;j<=i;j++){
				if(situado==true){
					mapaResultado[fil+j][col+i]= sensores.terreno[k];
					mapaDescubierto[fil+j][col+i]= sensores.terreno[k];

				}

				else
					mapaPrueba[x+j][y+i]=sensores.terreno[k];

				k++;

			}	
		}
	}


//Si nuestro jugador mira al SUR

	else if(brujula==2){  
				
		for(int i=0;i<=3;i++){
			for(int j=i;abs(j)<=i;j--){

				if(situado==true){
					mapaResultado[fil+i][col+j]= sensores.terreno[k];
					mapaDescubierto[fil+i][col+j]= sensores.terreno[k];

				}

				else
					mapaPrueba[x+i][y+j]=sensores.terreno[k];

				k++;
			}
		}

	}


//Si nuestro jugador mira al OESTE

	else if(brujula==3){     
		for(int i=0;i<=3;i++){
			for(int j=i;abs(j)<=i;j--){

				if(situado==true){
					mapaResultado[fil+j][col-i]= sensores.terreno[k];
					mapaDescubierto[fil+j][col-i]= sensores.terreno[k];

				}

				else
					mapaPrueba[x+j][y-i]=sensores.terreno[k];
				
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

void ComportamientoJugador::DecisionObjeto(unsigned char obstaculo){
Action accion;

int pos=-1;

		switch(obstaculo){

			case 'd':
				//Comprobar si tenemos llave en la mochila
				pos=Objeto_mochila(3); //El numero 3 es la llave!
				//hacer funcion que compruebe lo que tenemos en la mochila y ponga en la mano lo que queramos
			break;

			case 'l':
				//tirar hueso, mirar en la funcion
			cout<<"hola"<<endl;
			break;

			case '0':
				//meter en la mochila si no esta
			pos=Objeto_mochila(0);
				cout<<"ssdf"<<endl;
			break;

			case '1':
				//meter en la mochila si no esta
			pos=Objeto_mochila(1);
				accion=actPUSH;

			cout<<"dasf"<<endl;
			break;

			case '2':
			pos=Objeto_mochila(2);
				accion=actPUSH;

			cout<<"asdfs"<<endl;
				//meter en la mochila si no esta
			break;

			case '3':
			pos=Objeto_mochila(3);
				accion=actPUSH;

			cout<<"kkdkdkd"<<endl;
				//meter en la mochila si no esta
			break;

		}

	}



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

