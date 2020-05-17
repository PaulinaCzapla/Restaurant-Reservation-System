#ifndef FUNCTIONS_C
#define FUNCTIONS_C

#include "functions_header.h"

#define FILENAME_RESTAURANT "restaurant.txt"
#define FILENAME_RESERVATIONS "reservations.txt"
#define KEY -7

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




Restaurant* Find_Table(int num, Restaurant* head)
{

	Restaurant* table = NULL;
	Restaurant* tmphead = head;

	while (tmphead)
	{
		if (tmphead->table_number == num)
		{
			table = tmphead;
			break;
		}

		tmphead = tmphead->next;
	}
	return table;
}

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
	printf("check hours");
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
			result = Add_New_Reservation(position, NULL, headDat, time);
			break;
		}
		else
		{
			position->table = headTab;
			position->date = headDat;
			position->definitive = false;
			break;
		}
	}
	return result;
}

Hour* Check_Dates(Position* position, Restaurant* headTab, struct tm* time)
{
	printf("check dates");
	Date* headDat = headTab->date;
	Hour* result = NULL;
	bool conflict = false;
	while (headDat)
	{
		//if (headDat->date->tm_year == time->tm_year)
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

	if (position->table)
	{
		//Restaurant* table = Find_Table(position->table, head);

		if (position->table->date == NULL)
			result = Add_New_Reservation(position, position->table, NULL, time);

		else
			result = Check_Dates(position, position->table, time);
		return result;
	}

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

bool Is_Up_To_Date(struct tm* time_to_check)
{
	struct tm newtime;
	time_t current_time;
	time(&current_time);

	localtime_s(&newtime, &current_time);

	while (newtime.tm_year >= 100)
		newtime.tm_year -= 100;

	if (newtime.tm_year == time_to_check->tm_year)
	{
		if (newtime.tm_mon == time_to_check->tm_mon)
		{
			if (newtime.tm_mday <= time_to_check->tm_mday)
				return true;
		}

		if (newtime.tm_mon < time_to_check->tm_mon)
			return true;
	}
	if (newtime.tm_year < time_to_check->tm_year)
		return true;

	return false;
}

void Add_Reservation(Restaurant* headT)
{
	struct tm* data = malloc(sizeof(struct tm));
	struct Position* position = malloc(sizeof(struct Position));
	int amount, month;
	position->date = NULL;
	position->hour = NULL;
	position->table = NULL;
	position->definitive = true;
	char tmpo;

	for (;;)
	{
		printf(" Ilosc osob: ");
		if (scanf_s("%i", &amount) == 0)
		{
			while ((tmpo = getchar()) != '\n');
			Alert_Red(" Nieprawidlowa wartosc. \n");
			continue;
		}
		break;
	}
	for (;;)
	{
		for (;;)
		{
			printf(" Dzien: ");
			if (scanf_s("%i", &(data->tm_mday)) == 0)
			{
				while ((tmpo = getchar()) != '\n');
				Alert_Red(" Nieprawidlowa wartosc. \n");
				continue;
			}
			break;
		}
		for (;;)
		{
			printf(" Miesiac: ");
			if (scanf_s("%i", &month) == 0)
			{
				while ((tmpo = getchar()) != '\n');
				Alert_Red(" Nieprawidlowa wartosc. \n");
				continue;
			}
			data->tm_mon = month - 1;
			break;
		}
		for (;;)
		{
			printf(" Rok: ");
			if (scanf_s("%i", &(data->tm_year)) == 0)
			{
				while ((tmpo = getchar()) != '\n');
				Alert_Red(" Nieprawidlowa wartosc. \n");
				continue;
			}
			if (data->tm_year > 2000)
				data->tm_year = data->tm_year - 2000;
			break;
		}
		for (;;)
		{
			printf(" Godzina(bez minut): ");
			if (scanf_s("%i", &(data->tm_hour)) == 0)
			{
				while ((tmpo = getchar()) != '\n');
				Alert_Red(" Nieprawidlowa wartosc. ");
				continue;
			}
			break;
		}
		for (;;)
		{
			printf(" Minuty: ");
			if (scanf_s("%i", &(data->tm_min)) == 0)
			{
				while ((tmpo = getchar()) != '\n');
				Alert_Red(" Nieprawidlowa wartosc. \n");
				continue;
			}
			break;
		}
		if (Is_Up_To_Date(data) == false)
		{
			Alert_Red("\n Nieprawidlowa data. ");
			continue;
		}
		break;
	}
	Restaurant* tmp = NULL;

	Hour* reservation = Check_Tables(position, headT, data, amount);
	for (;;)
		if (reservation == NULL)
		{
			if (position->definitive == true)
			{
				Alert_Red(" Nie ma w tym dniu wolnego stolika.\n ");
				while ((tmpo = getchar()) != '\n');
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
		char tmp;

		printf(" Imie: ");
		scanf_s("%s", pdata->name, MAX_NAME);
		while ((tmp = getchar()) != '\n');
		printf(" Nazwisko: ");
		scanf_s("%s", pdata->last_name, MAX_L_NAME);
		while ((tmp = getchar()) != '\n');
		printf(" Numer telefonu:  ");
		scanf_s("%s", pdata->phone_number, MAX_PHONE_NUM);
		while ((tmp = getchar()) != '\n');

		printf("\n Dodac komentarz do zamowienia? ");
		printf("\n [1] Tak ");
		printf("\n [2] Nie \n");

		int choice;
		for (;;)
		{
			printf(">  ");
			scanf_s("%d", &choice);

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
				{
					pdata->comment[(strlen(pdata->comment) - 1)] = '\0';
				}
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
				//	(pdata->comment)[4] = '\n';
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
			reservation->pdata = pdata;
	}
	free(position);
	puts("\n\n Wcisnij dowolny klawisz aby kontynuowac...");
	getch();
	return;
}

void Show_Reservations(Restaurant* table)
{
	Restaurant* tmpR = table;
	Date* tmpD = NULL;
	Hour* tmpH = NULL;

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
					printf("\n  nr.%i gosci: %i ", tmpR->table_number, tmpH->customers);
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
	puts("\n\n Wcisnij dowolny klawisz aby kontynuowac...");
	getch();
}

void Convert_Date(char* date, struct tm* dtime, char* tmp)
{
	int  j = 0, k = 0;
	for (int i = 0; ; i++)
	{
		if (date[i] == '.' || date[i] == '\0')
		{
			if (k == 0)
			{
				dtime->tm_mday = atoi(tmp);
				j = 0;
				k++;
				continue;
			}
			if (k == 1)
			{
				dtime->tm_mon = atoi(tmp) - 1;
				j = 0;
				k++;
				continue;
			}
			if (k == 2)
			{
				dtime->tm_year = atoi(tmp);
				memset(tmp, '\0', 12);
				memset(date, '\0', 12);
				j = 0;
				k = 0;
				break;
			}
		}
		tmp[j] = date[i];
		j++;
	}
}

void Convert_Hour(char* hour, struct tm* dtime, char* tmp)
{
	int  j = 0, k = 0;
	for (int i = 0; hour[i] != '\0'; i++)
	{
		if (hour[i] == ':')
		{
			dtime->tm_hour = atoi(tmp);

			j = 0;
			tmp[j] = hour[i + 1];
			tmp[j + 1] = hour[i + 2];
			tmp[j + 2] = '\0';

			dtime->tm_min = atoi(tmp);

			memset(tmp, '\0', 12);
			memset(hour, '\0', 12);
			j = 0;
			break;
		}
		tmp[j] = hour[i];
		j++;
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
			Alert_Red(" Blad przy otwieraniu pliku ze stolikami.\n");
			return NULL;
		}
		else
		{
			int amount, table;

			int i = 0;
			Restaurant* head = NULL;
			while (fscanf_s(file_restaurant, "%i %i", &table, &amount) != EOF)
			{
				pH = Node(pH, table, amount);
			}
			fclose(file_restaurant);
			//	WYSWIETL(pH);
		}
	}
	return pH;
}

void Scrambler(char* text)
{
	int j = 0;
	if (sizeof(text) == MAX_COMMENT)
		j = 0;
	if (text)
	{
		for (int i = 0; i < strlen(text)-j; i++)
		{
			if (text[i] == ' ')
				continue;
			text[i] = text[i] + KEY;
		}
	}
}

void Descrambler(char* text)
{
	int j=0;
	if (sizeof(text) == MAX_COMMENT)
		j = 0;
	if (text)
	{
		for (int i = 0; i < strlen(text)-j; i++)
		{
			if (text[i] == ' ')
				continue;
			text[i] = text[i] - KEY;
		}
	}
}



bool Read_Reservations_File(Restaurant* Thead)
{
	FILE* file_reservations;
	errno_t err;

	err = fopen_s(&file_reservations, FILENAME_RESERVATIONS, "r");

	if (file_reservations != NULL)
	{
		if (err)
		{
			Alert_Red(" Nastapil blad przy otwieraniu pliku z rezerwacjami.\n");
			return false;
		}
		else
		{
			Hour* reservation = NULL;

			struct Position* position = malloc(sizeof(struct Position));
			position->date = NULL;
			position->hour = NULL;
			position->table = NULL;
			position->definitive = true;

			int  amount, table;
			char tmpc;
			char date[12];
			char hour[12];
			char tmp[12];

			Personal_Data pdata;
			struct tm time;
			struct tm* dtime = &time;

			while (fscanf_s(file_reservations, "%d", &table) != EOF)
			{
				fscanf_s(file_reservations, "%d", &amount);
				fscanf_s(file_reservations, "%s", date, 12);
				fscanf_s(file_reservations, "%s", hour, 12);
				fscanf_s(file_reservations, "%s", pdata.name, MAX_NAME);
				fscanf_s(file_reservations, "%s", pdata.last_name, MAX_L_NAME);
				fscanf_s(file_reservations, "%s", pdata.phone_number, MAX_PHONE_NUM);

				Descrambler(pdata.name);
				Descrambler(pdata.last_name);
				Descrambler(pdata.phone_number);
				

				position->table = Find_Table(table, Thead);
				Personal_Data* pdata2 = malloc(sizeof(Personal_Data));
				struct tm* dtime2 = malloc(sizeof(struct tm));

				/*		5 3 12.02.3920 14:30 Kasia Jasia 234234234 brak
							2 5 03.02.3920 13 : 40 Jadzia Bˆonka 345345345 uczulenie na to i na to i na to tez*/
				Convert_Date(date, &time, tmp);
				Convert_Hour(hour, dtime, tmp);

				if (Is_Up_To_Date(dtime) == false)
				{
					free(dtime2);
					free(pdata2);
					fgets(pdata.comment, MAX_COMMENT, file_reservations);

					continue;
				}

				if (fgets(pdata.comment, MAX_COMMENT, file_reservations))
				{
					if (pdata.comment == NULL)
					{
						Alert_Red(" \n Blad oczytu pliku reservations.csv - nieprawidlowy format.\n");
						return false;
					}
					if (pdata.comment[(strlen(pdata.comment) - 1)] != '\n')
						while ((tmpc = getchar()) != '\n');
					else
					{
						pdata.comment[(strlen(pdata.comment) - 1)] = '\0';
					}
					Descrambler(pdata.comment);
					*(pdata2) = pdata;
					*(dtime2) = time;

					reservation = Check_Tables(position, position->table, dtime2, amount);
					if (reservation)
					{
						reservation->customers = amount;
						reservation->pdata = pdata2;
					}
					else
					{
						Alert_Red(" \n Nieprawidlowe dane w pliku. \n");
						return false;
					}
					position->date = NULL;
					position->hour = NULL;
					position->table = NULL;
				}
				else
					break;
			}
			free(position);
			fclose(file_reservations);
		}
	}
	return true;


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
							strftime(buff, 30, "%d.%m.%y %H:%M", tmpD->date);
							//strncpy((tmpH->pdata->name), Scrambler(tmpH->pdata->name));
							Scrambler(tmpH->pdata->name);
							Scrambler(tmpH->pdata->last_name);
							Scrambler(tmpH->pdata->phone_number);
							Scrambler(tmpH->pdata->comment);
							fprintf(file_reservations, "%d %d %s %s %s %s %s \n", tmpR->table_number, tmpH->customers, buff, tmpH->pdata->name, tmpH->pdata->last_name, tmpH->pdata->phone_number, tmpH->pdata->comment);
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

bool Find_And_Delete_Phone_Number(char* num, Restaurant* head)
{
	Restaurant* tmpR = head;
	Date* tmpD = NULL;
	Hour* tmpH = NULL;

	Date* prevD = NULL;
	Hour* prevH = NULL;

	if (tmpR)
		while (tmpR)
		{
			tmpD = tmpR->date;
			while (tmpD)
			{
				tmpH = tmpD->hour;
				while (tmpH)
				{
					if (strcmp(num, tmpH->pdata->phone_number) == 0)
					{
						free(tmpH->pdata);

						if (prevH)
						{
							prevH->next = tmpH->next;
							free(tmpH);
							return true;
						}
						else
						{
							if (tmpH->next)
							{
								tmpD->hour = tmpH->next;
								free(tmpH);
								return true;
							}
							if (prevD)
							{
								prevD->next = tmpD->next;
								free(tmpH);
								return true;
							}
							else
							{
								if (tmpD->next)
								{
									tmpR->date = tmpD->next;
									free(tmpH);
									return true;
								}

								else
								{
									tmpR->date = NULL;
									free(tmpH);
									free(tmpD->date);
									free(tmpD);
									return true;
								}
							}
						}

					}
					prevH = tmpH;
					tmpH = tmpH->next;

				}
				prevD = tmpD;
				tmpD = tmpD->next;
			}
			tmpR = tmpR->next;
		}
	return false;

}

void Delete_Record(Restaurant* head) //po numerze telefonu
{
	printf("\n\n Wpisz numer telefonu osoby, ktorej rezerwacja ma zostac usunieta: ");
	char number[MAX_PHONE_NUM];
	char tmp;
	//while ((tmp = getchar()) != '\n');

	fgets(number, MAX_PHONE_NUM, stdin);
	if (number[(strlen(number) - 1)] != '\n')
		while ((tmp = getchar()) != '\n');
	else
	{
		number[(strlen(number) - 1)] = '\0';
	}

	if (Find_And_Delete_Phone_Number(number, head))
		puts(" \n\n Udalo sie usunac rezerwacje. \n");
	else
		Alert_Red(" \n\n Nie znaleziono takiego numeru. \n");

	puts("\n\n Wcisnij dowolny klawisz aby kontynuowac...");
	getch();
}

void Action_Center(Restaurant* Thead)
{
	//bool first = true;
	for (;;)
	{
		system("cls");
		Restaurant* head = Thead;
		printf("\n");
		printf("\n                                        Restauracyjny System Rezerwacji\n\n");
		printf("                                            [1] Dodaj Rezerwacje \n");
		printf("                                            [2] Usun Rezerwacje \n");
		printf("                                            [3] Wyswietl Rezerwacje \n");
		printf("                                            [4] Wprowadz zmiany w systemie znizkowym \n");
		printf("                                            [5] Edytuj stoliki \n");
		printf("                                            [0] Zamknij program \n");
		printf("\n");
		char ch, tmp;

		for (;;)
		{
			/*	if(first == false)
				while ((tmp = getchar()) != '\n');*/

				/*	first = false;*/

			printf(" >  ");
			scanf_s("%c", &ch, 1);

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
				Delete_Record(head);
				head = Thead;
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
				Alert_Red(" Nieprawidlowa opcja. \n");
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

	if (head == NULL)
	{
		Alert_Red(" Nastapil blad przy probie odczytu pliku ze stolikami. \n");
		return;
	}

	if (Read_Reservations_File(head) == false)
		return;


	Action_Center(head);

	//Show_Reservations(head);
	head = tmphead;
	Save_File(head);
	DeleteList(head);
	_CrtDumpMemoryLeaks();
}






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