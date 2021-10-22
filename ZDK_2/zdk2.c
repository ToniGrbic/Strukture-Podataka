#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define MAX (40)

struct _Student;
typedef struct _Student *Pozicija;
typedef struct _Student{
    char ime[MAX];
    char prezime[MAX];
    int godinaRodenja;
    Pozicija next;
}Student;

int UnesiNaPocetak(Pozicija P, char *ime, char *prezime, int godinaRodenja);
int UnesiNaKraj(Pozicija P, char *ime, char *prezime, int godinaRodenja);
int IspisiListu(Pozicija P);
int IspisiStudenta(Pozicija P);
int PretraziPoPrezimenu(Pozicija P, char *prezime);
Student* UnesiPodatke(char *ime, char *prezime, int godinaRodenja);
Student* TraziPrethodnog(Pozicija P, char *prezime);
int InsertNakon(Pozicija P, Pozicija noviStudent);
int IzbrisiStudenta(Pozicija P, char* prezime);

int main()
{
    Student Head = {.ime = " ", .prezime = " ", .godinaRodenja= 0, .next = NULL};
    Pozicija P = &Head;
    Pozicija Stud;
    UnesiNaPocetak(P,"toni", "grbic", 2001);
    UnesiNaPocetak(P,"mate", "matic", 2000);
    UnesiNaKraj(P,"ivo", "ivic", 2002);

    IspisiListu(P->next);
    PretraziPoPrezimenu(P, "matic");
    
    IzbrisiStudenta(P, "grbic");
    IspisiListu(P->next);
    IzbrisiStudenta(P, "matic");
    IspisiListu(P->next);
    //provjera za brisanje kada student nije u listi
    IzbrisiStudenta(P, "antic");
    IspisiListu(P->next);
    //brisanje zadnjeg elementa
    IzbrisiStudenta(P, "ivic");
    IspisiListu(P->next);
    return 0;
}

int UnesiNaPocetak(Pozicija P, char *ime, char *prezime, int godinaRodenja)
{
     Pozicija noviStud = NULL;
     noviStud = UnesiPodatke(ime, prezime, godinaRodenja);
    if(!noviStud){
        return -1;
    }
     InsertNakon(P, noviStud);
     return 0;
}

int UnesiNaKraj(Pozicija P, char *ime, char *prezime, int godinaRodenja)
{
    while(P->next!=NULL){
        P= P->next;
    }
    UnesiNaPocetak(P, ime, prezime, godinaRodenja);
    return 0;
}

int IspisiListu(Pozicija P)
{   Pozicija temp = P;
    while(temp != NULL){
        printf("ime: %s, prezime: %s, "
        "godinaRodenja: %d\n", temp->ime, temp->prezime, temp->godinaRodenja);
        temp = temp->next;
    }
    printf("\n");
    return 0;
}

int InsertNakon(Pozicija P, Pozicija noviStudent)
{
    noviStudent->next = P->next;
    P->next = noviStudent;
    return 0;
}

int IzbrisiStudenta(Pozicija P, char* prezime)
{
    Pozicija temp;
    P = TraziPrethodnog(P, prezime);
    if(!P){
        printf("u listi nepostoji student %s, brisanje neuspjesno\n", prezime);
        return 1;
    }
    temp = P->next;
    P->next = P->next->next;
    free(temp);
    return 0;
}

Student* UnesiPodatke(char *ime, char *prezime, int godinaRodenja)
{
     Pozicija temp = NULL;
     temp = (Pozicija)malloc(sizeof(Student));
    if(!temp){
        perror("pogreska u alokaciji memorije\n");
        return NULL;
    }
     strcpy(temp->ime, ime);
     strcpy(temp->prezime, prezime);
     temp->godinaRodenja = godinaRodenja;
     temp->next = NULL;
     return temp;
}

Student* TraziPrethodnog(Pozicija P, char *prezime)
{
    while(P->next!=NULL){
        if(strcmp(P->next->prezime, prezime) == 0){
            break;
        }
        P = P->next;
    }
    if(P->next == NULL){
        return NULL;
    }else{
        return P;
    }
}

int PretraziPoPrezimenu(Pozicija P, char *prezime)
{
    while(P!=NULL){
        if(strcmp(P->prezime, prezime) == 0){
            printf("student %s postoji\n", prezime);
            return 0; 
        }
        P = P->next;
    }
    printf("student %s nepostoji\n", prezime);
    return 1;
}