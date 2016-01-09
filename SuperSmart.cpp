// SuperSmart.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <sstream>
#include <string.h>
#include <windows.h>
#include <tchar.h>
#include <dos.h>

using namespace std;

/*Declaración de estructuras de la multilista*/
struct ronda_caja // estructura padre
{
	int num_caja;
	char empleado;
	int num_clientes;
	ronda_caja *der;
	ronda_caja *izq;
	struct lista_cliente *abajo;

};
struct lista_cliente // estructura hijo bicola con acceso restringida para los clientes
{
	int id_cliente;
	int estado;
	lista_cliente *ant;
	lista_cliente *sig;
	struct lista_carrito_comprando *abajo;
};

/*Fin Declaración de estructuras de mulitilista*/


/*Declaracion de estructura de datos auxiliares*/
struct bicola_carrito // bicola de carritos libres
{
	int num_car;
	bicola_carrito *cab1;
	bicola_carrito *fin1;
	bicola_carrito *cab2;
	bicola_carrito *fin2;
	bicola_carrito *der;
	bicola_carrito *izq;
	bicola_carrito *extract_b1();
	bicola_carrito *extract_b2();
};

struct bicola_cliente_espera // Bicola de clientes en espera de asignacion de carrito de compra
{
	int id_cliente;
	int estado;
	bicola_cliente_espera *cab1;
	bicola_cliente_espera *fin1;
	bicola_cliente_espera *cab2;
	bicola_cliente_espera *fin2;
	bicola_cliente_espera *der;
	bicola_cliente_espera *izq;
	bicola_cliente_espera *extract_b1_ce();
	bicola_cliente_espera *extract_b2_ce();
};

struct ronda_clientes_comprando
{
	int id_cliente;
	int estado;
	ronda_clientes_comprando *der;
	ronda_clientes_comprando *izq;
	ronda_clientes_comprando *IraCaja(int id_cliente);
	struct lista_carrito_comprando *abajo;
};
struct lista_carrito_comprando // estructura nieto lista simple de carritos asignados a clientes
{
	int num_carrito;
	lista_carrito_comprando *sig;
};

/*Fin Declaracion de estructura de datos auxiliares*/


/*Declaración de procedimientos*/
void crear_caja(int num_caja);
void crear_carrito(int cant_carrito, int num_caja, int num_car);
void Imprimir();
void Imprimir_Menu();
void insert_b1(bicola_carrito *nuevo);
void insert_b2(bicola_carrito *nuevo);
void insert_ce_b1(bicola_cliente_espera *nuevo);
void insert_ce_b2(bicola_cliente_espera *nuevo);
bool empty_b(bicola_carrito *cab1, bicola_carrito *cab2);
bool empty(bicola_carrito *cab);
bool empty_b_ce(bicola_cliente_espera *cab1, bicola_carrito *cab2);
bool empty_ce(bicola_cliente_espera *cab);
void gotoxy(int x, int y);
void crear_cliente(int id_cliente, int estado);
void asignar_carrito(int num_caja, int num_Carrito); 
void IraCaja(int id_cliente);
void Pagar(int num_caja);

void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x; coord.Y = y;
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hStdOut, coord);
}

/*Declaración de Variables Globales*/
ronda_caja *cabCajas = NULL;
bicola_carrito *cabCarritos1 = NULL;
bicola_carrito *cabCarritos2 = NULL;
bicola_carrito *finCarritos1 = NULL;
bicola_carrito *finCarritos2 = NULL;
bicola_cliente_espera *cabClienteEspera1 = NULL;
bicola_cliente_espera *cabClienteEspera2 = NULL;
bicola_cliente_espera *finClienteEspera1 = NULL;
bicola_cliente_espera *finClienteEspera2 = NULL;
ronda_clientes_comprando *cabComprando = NULL;
lista_carrito_comprando *cabCarritoComprando = NULL;
/*Fin Declaración de Variables Globales*/

/* Estructura principal */


void main()
{
	/*Declaración de variable local*/
	int opcion = 0, cantCajas, cantCarritos, id_cliente, estado, num_caja=0, num_car=0;

	system("cls");
	system("color f9");
	gotoxy(63, 12);
	printf("Bienvenido a SuperSmart");
	gotoxy(64, 14);
	printf("Configuracion Inicial");
	do{
		gotoxy(46, 16);
		printf("[1]. Digite la Cantidad de Cajas de Pago (Entre 1 y 8): [  ]");
		gotoxy(104, 16);
		cin >> cantCajas;
	} while (cantCajas<1 || cantCajas>8);
	crear_caja(cantCajas);
	do{
		gotoxy(44, 18);
		printf("[2]. Digite la Cantidad de Carritos de Compra (Entre 1 y 19): [  ]");
		gotoxy(107, 18);
		cin >> cantCarritos;
	} while (cantCarritos<1 || cantCarritos>19);
	crear_carrito(cantCarritos, num_caja, num_car);

	Imprimir();
	Imprimir_Menu();

	do{
		cin >> opcion;
		switch (opcion){
			case 1:{
				Imprimir();
				do{
					gotoxy(120, 26);
					printf("Digite Identificacion...");
					gotoxy(120, 27);
					printf("(Solo Numeros):");
					gotoxy(120, 28);
					cin >> id_cliente;
				} while (id_cliente < 0);
				do{
					gotoxy(120, 29);
					printf("Digite:");
					gotoxy(120, 30);
					printf("[1] para cliente Normal:");
					gotoxy(120, 31);
					printf("[2] para cliente Prioritario:");
					gotoxy(120, 32);
					cin >> estado;
				} while (estado < 1 || estado >2);

				crear_cliente(id_cliente, estado);

				Imprimir();
				Imprimir_Menu();
			}break;
			case 2:{
				Imprimir();
				do{
					gotoxy(120, 26);
					printf("Digite Identificacion...");
					gotoxy(120, 27);
					printf("(Solo Numeros):");
					gotoxy(120, 28);
					cin >> id_cliente;
				} while (id_cliente < 0);

				IraCaja(id_cliente);
				Imprimir();
				Imprimir_Menu();
			}break;
			case 3:{
				Imprimir();
				do{
					gotoxy(120, 26);
					printf("Digite el # de Caja...");
					gotoxy(120, 27);
					printf("(Solo Numeros):");
					gotoxy(120, 28);
					cin >> num_caja;
				} while (id_cliente < 0);

				Pagar(num_caja);
				Imprimir();
				Imprimir_Menu();
			}break;
			case 4:{
				exit(0);
			}break;
			default:{
				Imprimir();
				gotoxy(120, 32);
				printf("[[ Opcion Errada ]]");
				gotoxy(120, 33);
				system("pause");
				Imprimir();
				Imprimir_Menu();
			}
		}
	} while (opcion != 5);
}


void crear_caja(int cantCajas)
{
	for (int i = 0; i <= cantCajas - 1; i++)
	{
		ronda_caja *nuevo = new ronda_caja;
		nuevo->num_caja = i + 1;
		char s = 'E' + i;
		nuevo->empleado = s;
		nuevo->num_clientes = 0;
		if (cabCajas == NULL)
		{
			cabCajas = nuevo;
			nuevo->der = cabCajas;
			nuevo->izq = cabCajas;
		}
		else
		{
			nuevo->der = cabCajas;
			cabCajas->izq->der = nuevo;
			nuevo->izq = cabCajas->izq;
			cabCajas->izq = nuevo;
		}
		nuevo->abajo = NULL;
	}
}

void crear_carrito(int cant_carrito, int num_caja, int num_car){
	if (num_caja == 0){
		for (int i = 0; i <= cant_carrito - 1; i++){
			bicola_carrito *nuevo = new bicola_carrito();
			nuevo->num_car = i + 1;
			if ((i + 1) % 2 == 0){
				insert_b1(nuevo);
			}
			else{
				insert_b2(nuevo);
			}
		}
	}
	else{
		bicola_carrito *nuevo = new bicola_carrito();
		nuevo->num_car=num_car;
		if (num_caja % 2 == 0){
			insert_b1(nuevo);
		}
		else{
			insert_b2(nuevo);
		}
	}
}

void insert_b1(bicola_carrito *nuevo)
{
	if (!empty(cabCarritos1))
	{
		finCarritos1->der = nuevo;
		nuevo->izq = finCarritos1;
		finCarritos1 = nuevo;
		finCarritos1->der = NULL;
		cabCarritos2 = finCarritos1;
	}
	else
	{
		nuevo->der = NULL;
		nuevo->izq = NULL;
		finCarritos1 = nuevo;
		cabCarritos2 = nuevo;
		finCarritos2 = nuevo;
		cabCarritos1 = nuevo;
	}
}

void insert_b2(bicola_carrito *nuevo)
{
	if (!empty(cabCarritos2))
	{
		finCarritos2->izq = nuevo;
		nuevo->der = finCarritos2;
		finCarritos2 = nuevo;
		finCarritos2->izq = NULL;
		cabCarritos1 = finCarritos2;
	}
	else
	{
		nuevo->der = NULL;
		nuevo->izq = NULL;
		finCarritos1 = nuevo;
		cabCarritos2 = nuevo;
		finCarritos2 = nuevo;
		cabCarritos1 = nuevo;
	}
}


bool empty_b(bicola_carrito *cab1, bicola_carrito *cab2)
{
	if ((cab1 == NULL) && (cab2 == NULL))
	{
		return(true);
	}
	else
	{
		return(false);
	}
}


bool empty(bicola_carrito *cab)
{
	if (cab == NULL)
	{
		return(true);
	}
	else
	{
		return(false);
	}
}

bool empty_b_ce(bicola_cliente_espera *cab1, bicola_cliente_espera *cab2)
{
	if ((cab1 == NULL) && (cab2 == NULL))
	{
		return(true);
	}
	else
	{
		return(false);
	}
}


bool empty_ce(bicola_cliente_espera *cab)
{
	if (cab == NULL)
	{
		return(true);
	}
	else
	{
		return(false);
	}
}


struct bicola_carrito* extract_b1()
{
	bicola_carrito *q;
	q = cabCarritos1;
	cabCarritos1 = cabCarritos1->der;

	if (cabCarritos1 != NULL){
		cabCarritos1->izq = NULL;
	}

	q->der = NULL;
	if (cabCarritos1 == NULL){
		cabCarritos2 = NULL;
		finCarritos1 = NULL;
	}
	return q;
}

struct bicola_carrito* extract_b2()
{
	bicola_carrito *q = new bicola_carrito();
	q = cabCarritos2;
	cabCarritos2 = cabCarritos2->izq;
	if (cabCarritos2 != NULL){
		cabCarritos2->der = NULL;
	}

	finCarritos1 = cabCarritos2;

	q->izq = NULL;
	if (cabCarritos2 == NULL){
		cabCarritos1 = NULL;
		finCarritos2 = NULL;
	}
	return q;
}

struct bicola_cliente_espera* extract_b1_ce()
{
	bicola_cliente_espera *q;
	q = cabClienteEspera1;
	cabClienteEspera1 = cabClienteEspera1->der;

	if (cabClienteEspera1 != NULL){
		cabClienteEspera1->izq = NULL;
	}

	q->der = NULL;
	if (cabClienteEspera1 == NULL){
		cabClienteEspera2 = NULL;
		finClienteEspera1 = NULL;
	}
	return q;
}

struct bicola_cliente_espera* extract_b2_ce()
{
	bicola_cliente_espera *q;
	q = cabClienteEspera2;
	cabClienteEspera2 = cabClienteEspera2->izq;
	if (cabClienteEspera2 != NULL){
		cabClienteEspera2->der = NULL;
	}

	finClienteEspera1 = cabClienteEspera2;

	q->izq = NULL;
	if (cabClienteEspera2 == NULL){
		cabClienteEspera1 = NULL;
		finClienteEspera2 = NULL;
	}
	return q;
}


void crear_cliente(int id_cliente, int estado){
	if (empty_b(cabCarritos1, cabCarritos2)){
		bicola_cliente_espera *r = new bicola_cliente_espera();
		r->id_cliente = id_cliente;
		r->estado = estado;
		if (estado == 1){
			insert_ce_b1(r);
		}
		else{
			insert_ce_b2(r);
		}
		gotoxy(120, 32);
		printf("[[ Cliente creado ]]");
		gotoxy(120, 33);
		system("pause");
	}
	else{
		bicola_carrito *y = extract_b1();
		ronda_clientes_comprando *nuevo = new ronda_clientes_comprando();
		nuevo->id_cliente = id_cliente;
		nuevo->estado = estado;
		if (cabComprando == NULL)
		{
			cabComprando = nuevo;
			nuevo->der = cabComprando;
			nuevo->izq = cabComprando;
		}
		else
		{
			nuevo->der = cabComprando;
			cabComprando->izq->der = nuevo;
			nuevo->izq = cabComprando->izq;
			cabComprando->izq = nuevo;
		}
		lista_carrito_comprando *z = new lista_carrito_comprando();
		z->sig = NULL;
		z->num_carrito = y->num_car;
		nuevo->abajo = z;
		free(y);
		gotoxy(120, 32);
		printf("[[ Cliente creado ]]");
		gotoxy(120, 33);
		system("pause");
	}
}

void insert_ce_b1(bicola_cliente_espera *nuevo)
{
	if (!empty_ce(cabClienteEspera1))
	{
		finClienteEspera1->der = nuevo;
		nuevo->izq = finClienteEspera1;
		finClienteEspera1 = nuevo;
		finClienteEspera1->der = NULL;
		cabClienteEspera2 = finClienteEspera1;
	}
	else
	{
		nuevo->der = NULL;
		nuevo->izq = NULL;
		finClienteEspera1 = nuevo;
		cabClienteEspera2 = nuevo;
		finClienteEspera2 = nuevo;
		cabClienteEspera1 = nuevo;
	}
}

void insert_ce_b2(bicola_cliente_espera *nuevo)
{
	if (!empty_ce(cabClienteEspera2))
	{
		finClienteEspera2->izq = nuevo;
		nuevo->der = finClienteEspera2;
		finClienteEspera2 = nuevo;
		finClienteEspera2->izq = NULL;
		cabClienteEspera1 = finClienteEspera2;
	}
	else
	{
		nuevo->der = NULL;
		nuevo->izq = NULL;
		finClienteEspera1 = nuevo;
		cabClienteEspera2 = nuevo;
		finClienteEspera2 = nuevo;
		cabClienteEspera1 = nuevo;
	}
}

void IraCaja(int id_cliente)
{
	ronda_clientes_comprando *q = new ronda_clientes_comprando();
	bool sw = false;
	if (cabComprando == NULL)
	{
		gotoxy(120, 32);
		printf("[[ No hay Clientes Comp. ]]");
		gotoxy(120, 33);
		system("pause");

	}
	else
	{
		q = cabComprando;
		do{
			if (q->id_cliente == id_cliente)
			{
				sw = true;
			}
			else
			{
				q = q->der;
			}
		} while (q != cabComprando && sw == false);
		if (sw == false)
		{
			gotoxy(120, 32);
			printf("[[ Cliente no Existe ]]");
			gotoxy(120, 33);
			system("pause");

		}
		else
		{      // pregunto si el dato esta en la posicion de cab     
			ronda_clientes_comprando *p = new ronda_clientes_comprando();
			ronda_clientes_comprando *r = new ronda_clientes_comprando();
			if (q == cabComprando)
			{
				if (cabComprando->der == cabComprando){
					cabComprando->izq = NULL;
					cabComprando->der = NULL;
					cabComprando = NULL;
				}
				else{
					p = cabComprando->izq;
					cabComprando = q->der;
					cabComprando->izq = p;
					p->der = cabComprando;
				}
			}
			else
			{
				p = q->izq;
				r = q->der;
				p->der = r;
				r->izq = p;
			}
			sw = false;
			ronda_caja *j = cabCajas;
			ronda_caja *k = cabCajas->izq;
			ronda_caja *m = j;
			int menor = j->num_clientes;
			do{
				if (j->num_clientes < menor){
					menor = j->num_clientes;
					m = j;
				}
				if (k->num_clientes < menor){
					menor = k->num_clientes;
					m = k;
				}
				j = j->der;
				k = k->izq;
			} while (j->izq != k);
			//free(j);
			//free(k);
			lista_cliente *l = new lista_cliente();
			l->id_cliente = q->id_cliente;
			l->estado = q->estado;
			l->abajo = q->abajo;
			m->num_clientes = m->num_clientes + 1;

			if (m->abajo == NULL){
				l->ant = NULL;
				l->sig = NULL;
				m->abajo = l;
				gotoxy(120, 32);
				printf("[[ Cliente paso a Caja ]]");
				gotoxy(120, 33);
				system("pause");
			}
			else{
				lista_cliente *e = m->abajo;
				if (l->estado == 1){ //cliente estado normal
					while (e->sig != NULL){
						e = e->sig;
					}
					e->sig = l;
					l->ant = e;
				}
				else{ //cliente con prioridad
					sw = true;
					if (e->estado == 2){
						while (sw){
							if ((e->sig)->estado != 2){
								sw = false;
							}
							else{
								e = e->sig;
							}
						}
						l->sig = e->sig;
						l->ant = e;
						e->sig = l;
					}
					else{
						l->sig = e;
						e->ant = l;
						m->abajo = l;
						gotoxy(120, 32);
						printf("[[ Cliente paso a Caja ]]");
						gotoxy(120, 33);
						system("pause");

					}
				}
			}
		}
	}
}

void Pagar(int num_caja){

	bool sw1 = false, sw2 = false;
	ronda_caja *j = cabCajas;
	ronda_caja *k = cabCajas->izq;
	ronda_caja *m = j;
	do{
		if (j->num_caja == num_caja){
			sw1 = true;
		}
		else if (k->num_caja == num_caja){
			sw2 = true;
		}
		else{
			j = j->der;
			k = k->izq;
		}
	} while (j->izq != k && !sw1 && !sw2);
	if (sw1==false && sw2==false){
		gotoxy(120, 32);
		printf("[[ Caja no Existe ]]");
		gotoxy(120, 33);
		system("pause");

	}
	else{
		if (sw1){
			if (j->abajo ==NULL){
				gotoxy(120, 32);
				printf("[[ Caja sin Clientes ]]");
				gotoxy(120, 33);
				system("pause");

			}
			else{
				j->num_clientes = j->num_clientes - 1;
				lista_cliente *p = j->abajo;
				lista_carrito_comprando *q = p->abajo;
				if (empty_ce(cabClienteEspera1)){
					crear_carrito(1,j->num_caja,q->num_carrito);
					gotoxy(120, 32);
					printf("[[ Carrito Liberado ]]");
					gotoxy(120, 33);
					system("pause");
				}
				else{
					asignar_carrito(j->num_caja, q->num_carrito);
					gotoxy(120, 32);
					printf("[[ Carrito Asignado ]]");
					gotoxy(120, 33);
					system("pause");
				}

				if (p->sig == NULL){
					j->abajo = NULL;
				}
				else{
					lista_cliente *w = p->sig;
					w->ant = NULL;
					j->abajo = w;
				}
				gotoxy(120, 32);
				printf("[[ Cliente pago en Caja ]]");
				gotoxy(120, 33);
				system("pause");

				free(p);
			}
		}
		else{
			if (k->abajo == NULL){
				gotoxy(120, 32);
				printf("[[ Caja sin Clientes ]]");
				gotoxy(120, 33);
				system("pause");

			}
			else{
				k->num_clientes = k->num_clientes - 1;
				lista_cliente *p = k->abajo;
				lista_carrito_comprando *q = p->abajo;
				if (empty_ce(cabClienteEspera1)){
					crear_carrito(1, k->num_caja, q->num_carrito);
					gotoxy(120, 32);
					printf("[[ Carrito Liberado ]]");
					gotoxy(120, 33);
					system("pause");
				}
				else{
					asignar_carrito(k->num_caja, q->num_carrito);
					gotoxy(120, 32);
					printf("[[ Carrito Asignado ]]");
					gotoxy(120, 33);
					system("pause");
				}
				if (p->sig == NULL){
					k->abajo = NULL;
				}
				else{
					lista_cliente *w = p->sig;
					w->ant = NULL;
					k->abajo = w;
				}
				gotoxy(120, 32);
				printf("[[ Cliente pago en Caja ]]");
				gotoxy(120, 33);
				system("pause");

				free(p);
			}
		}
	}
}

void asignar_carrito(int num_caja, int num_carrito) {
	bicola_cliente_espera *y = extract_b1_ce();
	ronda_clientes_comprando *nuevo = new ronda_clientes_comprando();
	nuevo->id_cliente = y->id_cliente;
	nuevo->estado = y->estado;
	if (cabComprando == NULL)
	{
		cabComprando = nuevo;
		nuevo->der = cabComprando;
		nuevo->izq = cabComprando;
	}
	else
	{
		nuevo->der = cabComprando;
		cabComprando->izq->der = nuevo;
		nuevo->izq = cabComprando->izq;
		cabComprando->izq = nuevo;
	}
	lista_carrito_comprando *z = new lista_carrito_comprando();
	z->sig = NULL;
	z->num_carrito = num_carrito;
	nuevo->abajo = z;
	free(y);
}

void Imprimir() {
	system("cls");
	gotoxy(1, 1);
	printf("SuperSmart - [en linea]");

	gotoxy(1, 2);
	printf("------------------------------------------------------------------------------------------------------------------");

	gotoxy(1, 3);
	printf("[+] C a j a s");
	int x = 3;
	int z = 5;
	if (cabCajas != NULL)
	{
		ronda_caja *p = cabCajas;
		lista_cliente *u = NULL;
		do{
			z = 5;
			gotoxy(x, z);
			printf("[+] [Caja ");
			printf("%d",p->num_caja);
			printf("]");
			z++;
			gotoxy(x, z);
			printf("[Empleado ");
			printf("%c", p->empleado);
			printf("]");
			u = p->abajo;
			while (u!=NULL){
				z++;
				gotoxy(x, z);
				printf("[");
				printf("%d", u->id_cliente);
				printf("]");
				z++;
				gotoxy(x, z);
				printf("[Estado ");
				printf("%d", u->estado);
				printf("]");
				z++;
				gotoxy(x, z);
				lista_carrito_comprando *v = u->abajo;
				printf("[Carrito ");
				printf("%d", v->num_carrito);
				printf("]");
				u = u->sig;
			}
			p = p->der;
			x = x + 14;
		} while (p != cabCajas);
	}
	else
	{
		gotoxy(3, 6);
		printf("[No hay cajas de compra Creadas]");
	}

	gotoxy(135, 2);
	printf("--------------");

	gotoxy(136, 3);
	printf("[+] Carritos");
	x = 142;
	z = 5;
	if (!empty(cabCarritos1)){
		bicola_carrito *s = new bicola_carrito();
		bicola_carrito *cab1 = NULL, *cab2=NULL, *fin1=NULL, *fin2=NULL;
		while (!empty(cabCarritos1)){
			s = extract_b1();
			gotoxy(x, z);
			printf("[");
			printf("%d", s->num_car);
			printf("]");
			z++;

			if (!empty(cab1))
			{
				fin1->der = s;
				s->izq = fin1;
				fin1 = s;
				fin1->der = NULL;
				cab2 = fin1;
			}
			else
			{
				s->der = NULL;
				s->izq = NULL;
				fin1 = s;
				cab2 = s;
				fin2 = s;
				cab1 = s;
			}
		}
		while (!empty(cab1)){
			s = cab1;
			cab1 = cab1->der;

			if (cab1 != NULL){
				cab1->izq = NULL;
			}

			s->der = NULL;
			if (cab1 == NULL){
				cab2 = NULL;
				fin1 = NULL;
			}
			insert_b1(s);
		}
	}
	else{
		gotoxy(137, 6);
		printf("[No hay]");
	}

	gotoxy(118, 2);
	printf("--------------");

	gotoxy(119, 3);
	printf("[+] Clientes");
	gotoxy(119, 4);
	printf("   en Espera");
	z = 6;

	bicola_cliente_espera *t = new bicola_cliente_espera();
	bicola_cliente_espera *cabc1 = NULL, *cabc2 = NULL, *finc1 = NULL, *finc2 = NULL;
	if (!empty_ce(cabClienteEspera1)){
		while (!empty_ce(cabClienteEspera1)){
			t = extract_b1_ce();
			gotoxy(123, z);
			printf("[");
			printf("%d", t->id_cliente);
			printf("]");
			z++;
			gotoxy(123, z);
			printf("[Estado ");
			printf("%d", t->estado);
			printf("]");
			z++;
			if (!empty_ce(cabc1))
			{
				finc1->der = t;
				t->izq = finc1;
				finc1 = t;
				finc1->der = NULL;
				cabc2 = finc1;
			}
			else
			{
				t->der = NULL;
				t->izq = NULL;
				finc1 = t;
				cabc2 = t;
				finc2 = t;
				cabc1 = t;
			}
		}
		while (!empty_ce(cabc1)){
			t = cabc1;
			cabc1 = cabc1->der;

			if (cabc1 != NULL){
				cabc1->izq = NULL;
			}

			t->der = NULL;
			if (cabc1 == NULL){
				cabc2 = NULL;
				finc1 = NULL;
			}
			insert_ce_b1(t);
		}
	}
	else{
		gotoxy(119, 6);
		printf("No hay Clientes");

	}

	gotoxy(1, 24);
	printf("---------------------------------------------------------------------------------------------------------------------------------------------------");

	gotoxy(1, 25);
	printf("[+] Clientes Comprando");
	x = 3;
	z = 27;

	if (cabComprando != NULL)
	{
		ronda_clientes_comprando *q = cabComprando;
		do{
			gotoxy(x, 27);
			printf("[");
			printf("%d", q->id_cliente);
			printf("]");
			gotoxy(x, 28);
			printf("[Estado ");
			printf("%d", q->estado);
			printf("]");
			lista_carrito_comprando *r = q->abajo;
			gotoxy(x, 29);
			printf("[Carrito ");
			printf("%d", r->num_carrito);
			printf("]");
			q = q->der;
			x = x + 10;
		} while (q != cabComprando);
	}
	else
	{
		gotoxy(3, 27);
		printf("[No hay clientes comprando]");
	}

}

void Imprimir_Menu(){
	// Imprime el menu de opciones
	gotoxy(124, 26);
	printf("[[ Menu de Opciones ]]");
	gotoxy(120, 28);
	printf("[1]-[Nuevo Cliente]");
	gotoxy(120, 29);
	printf("[2]-[Ir a Caja]");
	gotoxy(120, 30);
	printf("[3]-[Pagar]");
	gotoxy(120, 31);
	printf("[4]-[Salir]");
	gotoxy(120, 33);
	printf("Digite Opcion [  ]");
	gotoxy(136, 33);

}