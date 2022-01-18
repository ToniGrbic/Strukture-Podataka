#include "zdk11.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int InputString(char *str)
{
	fgets(str, MAX, stdin);

	if (str[strlen(str) - 1] == '\n')
		str[strlen(str) - 1] = '\0';
	return 0;
}

int ReadFromFiles(char *file, hashT H)
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
		if (result == 2)
			InsertToHashListAndTreeSorted(H, drzava, gradoviFile);
	}
	fclose(fp);
	return 0;
}

hashT InicializeHashTab(int velTab)
{
	hashT H;
	H = (hashT)malloc(sizeof(hashTab));
	if (H == NULL) {
		printf("Greska kod inicijalizacije memorije!");
		return NULL;
	}
	H->velTab = velTab;
	H->hashLista = (Position*)malloc(sizeof(Position)*H->velTab);
	if (H->hashLista == NULL) {
		printf("Greska kod inicijalizacije memorije!");
		return NULL;
	}
	for (int i = 0; i < velTab; i++) {
		H->hashLista[i] = NULL;
	}
	return H;
}

int HashFunction(char *drzava, int velTab)
{
	
	int result = 0;
	for (int i = 0; i < 5; i++){
		if(drzava[i] == '\0')
			break;	/*ako drzava ima manje od 5 slova izlazimo iz petlje*/
		result += drzava[i];
	}
		
	return result % velTab;
}

int InsertToHashListAndTreeSorted(hashT H, char *drzava, char *gradoviFile)
{
	Position newEl = NULL;
	Lista L = NULL;
	int hashKey = HashFunction(drzava, H->velTab);

	newEl = createListEl(drzava);
	if (newEl == NULL)
		return -1;
	newEl->root = InsertGradoviToTree(gradoviFile, newEl->root);

	/* ako je 1. pozicija liste prazna stavi tu novi El, ako nije unesi sortirano u tu listu*/
	if(!H->hashLista[hashKey])
		H->hashLista[hashKey] = newEl;
	else {
		L = H->hashLista[hashKey];
		while (L->next != NULL && strcmp(L->next->drzava, drzava) < 0)
			L = L->next;
		InsertAfter(L, newEl);
	}
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

int InsertAfter(Position current, Position newEl)
{
	newEl->next = current->next;
	current->next = newEl;
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
	/* prvo uspoređuje po broj Stanovnika, ako je broj isti, usporeduje po nazivu grada*/
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
	}
	else if(strcmp(node->grad, grad) < 0){
		node->left = Insert(node->left, grad, brojSt);
	}
	else if(strcmp(node->grad, grad) > 0){
		node->right = Insert(node->right, grad, brojSt);
	}else{
		printf("Vec imamo isti grad: %s\n", node->grad);
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

int PrintAll(hashT H)
{
	Lista L = NULL;
	for (int i = 0; i < H->velTab; i++) {
		L = H->hashLista[i];
		if (L != NULL) {
			while (L != NULL) {
				printf("Drzava: %s\n", L->drzava);
				printf(" Grad:                Broj Stanovnika:\n");
				InOrder(L->root);
				L = L->next;
			}
		}
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

Position FindByCountryName(char *drzava, hashT H)
{
	Lista L = NULL;
	L = H->hashLista[HashFunction(drzava, H->velTab)];
	while (L != NULL) {
		if (strcmp(L->drzava, drzava) == 0) {
			return L;
		}
		L = L->next;
	}
	if (L == NULL) {
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

int DealocateHashTab(hashT H)
{
	Lista L = NULL;
	for(int i=0;i<H->velTab; i++)
	{
		L = H->hashLista[i];
		if (L != NULL) {
			DealocateList(L);
		}
	}
	return 0;
}

int DealocateList(Lista L)
{
	while(L->next != NULL){
		DealocateCvor(L);
	}
	return 0;
}

int DealocateCvor(Lista L){
	Position temp = NULL;

	DealocateTree(L->root);
    temp = L->next;
    L->next = temp->next;
    free(temp);
	return 0;
}

StabloPos DealocateTree(StabloPos node)
{
	if (!node){
        return NULL;
    }
    node->left = DealocateTree(node->left);
    node->right = DealocateTree(node->right);
    FreeTreeCvor(node);
	return NULL;
}

int FreeTreeCvor(StabloPos node){
	if (!node){
        return 0;
    }
    if (node->grad){
        free(node->grad);
    }
    free(node);
	return 0;
}
 