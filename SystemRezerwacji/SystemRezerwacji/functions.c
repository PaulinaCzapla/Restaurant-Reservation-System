#ifndef FUNCTIONS_C
#define FUNCTIONS_C

#include "functions_header.h"

#define FILENAME_RESTAURANT "restaurant.txt"
#define FILENAME_RESERVATIONS "reservations.csv"
#define _CRTDBG_MAP_ALLOC
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)


void WYSWIETL(Restaurant* pH)
{
	while (pH)
	{
		printf(" %i  %i \n", pH->table_number, pH->seats);
		pH = pH->next;
	}
}

//void WYSWIETLRESER(Reservations* pH)
//{
//	while (pH)
//	{
//		printf("%i %i %i %i %i %s %s %s %s %s \n ", pH->table_number, pH->seats, pH->date->day, pH->date->month, pH->date->year, pH->date->hour, pH->personal_data->name, pH->personal_data->last_name, pH->personal_data->phone_number, pH->comment);
//		pH = pH->next;
//	}
//}

void Alert_Red(char text[])
{
	HANDLE hOut;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hOut, FOREGROUND_RED);
	puts(text);
	SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

}

char* tr(char* str)
{
	static char buff[2000];
	char cp[] = "\245\206\251\210\344\242\230\276\253\244\217\250\235\343\340\227\275\215¹æê³ñóœ¿Ÿ¥ÆÊ£ÑÓŒ¯";
	if (strlen(str) >= sizeof(buff)) return str;
	char* bf = buff;
	while (*str)
	{
		char* pos = strchr(cp + 18, *str);
		*(bf++) = pos ? *(pos - 18) : *str;
		++str;
	}
	*bf = 0;
	return buff;
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
		node->date = NULL;
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
				else
					tmpHead = node;

				/*if (pHead == tmpHead)
					tmpHead = node;*/

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


Date* AddDateNode(Date* next, struct tm* time)
{
	Date* node;
	node = (Date*)malloc(sizeof(Date));

	if (node != NULL)
	{
		node->next = next;
		node->hour = NULL;
		node->date = time;
	}
	return node;
}
Date* DateNode(Restaurant* ptrTable, struct tm* time)
{
	Date* pHead = ptrTable->date;
	if (pHead == NULL)
	{
		Date* first = AddDateNode(NULL, time);
		ptrTable->date = first;
		return first;
	}
	else
	{
		Date* node = pHead;
		Date* prev = NULL;
		Date* tmpHead = pHead;
		bool is_added = false;
		while (pHead)
		{
			if (time->tm_year < pHead->date->tm_year)
			{
				//dodaj
				node = AddDateNode(pHead, time);
				if (prev)
					prev->next = node;
				is_added = true;
				break;
			}
			if (time->tm_year == pHead->date->tm_year)
			{

				if (time->tm_mon < pHead->date->tm_mon)
				{
					node = AddDateNode(pHead, time);
					if (prev)
						prev->next = node;
					is_added = true;
					break;
				}
				if (time->tm_mon == pHead->date->tm_mon)
				{
					if (time->tm_mday <= pHead->date->tm_mday)
					{
						node = AddDateNode(pHead, time);
						if (prev)
							prev->next = node;
						is_added = true;
						break;
					}

				}
				prev = pHead;
				pHead = pHead->next;
			}
		}

		if (is_added == false) //dodawanie na koniec
		{
			node = AddDateNode(NULL, time);
			prev->next = node;
		}
		ptrTable->date = node;
		return node;
	}
}


Hour* AddHourNode(Hour* next, struct tm* time)
{
	Hour* node;
	node = (Hour*)malloc(sizeof(Hour));

	if (node != NULL)
	{
		node->next = next;
		node->hour = time;
		node->pdata = NULL;
		node->customers = 0;
	}
	return node;
}
Hour* HourNode(Date* ptrDate, struct tm* time)
{
	Hour* pHead = ptrDate->hour;
	if (pHead == NULL)
	{
		Hour* first = AddHourNode(NULL, time);
		ptrDate->hour = first;
		return first;
	}
	else
	{
		Hour* node = pHead;
		Hour* prev = NULL;
		Hour* tmpHead = pHead;
		bool is_added = false;

		while (pHead)
		{

			int diff = time->tm_hour - pHead->hour->tm_hour;
			if (diff < 0)
				diff = -diff;

			int diffmin = time->tm_min - pHead->hour->tm_min;
			if (diffmin < 0)
				diffmin = -diffmin;


			if (time->tm_hour < pHead->hour->tm_hour)
			{
				//dodaj
				node = AddHourNode(pHead, time);
				prev->next = node;
				is_added = true;
				break;
			}
			if (time->tm_hour == pHead->hour->tm_hour)
			{

				if (time->tm_min <= pHead->hour->tm_min)
				{
					node = AddHourNode(pHead, time);
					prev->next = node;
					is_added = true;
					break;
				}

				prev = pHead;
				pHead = pHead->next;
			}
		}

		if (is_added == false) //dodawanie na koniec
		{
			node = AddHourNode(pHead, time);
			prev->next = node;
		}
		ptrDate->hour = node;
		return node;
	}
}


void DeleteList(Restaurant* head)
{
	Date* tmpD = NULL;
	Restaurant* tmpR = NULL;
	Hour* tmpH = NULL;
	Date* D = NULL;
	Restaurant* R = NULL;
	Hour* H = NULL;

	if (head)
		tmpR = head;

	while (tmpR)
	{
		if (tmpR->date)
		{
			tmpD = tmpR->date;
			while (tmpD)
			{
				if (tmpD->hour)
					tmpH = tmpD->hour;

				while (tmpH)
				{
					free(tmpH->pdata);
					free(tmpH->hour);
					H = tmpH;
					tmpH = tmpH->next;
					free(H);
				}
				//free(tmpD->date);
				D = tmpD;
				tmpD = tmpD->next;
				free(D);
			}
		}
		R = tmpR;
		tmpR = tmpR->next;
		free(R);
	}
}


Restaurant* Read_Restaurant_File()
{
	FILE* file_restaurant;
	errno_t err;
	Restaurant* pH = NULL;
	err = fopen_s(&file_restaurant, FILENAME_RESTAURANT, "r");
	if (file_restaurant != NULL)
	{
		if (err)
		{
			printf("Nie moge otworzyc pliku ze stolikami.\n");
		}
		else
		{
			int amount, table;


			/*		char tmp[30];
					char tmp2[30];

					while (fscanf_s(file_restaurant, "%i %s", tmp, tmp2) == 1)
					{
						if (tmp[strlen(tmp) - 1] == ';') tmp[strlen(tmp) - 1] = '\0';
						if (tmp2[strlen(tmp2) - 1] == ';') tmp2[strlen(tmp2) - 1] = '\0';

						table = atoi(tmp);
						amount = atoi(tmp2);
						if (amount >= limit)
								pH = Node(pH, table, amount);
					}*/

			int i = 0;
			Restaurant* head = NULL;
			while (fscanf_s(file_restaurant, "%i %i", &table, &amount) != EOF /*&& fscanf_s(file_restaurant, "%i", &amount) != EOF*/)
			{
				//if (amount >= limit)

				pH = Node(pH, table, amount);

			}
			fclose(file_restaurant);
			WYSWIETL(pH);
		}
	}
	return pH;
}

//void Read_Reservations_File()
//{
//	FILE* file_reservations;
//	errno_t err;
//
//	err = fopen_s(&file_reservations, FILENAME_RESERVATIONS, "r");
//	if (file_reservations != NULL)
//	{
//		if (err)
//		{
//			printf("Nie moge otworzyc pliku z rezerwacjami.\n");
//		}
//		else
//		{
//			int amount = 0, table = 0;
//
//			Personal_Data* pdata = (Personal_Data*)malloc(sizeof(Personal_Data));
//
//
//			Date_And_Hour* dateandhour;
//			dateandhour = (Date_And_Hour*)malloc(sizeof(Date_And_Hour));
//
//			char comment[MAX_COMMENT];
//			/*		char name[MAX_NAME];
//					char last_name[MAX_L_NAME];
//					char phone_number[MAX_PHONE_NUM];
//					char hour[MAX_HOUR];*/
//			int M = MAX_HOUR;
//			Reservations* pH = NULL;
//			if (dateandhour != NULL && pdata != NULL)
//				while (fscanf_s(file_reservations, "%i %i %i %i %i %[] %s %s %s %s", &table, &amount, &dateandhour->day, &dateandhour->month, &dateandhour->year,
//					(dateandhour->hour), (unsigned)_countof(dateandhour->hour), pdata->name, MAX_NAME, pdata->last_name, MAX_L_NAME, pdata->phone_number, MAX_PHONE_NUM, comment, (unsigned)_countof(comment)) != EOF)
//				{
//					//if (dateandhour != NULL)
//					//fscanf_s(file_reservations, "%i %i %i %s %s %s %s", &dateandhour->day, &dateandhour->month, &dateandhour->year, dateandhour->hour, MAX_HOUR, pdata->name, MAX_NAME, pdata->last_name, MAX_L_NAME, pdata->phone_number, MAX_PHONE_NUM, comment, MAX_COMMENT);
//					/*	fscanf_s(file_reservations, "%s %s %s ", &table, &amount, &dateandhour.day, &dateandhour.month, &dateandhour.year, hour, name, last_name, phone_number, comment)*/
//
//					pH = Node_Reservations(pH, pdata, dateandhour, table, amount, comment);
//				}
//
//
//			fclose(file_reservations);
//			WYSWIETLRESER(pH);
//		}
//	}
//}

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

Hour* Add_New_Reservation(Position* position, Restaurant* ptrTable, Date* ptrDate, struct tm* time)
{
	Hour* HeadHour = NULL;
	if (ptrDate)
	{
		HeadHour = HourNode(ptrDate, time);
		position->hour = HeadHour;
		position->date = ptrDate;
	}
	else if (ptrTable)
	{
		Date* HeadD = NULL;
		HeadD = DateNode(ptrTable, time);
		HeadHour = HourNode(HeadD, time);
		position->hour = HeadHour;
		position->date = HeadD;
	}
	position->definitive = true;

	assert(HeadHour);
	return HeadHour;
}

int Difference(struct tm* h1, Hour* h2)
{
	int diff = h1->tm_hour - h2->hour->tm_hour;
	if (diff < 0)
		diff = -diff;
	return diff;
}

Hour* Check_Hours(Position* position, Restaurant* headTab, Date* headDat, struct tm* time)
{
	Hour* headHour = headDat->hour;
	Hour* prev = NULL;
	Hour* result = NULL;
	while (true)
	{
		if (headHour == NULL)
			break;
		if (time->tm_hour > headHour->hour->tm_hour)
		{
			prev = headHour;
			headHour = headHour->next;
			continue;
		}

		if (Difference(time, headHour) >= DIFFERENCE && Difference(time, prev) >= DIFFERENCE)
		{
			//ok
			result = Add_New_Reservation(position, NULL, headDat, time);
			break;
		}
		else
		{
			position->table = headTab;
			position->date = headDat;
			position->definitive = false;
			break;
			/*	if (headTab == NULL)
					break;
				headTab = headTab->next;
				if (headTab == NULL)
					break;
				else
				{
					return CheckDates(headTab, time);
				}*/

		}
	}

	//assert(result);
	return result;
}

Hour* Check_Dates(Position* position, Restaurant* headTab, struct tm* time)
{
	//printf("check dates");
	Date* headDat = headTab->date;
	Hour* result = NULL;
	bool conflict = false;
	while (headDat)
	{
		if (headDat->date->tm_year == time->tm_year)
			if (headDat->date->tm_mon == time->tm_mon)
				if (headDat->date->tm_mday == time->tm_mday)
				{
					result = Check_Hours(position, headTab, headDat, time);
					conflict = true;
					break;
				}
		headDat = headDat->next;
	}
	if (conflict == false)
		result = Add_New_Reservation(position, headTab, NULL, time);

	return result;
}

Hour* Check_Tables(Position* position, Restaurant* head, struct tm* time, int amount)
{
	Hour* result = NULL;
	while (true)
	{
		if (head->seats >= amount)
		{
			if (head->date == NULL)
			{
				result = Add_New_Reservation(position, head, NULL, time);
				break;
			}
			else
			{
				result = Check_Dates(position, head, time);
				break;
			}
		}
		else
			head = head->next;

		if (head == NULL)
			break;

	}

	position->table = head;

	return(result);
}

void Add_Reservation(Restaurant* headT)
{
	struct tm* data = malloc(sizeof(struct tm));   //      !!!
	struct Position* position = malloc(sizeof(struct Position));      // !!!
	int amount, month;
	position->date = NULL;
	position->hour = NULL;
	position->table = NULL;
	position->definitive = true;

	printf(" Ilosc osob: ");
	scanf_s("%i", &amount);
	printf(" Dzien: ");
	scanf_s("%i", &(data->tm_mday));
	printf(" Miesiac: ");
	scanf_s("%i", &month);
	data->tm_mon = month - 1;
	printf(" Rok: ");
	scanf_s("%i", &(data->tm_year));
	printf(" Godzina(bez minut): ");
	scanf_s("%i", &(data->tm_hour));
	printf(" Minuty: ");
	scanf_s("%i", &(data->tm_min));


	Restaurant* tmp = NULL;

	Hour* reservation = Check_Tables(position, headT, data, amount);
	for (;;)
		if (reservation == NULL)
		{
			if (position->definitive == true)
			{
				printf(" Nie ma w tym dniu wolnego stolika.\n ");
				break;
			}
			else
			{
				tmp = position->table;
				if (tmp->next)
				{
					position->table = tmp->next;
					reservation = Check_Dates(position, tmp->next, data);
				}
				else
					position->definitive = true;
			}
		}
		else
			break;


	if (reservation)
	{
		reservation->customers = amount; //dodanie informacji o liczbie goœci
		printf("\n\n Udalo sie dodac rezerwacje. Prosze o podanie danych osobowych:  \n");

		Personal_Data* pdata = malloc(sizeof(Personal_Data));        //                           !!!


		printf(" Imie: ");
		scanf_s("%s", pdata->name, MAX_NAME);
		printf(" Nazwisko: ");
		scanf_s("%s", pdata->last_name, MAX_L_NAME);
		printf(" Numer telefonu:  ");
		scanf_s("%s", pdata->phone_number, MAX_PHONE_NUM);

		printf("\n Dodac komentarz do zamowienia? ");
		printf("\n [1] Tak ");
		printf("\n [2] Nie \n");

		int choice;
		for (;;)
		{
			printf(">  ");
			scanf_s("%d", &choice);
			char tmp;
			int i = 1;
			switch (choice)
			{
			case 1:
				printf("\n Podaj komentarz (max 200 znakow):  ");
				while ((tmp = getchar()) != '\n');
				fgets(pdata->comment, MAX_COMMENT, stdin);

				if (pdata->comment[(strlen(pdata->comment) - 1)] != '\n')
					while ((tmp = getchar()) != '\n');
				else
					pdata->comment[(strlen(pdata->comment) - 1)] = '\0';

				break;
			case 2:
			{
				while ((tmp = getchar()) != '\n');
				char* none = "brak";
				//strcpy(pdata->comment, none, 10);
				(pdata->comment)[0] = 'b';
				(pdata->comment)[1] = 'r';
				(pdata->comment)[2] = 'a';
				(pdata->comment)[3] = 'k';
				(pdata->comment)[4] = '\0';
				break;
			}

			default:
				while ((tmp = getchar()) != '\n');
				Alert_Red("\n Nieprawidlowa opcja. ");
				continue;
				break;

			}
			break;
		}
		if (pdata)
		{
			reservation->pdata = pdata;
		}
	//	free(pdata);
	}
	free(position);
	//free(data);

}

void Show_Reservations(Restaurant* table)
{
	Restaurant* tmpR = table;
	Date* tmpD = NULL;
	Hour* tmpH = NULL;

	Restaurant* head = table;
	if (tmpR)
		while (tmpR)
		{
			tmpD = tmpR->date;
			while (tmpD)
			{
				tmpH = tmpD->hour;
				while (tmpH)
				{
					char buff[30];
					printf("\n  nr.%d gosci: %d ", tmpR->table_number, tmpH->customers);
					strftime(buff, 30, " %d.%m.%y  %H:%M ", tmpD->date);
					printf(" %s ", buff);
					printf(" %s %s \n komentarz: %s \n", tmpH->pdata->last_name, tmpH->pdata->phone_number, tmpH->pdata->comment);
					tmpH = tmpH->next;
					printf("\n");
				}
				tmpD = tmpD->next;
			}
			tmpR = tmpR->next;
		}
	head = table;
}

void Save_File(Restaurant* table)
{
	FILE* file_reservations;
	errno_t err;

	err = fopen_s(&file_reservations, FILENAME_RESERVATIONS, "w");
	if (file_reservations != NULL)
	{
		if (err)
		{
			Alert_Red(" Nie udalo sie otworzyc pliku z rezerwacjami.");
		}
		else
		{
			Restaurant* tmpR = table;
			Date* tmpD = NULL;
			Hour* tmpH = NULL;

			Restaurant* head = table;
			if (tmpR)
				while (tmpR)
				{
					tmpD = tmpR->date;
					while (tmpD)
					{
						tmpH = tmpD->hour;
						while (tmpH)
						{
							char buff[30];
							strftime(buff, 30, "%d.%m.%y;%H:%M", tmpD->date);
							fprintf(file_reservations, "%d;%d;%s;%s;%s;%s;%s\n", tmpR->table_number, tmpH->customers, buff, tr(tmpH->pdata->name), tr(tmpH->pdata->last_name), tmpH->pdata->phone_number, tmpH->pdata->comment);
							tmpH = tmpH->next;
						}
						tmpD = tmpD->next;
					}
					tmpR = tmpR->next;
				}
			head = table;

			fclose(file_reservations);
		}
	}
}

void Action_Center(Restaurant* Thead)
{
	for (;;)
	{
		//system("cls");
		Restaurant* head = Thead;
		printf("\n Witaj w Restauracyjnym Systemie Rezerwacji\n\n");
		printf(" [1] Dodaj Rezerwacje \n");
		printf(" [2] Usun Rezerwacje \n");
		printf(" [3] Wyswietl Rezerwacje \n");
		printf(" [4] Wprowadz zmiany w systemie znizkowym \n");
		printf(" [5] Edytuj stoliki \n");
		printf(" [0] Zamknij program \n");
		char ch, tmp;
		int choice;
		for (;;)
		{
			printf(" >  ");
			//	scanf_s("%i", &choice);
			ch = getchar();

			while ((tmp = getchar()) != '\n');

			if (ch == '0')
				break;

			switch (ch)
			{
			case '1':
				Add_Reservation(head);
				head = Thead;
				break;
			case '2':
				break;
			case '3':
				Show_Reservations(head);
				head = Thead;
				break;
			case 4:
				break;
			case 5:
				break;
			default:
			{
				//while ((tmp = getchar()) != '\n');
				Alert_Red("\n Nieprawidlowa opcja. ");
				continue;
			}
			}
			break;
		}
		if (ch == '0')
			break;
	}
}

void Manager()
{
	Restaurant* head = Read_Restaurant_File();
	Restaurant* tmphead = head;

	Action_Center(head);
	head = tmphead;
	Save_File(head);
	DeleteList(head);
}




//void Read_Reservations()
//{
//
//}
//
//bool Is_Up_To_Date()
//{
//
//}
//
//void Delete_Record() //po numerze telefonu
//{
//
//}
//
//void Scrambler()
//{
//
//}
//
//void Descrambler()
//{
//
//}

#endif


//void Add_New_Table()
//void Modify_Tables()
//void Show_Specific_Reservations()
// | | | |