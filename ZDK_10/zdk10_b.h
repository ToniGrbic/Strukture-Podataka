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
int InsertToListSorted(Position current, Position newEl, int brStan, char *grad);
Position createListEl(char *grad, int brojStanovnika);
int InsertAfter(Position head, Position newEl);
int InsertGradoviToList(char *gradoviFile, Position head);
StabloPos Insert(StabloPos root, char *drzava, char *gradoviFile);
StabloPos createNewCvor(char *drzava, char *gradoviFile);
int PrintAll(StabloPos root);
int InOrder(StabloPos root);
StabloPos FindByCountryName(char *drzava, StabloPos root);
int PrintGradovi(Position head);
int PrintGradoviSaMinStan(Position head, int minBrStan);
/*dealokacija*/
int DealocateList(Position current);
int DealocateCvor(Position current);
StabloPos DealocateTree(StabloPos node);
int FreeTreeCvor(StabloPos node);