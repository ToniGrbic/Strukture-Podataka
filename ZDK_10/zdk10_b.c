#define _CRT_SECURE_NO_WARNINGS
#define MAX (256)
#define MAX_SIZE (40)
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

struct _VezanaListaEl;
struct _BinStabloCvor;
typedef struct _VezanaListaEl *Position;
typedef struct _BinStabloCvor *StabloPos;

typedef struct _VezanaListaEl
{
	char grad[MAX_SIZE];
	int brojStanovnika;
	Position next;

}VezanaListaEl;

typedef struct _BinStabloCvor
{
	char drzava[MAX_SIZE];
	StabloPos left;
	StabloPos right;
	Position head;
}BinStabloCvor;

int InputString(char *file);
StabloPos ReadFromFiles(char *file, StabloPos root);
StabloPos Insert(StabloPos root, char *drzava, char *gradoviFile);
StabloPos createNewCvor(char *drzava, char *gradoviFile);
int InsertGradoviToList(char *gradoviFile, Position head);
int InsertToListSorted(Position current, Position newEl, int brStan);
Position createListEl(char *grad, int brojStanovnika);
int InsertAfter(Position head, Position newEl);
int PrintAll(StabloPos root);
int InOrder(StabloPos root);
StabloPos FindByCountryName(char *drzava, StabloPos root);
int PrintGradovi(Position head);
int PrintGradoviSaMinStan(StabloPos root, int minBrStan);

int main() {
	char file[MAX] = "";
	StabloPos root = NULL;
	printf("Unesite ime datoteke:");
	InputString(file);
	root = ReadFromFiles(file, root);
	PrintAll(root);

	/*char Input[MAX]= "", drzava[MAX]= "";
	int minBrStanovnika;
	//Position current = NULL;
	do {
		printf("Upisite drzavu koju zelite pretrazit:\n");
		InputString(drzava);
		current = FindByCountryName(drzava, &Head);
		if (current != NULL) {
			printf("Unesite donju granicu broja stanovnika gradova:\n");
			scanf("%d", &minBrStanovnika); getchar();
			printf("Gradovi sa vise od %d stanovnika:\n", minBrStanovnika);
			PrintGradovi(current->root, minBrStanovnika);
		}
		printf("za nastavak unesite neku tipku, a za izlaz exit\n");
		InputString(Input);

	} while (strcmp(Input, "exit") != 0);*/
	return 0;
}

int InputString(char *str)
{
	fgets(str, MAX, stdin);

	if (str[strlen(str) - 1] == '\n')
		str[strlen(str) - 1] = '\0';
	return 0;
}

StabloPos ReadFromFiles(char *file, StabloPos root)
{
	FILE *fp = NULL;
	
	fp = fopen(file, "r");
	if (fp == NULL)
	{
		perror("pogreska u otvaranju datoteke");
		return NULL;
	}
	
	while (!feof(fp))
	{
		char buffer[MAX] = "";
		char drzava[MAX] = "", gradoviFile[MAX] = "";
		fgets(buffer, MAX, fp);
		sscanf(buffer, "%s %s", drzava, gradoviFile);
		root = Insert(root, drzava, gradoviFile);
	}
	
	fclose(fp);
	return root;
}

StabloPos Insert(StabloPos root, char *drzava, char *gradoviFile)
{	StabloPos temp = NULL;
	StabloPos node = root;
	if (node == NULL) {
		temp = createNewCvor(drzava, gradoviFile);
		return temp;
	}
	else if (strcmp(node->drzava, drzava) > 0) {
		node->left = Insert(node->left, drzava, gradoviFile);
	}
	else if (strcmp(node->drzava, drzava) < 0) {
		node->right = Insert(node->right, drzava, gradoviFile);
	}
	return root;
}

StabloPos createNewCvor(char *drzava,char *gradoviFile)
{
	VezanaListaEl Head = { .grad = "",.brojStanovnika = 0,.next = NULL };
	Position head = &Head;
	StabloPos q = NULL;
	q = (StabloPos)malloc(sizeof(BinStabloCvor));
	if (!q) {
		perror("pogreska u alokaciji memorije\n");
		return NULL;
	}
	strcpy(q->drzava, drzava);
	q->left = NULL;
	q->right = NULL;
	q->head = head;
	InsertGradoviToList(gradoviFile, q->head);
	return q;
}

int InsertGradoviToList(char *gradoviFile, Position head)
{
	Position current = head;
	
	FILE *fp = NULL;
	fp = fopen(gradoviFile, "r");
	if (fp == NULL) {
		perror("pogreska u otvaranju datoteke");
		return -1;
	}
	
	while (!feof(fp))
	{
		char buffer[MAX] = { 0 }, grad[MAX] = { 0 };
		int brojSt;
		fgets(buffer, MAX, fp);
		sscanf(buffer, "%s %d", grad, &brojSt);
		Position newEl = NULL;
		newEl = createListEl(grad, brojSt);
		InsertToListSorted(current, newEl, brojSt);
	}
	fclose(fp);
	return 0;
}

int InsertToListSorted(Position current, Position newEl, int brStan) {
	while (current->next != NULL && current->next->brojStanovnika > brStan)
		current = current->next;
	InsertAfter(current, newEl);
	return 0;
}

Position createListEl(char *grad, int brojStanovnika)
{
	Position q = NULL;
	q = (Position)malloc(sizeof(VezanaListaEl));
	if (!q) {
		perror("pogreska u alokaciji memorije\n");
		return NULL;
	}
	strcpy(q->grad, grad);
	q->brojStanovnika = brojStanovnika;
	q->next = NULL;

	return q;
}

int InsertAfter(Position head, Position newEl)
{
	newEl->next = head->next;
	head->next = newEl;
	return 0;
}

int PrintAll(StabloPos root)
{
	InOrder(root);
	return 0;
}

int InOrder(StabloPos root)
{
	StabloPos temp = root;
	if (temp != NULL) {
		InOrder(temp->left);
		printf("Drzava: %s\n", temp->drzava);
		PrintGradovi(temp->head);
		InOrder(temp->right);
	}
	return 0;
}

StabloPos FindByCountryName(char *drzava, StabloPos root)
{
	StabloPos node = root;
	if (node == NULL) {
		return NULL;
	}
	else if (strcmp(node->drzava, drzava) > 0) {
		FindByCountryName(drzava, root->left);
	}
	else if (strcmp(node->drzava, drzava) < 0) {
		FindByCountryName(drzava, root->right);
	}
	else {
		return node;
	}
}

int PrintGradovi(Position head)
{
	Position current = head->next;
	printf(" Grad:                Broj Stanovnika:\n");
	while (current != NULL) {
		printf(" %-20.20s %d\n", current->grad, current->brojStanovnika);
		current = current->next;
	}
	return 0;
}

int PrintGradoviSaMinStan(StabloPos node, int minBrStan)
{
	Position current = node->head->next;
	printf(" Grad:                Broj Stanovnika:\n");
	while (current != NULL) {
		if (current->brojStanovnika >= minBrStan)
			printf(" %-20.20s %d\n", current->grad, current->brojStanovnika);
		current = current->next;
	}
	return 0;
}