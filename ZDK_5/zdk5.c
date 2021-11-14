#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define MAX (40)
struct _StogEl;
typedef struct _StogEl *Position;
typedef struct _StogEl {
	double num;
	Position next;
}StogEl;

int InputFileName(char* file);
Position createEl(double num);
int InsertAfter(Position P, Position newEl);
int Push(Position head, double num);
int DeleteAfter(Position head);
int Pop(Position head, double *result);
int PerformOperation(Position head, char operation, double *result);
char* ReadFromFile(char *file);
int CalculatePostfixFromFile(Position head, char *file, double *result);
int DeleteAll(Position head);

int InputFileName(char *file)
{
	printf("Unesite ime datoteke:\n");
	fgets(file, MAX, stdin);

	if (file[strlen(file) - 1] == '\n')
		file[strlen(file) - 1] = '\0';
	return 0;
}

int main(){
    StogEl head = {.num = 0, .next = NULL};
    double result; 
    int status;
    char file[MAX] = {0};

    InputFileName(file);
    status = CalculatePostfixFromFile(&head, file, &result);
    if(status != 0){
        DeleteAll(&head);
        return -1;
    }
    printf("rezultat = %.2lf\n", result);
    DeleteAll(&head);
    return 0;
}

Position createEl(double num)
{
    Position newEl = NULL;
	newEl = (Position)malloc(sizeof(StogEl));
	if (!newEl) {
		perror("pogreska u alokaciji memorije\n");
		return NULL;
	}
	newEl->num = num;
	newEl->next = NULL;
	return newEl;
}

int InsertAfter(Position head, Position newEl)
{
    newEl->next = head->next;
    head->next = newEl;
    return 0;
}

int Push(Position head, double num)
{
    Position newEl = NULL;
    newEl = createEl(num);
    if(!newEl){
        return -1;
    }
    InsertAfter(head, newEl);
    return 0;
}

int DeleteAfter(Position head)
{
    Position temp = NULL;
	temp = head->next;
    if(!temp){
        return 0;
    }
	head->next = temp->next;
	free(temp);
	return 0;
}

int Pop(Position head, double *result)
{
    Position first = NULL;
    first = head->next;
    if(!first){
        printf("nema vise elemenata na stogu, greska\n");
        return -1;
    }
    *result = first->num;
    DeleteAfter(head);
}

int PerformOperation(Position head, char operation, double *result)
{
    double operand1 = 0, operand2 = 0;
    int status1 = 0, status2 = 0, status = 0;

    status2 = Pop(head, &operand2);
    if(status2 != 0){
        return -1;
    }
    status1 = Pop(head, &operand1);
    if(status1 != 0){
        return -2;
    }

    switch(operation)
    {
        case '+':
            *result = operand1 + operand2;
            break;
        case '-':
            *result = operand1 - operand2;
            break;
        case '*':
            *result = operand1 * operand2;
            break;
        case '/':
            if(operand2 == 0){
                printf("nemozemo djeliti sa 0, greska!");
                return -3;
            }
             *result = operand1 / operand2;
            break;
        default:
            printf("ova operacija nije podrzana u trenutnoj verziji\n");
            return -4;
    }
        status = Push(head, *result);
        return status;
}

char* ReadFromFile(char *file)
{
    FILE *fp = NULL;
    int fileLength;
    char *buffer = NULL;

    fp = fopen(file, "rb");

    if(!fp){
        perror("neuspjesno otvaranje datoteke!\n");
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    fileLength = ftell(fp);
    buffer = (char*)calloc(fileLength+1, sizeof(char));

    if(!buffer){
        perror("neuspjesna alokacija memorije");
        return NULL;
    }
    rewind(fp);

    fread(buffer, sizeof(char), fileLength, fp);
    printf("buffer: %s\n", buffer);
    fclose(fp);
    return buffer;
}

int CalculatePostfixFromFile(Position head, char *file, double *result)
{
    int numBytes, status;
    double number;
    char *buffer  = NULL, *currentBuffer = NULL, operation;
    
    buffer = ReadFromFile(file);
    if(!buffer){
        return -1;
    }
    currentBuffer = buffer;

    while(strlen(currentBuffer)>0)
    {
        status = sscanf(currentBuffer, " %lf %n", &number, &numBytes);
        if( status == 1){
            status = Push(head, number);
            if(status != 0)
            {
                free(buffer);
                return -2;
            }
            currentBuffer += numBytes;
        }else{
            sscanf(currentBuffer, " %c %n", &operation, &numBytes);
            status = PerformOperation(head, operation, result);
            if(status != 0)
            {
                free(buffer);
                return -3;
            }
            currentBuffer += numBytes;
            number = *result;
        }
    }
    free(buffer);

    Position first = NULL;
    first = head->next;

    if(first == NULL){
        printf("greska, u stogu nemamo rezultat, provjerite datoteku\n");
        return -4;
    }
    if(first->next == NULL){
        printf("rezultat je uspjesno izracunat\n");
        return 0;
    }else{
        printf("U stogu je ostalo vise elemenata, neuspjesno\n");
        return -5;
    }
    free(first);
}

int DeleteAll(Position head)
{
    while(head->next != NULL)
    {
        DeleteAfter(head);
    }
    printf("memorija dealocirana\n");
}





