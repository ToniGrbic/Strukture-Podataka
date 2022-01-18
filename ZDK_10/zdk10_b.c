#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "zdk10_b.h"

int main() {
	char file[MAX] = { 0 }, Input[MAX]= { 0 }, drzava[MAX]= { 0 };
	StabloPos root = NULL;
	StabloPos current = NULL;
	int minBrStanovnika;

	printf("Unesite ime datoteke:");
	InputString(file);
	root = ReadFromFiles(file, root);
	PrintAll(root);

	do {
		printf("Upisite drzavu koju zelite pretrazit:\n");
		InputString(drzava);
		current = FindByCountryName(drzava, root);
		if (current != NULL) {
			printf("Drzava %s postoji!\n", current->drzava);
			printf("Unesite donju granicu broja stanovnika gradova:\n");
			scanf("%d", &minBrStanovnika); getchar();
			printf("Gradovi sa %d ili vise stanovnika:\n", minBrStanovnika);
			PrintGradoviSaMinStan(current->head, minBrStanovnika);
		}else{
			printf("Drzava %s nepostoji!\n", drzava);
		}
		printf("za nastavak unesite neku tipku, a za izlaz exit\n");
		InputString(Input);

	} while (strcmp(Input, "exit") != 0);
	root = DealocateTree(root);
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
	char buffer[MAX] = { 0 };
	char drzava[MAX] = { 0 };
	char gradoviFile[MAX] = { 0 };
	int result = 0;
	fp = fopen(file, "r");
	if (fp == NULL)
	{
		perror("pogreska u otvaranju datoteke");
		return NULL;
	}
	
	while (!feof(fp))
	{
		fgets(buffer, MAX, fp);
		result=sscanf(buffer, "%s %s", drzava, gradoviFile);
		if(result == 2)
			root = Insert(root, drzava, gradoviFile);
	}
	
	fclose(fp);
	return root;
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

int InsertGradoviToList(char *gradoviFile, Position head)
{
	Position newEl = NULL;
	Position current = head;
	char buffer[MAX] = { 0 };
	char grad[MAX] = { 0 };
	int brojSt = 0;
	int result = 0;
	FILE *fp = NULL;
	
	fp = fopen(gradoviFile, "r");
	if (fp == NULL) {
		perror("pogreska u otvaranju datoteke");
		return -1;
	}
	
	while (!feof(fp))
	{
		fgets(buffer, MAX, fp);
		result = sscanf(buffer, "%s %d", grad, &brojSt);
		if (result == 2) {
			newEl = createListEl(grad, brojSt);
			if(!newEl) {
				fclose(fp);
				return -2;
			}
			InsertToListSorted(current, newEl, brojSt, grad);
		}
	}
	fclose(fp);
	return 0;
}

int InsertToListSorted(Position current, Position newEl, int brStan, char *grad) {
	while (current->next != NULL && 
	(current->next->brojStanovnika > brStan || 
	(current->next->brojStanovnika == brStan  && strcmp(current->next->grad, grad)>0)))
		current = current->next;
	InsertAfter(current, newEl);
	return 0;
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
	else {
		printf("imamo istu drzavu\n");
	}
	return root;
}

StabloPos createNewCvor(char *drzava,char *gradoviFile)
{
	StabloPos q = NULL;
	q = (StabloPos)malloc(sizeof(BinStabloCvor));
	if (!q) {
		perror("pogreska u alokaciji memorije\n");
		return NULL;
	}
	strcpy(q->drzava, drzava);
	q->left = NULL;
	q->right = NULL;
	q->head = malloc(sizeof(VezanaListaEl));
	if (!q->head) {
		perror("pogreska u alokaciji memorije\n");
		free(q);
		return NULL;
	}
	else {
		q->head->next = NULL;
		q->head->brojStanovnika = 0;
		strcpy(q->head->grad, "");
	}
		
	InsertGradoviToList(gradoviFile, q->head);
	return q;
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

int PrintGradoviSaMinStan(Position head, int minBrStan)
{
	Position current = head->next;
	printf(" Grad:                Broj Stanovnika:\n");
	while (current != NULL) {
		if (current->brojStanovnika >= minBrStan)
			printf(" %-20.20s %d\n", current->grad, current->brojStanovnika);
		current = current->next;
	}
	return 0;
}

StabloPos DealocateTree(StabloPos node)
{
	if (!node) {
		return NULL;
	}
	node->left = DealocateTree(node->left);
	node->right = DealocateTree(node->right);
	FreeTreeCvor(node);
	return NULL;
}

int FreeTreeCvor(StabloPos node) {
	if (!node) {
		return 0;
	}
	DealocateList(node->head);
	free(node);
	return 0;
}

int DealocateList(Position current)
{
	while(current->next != NULL){
		DealocateCvor(current);
	}
	return 0;
}

int DealocateCvor(Position current){
	Position temp = NULL;
    temp = current->next;
    current->next = temp->next;
    free(temp);
	return 0;
}