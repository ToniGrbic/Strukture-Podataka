#define _CRT_SECURE_NO_WARNINGS
#define MAX (256)
#define MAX_SIZE (40)
#pragma once

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
int PrintGradoviSaMinBrSt(StabloPos root, int minBrStan);

int DealocateList(Position current);
int DealocateCvor(Position current);
StabloPos DealocateTree(StabloPos node);
int FreeTreeCvor(StabloPos node);