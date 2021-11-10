#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define MAX (40)
#define MAX_CHAR (1024)

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
int UnesiSortirano(Pozicija P, char *ime, char *prezime, int godinaRodenja);
int UnesiNakonStud(Pozicija P, char *ime, char *prezime, int godinaRodenja, char *prezimePrije);
int UnesiPrijeStud(Pozicija P, char *ime, char *prezime, int godinaRodenja, char *prezimeNakon);
int PretraziPoPrezimenu(Pozicija P, char *prezime);
Student* UnesiPodatke(char *ime, char *prezime, int godinaRodenja);
Student* TraziPrethodnog(Pozicija P, char *prezime);
Student* TraziTrenutnog(Pozicija P, char *prezime);
int InsertNakon(Pozicija P, Pozicija noviStudent);
int IspisiListu(Pozicija P);
int IzbrisiStudenta(Pozicija P, char* prezime);
int IzbrisiListu(Pozicija P);
int Izbrisi(Pozicija P);
int UnesiListuUDatoteku(Pozicija P, char *datoteka);
int UcitajListuIzDatoteke(Pozicija P, char *datoteka);

int main()
{
    Student Head = {.ime = " ", .prezime = " ", .godinaRodenja= 0, .next = NULL};
    Pozicija P = &Head;
    Student Head2 = {.ime = " ", .prezime = " ", .godinaRodenja= 0, .next = NULL};
    Pozicija Q = &Head2;

    int odabir;
    char ime[MAX];
    char prezime[MAX], prezimePrije[MAX], prezimeNakon[MAX], pretraziPrezime[MAX], izbrisiStudenta[MAX];
    int godinaRodenja;
    char znak;

    do{
        printf("MENI ZA VEZANU LISTU:\n"
            	"0.Izlaz\n"
                "1.UnesiSortirano\n"
                "2.UnesiNaPocetak\n"
                "3.UnesiNaKraj\n"
                "4.UnesiNakonStud\n"
                "5.UnesiPrijeStud\n"
                "6.UnesiListuUDatoteku\n"
                "7.UcitajListuIzDatoteke\n"
                "8.IspisiListu\n"
                "9.PretraziPoPrezimenu\n"
                "10.IzbrisiStudeta\n");
        printf("\n");

        printf("Unesite opciju 0 do 10: ");
        do{
            scanf("%d", &odabir);
            if(odabir < 0 || odabir > 10)
                printf("krivi unos, pokusajte ponovo...\n");
            
        }while( odabir < 0 || odabir > 10);
        
        if(odabir != 0 && odabir < 6){
            printf("Unos podataka za studenta:\n");
            printf("Ime: ");
            scanf("%s", ime);
            printf("Prezime: ");
            scanf("%s", prezime);
            printf("GodinaRodenja: ");
            scanf("%d", &godinaRodenja); 
        }
        
        switch(odabir){
            case 0:
                 printf("Izlaz...\n");
                 break;
            case 1: 
                UnesiSortirano(P, ime, prezime, godinaRodenja);
                break;
            case 2:
                UnesiNaPocetak(P, ime, prezime, godinaRodenja);
                break;
            case 3:
                UnesiNaKraj(P, ime, prezime, godinaRodenja);
                break;
            case 4:
                printf("Unesi prezime nakon kojeg ce se unjeti novi element:\n");
                scanf("%s", prezimePrije);
                UnesiNakonStud(P, ime, prezime, godinaRodenja, prezimePrije);
                break;
            case 5:
                printf("Unesi prezime prije kojeg ce se unjeti novi element:\n");
                scanf("%s", prezimeNakon);
                UnesiPrijeStud(P, ime, prezime, godinaRodenja, prezimeNakon);
                break;
            case 6:
                if(UnesiListuUDatoteku(P, "osobeUnos.txt") == 0){
                    printf("Lista uspjesno unesena u datoteku!\n");
                }else{
                    printf("Neuspjesni unos, greska!\n");
                }
                break;
            case 7:
                if(UcitajListuIzDatoteke(Q, "osobe.txt") == 0){
                    printf("Lista uspjesno ucitanja iz datoteke!\n");
                }else{
                    printf("Neuspjesno ucitavanje, greska!\n");
                }
                break;
            case 8:
                printf("Lista P:\n");
                IspisiListu(P->next);
                printf("Lista Q:\n");
                IspisiListu(Q->next);
                break;
            case 9:
                printf("Unesi prezime studenta kojeg zelite pretrazit: ");
                scanf("%s", pretraziPrezime);
    	        PretraziPoPrezimenu(P, pretraziPrezime);
                break;
            case 10:
                printf("Unesi prezime studenta kojeg zelite izbrisati: ");
                scanf("%s", izbrisiStudenta);
                if(IzbrisiStudenta(P, izbrisiStudenta) == 0){
                    printf("Student %s uspjesno izbrisan!\n", izbrisiStudenta);
                }
                break;
        }
        if(odabir != 0){
            printf("pritisnite neku tipku za nastavak...");
            getch();
        }
        printf("\n");
        
    }while(odabir != 0);

    if(P->next != NULL){
        IzbrisiListu(P);
    }

    if(Q->next != NULL){
        IzbrisiListu(Q);
    }
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

int UnesiNakonStud(Pozicija P, char *ime, char *prezime, int godinaRodenja, char *prezimePrije)
{
    P = TraziTrenutnog(P, prezimePrije);
    if(!P){
        printf("Student %s nepostoji\n", prezime);
        return 1;
    }
    UnesiNaPocetak(P, ime, prezime, godinaRodenja);
    return 0;
}

int UnesiPrijeStud(Pozicija P, char *ime, char *prezime, int godinaRodenja, char *prezimeNakon)
{
    P = TraziPrethodnog(P, prezimeNakon);
    if(!P){
        printf("Student %s nepostoji\n", prezime);
        return 1;
    }
    UnesiNaPocetak(P, ime, prezime, godinaRodenja);
    return 0;
}

int UnesiSortirano(Pozicija P, char *ime, char *prezime, int godinaRodenja)
{
    while(P->next != NULL && strcmp(P->next->prezime, prezime)<0){
        P=P->next;
    }
    UnesiNaPocetak(P, ime, prezime, godinaRodenja);
}

int IspisiListu(Pozicija P)
{
    if(P == NULL){
        printf("Lista je prazna\n\n");
        return 0;
    }
   Pozicija temp = P;
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
    P = TraziPrethodnog(P, prezime);
    if(!P){
        printf("u listi nepostoji student %s, brisanje neuspjesno\n", prezime);
        return 1;
    }
    Izbrisi(P);
    return 0;
}

int IzbrisiListu(Pozicija P)
{
    if(P->next = NULL){
        printf("lista je prazna\n");
    }
    while(P->next != NULL)
    {
        Izbrisi(P);
    }
    return 0;
}

int Izbrisi(Pozicija P){
    Pozicija temp = NULL;
    temp = P->next;
    P->next = P->next->next;
    free(temp);
}

int PretraziPoPrezimenu(Pozicija P, char *prezime)
{
    Pozicija Stud = NULL;
    Stud = TraziTrenutnog(P, prezime);
    if(!Stud){
        printf("student %s nepostoji\n", prezime);
    }else{
        printf("student %s postoji\n", Stud->prezime);
    }
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
            return P;
        }
        P = P->next;
    }
    if(P->next == NULL)
        return NULL;
}

Student* TraziTrenutnog(Pozicija P, char *prezime)
{
    while(P->next!=NULL){
        if(strcmp(P->prezime, prezime) == 0){
            return P; 
        }
        P = P->next;
    }
    if(P->next == NULL)
        return NULL;
}

int UnesiListuUDatoteku(Pozicija P, char *datoteka)
{
    FILE *fp = NULL;
    fp = fopen(datoteka, "w");
    if(!fp){
        printf("pogreska u otvaranju datoteke!\n");
        return -1;
    }
    Pozicija temp = P;
    while(temp != NULL){
        fprintf(fp, "%s\t", temp->ime);
        fprintf(fp, "%s\t", temp->prezime);
        fprintf(fp, "%d\n", temp->godinaRodenja);
        temp = temp->next;
    }
    fclose(fp);
    return 0;
}

int UcitajListuIzDatoteke(Pozicija Q, char *datoteka)
{
    FILE *fp = NULL;
    char buffer[MAX_CHAR] = { 0 };
    fp = fopen(datoteka, "r");
   if(!fp){
       printf("otvaranje datoteke neuspjelo\n");
       return -1;
    }
   if(Q->next != NULL){
       printf("lista nije prazna, molimo ucitajte praznu listu\n");
       return 1;
    }
    while(!feof(fp)){
        Pozicija Stud = NULL;
        Stud = (Pozicija)malloc(sizeof(Student));
        if(!Stud){
            printf("greska u alokaciji memorije\n");
            return -1;
        }
        fgets(buffer, MAX_CHAR, fp);
        if(sscanf(buffer, "%s %s %d", Stud->ime, Stud->prezime, &Stud->godinaRodenja) == 3)
        {
            InsertNakon(Q, Stud);
            Q = Q->next;
        }
    }
    fclose(fp);
    return 0;
}

