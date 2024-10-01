#include <iostream>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <locale.h>
#include "Memoria.h"
#define TAMMIN 32
#define PRO 100
using namespace std;

int numeroP=0;
int ProcComp=0;

struct Proceso {
	char nombre[20];
	int tam;
	int quantum;
	int ordenllegada;
	bool termino; //si sus quantum han llegado a 0
};

struct Cola {
	int front;
	int end;
	Proceso *datos;
}; //Cola de procesos


//FUNCIONES
Cola *crearcola(Cola *, int);
bool ColaVacia(Cola *);
bool ColaLlena(Cola *);
void insertarCola(Cola *, const Proceso &);
Proceso *crearProceso(Proceso *, int, int);
void mostrarProceso(Proceso);
void mostrarCola(Cola *q, int);
int tamProceso(Cola *q);
Proceso sacarElemento(Cola *q);

//Variables globales para el control del programa

int velocidad=1; 						//Indica la velocidad a la que se lleva la simulacion
int modo=1; 							//Selecciona entre dos modos: llevar la simulacion completa hasta terminar, o terminar la somulacion cuando el usuario lo indique
int inicio=1; 							//hacer la simulacion desde el inicio o continuar con los mismos valores
int repetir=1; 							//repetir la simulacion o no
int frag=0;


int main(int argc, char** argv){

	cout << "\t\t\t BUDDY SYSTEM Y ROUND ROBIN"  << endl<< endl;
	system("pause");
	system("cls");
	int tamMM=0, tamMP=0, tiempo=0, numQuan=0, op=0, auxtam=0, seguir=1;
	int cont=0, tamPrimer=0;
	bool continua=false;
	cout  << endl;
	cout <<  "\n\n\t\t\t    TAMANO DE LA MEMORIA: 1 MB = 1024 kbs  "<< endl;
	tamMM= 1024;															//Tamaño de la memoria
	Memoria *lista = new Memoria(tamMM);									//Crear objeto dinamico de tipo Memoria
	cout << endl << endl << "\t\t DATOS DEL PROCESO";
	cout << "\t\tTamano maximo de memoria de los procesos (en kb): ";
	
	do{
		cin >> tamMP;														
		if(tamMP < TAMMIN){												//La particion del proceso debe ser mayor a 32
			system("cls");
			cout << endl << "\t\t\tTamano de particion: invalido \n\t\t\tIngresar nuevo valor: ";
			cin >> tamMP;
		}else if(tamMP >= tamMM){
			system("cls");
			cout << endl << "\t\tParticion mayor al tamano de la memoria \n\t\t\t     Ingresa nuevo valor: ";
			cin >> tamMP;
		}
	} while (tamMP<TAMMIN || tamMP>tamMM);
	
	
	cout << endl << "\t\t\t   Numero de quantum del Proceso: ";					//Valor para el quanto de tiempo para PROCESOS
	do {
		cin>>numQuan;
		if(numQuan <= 0){
			cout << "\t\tNumero de quantum invalido, ingresa nuevo valor" << endl;
		}
		cout << "\t\t\t\t\t";
	} while(numQuan<=0);
	cout << endl << "\t\t\t   Numero de quantum del Sistema: ";					//Valor para el quanto de tiempo para el SISTEMA	
	cin >> tiempo;
	
	Cola *q;
	q = crearcola(q,1);
	Cola *ex;
	ex = crearcola(ex,2);
	cout  <<  "\n\n\n";
	system("pause");
	system("cls");

	srand(time(NULL));
	Proceso *aux;
	for(int k=0; k<PRO; k++) {
		aux = crearProceso(aux,tamMP,numQuan);
		insertarCola(q, *aux);
	}

	system("cls");
	
	cout  << "\t\t MEMORIA DISPONIBLE DE "  <<  tamMM  << " KB" << endl;

	lista->mostrarLista();

	cout << endl << endl;
	system("pause");
	system("cls");
	cout << "\t\t\t\t Modo de la simulacion: " << endl;
	

	do {
		tamPrimer = tamProceso(q);
		cout << endl << "Valor del tamano del primer proceso " << tamPrimer << endl << endl;
		
		//Verificaremos si la cola no esta llena o si el tamaño del va
		continua = lista->siEntra(tamPrimer);
		//Caso en que la cola todavia no esta llena
		if(continua) {
			lista->veloc = 1;
			cout << "El proceso si puede entrar" << endl << endl;
			Proceso a = sacarElemento(q);				//Buscaremos el proceso 1 en cola, FIFO
			mostrarProceso(a);							//Mostraremos info del proceso 
			cout << endl << endl;
			
			cout << "PROCESO:" << endl;
			//							FORMA DE ESTRUCTURA REQUERIDA 
			cout << "[" << a.ordenllegada << "," << a.tam << "," << a.quantum << "]";
			
			
			cout << endl << endl;
			
			
			//Mostramos lista de todos los procesos
			lista->mostrarLista();
			//Buscamos 
			lista->buscaEspacio(a.tam, tamMM, a.ordenllegada);
			
			cout << endl << "Quantum de procesamiento del sistema: " << tiempo << endl;
			
			//cout << endl << endl << "HUECOS:" << endl;
			/*
			cout << "[0," << lista->getfrag() << ",0]";
			lista->setfrag(0);
			*/
			
			
			a.quantum -= tiempo; 			//quantum del proceso - quantum del sistema
			
			if(a.quantum <= 0){				
				a.quantum = 0;
			}
			cout << endl << endl << "Quantums restantes del proceso: " << a.quantum << endl;
			cout << "Atendiendo proceso: ";
			lista->mostrarLista();
			
			if(a.quantum > 0){				//En dado caso que hayan restado quantums, que no haya terminado el proceso
				cout<<endl<< "El proceso tiene "<<a.quantum << " quantum de tiempo"<<endl;
				cout<<"Regresa a la cola de listos" << endl;
				insertarCola(ex, a);
				cont++;
			}
			else{
				cout << endl << endl << "El proceso tiene " << a.quantum << " quantum de tiempo" << endl;
				cout << "El proceso termino su ejecucion" << endl;
				lista->sacarProceso(a.ordenllegada);
				lista->unirMemoria();
				cout << endl;
				lista->mostrarLista();
			}
			//cout << endl << endl;
			//cout << endl << endl;
			mostrarCola(ex, cont);

			
		
			
			
			system("pause");
			system("cls");
		} 
		//								El proceso no pudo entrar a la cola, esto debido a que la memoria esta llena
		else{
			cout << "El Proceso no puede entrar " << endl << endl;
			cout << "Debe continuar con la cola de los procesos en memoria" << endl;
			mostrarCola(ex, cont);
			system("pause");
			system("cls");
			Proceso b = sacarElemento(ex);
			mostrarProceso(b);
			//cout << endl << endl;
			
			cout << "PROCESO:" << endl;
			cout << "[" << b.ordenllegada << "," << b.tam << "," << b.quantum << "]";
			
		
			cout << endl << "Quantum de procesamiento del sistema: " << tiempo << endl;
			
			//cout << "[0," << lista->getfrag() << ",0]";
			//lista->setfrag(0);
			
			b.quantum -= tiempo;
			
			if(b.quantum<=0) {
				b.quantum=0;
			}
			cout << endl << endl << "Quantum restantes del proceso: " << b.quantum << endl;
			cout << endl << endl;
			cout << "Atendiendo proceso: ";
			lista->mostrarLista();
			
			if(b.quantum > 0) {
														//En caso que el proceso no haya terminado de ejecutarse
				cout << endl << endl << "El proceso tiene " << b.quantum << " quantum de tiempo" << endl;
				cout << "Regresa a la cola de listos" << endl;
				insertarCola(ex, b);								//Lo mandamos a ex, ya que es el final de la cola
				cont++;
			} else {
				cout << endl << endl << "El proceso tiene " << b.quantum << " quantum de tiempo" << endl;
				cout << "El proceso termino su ejecucion" << endl;
				lista->sacarProceso(b.ordenllegada);
				lista->unirMemoria();
				cout << endl;
				lista->mostrarLista();
			}

			cout << endl << endl;
			mostrarCola(ex, cont);

			
			
			
			
			system("pause");
			system("cls");

		} 	
		if(modo == 1) {
			cout << "\t\t\t    Desea seguir con la ejecucion? " << endl;
			cout << "\t\tSelecciona 1 para continuar, selecciona 0 para finalizar" << endl;
			cout << "\t\t\t\t\t";
			cin >> seguir;
			system("cls");
		}
		continua = false;
	} while (seguir == 1);


	cout << "\t\t\t\tPROCESOS FINALIZADOS" << endl << endl;
	system("cls");
	cout  <<  "\n\n\nFIN DE PROGRAMA" <<  endl;
	system("pause");

	
	delete(q);
	return 0;
}

Cola *crearcola(Cola *q,int dup){
	q = (Cola *)malloc(sizeof(Cola));
	q->front=0;
	q->end=-1;
	q->datos = new Proceso [PRO*dup];
	return q;
}

void insertarCola(Cola *q, const Proceso &aux){
	//cout << "SI INSERTE" << endl;
	if(ColaLlena(q)) {
		cout << "La cola esta llena" << endl;
	} else {
		q->end++;
		q->datos[q->end] = aux;
	}
}

Proceso *crearProceso(Proceso *aux, int tamMP, int numQuan){
	aux = new (Proceso);
	char num[3];
	strcpy(aux->nombre,"Proceso ");
	fflush(stdin);
	
	itoa(numeroP, num, 10);				
	strcat(aux->nombre, num);				//Poner el numero del proceso de int a char
	aux->tam = rand()% tamMP;			
	
	if(aux->tam < TAMMIN) {
		aux->tam = TAMMIN;					//Si el tamaño es menor que el minimo 32, entonces volverlo lo minimo
	} else if (aux->tam > tamMP) {
		aux->tam = tamMP;					//Si el tamaño es mayor, ese será su propio tamaño
	}
		
	aux->quantum = rand()% numQuan;			//Valor de quantum aleatorio
	if(aux->quantum==0) {					//Unico caso especial
		aux->quantum=1;
	}
	
	aux->ordenllegada=numeroP;				//Numero de proceso a partir de la variable global
	numeroP++;	
	aux->termino=false;						//Siempre se deberá inicializar en false
	return aux;
}

void mostrarProceso (Proceso aux) {

}

void mostrarCola(Cola *q, int numPro) {
	for(int l=q->front; l<q->end+1; l++) {
		mostrarProceso(q->datos[l]);
		cout << endl << endl;
	}
}

bool ColaVacia(Cola *q) {
	if(q->front == (q->end +1))
		return true;
	return false;
}

bool ColaLlena(Cola *q) {
	if(q->end == (PRO - 1))
		return true;
	return false;
}

Proceso sacarElemento(Cola *q) {
	if(ColaVacia(q)) {
		cout << "La cola esta vacia" << endl;
	} else {                    
		Proceso a = q->datos[q->front];			//a sera el 1er proceso en cola 
		q->front++;								//El 2ndo parasara ahora a ser el 1ero en cola
		return a;
	}
	exit(0);
}

int tamProceso(Cola *q) {
	int valorProceso=0;
	valorProceso= q->datos[q->front].tam;
	return valorProceso;
}