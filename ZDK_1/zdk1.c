#include "stdio.h"
#include "stdlib.h"
#define MAX_BR_BODOVA (80)
#define MAX_CHAR (1024)

typedef struct{
    char ime[30];
    char prezime[30];
    int bodovi;
}student;

int brojStudenata(char *dat){
    FILE *fp = NULL;
    fp = fopen(dat, "r");
    int brRedova = 0;
    char buffer[MAX_CHAR] = { 0 };

    if(!fp){
        printf("pogreska u otvaranju datoteke\n");
        free(fp);
        return -1;
    }
    while(!feof(fp)){
        fgets(buffer, MAX_CHAR, fp);
        brRedova++;
    }
    fclose(fp);
    return brRedova;
}

student* UnesiStudenteIzDat(int brRed, char *dat){
    FILE *fp = NULL;
    fp = fopen(dat, "r");
    student* poljeStudenata;
    int i=0;
    
    if(!fp){
        printf("pogreska u otvaranju datoteke\n");
        free(fp);
        return NULL;
    }
    poljeStudenata = (student*)malloc(sizeof(student)*brRed);

    if(!poljeStudenata){
        printf("pogreska u alokaciji memorije\n");
        free(poljeStudenata);
        return NULL;
    }

    while(!feof(fp)){
        fscanf(fp,"%s", poljeStudenata[i].ime);
        fscanf(fp,"%s", poljeStudenata[i].prezime);
        fscanf(fp,"%d", &poljeStudenata[i].bodovi);
        i++;
    }
    fclose(fp);
    return poljeStudenata;
}

int main(){
    student *poljeStudenata;
    int brRedova = 0, i;
    float relativniBodovi = 0.0;

    brRedova = brojStudenata("studenti.txt");
    poljeStudenata = UnesiStudenteIzDat(brRedova, "studenti.txt");

    if(!poljeStudenata){
        return 1;
    }
    
    printf("IME\tPREZIME\tA_BODOVI R_BODOVI\n");
    for(i=0; i<brRedova; i++){
        relativniBodovi = ((float)poljeStudenata[i].bodovi)/MAX_BR_BODOVA * 100.00;

        printf("%s\t%s\t%d\t %.2f\n", 
                poljeStudenata[i].ime, poljeStudenata[i].prezime, 
                poljeStudenata[i].bodovi, relativniBodovi);
    }
    free(poljeStudenata);
    return 0;
}