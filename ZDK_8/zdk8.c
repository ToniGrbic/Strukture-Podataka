#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"

struct _BinStabloCvor;
typedef struct _BinStabloCvor *StabloPos;
typedef struct _BinStabloCvor
{
	int data;
	StabloPos left;
	StabloPos right;
}BinStabloCvor;

StabloPos createNewCvor(int broj);
StabloPos Insert(StabloPos root, int broj);
StabloPos deleteCvor(StabloPos root, int broj);
StabloPos getMax(StabloPos currentCvor);
StabloPos getMin(StabloPos currentCvor);
StabloPos getElement(StabloPos root, int broj);
int InOrder(StabloPos root);
int PreOrder(StabloPos root);
int PostOrder(StabloPos root);

int main(){
	
	StabloPos root = NULL, trazeniCvor = NULL, currentCvor = NULL, noviCvor = NULL;
	int opcija, broj;
	do{
		printf("MENI ZA BINARNO STABLO PRETRAZIVANJA:\n");
		printf("0.Izlaz\n");
		printf("1.Unesi element u stablo\n");
		printf("2.Pretrazi element u stablu\n");
		printf("3.Izbrisi element iz stabla\n");
		printf("4.InOrder ispis\n");
		printf("5.PreOrder ispis\n");
		printf("6.PostOrder ispis\n");

		printf("Upisite vasu opciju:");
		do{
			scanf("%d", &opcija);
			if(opcija < 0 || opcija > 6){
				printf("ta opcija nepostoji, pokusajte ponovo...\n");
			}
		}while(opcija < 0 || opcija > 6);

		switch(opcija){
			case 0:
				printf("Izlaz...\n");
				break;
			case 1:
				printf("Unesite broj koji zelite pohranit\n");
				scanf("%d", &broj);
				if(root==NULL){
					root = Insert(root, broj);
					printf("root je %d\n", root->data);
				}else{
					noviCvor = Insert(root, broj);
					if(noviCvor != NULL){
						printf("Uspjesan unos\n");
					}else{
						printf("Neuspjesan unos\n");
					}
				}
				break;
			case 2:
				printf("Unesite broj koji zelite pretrazit\n");
				scanf("%d", &broj);
				trazeniCvor = getElement(root, broj);
				if(trazeniCvor != NULL){
					printf("Cvor sa brojem %d postoji\n", trazeniCvor->data);
				}else{
					printf("Cvor sa brojem %d nepostoji\n", broj);
				}
				break;
			case 3:
				printf("Unesite broj koji zelite izbrisati\n");
				scanf("%d", &broj);
				currentCvor = deleteCvor(root, broj);
				if(currentCvor != NULL){
					printf("Cvor uspjesno izbrisan\n");
				}else{
					printf("Taj Cvor nepostoji\n");
				}
				break;
			case 4:
				InOrder(root);
				printf("\n");
				break;
			case 5:
				PreOrder(root);
				printf("\n");
				break;
			case 6:
				PostOrder(root);
				printf("\n");
				break;
		}
		if(opcija != 0){
            printf("pritisnite neku tipku za nastavak...");
            getch();
		}
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

StabloPos Insert(StabloPos root, int broj)
{
	if(root==NULL){
		StabloPos temp = NULL;
		temp = createNewCvor(broj);
		return temp;

	}else if(root->data > broj){
		root->left = Insert(root->left, broj);

	}else if(root->data < broj){
		root->right = Insert(root->right, broj);

	}else{
		printf("Vec postoji Cvor sa brojem %d\n", broj);
	}
	return root;
}
StabloPos deleteCvor(StabloPos root, int broj)
{
	StabloPos currentCvor = root;
	StabloPos temp = NULL;
	if(currentCvor==NULL){
		return currentCvor;
	}else if(currentCvor->data > broj){
		currentCvor->left = deleteCvor(currentCvor->left, broj);

	}else if(currentCvor->data < broj){
		currentCvor->right = deleteCvor(currentCvor->right, broj);

	}else{
		if(currentCvor->left !=NULL && currentCvor->right != NULL)
		{
			temp = currentCvor;
			temp = getMax(currentCvor->left);
			currentCvor->data = temp->data;
			currentCvor->left = deleteCvor(currentCvor->left, temp->data);
		}else{

		    temp = currentCvor;
			if(currentCvor->left == NULL ){
				currentCvor = currentCvor->right;
				free(temp);

			}else if(currentCvor->right == NULL){
				currentCvor = currentCvor->left;
				free(temp);
			}
		}
	}
	return currentCvor;
}

StabloPos getMax(StabloPos currentCvor)
{
	while(currentCvor->right != NULL)
	{
		currentCvor = currentCvor->right;
	}
	return currentCvor;
}

StabloPos getMin(StabloPos currentCvor)
{
	while(currentCvor->left != NULL)
	{
		currentCvor = currentCvor->left;
	}
	return currentCvor;
}

StabloPos getElement(StabloPos root, int broj)
{
	StabloPos currentCvor = root;
	if(currentCvor == NULL){
		return NULL;
	}else if(currentCvor->data > broj){
		return getElement(currentCvor->left, broj);
	}else if(currentCvor->data < broj){
		return getElement(currentCvor->right, broj);
	}else{
		return currentCvor;
	}
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

int PreOrder(StabloPos root)
{
	StabloPos temp = root;
	if(temp != NULL){
		printf("%d ", temp->data);
		PreOrder(temp->left);
		PreOrder(temp->right);
	}
	return 0;
}

int PostOrder(StabloPos root)
{
	StabloPos temp = root;
	if(temp != NULL){
		PostOrder(temp->left);
		PostOrder(temp->right);
		printf("%d ", temp->data);
	}
	return 0;
}

