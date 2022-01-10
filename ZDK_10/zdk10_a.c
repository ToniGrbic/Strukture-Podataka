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
	char drzava[MAX_SIZE];
	Position next;
	StabloPos root;
}VezanaListaEl;

typedef struct _BinStabloCvor
{
	int brojStanovnika;
	char grad[MAX_SIZE];
	StabloPos left;
	StabloPos right;
}BinStabloCvor;

int InputString(char *file);
int ReadFromFiles(char *file, Position head);
int InsertToListAndTreeSorted(Position head, char *drzava, char *gradovi);
Position createListEl(char *drzava);
int InsertAfter(Position head, Position newEl);
StabloPos InsertGradoviToTree(char *gradoviFile, StabloPos root);
StabloPos Insert(StabloPos root, char *grad, int brojSt);
StabloPos createNewCvor(char *grad, int brojSt);
int PrintAll(Position head);
int InOrder(StabloPos root);
Position FindByCountryName(char *drzava, Position head);
int PrintGradovi(StabloPos root, int minBrStan);

int main() {
	char file[MAX] = "";
	VezanaListaEl Head = { .drzava = "",.next = NULL,.root = NULL };
	printf("Unesite ime datoteke:");
	InputString(file);
	ReadFromFiles(file, &Head);
	PrintAll(&Head);

	char Input[MAX]= "", drzava[MAX]= "";
	int minBrStanovnika;
	Position current = NULL;
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

	} while (strcmp(Input, "exit") != 0);
	return 0;
}

int InputString(char *str)
{
	fgets(str, MAX, stdin);

	if (str[strlen(str) - 1] == '\n')
		str[strlen(str) - 1] = '\0';
	return 0;
}

int ReadFromFiles(char *file, Position head)
{
	char buffer[MAX], drzava[MAX], gradoviFile[MAX];
	FILE *fp = NULL;
	int result = 0;
	fp = fopen(file, "r");
	if (fp == NULL)
	{
		perror("pogreska u otvaranju datoteke");
		return -1;
	}
	while (fgets(buffer, MAX, fp) != NULL)
	{
		result = sscanf(buffer, "%s %s", drzava, gradoviFile);
		if(result == 2)
			InsertToListAndTreeSorted(head, drzava, gradoviFile);
		
	}
	fclose(fp);
	return 0;
}

int InsertToListAndTreeSorted(Position head, char *drzava, char *gradoviFile)
{
	Position current = head;
	Position newEl = NULL;
	newEl=createListEl(drzava);
	if (newEl == NULL)
		return -1;
	newEl->root = InsertGradoviToTree(gradoviFile, newEl->root);

	while (current->next != NULL && strcmp(current->next->drzava, drzava) < 0)
		current = current->next;
	InsertAfter(current, newEl);
	return 0;
}

Position createListEl(char *drzava)
{
	Position q = NULL;
	q = (Position)malloc(sizeof(VezanaListaEl));
	if (!q) {
		perror("pogreska u alokaciji memorije\n");
		return NULL;
	}
	strcpy(q->drzava, drzava);
	q->next = NULL;
	q->root = NULL;
	return q;
}

int InsertAfter(Position head, Position newEl)
{
	newEl->next = head->next;
	head->next = newEl;
	return 0;
}

StabloPos InsertGradoviToTree(char *gradoviFile, StabloPos root)
{
	char buffer[MAX], grad[MAX];
	int brojSt;
	FILE *fp = NULL;
	fp = fopen(gradoviFile, "r");
	if (fp == NULL) {
		perror("pogreska u otvaranju datoteke");
		return NULL;
	}
	while (fgets(buffer, MAX, fp) != NULL)
	{
		sscanf(buffer, "%s %d", grad, &brojSt);
		root = Insert(root, grad, brojSt);
	}
	fclose(fp);
	return root;
}

StabloPos Insert(StabloPos root, char *grad, int brojSt)
{
	StabloPos node = root;
	if (node == NULL) {
		StabloPos temp = NULL;
		temp = createNewCvor(grad, brojSt);
		return temp;
	}
	else if (node->brojStanovnika < brojSt) {
		node->left = Insert(node->left, grad, brojSt);
	}
	else if (node->brojStanovnika > brojSt) {
		node->right = Insert(node->right, grad, brojSt);
	}else{
		printf("Vec imamo istu drzavu\n");
	}
	return root;
}

StabloPos createNewCvor(char *grad, int brojSt)
{
	StabloPos q = NULL;
	q = (StabloPos)malloc(sizeof(BinStabloCvor));
	if (!q) {
		perror("pogreska u alokaciji memorije\n");
		return NULL;
	}
	strcpy(q->grad, grad);
	q->brojStanovnika = brojSt;
	q->left = NULL;
	q->right = NULL;
	return q;
}

int PrintAll(Position head)
{
	Position current = head->next;
	while (current != NULL)
	{
		printf("Drzava: %s\n", current->drzava);
		printf(" Grad:                Broj Stanovnika:\n");
		InOrder(current->root);
		current = current->next;
	}
	return 0;
}

int InOrder(StabloPos root)
{
	StabloPos temp = root;
	if (temp != NULL) {
		InOrder(temp->left);
		printf(" %-20.20s %d\n", temp->grad, temp->brojStanovnika);
		InOrder(temp->right);
	}
	return 0;
}

Position FindByCountryName(char *drzava, Position head)
{
	Position current = head->next;
	while (current != NULL) {
		if (strcmp(current->drzava, drzava) == 0) {
			return current;
		}
		current = current->next;
	}
	if (current == NULL) {
		printf("drzava %s nepostoji\n", drzava);
	}
	return NULL;
}

int PrintGradovi(StabloPos root, int minBrStan)
{
	StabloPos temp = root;
	if (temp != NULL) {
		PrintGradovi(temp->left, minBrStan);
		if (temp->brojStanovnika >= minBrStan) 
			printf(" %-20.20s %d\n", temp->grad, temp->brojStanovnika);
		PrintGradovi(temp->right, minBrStan);
	}
	return 0;
}