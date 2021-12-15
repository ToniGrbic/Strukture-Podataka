#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define MAX (256)

struct _Dir;
typedef struct _Dir *Position;
typedef struct _Dir {
	char dir_name[MAX];
	Position child;
	Position sibling;
}Dir;

struct _StogEl;
typedef struct _StogEl *StogPos;
typedef struct _StogEl {
	StogPos next;
	Position data;
}StogEl;

Position MakeDir(Position current, char *dir_name);
Position Insert(Position current, Position q);
Position FindDirByName(char *dir_name, Position current, StogPos head);
Position createDirEl(char *dir_name);
StogPos createStackEl(Position current);
Position Pop(StogPos head);
int Push(Position current, StogPos head);
int InsertAfter(StogPos head, StogPos newEl);
int PrintDir(Position current);
int DirPath(StogPos head);
int main()
{
	char Input[MAX], Dir_name[MAX], Command[MAX];
	Dir C = { .child = NULL,.sibling = NULL,.dir_name = "C:" };
	StogEl Head = { .data = NULL,.next = NULL };
	Position current = &C;
	StogPos head = &Head;

	Push(current, head);
	DirPath(head);

	do {
		fgets(Input, MAX, stdin);
		sscanf(Input, "%s %s", Command, Dir_name);
		if (strcmp(Command, "md") == 0)
		{
			current = MakeDir(current, Dir_name);
			DirPath(head);
		}
		else if (strcmp(Command, "cd") == 0) {

			current = FindDirByName(Dir_name, current, head);
			DirPath(head);
		}
		else if (strcmp(Command, "cd..") == 0) {
			current = Pop(head);
			DirPath(head);
		}
		else if (strcmp(Command, "dir") == 0) {
			PrintDir(current);
			DirPath(head);
		}
		else if (strcmp(Input, "exit\n") == 0) {
			printf("Izlaz...\n");
		}
		else {
			printf("Neispravni Command, pokusajte ponovo...\n");
			DirPath(head);
		}
	} while (strcmp(Input, "exit\n") != 0);

	return 0;
}

Position MakeDir(Position current, char *file_name)
{
	Position newDir;
	newDir = createDirEl(file_name);
	current->child = Insert(current->child, newDir);
	return current;
}

Position Insert(Position current, Position q)
{
	if (current == NULL)
	{
		return q;
	}
	if (strcmp(current->dir_name, q->dir_name) > 0)
	{
		q->sibling = current;
		return q;
	}
	else if (strcmp(current->dir_name, q->dir_name) < 0) {
		current->sibling = Insert(current->sibling, q);
	}
	else {
		printf("Vec postoji dir sa imenom %s\n", q->dir_name);
		free(q);
	}
	return current;
}

Position FindDirByName(char *dir_name, Position current, StogPos head)
{
		if (current->child == NULL)
		{
			printf("Taj direktorij nepostoji\n");
			return current;
		}
		Position parentDir = current;
		Position childDir = current->child;

		while (childDir != NULL)
		{
			if (strcmp(childDir->dir_name, dir_name) == 0)
			{
				Push(childDir, head);
				return childDir;
			}
			childDir = childDir->sibling;
		}
		printf("Taj direktorij nepostoji.\n");
		return parentDir;
	
}

Position createDirEl(char *dir_name) {
	Position newDirEl = NULL;
	newDirEl = (Position)malloc(sizeof(Dir));
	if (!newDirEl) {
		perror("pogreska u alokaciji memorije\n");
		return NULL;
	}
	strcpy(newDirEl->dir_name, dir_name);
	newDirEl->child = NULL;
	newDirEl->sibling = NULL;
	return newDirEl;
}

StogPos createStackEl(Position current)
{
	StogPos newStogEl = NULL;
	newStogEl = (StogPos)malloc(sizeof(StogEl));
	if (!newStogEl) {
		perror("pogreska u alokaciji memorije\n");
		return NULL;
	}
	newStogEl->data = current;
	return newStogEl;
}

Position Pop(StogPos head)
{
	StogPos nextStogEl = head->next, tempHead = head;

	if (nextStogEl->next == NULL)
	{
		return nextStogEl->data;
	}
	Position Dir = nextStogEl->next->data;
	tempHead->next = nextStogEl->next;
	free(nextStogEl);
	return Dir;
}

int Push(Position current, StogPos head)
{
	StogPos newEl = createStackEl(current);
	StogPos tempHead = head;
	if (newEl == NULL)
		return -1;

	InsertAfter(tempHead, newEl);
	return 0;
}

int InsertAfter(StogPos head, StogPos newEl)
{
	newEl->next = head->next;
	head->next = newEl;
	return 0;
}

int PrintDir(Position current)
{
	Position tempCurrent = current;
	if (tempCurrent->child == NULL)
		printf("Direktorij je prazan!\n");

	printf("%s :\n", tempCurrent->dir_name);
	tempCurrent = tempCurrent->child;

	while (tempCurrent != NULL)
	{
		printf(" ->%s\n", tempCurrent->dir_name);
		tempCurrent = tempCurrent->sibling;
	}
	return 0;
}

int DirPath(StogPos head)
{
	StogPos tempHead = head;
	tempHead = tempHead->next;
	char Path[MAX] = "";
	char currentDir[MAX] = "";

	while (tempHead != NULL)
	{
		strcpy(currentDir, tempHead->data->dir_name);
		strcat(currentDir, "/");
		strcat(currentDir, Path);
		strcpy(Path, currentDir);
		tempHead = tempHead->next;
	}
	printf("%s> ", Path);
	return 0;
}