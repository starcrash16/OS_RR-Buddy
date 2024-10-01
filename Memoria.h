#include <iostream>
#include <windows.h>
#include <time.h>
#define TAMMIN 32
using namespace std;

typedef int tipo;						//Definimos tipo = int para una mayor facilidad

typedef struct Elemento { 						//Estructura
	tipo dato; 							//Tipo de dato entero
	bool ocupado; 
	tipo idPro;
	Elemento *izq; 						//Puntero que apunta a la izquierda del nodo
	Elemento *der; 						//Puntero que apunta a la derecha del nodo
} Nodo;

class Memoria { 						//clase
	public:
		int frag;
		int veloc;
		Nodo *inicio; 					//Puntero al inicio de la lista
		Nodo *fin; 						//Puntero al final de la lista
		//Constructor
		Memoria(void); 					//Constructor por defecto
		Memoria (tipo dato); 			//Constructor con parametros
		//Metodos
		Nodo *crearNodo(tipo x, Nodo *i, Nodo *d); //Metodo para crear nodo
		void mostrarLista(); 			//Metodo mostrar de inicio a fin
		bool esVacia();
		//Metodos para insertar elemento
		void insertaInicio(tipo x);
		void insertaFinal(tipo x);
		void elimina(tipo x); 			//Elimina un dato en especifico
		void eliminaActual(Nodo*);
		void insertaDespuesDe(tipo x, tipo y);
		void insertaAntesDe(tipo x, Nodo*);
		void buscaEspacio(int, int, int);
		void partir(Nodo*, int, int, int); 
		void sacarProceso(int);
		void unirMemoria();  
		float porcentajeMemoria(int); 
		bool siEntra(int);
		void setOcupado(bool);
		void setIdPro(tipo);
		int getfrag(); 
		void setfrag(int);
		tipo getIdPro();
		bool getOcupado(int);
		tipo getvalor(int); 
		inline void setVelocidad(int vel){
			this->veloc = vel;
		}
		~Memoria();						//destructor
};

Memoria::Memoria(void) {
	fin=inicio=NULL;			
}

Memoria::Memoria(tipo dato) { 
	fin=inicio=crearNodo(dato,NULL,NULL);
}

bool Memoria::esVacia() {
	if(inicio==NULL) {
		return true;
	}
	return false;
}

Nodo* Memoria::crearNodo(tipo x, Nodo *i, Nodo *d) {
	Nodo *nuevo; 						//Crea nuevo nodo
	nuevo = new(Nodo); 					//Se le asigna memoria
	nuevo->dato = x; 
	nuevo->ocupado = false; 
	nuevo->idPro = -1; 
	nuevo->izq = i;						//Apuntan a NULL
	nuevo->der = d; 
	return nuevo;
}

void Memoria::mostrarLista() {
	int k;
	Nodo *q = inicio; 						//Puntero al inicio
	for(k=0; q; q=q->der) { 				//Recorre la lista hasta el final
		if(q->ocupado) {
			cout<< "[" << "("<<q->idPro << "), "<< q->dato << "]";
		}
		k++;
	}
}

void Memoria::insertaInicio(tipo x) {
	Nodo *nuevo = crearNodo(x,NULL,NULL); 		//crea nuevo nodo
	nuevo->der = inicio; 						//nuevo apunta a inicio por la derecha
	inicio->izq = nuevo; 						//inicio apunta nuevo por la izquierda
	inicio = nuevo; 							//se recorre el inicio
}

void Memoria::insertaFinal(tipo x) {
	Nodo *nuevo = crearNodo(x,NULL,NULL);
	nuevo->izq = fin; 							//Nuevo apunta a fin por la izquierda
	fin->der = nuevo; 							//Fin apunta a nuevo por la derecha
	fin = nuevo; 									//Fin se recorre
}

void Memoria::elimina(tipo x) {					
	Nodo *q=inicio; 							//Puntero al inicio
	if(inicio->dato == x && !q->ocupado) { 		//Si el dato a eliminar esta al inicio
		inicio = inicio->der; 					//Inicio se recorre hacia la derecha
	}else{
		while(q->der!=NULL) { 					//Recorre q mientras q->der no sea NULL
			q=q->der; 
			if(q->dato==x && !q->ocupado) { 	//si encuentra el dato
				if(q->der==NULL) { 
					q=q->izq; 					//regresa q a la izquierda
					fin=q; 						//se recorre el final hacia q
					q->der=NULL; 				//q->der apunta a NULL
				} else {
					q->der->izq=q->izq; 		//el puntero de q a la derecha apunta a q->izq
					q->izq->der=q->der; 		//el puntero de q a la izquierda apunta a q->der
					q=q->der; 					//q se recorre a la derecha
				}
				break; 							//se rompe el ciclo
			}
		}
	}
}


tipo Memoria::getvalor(int cont) { 				//Retorna el valor del nodo que se encuentra en el inicio
	Nodo *q=inicio; 
	for(int i=0; i<cont; i++) {
		q=q->der; 
	}
	return q->dato; 
}

void Memoria::setOcupado(bool ocup) {
	inicio->ocupado=ocup;
}

void Memoria::setIdPro(tipo id) {
	inicio->idPro=id;
}

void Memoria::insertaDespuesDe(tipo x, tipo y){
	Nodo *nuevo=crearNodo(x,NULL,NULL); 
	Nodo *q=inicio; 
	while (q->der!=NULL) {
		if(q->dato==y) {
			nuevo->izq=q; 							//nuevo por la izquierda apunta a q
			q->der->izq=nuevo; 						//el puntero a la derecha de q apunta nuevo por la izquierda
			nuevo->der=q->der; 						//nuevo por la derecha apunta a donde apunta q por la derecha
			q->der=nuevo; 							//q apunta a nuevo por la derecha
			break;
		}
		q=q->der; 									//se recorre q
	}
	if(q->dato==y && q->der==NULL) { 				//si el dato esta al final de la lista
		insertaFinal(x); 							//llama a "insertaFinal"
	}
}

void Memoria::insertaAntesDe(tipo x, Nodo *aux) {
	Nodo *nuevo=crearNodo(x,NULL,NULL);
	Nodo *q=inicio; 								//puntero al inicio
	if(q==aux) { 									//si el dato esta al inicio llama a "insertaInicio"
		insertaInicio(x);
	} else {
		while(q->der!=NULL) {
			q=q->der; 								//se recorre el puntero mientras no sea NULL
			if(q==aux) { 							//si encuentra el dato
				nuevo->der=q; 						//el nuevo apunta a q por la izquierda
				q->izq->der=nuevo; 					//el puntero al la izquierda de q apunta a nuevo
				nuevo->izq=q->izq; 					//nuevo apunta hacia donde apunta q por la izquierda
				q->izq=nuevo; 						//q por la izquierda apunta a nuevo
				break; 
			}
		}
	}
}

void Memoria::eliminaActual(Nodo *aux) {	
	Nodo *q=inicio; 
	if(inicio==aux) { 								//si el dato a eliminar esta al inicio
		inicio=inicio->der; 						//inicio se recorre hacia la derecha
	} else {
		while(q->der!=NULL) {
			q=q->der;
			if(q==aux) {
				if(q->der==NULL) {
					q=q->izq; 
					fin=q; 
					q->der=NULL;
				} else {
					q->der->izq=q->izq; 
					q->izq->der=q->der; 			//el puntero de q a la izquierda apunta a q->der
					q=q->der; 
				}
				break;
			}
		}
	}
}

void Memoria::buscaEspacio(int aTam, int tamMM, int id) {
	Nodo *aux = inicio; 
	bool encontro=false;
	while (aux!=NULL) {
	 	if(aux->dato >= aTam && !aux->ocupado) {
			if((aux->dato/2) >= aTam) {
				partir(aux,aux->dato, aTam, id); 
				break;
			}
			aux->ocupado = true;
			encontro = true;
			aux->idPro = id; 
			frag = aux->dato-aTam; 
			cout << endl;
			break;        
		}
		aux = aux->der; 
	}
	cout<<endl;
	mostrarLista(); 		// NOOO
}
  
void Memoria::partir(Nodo* aux, int tamEsp, int tamPro, int id) {
	int tam = tamEsp/2; 
	if(tamPro > tam) {
		aux->ocupado = true;
		aux->idPro = id;
		frag = tamEsp - tamPro;
		return; 
	}
	if(tamPro <= tamEsp) { 
		insertaAntesDe(tam,aux); 
		insertaAntesDe(tam,aux); 
		cout << endl;
		aux = aux->izq->izq;
		eliminaActual(aux->der->der);
		//mostrarLista();
		partir(aux,tam,tamPro,id); 
	}
}

void Memoria::unirMemoria() {
	Nodo *q=inicio; 
	while(q->der!= NULL) {
		if(q->dato == q->der->dato) {
			if (!q->ocupado && !q->der->ocupado){
				insertaAntesDe(q->dato*2, q);
				elimina(q->dato); 
				elimina(q->dato); 
				mostrarLista();
				cout<<endl;
				unirMemoria(); 
			}
		} 
		q=q->der; 
		
	}
	return;
}

bool Memoria::siEntra(int aTam) {
	Nodo *aux = inicio; 
	
	while (aux != NULL) {
		if(aux->dato >= aTam && !aux->ocupado) {
			return true;
		}
		aux=aux->der;
	}
	return false;
}

void Memoria::sacarProceso(int id) {
	Nodo *q = inicio; 
	while(q != NULL) {
		if(q->idPro == id) {
			q->ocupado = false; 
			q->idPro = -1; 
			break;
		}
		q = q->der;
	}
}

bool Memoria::getOcupado(int cont) {
	Nodo *q = inicio; 
	for(int i = 0; i<cont; i++) {
		q = q->der; 
	} 
	return q->ocupado; 
}

int Memoria::getfrag() {
	return frag;
}

void Memoria::setfrag(int frag) {
	this->frag = frag;
} 

float Memoria::porcentajeMemoria(int tamMM) { 
	float valor = 100;  
	float sub = 0, total = 0; 
	Nodo *q = inicio; 
	valor /= tamMM;
	while(q != NULL) {
		if(q->ocupado) {
			sub = valor*q->dato; 
			total += sub;
		}
		q = q->der; 
	}  
	return total;
}


Memoria::~Memoria() {
	if(!esVacia()) { 					//si la lista esta vacia
		while(inicio) { 				//mientras inicio exista
			Nodo *temp = inicio;		//crea variable temporal
			inicio = inicio->der;		//el inicio se recorre a la derecha
			delete temp; 				//elimina la variable temporal
		}
	}
}