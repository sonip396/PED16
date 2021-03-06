/*
 * tabbporo.cpp
 *
 *  Created on: Apr 5, 2016
 *      Author: jorge
 */

#include "../include/tabbporo.h"
#include <queue>

TNodoABB::TNodoABB():iz(), de(), item() {}

TNodoABB::TNodoABB(const TNodoABB &origen){
	Copia(origen);
}

TNodoABB::~TNodoABB() {} //Llama a los destructores de cada tipo de dato automáticamente

void TNodoABB::Copia(const TNodoABB &origen){
	(this)->item = origen.item;
	if(!origen.de.EsVacio())
		(this)->de = origen.de;
	if(!origen.iz.EsVacio())
		(this)->iz = origen.iz;
}
TNodoABB& TNodoABB::operator =(const TNodoABB &origen) {
	if(this != &origen)
	{
		(this)->~TNodoABB();
		Copia(origen);
	}
	return *this;
}

/*
 * ****************************************************************************
 */
void TABBPoro::InordenAux(TVectorPoro &vector, int &posicion) const{
	if(!EsVacio())
	{
		nodo->iz.InordenAux(vector, posicion);
		vector[posicion] = nodo->item;
		posicion++;
		nodo->de.InordenAux(vector, posicion);
	}
}

void TABBPoro::PreordenAux(TVectorPoro &vector, int &posicion) const {
	if(!EsVacio())
	{
		vector[posicion] = nodo->item;
		posicion++;
		nodo->iz.PreordenAux(vector, posicion);
		nodo->de.PreordenAux(vector, posicion);
	}
}

void TABBPoro::PostordenAux(TVectorPoro &vector, int &posicion) const {
	if(!EsVacio())
	{
		nodo->iz.PostordenAux(vector, posicion);
		nodo->de.PostordenAux(vector, posicion);
		vector[posicion] = nodo->item;
		posicion++;
	}
}

void TABBPoro::NivelesAux(TVectorPoro &vector, int &posicion) const {
	TNodoABB *aux;
	queue<TNodoABB *> cola;
	cola.push((*this).nodo);
	while(!cola.empty())
	{
		aux = cola.front();
		vector[posicion] = cola.front()->item;
		posicion++;
		cola.pop();
		if(!aux->iz.EsVacio())
			cola.push(aux->iz.nodo);
		if(!aux->de.EsVacio())
			cola.push(aux->de.nodo);
	}
}

TABBPoro::TABBPoro(): error() {
	nodo = NULL;
}

TABBPoro::TABBPoro(const TABBPoro &origen) {
	Copiar(origen);
}

TABBPoro::~TABBPoro() {
	if(!EsVacio())
	{
		nodo = NULL;
		delete nodo;
	}
}

void TABBPoro::Copiar(const TABBPoro &origen){
	if(origen.nodo != NULL)
	{
		TNodoABB *aux = new TNodoABB();
		aux->item = origen.nodo->item;
		nodo = aux;
		nodo->iz.Copiar(origen.nodo->iz);
		nodo->de.Copiar(origen.nodo->de);
	}
	else
		nodo = NULL;
}

TABBPoro& TABBPoro::operator =(const TABBPoro &origen) {
	if(this != &origen)
	{
		(*this).~TABBPoro();
		Copiar(origen);
	}
	return *this;
}

bool TABBPoro::CompruebaNodos(const TABBPoro &otroArbol) const {
	if(EsVacio())
		return true;
	else
	{
		return (otroArbol.Buscar(nodo->item) && nodo->iz.CompruebaNodos(otroArbol) && nodo->de.CompruebaNodos(otroArbol));
	}
}

bool TABBPoro::operator ==(const TABBPoro &otroArbol) const {
	if(Nodos() == otroArbol.Nodos())
		return CompruebaNodos(otroArbol);
	else
		return false;
}

bool TABBPoro::EsVacio() const{
	return nodo == NULL;
}
bool TABBPoro::EsHoja() const{
	return (nodo->de.EsVacio() && nodo->iz.EsVacio());
}

bool TABBPoro::FiltraVolumen(const int &volumen) {
	if(nodo->item.Volumen() == volumen)
		return false;
	else if(volumen < nodo->item.Volumen() && !nodo->iz.EsVacio())
		nodo->iz.FiltraVolumen(volumen);
	else if(volumen > nodo->item.Volumen() && !nodo->de.EsVacio())
		nodo->de.FiltraVolumen(volumen);
	else
		return true;
}

void TABBPoro::BuscaInserta(TNodoABB *nuevo){
	//Comprobar recursivamente que sea menor o mayor que el elemento nodo de la raiz e introducir donde quepa
	if(nuevo->item.Volumen() < nodo->item.Volumen())
	{
		//recursión hacia la izquierda del árbol
		if(!nodo->iz.EsVacio())
			nodo->iz.BuscaInserta(nuevo);
		else
			nodo->iz.nodo = nuevo;
	}
	else if(nuevo->item.Volumen() > nodo->item.Volumen())
	{
		//recursión hacia la derecha del árbol
		if(!nodo->de.EsVacio())
			nodo->de.BuscaInserta(nuevo);
		else
			nodo->de.nodo = nuevo;
	}
}

bool TABBPoro::Insertar(const TPoro &poro) {
	//se crea el nodo a insertar
	TNodoABB *aux = new TNodoABB;;
	aux->item = poro;
	if(EsVacio())
	{
		nodo = aux;
		return true;
	}
	else if(FiltraVolumen(poro.Volumen()))
	{
		BuscaInserta(aux);
		return true;
	}
	return false;
}

bool TABBPoro::Borrar(const TPoro &poro) {
	if(!EsVacio() && Buscar(poro))
	{
		if(nodo->item == poro)
		{
			if(EsHoja() || nodo->iz.EsVacio() || nodo->de.EsVacio())
			{
				//El nodo es sustituido por el hijo
				SustituyePorHijo();
				return true;
			}
			else
			{
				SustituyeMayorIzq();
				return true;
			}
		}
		else if(poro.Volumen() < nodo->item.Volumen() && !nodo->iz.EsVacio())
		nodo->iz.Borrar(poro);
		else if(poro.Volumen() > nodo->item.Volumen() && !nodo->de.EsVacio())
		nodo->de.Borrar(poro);
	}
	else
		return false;
}

void TABBPoro::SustituyePorHijo()
{
	TABBPoro aux;
	if(EsHoja())
		(*this).~TABBPoro();
	else if(!nodo->iz.EsVacio())
	{
		//sustituir por el hijo izquierdo
		aux = nodo->iz;
		(*this) = aux;
	}
	else if(!nodo->de.EsVacio())
	{
		//sustituir por el hijo derecho
		aux = nodo->de;
		(*this) = aux;
	}
}
void TABBPoro::SustituyeMayorIzq()
{
	TNodoABB *mayorIzq = nodo;
	TNodoABB *aux = nodo; //apoyo para intercambiar los valores de los nodos
	if(!nodo->iz.EsVacio())
	{
		//moverse 1 a la izq y todo a la derecha hasta que esté vacío
		mayorIzq = nodo->iz.nodo;
		while(!mayorIzq->de.EsVacio())
			mayorIzq = mayorIzq->de.nodo;
		nodo->item = mayorIzq->item;
		nodo->iz.Borrar(mayorIzq->item); //Borra en este subarbol el mayor de la izquierda que acaba de ser sustituido
	}
}

bool TABBPoro::Buscar(const TPoro &poro) const {
	if(EsVacio())
		return false;
	else
		return BuscarAux(poro);
}

bool TABBPoro::BuscarAux(const TPoro &poro) const
{
	if(nodo->item == poro)
		return true;
	else if(poro.Volumen() < nodo->item.Volumen() && !nodo->iz.EsVacio())
		nodo->iz.Buscar(poro);
	else if(poro.Volumen() > nodo->item.Volumen() && !nodo->de.EsVacio())
		nodo->de.Buscar(poro);
	else
		return false;
}

TPoro TABBPoro::Raiz() const {
	if(!EsVacio())
		return nodo->item;
	else
		return error;
}

int TABBPoro::Altura() const {
	int a1, a2;
	if(!EsVacio())
	{
		a1 = nodo->iz.Altura();
		a2 = nodo->de.Altura();
		return (1 + (a1<a2 ? a2:a1));
	}
	else return 0;
}

int TABBPoro::Nodos() const {
	return NodosAux(0);
}
int TABBPoro::NodosAux(int total) const
{
	if(!EsVacio())
	{
		total = 1 + nodo->iz.NodosAux(total) + nodo->de.NodosAux(total);
	}
	return total;
}

int TABBPoro::NodosHoja() const {
	int a1=0;
	if(!EsVacio())
	{
		a1 = a1 + nodo->iz.NodosHoja() + nodo->de.NodosHoja();
		if(EsHoja())
			a1++;
	}
	return a1;
}

TVectorPoro TABBPoro::Inorden() const {
	//Posición en el vector que almacena el recorrido
	int posicion = 1;
	//Vector del tamaño adecuado para almacenar todos los nodos
	TVectorPoro v(Nodos());
	InordenAux(v, posicion);
	return v;
}

TVectorPoro TABBPoro::Preorden() const {
	//Posición en el vector que almacena el recorrido
	int posicion = 1;
	//Vector del tamaño adecuado para almacenar todos los nodos
	TVectorPoro v(Nodos());
	PreordenAux(v, posicion);
	return v;
}

TVectorPoro TABBPoro::Postorden() const {
	//Posición en el vector que almacena el recorrido
	int posicion = 1;
	//Vector del tamaño adecuado para almacenar todos los nodos
	TVectorPoro v(Nodos());
	PostordenAux(v, posicion);
	return v;
}


TVectorPoro TABBPoro::Niveles() const {
	TVectorPoro recorrido;
	if(!EsVacio())
	{
		recorrido.Redimensionar(Nodos());
		int pos = 1;
		NivelesAux(recorrido, pos);
	}

	return recorrido;
}

TABBPoro TABBPoro::operator +(const TABBPoro &arbol) {
	TABBPoro aux((*this));
	TVectorPoro vectorArbol(arbol.Inorden());
	for(int i=1; i<=vectorArbol.Longitud(); i++)
		aux.Insertar(vectorArbol[i]);
	return aux;
}

TABBPoro TABBPoro::operator -(const TABBPoro &arbol) {
	TVectorPoro vectorThis = Inorden();
	TABBPoro aux;
	for (int i=1; i<=vectorThis.Longitud(); i++)
	{
		if(!arbol.Buscar(vectorThis[i]))
			aux.Insertar(vectorThis[i]);
	}
	return aux;
}

ostream & operator<<(ostream &os, const TABBPoro &arbol)
{
	os << arbol.Niveles();
	return os;
}
