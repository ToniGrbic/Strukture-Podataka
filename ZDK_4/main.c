#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define MAX (80)
#define MAX_CHAR (1024)

struct _Polinom;
typedef struct _Polinom *listPoli;
typedef struct _Polinom{
    int Koef;
    int Pot;
    listPoli next;
}Polinom;

int UnesiPolinomeIzDatoteke( listPoli Pol1, listPoli Pol2, char *datoteka);
int UnesiSortirano(listPoli P, int koef, int pot);
int FormatirajIUnesi(listPoli Pol, char *buffer);
int ZbrojDvaPolinoma(listPoli Sum, listPoli P1, listPoli P2);
int UmnozakDvaPolinoma(listPoli Product, listPoli P1, listPoli P2);
Polinom* UnosPodataka(int koef, int pot);
int InsertNakon(listPoli P, listPoli Q);
int IspisiPolinom(listPoli P);
int UnesiImeDatoteke(char *datoteka);
int IzbrisiNakon(listPoli P);
int OslobodiMemoriju(listPoli P);

int main(){
 char datoteka[MAX] ={0};
 
 Polinom Pol1 = {.Koef=0, .Pot=0, .next=NULL};
 Polinom Pol2 = {.Koef=0, .Pot=0, .next=NULL};
 Polinom zbrojPol = {.Koef=0, .Pot=0, .next=NULL};
 Polinom umnozakPol = {.Koef=0, .Pot=0, .next=NULL};

 UnesiImeDatoteke(datoteka);
  
 if(UnesiPolinomeIzDatoteke(&Pol1, &Pol2, datoteka) != 0){
     return 1;
 }
 IspisiPolinom(Pol1.next);
 IspisiPolinom(Pol2.next);
 
 ZbrojDvaPolinoma(&zbrojPol, &Pol1, &Pol2);
 IspisiPolinom(zbrojPol.next);
 
 UmnozakDvaPolinoma(&umnozakPol, &Pol1, &Pol2);
 IspisiPolinom(umnozakPol.next);
 
 OslobodiMemoriju(&Pol1);
 OslobodiMemoriju(&Pol2);
 OslobodiMemoriju(&zbrojPol);
 OslobodiMemoriju(&umnozakPol);
 return 0;
}

int UnesiPolinomeIzDatoteke( listPoli Pol1, listPoli Pol2, char *datoteka)
{
    char buffer[MAX_CHAR]= {0};
    int returnValue = 0;
    FILE *fp = NULL;
    fp = fopen(datoteka, "r");
    if(!fp){
        printf("pogreska u otvaranju datoteke\n");
        return -1;
    }
    fgets(buffer, MAX_CHAR, fp);
    returnValue= FormatirajIUnesi(Pol1, buffer);
    if(returnValue != 0){
        return 1;
    }

    fgets(buffer, MAX_CHAR, fp);
    returnValue = FormatirajIUnesi(Pol2, buffer);
    if(returnValue != 0){
        return 1;
    }
    fclose(fp);
    return 0;
}

int FormatirajIUnesi(listPoli P, char *buffer)
{
    char *currentBuffer = buffer;
    int koef=0, pot=0;
    int brojBajt = 0;

    while(strlen(currentBuffer) > 0)
    {
        if(sscanf(currentBuffer, "%d %d %n", &koef, &pot, &brojBajt) == 2){
            UnesiSortirano(P, koef, pot);
            currentBuffer += brojBajt;
        }else{
            printf("datoteka nije ispravna\n");
            return 1;
        }
    }
    return 0;
}

int UnesiSortirano(listPoli P, int koef, int pot)
{
    listPoli temp = P;
    if(koef == 0)
        return 0;
    
    while(P->next!= NULL && P->next->Pot >= pot){
        P=P->next;
    }
    
    if(P!=NULL && P->Pot == pot){
        P->Koef += koef;
    }else{
        listPoli Q = NULL;
        Q = UnosPodataka(koef, pot);
        InsertNakon(P, Q);
    }

    while(temp->next!= NULL)
    {
        if(temp->next->Koef == 0){
            IzbrisiNakon(temp);
        }
        temp=temp->next;
    }
    free(temp);
    return 0;
}

int ZbrojDvaPolinoma(listPoli Sum, listPoli P1, listPoli P2){
    listPoli temp = NULL;
    listPoli tempP1 = P1;
    listPoli tempP2 = P2;

    printf("Zbroj polinoma:\n");
    while(tempP1 != NULL && tempP2 != NULL)
    {
        if( tempP1->Pot == tempP2->Pot ){
            UnesiSortirano(Sum, tempP1->Koef + tempP2->Koef, tempP1->Pot);
            tempP1=tempP1->next;
            tempP2=tempP2->next;

        }else if(tempP1->Pot > tempP2->Pot){
            UnesiSortirano(Sum, tempP1->Koef, tempP1->Pot);
            tempP1=tempP1->next;
        }else{
            UnesiSortirano(Sum, tempP2->Koef, tempP2->Pot);
            tempP2=tempP2->next;
        }
    }

    if(tempP1 == NULL){
        temp = tempP2;
    }else{
        temp = tempP1;
    }

    while(temp != NULL){
        UnesiSortirano(Sum, temp->Koef, temp->Pot);
        temp = temp->next;
    }
    free(temp);
    free(tempP1);
    free(tempP2);
    return 0;
}

int UmnozakDvaPolinoma(listPoli Product, listPoli P1, listPoli P2)
{
    printf("Umnozak polinoma:\n");
    listPoli startP2 = P2;
    listPoli tempP1 = P1;
    listPoli tempP2 = P2;
    
    while(tempP1!=NULL){
        while(tempP2!=NULL){
            UnesiSortirano(Product, tempP1->Koef * tempP2->Koef, tempP1->Pot + tempP2->Pot);
            tempP2=tempP2->next;
        }
        tempP2 = startP2;
        tempP1=tempP1->next;
    }
    free(startP2);
    free(tempP1);
    free(tempP2);
    return 0;
}

Polinom* UnosPodataka(int koef, int pot)
{
    listPoli Q = NULL;
    Q = (listPoli)malloc(sizeof(Polinom));
    if(!Q){
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

int IspisiPolinom(listPoli P)
{
    if(P==NULL){
        printf("nema elemenata u listi\n");
        return 1;
    }
    while(P!=NULL){
        
        if(P->Pot == 0){
            printf("%d", P->Koef);
        }else if(P->Koef == 1){
            printf("x^%d", P->Pot);
        }else{
            printf("%dx^%d", P->Koef, P->Pot);
        }

        if(P->next != NULL && P->next->Koef > 0){
            printf("+");
        }else if( P->next != NULL && P->next->Koef < 0){
            printf("");
        }else if(P->next == NULL){
            printf("\n");
        }
        P=P->next;
    }
    return 0;
}

int UnesiImeDatoteke(char *datoteka)
{
    printf("Unesite ime datoteke:\n");
	fgets(datoteka, MAX, stdin);
	
    if(datoteka[strlen(datoteka)-1] == '\n')
        datoteka[strlen(datoteka)-1] = '\0';
    return 0;
}

int IzbrisiNakon(listPoli P)
{
    listPoli temp = NULL;
    temp = P->next;
    P->next = P->next->next;
    free(temp);
    return 0;
}

int OslobodiMemoriju(listPoli P)
{ 
    while(P->next != NULL){
        IzbrisiNakon(P);
    }
    return 0;
}

