#define _CRT_SECURE_NO_WARNINGS
#define MAX (100)
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
struct _BinStabloCvor;
typedef struct _BinStabloCvor *StabloPos;
typedef struct _BinStabloCvor
{
	int data;
	StabloPos left;
	StabloPos right;
}BinStabloCvor;

struct _VezanaListaEl;
typedef struct _VezanaListaEl *Position;
typedef struct _VezanaListaEl {
	int data;
	Position next;
}VezanaListaEl;

StabloPos createNewCvor(int broj);
Position createListEl(int broj);
StabloPos Insert(StabloPos root, int broj);
int Replace(StabloPos root);
int inOrderToList(StabloPos root, Position head);
int InsertToEndOfList(Position current,  StabloPos root);
int printList(Position head);
int DeleteAfter(Position P);
int DeleteList(Position P);
int InputFileName(char *file);
int InsertListToFile(Position head, char *datoteka);
int InOrder(StabloPos root);
int InsertAfter(Position current, Position newEl);

int main(){
		time_t t;
		srand((unsigned) time(&t));

		StabloPos root = NULL;
		VezanaListaEl Head = {.data=0, .next=NULL};
		Position head = &Head;
		char datoteka[MAX] = {0};
		int opcija,broj;
		InputFileName(datoteka);

		do{
			printf("MENI ZA BINARNO STABLO:\n");
			printf("0.Izlaz\n");
			printf("1.Unesi random element u stablo(raspon <10,90>)\n");
			printf("2.Unesi element u stablo\n");
			printf("3.Replace\n");
			printf("4.Unesi u datoteku\n");
			printf("5.Ispisi listu\n");
			printf("6.InOrder ispis\n");
			printf("Upisite vasu opciju:");
			do{
				scanf("%d", &opcija);
				if(opcija < 0 || opcija > 7){
					printf("ta opcija nepostoji, pokusajte ponovo...\n");
				}
			}while(opcija < 0 || opcija > 7);

		switch(opcija){
			case 0:
				printf("Izlaz...\n");
				break;
			case 1:
				broj = (rand() %(90 - 10 + 1)) + 10;
				printf("Random broj je: %d\n", broj);
				root = Insert(root, broj);
				break;
			case 2:
				printf("Unesite broj koji zelite pohranit:\n");
				scanf("%d", &broj);
				root = Insert(root, broj);
				break;
			case 3:
			    Replace(root);
				break;
			case 4:
				inOrderToList(root, head);
				InsertListToFile(head, datoteka);
				DeleteList(head);
				break;
			case 5:
				printList(head);
				break;
			case 6:
				InOrder(root);
				printf("\n");
				break;
		}
		if(opcija != 0){
            printf("pritisnite neku tipku za nastavak...");
            getch();
		}
		printf("\n");
	}while(opcija != 0);
	return 0;
}

StabloPos createNewCvor(int broj)
{
	StabloPos q = NULL;
	q=(StabloPos)malloc(sizeof(BinStabloCvor));
	if(!q){
		perror("pogreska u alokaciji memorije\n");
		return NULL;
	}
	q->data = broj;
	q->left = NULL;
	q->right = NULL;
	return q;
}

Position createListEl(int broj)
{
	Position q = NULL;
	q=(Position)malloc(sizeof(VezanaListaEl));
	if(!q){
		perror("pogreska u alokaciji memorije\n");
		return NULL;
	}
	q->data = broj;
	q->next = NULL;
	return q;
}

StabloPos Insert(StabloPos root, int broj)
{
	StabloPos node = root;
	if(node==NULL){
		StabloPos temp = NULL;
		temp = createNewCvor(broj);
		return temp;

	}else if(node->data < broj){
		node->left = Insert(node->left, broj);

	}else if(node->data > broj){
		node->right = Insert(node->right, broj);

	}else{
		node->left = Insert(node->left, broj);
	}
	return root;
}

int Replace(StabloPos root)
{
	int leftSum = 0, rightSum =0, prev;
	if(root == NULL)
	{
		return 0;
	}
    prev = root->data;
	leftSum += Replace(root->left);
	rightSum += Replace(root->right);
	root->data = leftSum + rightSum;
	return root->data + prev;
}

int inOrderToList(StabloPos root, Position head)
{
    StabloPos temp = root;
    if(temp == NULL)
        return 0;
    inOrderToList(temp->left, head);
	InsertToEndOfList(head, temp);
	inOrderToList(temp->right, head);
}

int InsertToEndOfList(Position head,  StabloPos node)
{
    Position current = head;
	Position newEl = createListEl(node->data);
	if (newEl == NULL)
		return -1;

    while(current->next != NULL)
        current = current->next;
	InsertAfter(current, newEl);
	return 0;
}

int printList(Position head)
{
    Position current = head->next;
    while(current != NULL)
    {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

int DeleteAfter(Position P){
    Position temp = NULL;
    temp = P->next;
    P->next = P->next->next;
    free(temp);
}

int DeleteList(Position P)
{
    if(P->next = NULL){
        printf("lista je prazna\n");
    }
    while(P->next != NULL)
    {
        DeleteAfter(P);
    }
    return 0;
}

int InOrder(StabloPos root)
{
	StabloPos temp = root;
	if(temp != NULL){
		InOrder(temp->left);
		printf("%d ", temp->data);
		InOrder(temp->right);
	}
	return 0;
}

int InsertAfter(Position head, Position newEl)
{
	newEl->next = head->next;
	head->next = newEl;
	return 0;
}

int InputFileName(char *file)
{
	printf("Unesite ime datoteke:\n");
	fgets(file, MAX, stdin);

	if (file[strlen(file) - 1] == '\n')
		file[strlen(file) - 1] = '\0';
	return 0;
}

int InsertListToFile(Position head, char *datoteka)
{
    FILE *fp = NULL;
    fp = fopen(datoteka, "a");
    if(!fp){
        printf("pogreska u otvaranju datoteke!\n");
        return -1;
    }
    Position temp = head->next;
    while(temp != NULL){
        fprintf(fp, "%d ", temp->data);
        temp = temp->next;
    }
	fprintf(fp,"\n");
    fclose(fp);
    return 0;
}