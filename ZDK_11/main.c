#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "zdk11.h"
#include "zdk11.c"

int main() {
	char file[MAX] = { 0 }, Input[MAX] = { 0 }, drzava[MAX] = { 0 };
	int velTab = VEL_TAB, minBrStanovnika=0;
	hashT H = NULL;
	Position current = NULL;
	H = InicializeHashTab(velTab);
	printf("Unesite ime datoteke:");
	InputString(file);
	ReadFromFiles(file, H);
	PrintAll(H);

	do {
		printf("Upisite drzavu koju zelite pretrazit:\n");
		InputString(drzava);
		current = FindByCountryName(drzava, H);
		if (current != NULL) {
			printf("Drzava %s postoji!\n", current->drzava);
			printf("Unesite donju granicu broja stanovnika gradova:\n");
			scanf("%d", &minBrStanovnika); getchar();
			printf("Gradovi sa %d ili vise stanovnika:\n", minBrStanovnika);
			PrintGradovi(current->root, minBrStanovnika);
		}else{
			printf("Drzava %s nepostoji!\n", drzava);
		}
		printf("za nastavak unesite neku tipku, a za izlaz exit\n");
		InputString(Input);

	} while (strcmp(Input, "exit") != 0);
	
	DealocateHashTab(H);
	return 0;
}