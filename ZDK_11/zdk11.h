#define _CRT_SECURE_NO_WARNINGS
#define MAX (256)
#define MAX_SIZE (40)
#define VEL_TAB (11)
#pragma once
struct _VezanaListaEl;
struct _BinStabloCvor;
struct _hashTab;
typedef struct _VezanaListaEl *Position;
typedef struct _VezanaListaEl *Lista;
typedef struct _BinStabloCvor *StabloPos;
typedef struct _hashTab *hashT;

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

typedef struct _hashTab
{
	int velTab;
	Lista *hashLista;
}hashTab;

int InputString(char *file);
int ReadFromFiles(char *file, hashT H);
hashT InicializeHashTab(int velTab);
int HashFunction(char *drzava, int velTab);
int InsertToHashListAndTreeSorted(hashT H, char *drzava, char *gradovi);
Position createListEl(char *drzava);
int InsertAfter(Position head, Position newEl);
StabloPos InsertGradoviToTree(char *gradoviFile, StabloPos root);
StabloPos Insert(StabloPos root, char *grad, int brojSt);
StabloPos createNewCvor(char *grad, int brojSt);
int PrintAll(hashT H);
int InOrder(StabloPos root);
Position FindByCountryName(char *drzava, hashT H);
int PrintGradovi(StabloPos root, int minBrStan);
/*Dealokacija*/
int DealocateHashTab(hashT H);
int DealocateList(Lista L);
int DealocateCvor(Lista L);
StabloPos DealocateTree(StabloPos node);
int FreeTreeCvor(StabloPos node);