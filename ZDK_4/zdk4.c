#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define MAX (80)
#define MAX_CHAR (1024)

struct _Polinom;
typedef struct _Polinom *listPoli;
typedef struct _Polinom {
	int Koef;
	int Pot;
	listPoli next;
}Polinom;

int UnesiPolinomeIzDatoteke(listPoli P1, listPoli P2, char *datoteka);
int UnesiSortirano(listPoli P, int koef, int pot);
int FormatirajIUnesi(listPoli P, char *buffer);
int ZbrojDvaPolinoma(listPoli Sum, listPoli P1, listPoli P2);
int UmnozakDvaPolinoma(listPoli Product, listPoli P1, listPoli P2);
Polinom* UnosPodataka(int koef, int pot);
int InsertNakon(listPoli P, listPoli Q);
int IspisiPolinom(listPoli P, char *imePol);
int UnesiImeDatoteke(char *datoteka);
int IzbrisiNakon(listPoli P);
int IzbrisiElSaKoef0(listPoli P);
int OslobodiMemoriju(listPoli P);

int main() {
	char datoteka[MAX] = { 0 };

	Polinom Pol1 = { .Koef = 0,.Pot = 0,.next = NULL };
	Polinom Pol2 = { .Koef = 0,.Pot = 0,.next = NULL };
	Polinom zbrojPol = { .Koef = 0,.Pot = 0,.next = NULL };
	Polinom umnozakPol = { .Koef = 0,.Pot = 0,.next = NULL };

	UnesiImeDatoteke(datoteka);

	UnesiPolinomeIzDatoteke(&Pol1, &Pol2, datoteka);

	IspisiPolinom(Pol1.next, "P1");
	IspisiPolinom(Pol2.next, "P2");

	ZbrojDvaPolinoma(&zbrojPol, Pol1.next, Pol2.next);
	IspisiPolinom(zbrojPol.next, "P1 + P2");

	UmnozakDvaPolinoma(&umnozakPol, Pol1.next, Pol2.next);
	IspisiPolinom(umnozakPol.next, "P1 * P2");

	OslobodiMemoriju(&Pol1);
	OslobodiMemoriju(&zbrojPol);
	OslobodiMemoriju(&umnozakPol);
	OslobodiMemoriju(&Pol2);

	system("pause");
	return 0;
}

int UnesiPolinomeIzDatoteke(listPoli P1, listPoli P2, char *datoteka)
{
	char buffer[MAX_CHAR] = { 0 };
	int returnValue = 0;
	FILE *fp = NULL;
	fp = fopen(datoteka, "r");
	if (!fp) {
		printf("pogreska u otvaranju datoteke\n");
		return -1;
	}
	fgets(buffer, MAX_CHAR, fp);
	returnValue = FormatirajIUnesi(P1, buffer);
	if (returnValue != 0) {
		return 1;
	}

	fgets(buffer, MAX_CHAR, fp);
	returnValue = FormatirajIUnesi(P2, buffer);
	if (returnValue != 0) {
		return 1;
	}
	fclose(fp);
	return 0;
}

int FormatirajIUnesi(listPoli P, char *buffer)
{
	char *currentBuffer = buffer;
	int koef = 0, pot = 0;
	int brojBajt = 0;

	while (strlen(currentBuffer) > 0)
	{
		if (sscanf(currentBuffer, "%d %d %n", &koef, &pot, &brojBajt) == 2) {
			UnesiSortirano(P, koef, pot);
			currentBuffer += brojBajt;
		}
		else {
			printf("datoteka nije ispravna\n");
			return 1;
		}
	}
	return 0;
}

int UnesiSortirano(listPoli P, int koef, int pot)
{

	if (koef == 0)
		return 0;

	while (P->next != NULL && P->next->Pot >= pot) {
		P = P->next;
	}

	if (P != NULL && P->Pot == pot) {
		P->Koef += koef;
	}
	else {
		listPoli noviEl = NULL;
		noviEl = UnosPodataka(koef, pot);
		InsertNakon(P, noviEl);
	}

	return 0;
}

int ZbrojDvaPolinoma(listPoli Sum, listPoli P1, listPoli P2) {
	listPoli temp = NULL;
	listPoli tempP1 = NULL;
	listPoli tempP2 = NULL;
	tempP1 = P1;
	tempP2 = P2;

	printf("Zbroj polinoma:\n");
	while (tempP1 != NULL && tempP2 != NULL)
	{
		if (tempP1->Pot == tempP2->Pot) {
			UnesiSortirano(Sum, tempP1->Koef + tempP2->Koef, tempP1->Pot);
			tempP1 = tempP1->next;
			tempP2 = tempP2->next;

		}
		else if (tempP1->Pot > tempP2->Pot) {
			UnesiSortirano(Sum, tempP1->Koef, tempP1->Pot);
			tempP1 = tempP1->next;
		}
		else {
			UnesiSortirano(Sum, tempP2->Koef, tempP2->Pot);
			tempP2 = tempP2->next;
		}
	}

	if (tempP1 == NULL) {
		temp = tempP2;
	}
	else {
		temp = tempP1;
	}

	while (temp != NULL) {
		UnesiSortirano(Sum, temp->Koef, temp->Pot);
		temp = temp->next;
	}
	free(temp);
	return 0;
}

int UmnozakDvaPolinoma(listPoli Product, listPoli P1, listPoli P2)
{
	printf("Umnozak polinoma:\n");
	listPoli startP2 = NULL;
	listPoli tempProduct = NULL;
	tempProduct = Product;
	startP2 = P2;
	
	while (P1!= NULL) {
		while (P2 != NULL) {
			UnesiSortirano(Product, P1->Koef * P2->Koef, P1->Pot + P2->Pot);
			P2 = P2->next;
		}
		P2 = startP2;
		P1 = P1->next;
	}
	IzbrisiElSaKoef0(tempProduct);
	return 0;
}

Polinom* UnosPodataka(int koef, int pot)
{
	listPoli Q = NULL;
	Q = (listPoli)malloc(sizeof(Polinom));
	if (!Q) {
		perror("pogreska u alokaciji memorije\n");
		return NULL;
	}
	Q->Koef = koef;
	Q->Pot = pot;
	Q->next = NULL;
	return Q;
}

int InsertNakon(listPoli P, listPoli Q)
{
	Q->next = P->next;
	P->next = Q;
	return 0;
}

int IspisiPolinom(listPoli P, char *imePol)
{
	if (P == NULL) {
		printf("nema elemenata u listi\n");
		return 1;
	}
	printf("%s = ", imePol);

	while (P != NULL) {

		if (P->Pot == 0) {
			printf("%d", P->Koef);
		}
		else if (P->Koef == 1) {
			printf("x^%d", P->Pot);
		}
		else {
			printf("%dx^%d", P->Koef, P->Pot);
		}

		if (P->next != NULL && P->next->Koef > 0) {
			printf("+");
		}
		else if (P->next != NULL && P->next->Koef < 0) {
			printf("");
		}
		
		P = P->next;
	}
	printf("\n");
	return 0;
}

int UnesiImeDatoteke(char *datoteka)
{
	printf("Unesite ime datoteke:\n");
	fgets(datoteka, MAX, stdin);

	if (datoteka[strlen(datoteka) - 1] == '\n')
		datoteka[strlen(datoteka) - 1] = '\0';
	return 0;
}

int IzbrisiNakon(listPoli P)
{
	listPoli temp = NULL;
	temp = P->next;
	P->next = temp->next;
	free(temp);
	return 0;
}

int IzbrisiElSaKoef0(listPoli P)
{
	while (P->next != NULL) {
		if (P->next->Koef == 0) {
			IzbrisiNakon(P);
		}
		P = P->next;
	}
	return 0;
}

int OslobodiMemoriju(listPoli P)
{
	while (P->next != NULL) {
		IzbrisiNakon(P);
	}
	return 0;
}