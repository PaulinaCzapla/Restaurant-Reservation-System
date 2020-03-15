#ifndef FUNCTIONS_C
#define FUNCTIONS_C
#include "functions_header.h"

#define FILENAME_RESTAURANT "restaurant.txt"
#define FILENAME_RESERVATIONS "reservations.txt"


void WYSWIETL(Restaurant* pH)
{
	while (pH)
	{
		printf(" %i  %i \n", pH->table_number, pH->seats);
		pH = pH->next;
	}
}

void WYSWIETLRESER(Reservations* pH)
{
	while (pH)
	{
		printf("%i %i %i %i %i %s %s %s %s %s \n ", pH->table_number, pH->seats, pH->date->day, pH->date->month, pH->date->year, pH->date->hour, pH->personal_data->name, pH->personal_data->last_name, pH->personal_data.phone_number, pH->comment);
		pH = pH->next;
	}
}

Restaurant* First_Node(Restaurant* prevpointer, Restaurant* nextpointer, int number, int amount)
{
	Restaurant* node;
	node = (Restaurant*)malloc(sizeof(Restaurant));

	if (node != NULL)
	{
		node->prev = prevpointer;
		node->next = nextpointer;
		node->table_number = number;
		node->seats = amount;
	}
	return node;
}

Restaurant* Node(Restaurant* pHead, int number, int amount)
{
	if (pHead == NULL)
	{
		Restaurant* first = First_Node(NULL, NULL, number, amount);
		return first;
	}
	else
	{
		Restaurant* node = pHead;
		Restaurant* prev = NULL;
		Restaurant* tmpHead = pHead;

		while (pHead)
		{
			if (amount <= pHead->seats)
			{
				node = First_Node(prev, pHead, number, amount);
				pHead->prev = node;

				if (prev)
					prev->next = node;

				if (pHead == tmpHead)
					tmpHead = node;

				break;
			}

			prev = pHead;
			pHead = pHead->next;

			if (pHead == NULL)
			{
				node = First_Node(prev, NULL, number, amount);
				prev->next = node;
				break;
			}
		}

		return tmpHead;
	}
}

Reservations* First_Node_Reservations(Reservations* nextpointer, Personal_Data pdata, Date_And_Hour dateandhour, unsigned int table_number, unsigned int amount, char* comment)
{
	Reservations* node;
	node = (Reservations*)malloc(sizeof(Reservations));

	if (node != NULL)
	{
		node->next = nextpointer;
		node->personal_data = pdata;
		node->date = dateandhour;
		node->table_number = table_number;
		node->seats = amount;
	}
	return node;
}

Reservations* Node_Reservations(Reservations* pHead, Personal_Data pdata, Date_And_Hour dateandhour, unsigned int table_number, unsigned int amount,char* comment)
{
	if (pHead == NULL)
	{
		Reservations* first = First_Node_Reservations(NULL, pdata, dateandhour, table_number,  amount, comment);
		return first;
	}
	else
	{
		Reservations* tmphead = pHead;

		while (pHead->next)
			pHead = pHead->next;

		pHead->next = First_Node_Reservations(NULL, pdata, dateandhour, table_number, amount, comment);
		pHead = tmphead;
		return pHead;
	}
}

void Read_Restaurant_File(int limit)
{
	FILE* file_restaurant;
	errno_t err;

	err = fopen_s(&file_restaurant, FILENAME_RESTAURANT, "r");
	if (file_restaurant != NULL)
	{
		if (err)
		{
			printf("Nie moge otworzyc pliku.\n");
		}
		else
		{
			int amount, table;
			Restaurant* pH = NULL;

			while (fscanf_s(file_restaurant, "%i %i", &table, &amount) != EOF /*&& fscanf_s(file_restaurant, "%i", &amount) != EOF*/)
			{
				if (amount >= limit)
					pH = Node(pH, table, amount);
			}


			fclose(file_restaurant);
			WYSWIETL(pH);
		}
	}
}

void Read_Reservations_File()
{
	FILE* file_reservations;
	errno_t err;

	err = fopen_s(&file_reservations, FILENAME_RESERVATIONS, "r");
	if (file_reservations != NULL)
	{
		if (err)
		{
			printf("Nie moge otworzyc pliku z rezerwacjami.\n");
		}
		else
		{
			int amount=0, table=0;
			 Personal_Data pdata; 
				Date_And_Hour dateandhour; 
			char comment [MAX_COMMENT];
			char name[MAX_NAME];
			char last_name[MAX_L_NAME];
			char phone_number[MAX_PHONE_NUM];
			char hour[MAX_HOUR];

			Reservations* pH = NULL;

			while (fscanf_s(file_reservations, "%i %i %i %i %i %s %s %s %s %s", &table, &amount, &dateandhour.day , &dateandhour.month, &dateandhour.year, hour, name, last_name ,phone_number, comment) != EOF)
			{
				
				pdata.name = name;
				pdata.last_name = last_name;
				pdata.phone_number = phone_number;

				pH = Node_Reservations( pH,  pdata,  dateandhour,  table,  amount, comment);
			}


			fclose(file_reservations);
			WYSWIETLRESER(pH);
		}
	}
}

//void Open_Reservations_File()
//{
//
//}
//Restaurant* Check_Availability(int amount, int day, int month, int year)
//{
//	Read_Restaurant_File(FILENAME_RESTAURANT, amount);
//
//	return NULL;
//}
//
//void Add_Reservation()
//{
//	int amount, day, month, year;
//	printf(" Ilosc osob: ");
//	scanf_s("%i", &amount);
//	printf(" Dzien: ");
//	scanf_s("%i", &day);
//	printf(" Miesiac: ");
//	scanf_s("%i", &month);
//	printf(" Rok: ");
//	scanf_s("%i", &year);
//
//
//	FILE* file_reservations;
//	errno_t err;
//
//	err = fopen_s(&file_reservations, FILENAME_RESERVATIONS, "r");
//	if (file_reservations != NULL)
//	{
//		if (err)
//		{
//			printf("Nie moge otworzyc pliku.\n");
//		}
//		else
//		{
//			int amount, table, amountf, dayf, monthf, yearf, hourfor, hourup;
//			char phone[12];
//			char name[20], lastname[20];
//
//			Restaurant* pH = NULL;
//
//			while (fscanf_s(file_reservations, "%i %i %i %i %i %i %i %20s %20s %12s", &table, &amountf, &dayf, &monthf, &yearf, &hourfor, &hourup, &name, &lastname, &phone) != EOF )
//			{
//		
//					pH = Node(pH, table, amount);
//			}
//
//
//			fclose(file_restaurant);
//			WYSWIETL(pH);
//		}
//	}
//	Restaurant* available = Check_Availability(amount, day, month, year);
//
//}
//
//void Action_Center()
//{
//	printf("Witaj w Restauracyjnym Systemie Rezerwacji\n");
//	printf("[1] Dodaj Rezerwacje \n");
//	printf("[2] Usun Rezerwacje \n");
//	printf("[3] Wyswietl Rezerwacje \n");
//	printf("[4] Wprowadz zmiany w systemie znizkowym \n");
//	printf("[5] Edytuj stoliki \n");
//	printf("[0] Zamknij program \n");
//
//	int choice;
//	scanf_s("%i", &choice);
//
//	switch (choice)
//	{
//		case 1:
//			Add_Reservation();
//			break;
//		case 2:
//			break;
//
//	}
//
//
//
//}
#endif