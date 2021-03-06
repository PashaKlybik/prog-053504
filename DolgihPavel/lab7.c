#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "malloc.h"
#include "Windows.h"
#include "List.h"

int CheckInputInt(char buff[]); // �������� �� ���� int32
double CheckInputDouble(char buff[]); // �������� �� ���� double
void MakeTransaction(List*); // ����������� ����� ������ � ���������� � �������
int Transaction(List*, Account*, Account*, double); // ���������� ����� �� �������
void MakeDeletion(List*); // ���������� ��������
void Add(List*); // ���������� ���������� �����
void MakeShowOperations(List*); // ���������� ������ ���� �������� � ��������� �� ���
void PrintInterface(List*); // �������� ��������� ���������
void MakeWriteToFile(List*); // ���������� ������ � ����
void MakePut(List* list);// ���������� ����� �� ����
void MakeWithdraw(List* list);// ������ ����� �� �����

int ID = 0; // ������� ID ���������
time_t currentTime;

int main()
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	List* list = (List*)calloc(1, sizeof(List));
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	currentTime = time(NULL);
	char key = 'q';
	PrintInterface(list);

	do
	{
		key = getchar();
		rewind(stdin);
		// �������� �� ���� ��������
		while (key != 'a' && key != 't' && key != 's' && key != 'n'
			&& key != 'm' && key != 'z' && key != 'y' && key != 'i'
			&& key != 'w' && key != 'c' && key != 'q' && key != 'p' && key != 'u')
		{
			printf("Invalid input\n");
			key = getchar();
			rewind(stdin);
		}

		switch (key)
		{
		case 'a':
			Add(list);
			break;
		case 't':
			MakeTransaction(list);
			break;
		case 's':
			Sort(list);
			system("cls");
			PrintInterface(list);
			break;
		case 'u':
			if (AddYear(&currentTime))
			{
				ClearOperations(list);
			}
			SkipYear(list);
			system("cls");
			PrintInterface(list);
			break;
		case 'n':
			if (AddMonth(&currentTime))
			{
				ClearOperations(list);
			}
			SkipMonth(list);
			system("cls");
			PrintInterface(list);
			break;
		case 'm':
			if (AddDay(&currentTime))
			{
				ClearOperations(list);
			}
			SkipDay(list);
			system("cls");
			PrintInterface(list);
			break;
		case 'z':
			MakeDeletion(list);
			break;
		case 'y':
			MakeShowOperations(list);
			break;
		case 'w':
			MakeWriteToFile(list);
			break;
		case 'p':
			MakePut(list);
			break;
		case 'i':
			MakeWithdraw(list);
			break;
		case 'c':
			Clear(list); // ������ ������� ������
			ID = 0;
			system("cls");
			puts("����������� :\n");
			puts("������ ������� ������\n");
			PrintInterface(list);
			break;
		}
	} while (key != 'q');
	system("cls");
	Clear(list);
	return 0;
}




void MakeTransaction(List* list)
{
	char buff[1000];
	puts("\n������� ID ����������� : ");
	int fromID = CheckInputInt(buff);
	Account* fromAcc = Find(list, fromID); // ������� �����������
	if (fromAcc != NULL) fromAcc->isChosenForTransaction = 1;
	else
	{
		system("cls");
		puts("����������� :\n");
		puts("������ ����������\n");
		PrintInterface(list);
		return;
	}
	system("cls");
	PrintInterface(list);
	puts("\n������� ID ���������� : ");
	int toID = CheckInputInt(buff);
	Account* toAcc = Find(list, toID); // ������� ����������
	if (toAcc != NULL && toID != fromID) toAcc->isChosenForTransaction = 1;
	else
	{
		system("cls");
		puts("����������� :\n");
		puts("������ ����������\n");
		fromAcc->isChosenForTransaction = 0;
		PrintInterface(list);
		return;
	}
	system("cls");
	PrintInterface(list);
	puts("\n������� ����� : ");
	double sum = CheckInputDouble(buff);
	if (Transaction(list, fromAcc, toAcc, sum))
	{
		system("cls");
		puts("����������� :\n");
		puts("��������� ���������� :\n");
		printf("����������� :   ID : %d\n", fromAcc->ID);
		printf("���������� :   ID : %d\n", toAcc->ID);
		printf("����� : %.2lf\n\n", sum);
		PrintInterface(list);
		// ������ ���� ������ �������� � ��� ��������
		if (fromAcc->operations == NULL)
		{
			fromAcc->operations = (char*)calloc(1, 100);
		}
		else
		{
			fromAcc->operations = (char*)realloc(fromAcc->operations, 100);
		}

		if (toAcc->operations == NULL)
		{
			toAcc->operations = (char*)calloc(1, 100);
		}
		else
		{
			toAcc->operations = (char*)realloc(toAcc->operations, 100);
		}

		char s[100];
		for (int i = 0; i < 100; i++) s[i] = '\0';
		char* ID1 = (char*)malloc(10);
		char* ID2 = (char*)malloc(10);
		char* SUM = (char*)malloc(15);
		_itoa(fromAcc->ID, ID1, 10);
		_itoa(toAcc->ID, ID2, 10);
		sprintf(SUM, "%.2lf", sum);
		strcpy(s, "ID: ");
		strcat(s, ID1);
		strcat(s, " => ID: ");
		strcat(s, ID2);
		strcat(s, " ����� : ");
		strcat(s, SUM);
		strcat(s, "\n");
		strcat(fromAcc->operations, s);
		strcat(toAcc->operations, s);
	}
	else
	{
		system("cls");
		puts("����������� :\n");
		puts("������ ����������\n");
		PrintInterface(list);
	}
}

void MakePut(List* list)
{
	char buff[1000];
	puts("\n������� ID ����� �� ������� ������ �������� ������ : ");
	int toID = CheckInputInt(buff);
	Account* toAcc = Find(list, toID); // ������� �������
	if (toAcc != NULL) toAcc->isChosenForTransaction = 1;
	else
	{
		system("cls");
		puts("����������� :\n");
		puts("������ ����������\n");
		PrintInterface(list);
		return;
	}
	system("cls");
	PrintInterface(list);
	puts("\n������� ����� : ");
	double sum = CheckInputDouble(buff);
	Put(toAcc, sum);
	system("cls");
	puts("����������� :\n");
	puts("��������� ���������� :\n");
	printf("���������� : ID : %d\n", toAcc->ID);
	printf("����� : %.2lf\n\n", sum);
	PrintInterface(list);

	if (toAcc->operations == NULL)
	{
		toAcc->operations = (char*)calloc(1, 100);
	}
	else
	{
		toAcc->operations = (char*)realloc(toAcc->operations, 100);
	}

	char s[100];
	for (int i = 0; i < 100; i++) s[i] = '\0';
	char* SUM = (char*)malloc(15);
	sprintf(SUM, "%.2lf", sum);
	strcat(s, "���������� �������. ����� ���������� : ");
	strcat(s, SUM);
	strcat(s, "\n");
	strcat(toAcc->operations, s);
}

void MakeWithdraw(List* list)
{
	char buff[1000];
	puts("\n������� ID ����� c �������� ������ ����� ������ : ");
	int fromID = CheckInputInt(buff);
	Account* fromAcc = Find(list, fromID); // ������� �������
	if (fromAcc != NULL) fromAcc->isChosenForTransaction = 1;
	else
	{
		system("cls");
		puts("����������� :\n");
		puts("������ ����������\n");
		PrintInterface(list);
		return;
	}
	system("cls");
	PrintInterface(list);
	puts("\n������� ����� : ");
	double sum = CheckInputDouble(buff);
	if (Withdraw(fromAcc, sum, currentTime))
	{
		system("cls");
		puts("����������� :\n");
		puts("��������� ���������� :\n");
		printf("C����� �� ����� ID : %d\n", fromAcc->ID);
		printf("����� ������: %.2lf\n\n", sum);
		PrintInterface(list);
		if (fromAcc->operations == NULL)
		{
			fromAcc->operations = (char*)calloc(1, 100);
		}
		else
		{
			fromAcc->operations = (char*)realloc(fromAcc->operations, 100);
		}
		char s[100];
		for (int i = 0; i < 100; i++) s[i] = '\0';
		char* SUM = (char*)malloc(15);
		sprintf(SUM, "%.2lf", sum);
		strcat(s, "������ �� �����. ����� ������ : ");
		strcat(s, SUM);
		strcat(s, "\n");
		strcat(fromAcc->operations, s);
	}
	else
	{
		system("cls");
		puts("����������� :\n");
		puts("������ ����������\n");
		PrintInterface(list);
	}
}

int Transaction(List* list, Account* fromAcc, Account* toAcc, double sum)
{
	fromAcc->isChosenForTransaction = 0;
	toAcc->isChosenForTransaction = 0;
	if (Withdraw(fromAcc, sum, currentTime))
	{
		Put(toAcc, sum);
		return 1;
	}
	return 0;
}

void MakeDeletion(List* list)
{
	char buff[1000];
	printf("\n������� ID �����, ������� ���������� ������� : ");
	int ID = CheckInputInt(buff);
	int index = FindIndex(list, ID);
	if (index == -1)
	{
		system("cls");
		puts("����������� : \n");
		puts("ID �� ������\n");
		PrintInterface(list);
		return;
	}
	else
	{
		Account* acc = Find(list, ID);
		system("cls");
		puts("����������� : \n");
		printf("������ ���� ID : %d\n����� : %.2lf\n", ID, acc->sum);
		if (acc->type == 1) puts("��� : �� �������������\n");
		else puts("��� : ����������\n");
		Delete(list, index);
		PrintInterface(list);
	}
}

void Add(List* list)
{
	char buff[1000];
	Account acc;
	puts("\n������� ��� ����� (1 - �� �������������, 2 - ����������) : ");
	acc.type = CheckInputInt(buff);
	while (acc.type != 1 && acc.type != 2)
	{
		puts("Invalid input");
		acc.type = CheckInputInt(buff);
	}
	puts("\n������� ��� : ");
	acc.name = (char*)calloc(40, sizeof(char));
	gets(acc.name);
	puts("\n������� ��������� ����� �� ����� : ");
	acc.sum = CheckInputDouble(buff);
	puts("\n������� �������� : ");
	acc.percentage = CheckInputDouble(buff);

	if (acc.type == 2)
	{
		puts("\n������� �������� � ������� ��� ������ ������� : ");
		acc.months = CheckInputInt(buff);
	}
	else
	{
		acc.months = 0;
	}
	acc.opened = currentTime;
	acc.ID = ID;
	ID++;
	acc.operations = NULL;
	PushBack(list, acc);
	system("cls");
	PrintInterface(list);
}

void MakeShowOperations(List* list)
{
	char buff[1000];
	printf("\n������� ID �����, �������� �� ��� �������� ���������� : ");
	int ID = CheckInputInt(buff);
	Account* acc = Find(list, ID);
	if (acc == NULL)
	{
		system("cls");
		puts("����������� : \n");
		puts("ID �� ������\n");
		PrintInterface(list);
		return;
	}
	else
	{
		system("cls");
		puts("����������� : \n");
		printf("�������� � ID : %d\n", acc->ID);
		if (acc->operations == NULL) puts("None");
		else puts(acc->operations);
		printf("\n");
		PrintInterface(list);
	}

}

int CheckInputInt(char buff[]) { // �������� �� ���� int32
	int find_char, i, n;
	while (1) {
		find_char = 0;
		gets_s(buff, 1000);
		for (i = 0; i < strlen(buff); i++) {
			if (buff[i] < '0' || buff[i] > '9') {
				find_char = 1;
				break;
			}
		}
		if (find_char == 0) n = atoi(buff);
		if (find_char == 0 && n < _CRT_INT_MAX) {
			break;
		}
		else printf("Invalid input\n");
	}
	return n;
}

double CheckInputDouble(char buff[]) {
	int find_char, i;
	double n;

	while (1) {
		find_char = 0;
		gets_s(buff, 1000);
		for (i = 0; i < strlen(buff); i++) {
			if ((buff[i] < '0' || buff[i] > '9') && buff[i] != '.') {
				find_char = 1;
				break;
			}
		}
		if (find_char == 0) n = atof(buff);

		if (find_char == 0 && n < _CRT_INT_MAX && n > 0) {
			break;
		}
		else printf("Invalid input!\n");
	}
	return n;
}

void MakeWriteToFile(List* list)
{
	FILE* fptr = fopen("File.txt", "w");
	WriteToFile(list, fptr);
	fclose(fptr);
	system("cls");
	puts("����������� :\n");
	puts("������ ������� ��������� � ����\n");
	PrintInterface(list);
}

void PrintInterface(List* list)
{
	printf("������� ���� : %s \n\n", TimeToStr(currentTime));
	PrintList(list);
	puts("\n'a' - �������� ����");
	puts("'t' - �������� ����������");
	puts("'P' - ��������� ������ �����");
	puts("'i' - ����� ������ �� �����");
	puts("'s' - ������������� �� �����");
	puts("'u' - ���������� ���");
	puts("'n' - ���������� �����");
	puts("'m' - ���������� ����");
	puts("'z' - ������� ���� �� ID");
	puts("'y' - ���������� �������� � ��������� �� ���");
	puts("'w' - ��������� ���������� � ����");
	puts("'c' - �������� ������");
	puts("'q' - �����");
}