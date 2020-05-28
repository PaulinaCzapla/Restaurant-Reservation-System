<<<<<<< Updated upstream
#ifndef FUNCTIONS_C
#define FUNCTIONS_C
#include "functions_header.h"
=======
/** @file */

#ifndef FUNCTIONS_C
#define FUNCTIONS_C

#include "functions.h"


void Alert_Red(char* text)
{
	HANDLE hOut;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hOut, FOREGROUND_RED);
	puts(text);
	SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

}
void Green_Text(char* text)
{
	HANDLE hOut;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hOut, FOREGROUND_GREEN);
	puts(text);
	SetConsoleTextAttribute(hOut, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

}

bool Ask_Before_Delete(Hour* hour)
{
	char ch, tmp;
	char buff[30];

	for (;;)
	{
		strftime(buff, 30, " %d.%m.%y  %H:%M ", hour->hour);
		printf(" \n Czy na pewno chcesz usunac rezerwacje:  %s %s %s %s?", buff, hour->pdata->name, hour->pdata->last_name, hour->pdata->phone_number);
		printf(" \n\n [1] Tak");
		printf(" \n [2] Nie \n");
		printf(" >  ");
		scanf_s("%c", &ch, 1);

		while ((tmp = getchar()) != '\n');

		switch (ch)
		{
		case '1':
			return true;
		case '2':
			return false;
		default:
		{
			Alert_Red(" Nieprawidlowa opcja. \n");
			continue;
		}
		}
		break;
	}
}

short int Find_And_Delete_by_Phone_Number(char* num, Tables* head, bool ask)
{
	Tables* tmpR = head;
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
						if (ask)
							if (!Ask_Before_Delete(tmpH))
								return 2;

						free(tmpH->pdata);

						if (prevH)
						{
							prevH->next = tmpH->next;
							free(tmpH);
							return 1;
						}
						else
						{
							if (tmpH->next)
							{
								tmpD->hour = tmpH->next;
								free(tmpH);
								return 1;
							}
							if (prevD)
							{
								prevD->next = tmpD->next;
								free(tmpH);
								free(tmpD->date);
								free(tmpD);
								return 1;
							}
							else
							{
								if (tmpD->next)
								{
									tmpR->date = tmpD->next;
									free(tmpH);
									free(tmpD->date);
									free(tmpD);
									return 1;
								}

								else
								{
									tmpR->date = NULL;
									free(tmpH);
									free(tmpD->date);
									free(tmpD);
									return 1;
								}
							}
						}
					}
					prevH = tmpH;
					tmpH = tmpH->next;

				}
				prevH = NULL;
				prevD = tmpD;
				tmpD = tmpD->next;
			}
			prevD = NULL;
			tmpR = tmpR->next;
		}
	return 0;

}
void Delete_Record(Tables* head)
{
	if (head == NULL)
	{
		Alert_Red(" \n Brak stolikow w systemie. Dodaj informacje o stolikach w Edytuj stoliki -> Dodaj stolik. \n\n");
		puts("\n\n Wcisnij dowolny klawisz aby kontynuowac...");
		getch();
		return;
	}

	printf("\n\n Wpisz numer telefonu osoby, ktorej rezerwacja ma zostac usunieta: ");
	char number[MAX_PHONE_NUM];
	char tmp;

	fgets(number, MAX_PHONE_NUM, stdin);
	if (number[(strlen(number) - 1)] != '\n')
		while ((tmp = getchar()) != '\n');
	else
	{
		number[(strlen(number) - 1)] = '\0';
	}
	int a = Find_And_Delete_by_Phone_Number(number, head, true);
	if (a == 1)
		Green_Text(" \n\n Udalo sie usunac rezerwacje. \n");
	else if (a == 0)
		Alert_Red(" \n\n Nie znaleziono takiego numeru. \n");

	puts("\n\n Wcisnij dowolny klawisz aby kontynuowac...");
	getch();
}



Hour* Add_New_Reservation(Position* position, Tables* ptrTable, Date* ptrDate, struct tm* time, float how_long)
{
	Hour* HeadHour = NULL;
	if (ptrDate)
	{
		HeadHour = Hour_Node(&ptrDate, time, how_long);
		position->hour = HeadHour;
		position->date = ptrDate;
	}
	else if (ptrTable)
	{
		Date* HeadD = NULL;
		HeadD = Date_Node(&ptrTable, time);
		HeadHour = Hour_Node(&HeadD, time, how_long);
		position->hour = HeadHour;
		position->date = HeadD;
	}
	position->definitive = true;

	return HeadHour;
}

float Hours_Difference(struct tm* h1, struct tm* h2)
{
	float diff = 1000;
	if (h1->tm_year == h2->tm_year)
		if (h1->tm_mon == h2->tm_mon)
			if (h1->tm_mday == h2->tm_mday)
			{
				int diffH = h2->tm_hour - h1->tm_hour;
				float diffM = h2->tm_min - h1->tm_min;

				if (diffM < 0)
				{
					diffH--;
					diffM = 60 + diffM;
				}
				diffM = (diffM * 100) / 6000;
				diff = diffH + diffM;

			}
	return diff;
}

bool Check_Conflict(Date* prev, Date* next, struct tm* currtime, float how_long)
{
	bool result = true;
	float h_to_midn_current, h_to_mid_np, min_to_midn_current, min_to_midn_np;

	if (next)
	{
		if (currtime->tm_hour != 0)
			h_to_midn_current = 24 - (currtime->tm_hour);
		else
			h_to_midn_current = 0;

		h_to_mid_np = next->hour->hour->tm_hour;

		min_to_midn_current = 60 - (currtime->tm_min);
		min_to_midn_current = (min_to_midn_current * 100) / 6000;

		min_to_midn_np = next->hour->hour->tm_min;

		if (h_to_midn_current + h_to_mid_np + min_to_midn_current + min_to_midn_np > how_long)
			result = false;
	}
	if (prev != NULL)
	{
		Hour* tmp = prev->hour;

		while (tmp->next)
			tmp = tmp->next;

		h_to_midn_current = currtime->tm_hour;

		if (tmp->hour->tm_hour != 0)
			h_to_mid_np = 24 - (tmp->hour->tm_hour);
		else
			h_to_mid_np = 0;

		min_to_midn_np = 60 - (tmp->hour->tm_min);
		min_to_midn_np = (min_to_midn_np * 100) / 6000;

		min_to_midn_current = currtime->tm_min;

		if (h_to_midn_current + h_to_mid_np + min_to_midn_current + min_to_midn_np > prev->hour->how_long)
			result = false;
	}
	return result;
}

Hour* Check_Hours(Position* position, Tables* ptrTable, Date* ptrDate, struct tm* time, float how_long, Date* prevDate, Date* nextDate)
{
	Hour* headHour = ptrDate->hour;
	Hour* prev = NULL;
	Hour* result = NULL;
	while (true)
	{
		if (headHour == NULL)
			break;

		if (time->tm_hour <= headHour->hour->tm_hour)
		{
			if (Hours_Difference(time, headHour->hour) >= how_long && Check_Conflict(prevDate, nextDate, time, how_long))
			{
				if (prev)
				{
					if (Hours_Difference(time, prev->hour) >= prev->how_long)
					{
						result = Add_New_Reservation(position, NULL, ptrDate, time, how_long);
						break;
					}
				}
				else
				{
					result = Add_New_Reservation(position, NULL, ptrDate, time, how_long);
					break;
				}
			}
			else
			{
				position->table = ptrTable;
				position->date = ptrDate;
				position->definitive = false;
				break;
			}
		}
		if (time->tm_hour > headHour->hour->tm_hour)
		{
			if (headHour->next == NULL)
			{
				if (Hours_Difference(headHour->hour, time) >= headHour->how_long && Check_Conflict(prevDate, nextDate, time, how_long))
				{
					result = Add_New_Reservation(position, NULL, ptrDate, time, how_long);
					break;
				}
				else
					position->definitive = false;
			}
			prev = headHour;
			headHour = headHour->next;
		}
	}
	return result;
}

Hour* Check_Dates(Position* position, Tables* ptrTable, struct tm* time, float how_long)
{
	Date* headDat = ptrTable->date;
	Date* prev = NULL;
	Hour* result = NULL;
	bool conflict = false;
	while (headDat)
	{

		if (headDat->date->tm_year == time->tm_year)
			if (headDat->date->tm_mon == time->tm_mon)
				if (headDat->date->tm_mday == time->tm_mday)
				{
					result = Check_Hours(position, ptrTable, headDat, time, how_long, prev, headDat->next);
					conflict = true;
					break;
				}

		if (headDat->date->tm_year == time->tm_year)
		{
			if (headDat->date->tm_mon - time->tm_mon == 1)
			{
				if ((headDat->date->tm_mday == 1 && time->tm_mday == 30) || (headDat->date->tm_mday == 1 && time->tm_mday == 31) || (time->tm_mon == 1 && (headDat->date->tm_mday == 1 && time->tm_mday == 28)))
				{
					if (prev)
					{
						if (prev->date->tm_year == time->tm_year && prev->date->tm_mon == time->tm_mon && (time->tm_mday - prev->date->tm_mday) == 1)
						{
							result = Check_Hours(position, ptrTable, headDat, time, how_long, prev, headDat);
							conflict = true;
							break;
						}
					}
					result = Check_Hours(position, ptrTable, headDat, time, how_long, NULL, headDat);
					conflict = true;
					break;
				}
			}
			if (headDat->date->tm_mon - time->tm_mon == -1)
			{
				if ((headDat->date->tm_mday == 31 && time->tm_mday == 1) || (headDat->date->tm_mday == 30 && time->tm_mday == 1) || (headDat->date->tm_mon == 1 && (headDat->date->tm_mday == 28 && time->tm_mday == 1)))
				{
					if (headDat->next)
					{
						if (headDat->next->date->tm_year == time->tm_year && headDat->next->date->tm_mon == time->tm_mon && (headDat->next->date->tm_mday - time->tm_mday) == 1)
						{
							result = Check_Hours(position, ptrTable, headDat, time, how_long, headDat, headDat->next);
							conflict = true;
							break;
						}
					}
					result = Check_Hours(position, ptrTable, headDat, time, how_long, headDat, NULL);
					conflict = true;
					break;
				}
			}
		}
		if (headDat->date->tm_mon == time->tm_mon && headDat->date->tm_year == time->tm_year)
		{
			if (headDat->date->tm_mday + 1 == time->tm_mday)
			{
				if (headDat->next)
				{
					if (headDat->next->date->tm_year == time->tm_year && headDat->next->date->tm_mon == time->tm_mon && (headDat->next->date->tm_mday - time->tm_mday) == 1)
					{
						result = Check_Hours(position, ptrTable, headDat, time, how_long, headDat, headDat->next);
						conflict = true;
						break;
					}
				}

				result = Check_Hours(position, ptrTable, headDat, time, how_long, headDat, NULL);
				conflict = true;
				break;
			}
			if (headDat->date->tm_mday - 1 == time->tm_mday)
			{
				if (prev)
				{
					if (prev->date->tm_year == time->tm_year && prev->date->tm_mon == time->tm_mon && (time->tm_mday - prev->date->tm_mday) == 1)
					{
						result = Check_Hours(position, ptrTable, headDat, time, how_long, prev, headDat);
						conflict = true;
						break;
					}
				}
				result = Check_Hours(position, ptrTable, headDat, time, how_long, NULL, headDat);
				conflict = true;
				break;
			}
		}

		prev = headDat;
		headDat = headDat->next;
	}
	if (conflict == false)
		result = Add_New_Reservation(position, ptrTable, NULL, time, how_long);

	return result;
}

Hour* Check_Tables(Position* position, Tables* head, struct tm* time, int amount, float how_long)
{
	Hour* result = NULL;

	if (position->table)
	{
		if (position->table->date == NULL)
			result = Add_New_Reservation(position, position->table, NULL, time, how_long);
		else
			result = Check_Dates(position, position->table, time, how_long);
		return result;
	}

	while (true)
	{
		if (head->seats >= amount)
		{
			if (head->date == NULL)
			{
				result = Add_New_Reservation(position, head, NULL, time, how_long);
				break;
			}
			else
			{
				result = Check_Dates(position, head, time, how_long);
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


bool Check_Discounts_Repetitions(Discounts* head, int percent)
{
	while (head)
	{
		if (percent == head->percent)
			return false;
		head = head->next;
	}
	return true;
}

void Show_Discounts(Discounts* head)
{
	printf("\n");
	printf("----------------------------------------------------------------------------------------------------------------------");
	while (head)
	{
		printf("\n nazwa: %-15s znizka: %i%%\n", head->discount_name, head->percent);

		head = head->next;
	}
	printf("----------------------------------------------------------------------------------------------------------------------\n");
}

Discounts* Add_New_Discount_Type(Discounts* head)
{
	char tmp;
	char name_disc[MAX_DISCOUNT_NAME];
	int percent;
	Discounts* disc = NULL;
	printf("\n Podaj nazwe znizki (max %i znakow): ", MAX_DISCOUNT_NAME);

	fgets(name_disc, MAX_DISCOUNT_NAME, stdin);

	if (name_disc[(strlen(name_disc) - 1)] != '\n')
		while ((tmp = getchar()) != '\n');
	else
		name_disc[(strlen(name_disc) - 1)] = '\0';

	for (;;)
	{
		printf("\n Podaj wielkosc znizki: ");
		if (scanf_s("%i", &percent) == 0 || percent < 0 || percent >100)
		{
			while ((tmp = getchar()) != '\n');
			Alert_Red(" Nieprawidlowa wartosc. \n");
			continue;
		}
		while ((tmp = getchar()) != '\n');
		break;
	}

	if (Check_Discounts_Repetitions(head, percent))
	{
		if (disc = Discount_Node(&head, name_disc, percent))
			Green_Text("\n\n Udalo sie pomyslnie dodac znizke. ");
		else
			Alert_Red("\n\n Wystapil blad przy dodawaniu znizki.");
	}
	else
	{
		Alert_Red(" Taka znizka juz istnieje! ");
	}

	puts("\n\n Wcisnij dowolny klawisz aby kontynuowac...");
	getch();
	return disc;
}

Discounts* Find_Discount(Discounts* head, int percent)
{
	while (head)
	{
		if (percent == head->percent)
			return head;
		head = head->next;
	}
	return NULL;
}

void Delete_Discount_Safely(Discounts** head, Discounts* found)
{
	Discounts* tmphead = *head;
	Discounts* tmp = *head;
	Discounts* prev = NULL;

	while (tmp)
	{
		if (tmp == found)
		{
			if (prev)
				prev->next = tmp->next;

			else
				tmphead = tmp->next;

			free(tmp);
			break;
		}
		prev = tmp;
		tmp = tmp->next;
	}
	*head = tmphead;
}

void Delete_Discount_From_Reservation(Discounts* found, Tables* reservationsHead)
{
	Tables* tmpR = reservationsHead;
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
					if (tmpH->discount == found)
						tmpH->discount = NULL;

					tmpH = tmpH->next;

				}
				tmpD = tmpD->next;
			}
			tmpR = tmpR->next;
		}
}

void Delete_Discount(Discounts** head, Tables* reservationsHead)
{
	char tmp, ch;
	int percent;
	Discounts* found = NULL;

	for (;;)
	{
		printf("\n Podaj wielkosc znizki do usuniecia: ");
		if (scanf_s("%i", &percent) == 0)
		{
			while ((tmp = getchar()) != '\n');
			Alert_Red(" Nieprawidlowa wartosc. \n");
			continue;
		}
		break;
	}
	while ((tmp = getchar()) != '\n');

	if (found = Find_Discount(*head, percent))
	{
		for (;;)
		{
			Alert_Red(" \n UWAGA! Po usunieciu, znizka zostanie anulowana dla wszystkich klientow!");
			printf(" \n Czy na pewno chcesz usunac:  nazwa znizki: %-s  znizka: %i?", found->discount_name, found->percent);
			printf(" \n\n [1] Tak");
			printf(" \n [2] Nie \n");
			printf(" >  ");
			scanf_s("%c", &ch, 1);

			while ((tmp = getchar()) != '\n');

			switch (ch)
			{
			case '1':
				Delete_Discount_From_Reservation(found, reservationsHead);
				Delete_Discount_Safely(head, found);
				Green_Text(" \n Znizka zostala usunieta. \n");
				break;
			case '2':
				break;
			default:
				Alert_Red(" Nieprawidlowa opcja. \n");
				continue;
			}
			break;
		}
	}
	else
		Alert_Red(" Nie znaleziono takiej znizki. \n");

	puts("\n\n Wcisnij dowolny klawisz aby kontynuowac...");
	getch();
}

void Discount_Manager(Discounts** head, Tables* reservationsHead)
{
	Discounts* newdisc = NULL;
	char ch, tmp;

	for (;;)
	{
		system("cls");
		printf("\n");
		printf("\n\n");
		printf("                                            [1] Wyswietl wszystkie znizki \n");
		printf("                                            [2] Dodaj znizke \n");
		printf("                                            [3] Usun znizke \n");
		printf("                                            [0] Wyjdz \n");
		printf("\n");

		for (;;)
		{
			printf(" >  ");
			scanf_s("%c", &ch, 1);

			while ((tmp = getchar()) != '\n');

			if (ch == '0')
				break;

			switch (ch)
			{
			case '1':
				if (*head == NULL)
				{
					Alert_Red(" \n Brak znizek. \n\n");
					puts("\n\n Wcisnij dowolny klawisz aby kontynuowac...");
					getch();
					break;
				}
				Show_Discounts(*head);
				puts("\n\n Wcisnij dowolny klawisz aby kontynuowac...");
				getch();
				break;
			case '2':
				newdisc = Add_New_Discount_Type(*head);
				if ((*head) == NULL)
					*head = newdisc;
				break;
			case '3':
				if (*head == NULL)
				{
					Alert_Red(" \n Brak znizek. \n\n");
					puts("\n\n Wcisnij dowolny klawisz aby kontynuowac...");
					getch();
					break;
				}
				Delete_Discount(head, reservationsHead);
				break;
			default:
				Alert_Red(" Nieprawidlowa opcja. \n");
				continue;
			}
			break;
		}
		if (ch == '0')
			break;
	}
}

bool Connect_Discount_And_Record(Hour** reservation, Discounts** headDiscounts, int percent)
{
	Discounts* tmp = Find_Discount(*headDiscounts, percent);
	if (tmp)
	{
		(*reservation)->discount = tmp;
		return true;
	}
	return false;
}

void Add_Discount_To_Reservation(Hour* reservation, Discounts* head)
{
	int choice, percent;
	char tmp;

	printf("\n\n Dodac znizke? ");
	printf("\n [1] Tak ");
	printf("\n [2] Nie \n");

	for (;;)
	{
		printf(" >  ");
		scanf_s("%d", &choice);

		int i = 1;
		switch (choice)
		{
		case 1:
			for (;;)
			{
				printf(" Podaj %% znizki: ");
				if (scanf_s("%i", &percent) == 0)
				{
					while ((tmp = getchar()) != '\n');
					Alert_Red(" Nieprawidlowa wartosc. \n");
					continue;
				}
				break;
			}

			if (Connect_Discount_And_Record(&reservation, &head, percent))
			{
				Green_Text(" Dodano znizke do zamowienia. \n");
				break;
			}

			else
				Alert_Red(" Nie ma takiej znizki. \n");
			printf("\n Dodac inna znizke? ");
			printf("\n [1] Tak ");
			printf("\n [2] Nie \n");
			continue;

			break;
		case 2:
			return;
			break;
		default:
			while ((tmp = getchar()) != '\n');
			Alert_Red(" Nieprawidlowa opcja. \n");
			continue;
		}
		break;
	}
}


void Add_Reservation(Tables* headT, Discounts* headDiscounts)
{
	struct tm* data = malloc(sizeof(struct tm));
	struct Position* position = malloc(sizeof(struct Position));
	int amount, month;
	float how_long;
	position->date = NULL;
	position->hour = NULL;
	position->table = NULL;
	position->definitive = true;
	char tmpo;

	printf("\n");

	if (headT == NULL)
	{
		Alert_Red(" \n Brak stolikow w systemie. Dodaj informacje o stolikach w Edytuj stoliki -> Dodaj stolik. \n\n");
		puts("\n\n Wcisnij dowolny klawisz aby kontynuowac...");
		getch();
		return;
	}

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
			if (scanf_s("%i", &(data->tm_mday)) == 0 || data->tm_mday < 1 || data->tm_mday > 31)
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
			if (scanf_s("%i", &month) == 0 || month > 12 || month < 1)
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
			if (scanf_s("%i", &(data->tm_year)) == 0 || data->tm_year < 0)
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
			if (scanf_s("%i", &(data->tm_hour)) == 0 || data->tm_hour > 23 || data->tm_hour < 0)
			{
				while ((tmpo = getchar()) != '\n');
				Alert_Red(" Nieprawidlowa wartosc. \n");
				continue;
			}
			break;
		}
		for (;;)
		{
			printf(" Minuty: ");
			if (scanf_s("%i", &(data->tm_min)) == 0 || data->tm_min >= 60 || data->tm_min < 0)
			{
				while ((tmpo = getchar()) != '\n');
				Alert_Red(" Nieprawidlowa wartosc. \n");
				continue;
			}
			break;
		}
		for (;;)
		{
			printf(" Jak dlugo: ");
			if (scanf_s("%f", &how_long) == 0 || how_long < 0)
			{
				while ((tmpo = getchar()) != '\n');
				Alert_Red(" Nieprawidlowa wartosc. \n");
				continue;
			}
			break;
		}
		if (Is_Up_To_Date(data) == false)
		{
			Alert_Red(" Nieprawidlowa data. \n");
			continue;
		}
		break;
	}

	Tables* tmp = NULL;

	Hour* reservation = Check_Tables(position, headT, data, amount, how_long);
	for (;;)
		if (reservation == NULL)
		{
			if (position->definitive == true)
			{
				Alert_Red(" Nie ma wolnego stolika.\n ");
				while ((tmpo = getchar()) != '\n');
				break;
			}
			else
			{
				tmp = position->table;
				if (tmp->next)
				{
					position->table = tmp->next;
					reservation = Check_Dates(position, tmp->next, data, how_long);
				}
				else
					position->definitive = true;
			}
		}
		else
			break;

	if (reservation)
	{

		Green_Text("\n\n Udalo sie dodac rezerwacje. Prosze o podanie danych osobowych:  \n");

		if (headDiscounts)
		{
			Show_Discounts(headDiscounts);
			Add_Discount_To_Reservation(reservation, headDiscounts);
		}

		reservation->customers = amount;

		Personal_Data* pdata = malloc(sizeof(Personal_Data));

		char tmp;
		bool number = true;
		printf("\n");
		printf(" Imie: ");
		scanf_s("%s", pdata->name, MAX_NAME);
		while ((tmp = getchar()) != '\n');
		printf(" Nazwisko: ");
		scanf_s("%s", pdata->last_name, MAX_L_NAME);
		while ((tmp = getchar()) != '\n');

		for (;;)
		{
			printf(" Numer telefonu:  ");
			scanf_s("%s", pdata->phone_number, MAX_PHONE_NUM);
			while ((tmp = getchar()) != '\n');

			for (int i = 0; i < strlen(pdata->phone_number); i++)
				if (!isdigit((pdata->phone_number)[i]))
				{
					Alert_Red(" Numer telefonu moze zawierac tylko cyfry. \n");
					number = false;
					break;
				}
			if (number == false)
			{
				number = true;
				continue;
			}

			else
				break;
		}

		printf("\n Dodac komentarz do zamowienia? ");
		printf("\n [1] Tak ");
		printf("\n [2] Nie \n");

		int choice;
		for (;;)
		{
			printf(" >  ");
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
			reservation->pdata = pdata;
	}
	free(position);
	puts("\n\n Wcisnij dowolny klawisz aby kontynuowac...");
	getch();
	return;
}


Tables* Find_Table(int num, Tables* head)
{
	Tables* table = NULL;
	Tables* tmphead = head;

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

void Show_Tables(Tables* pH)
{
	printf("\n");
	printf("----------------------------------------------------------------------------------------------------------------------");
	while (pH)
	{
		printf("\n stolik nr %-3i   max. liczba osob: %-3i \n", pH->table_number, pH->seats);
		pH = pH->next;
	}
	printf("----------------------------------------------------------------------------------------------------------------------");
	puts("\n\n Wcisnij dowolny klawisz aby kontynuowac...");
	getch();
}

void Add_New_Table(Tables** head)
{
	int tabnum, amount;
	char tmp;
	for (;;)
	{
		printf("\n Podaj numer stolika: ");

		if (scanf_s("%i", &tabnum) == 0)
		{
			while ((tmp = getchar()) != '\n');
			Alert_Red(" Nieprawidlowa wartosc. \n");
			continue;
		}
		else
			if (Find_Table(tabnum, *head))
			{
				Alert_Red(" Taki stolik juz istnieje. Numery stolikow nie moga sie powtarzac. \n");
				continue;
			}

		break;
	}
	for (;;)
	{
		printf(" Podaj maksymalna ilosc osob przy stoliku: ");
		if (scanf_s("%i", &amount) == 0)
		{
			while ((tmp = getchar()) != '\n');
			Alert_Red(" Nieprawidlowa wartosc. \n");
			continue;
		}
		break;
	}

	Tables* node = Table_Node(head, tabnum, amount);

	if (node != *head)
		(*head) = node;

	Green_Text(" \n Udalo sie dodac stolik. ");
	puts("\n\n Wcisnij dowolny klawisz aby kontynuowac...");
	getch();
	while ((tmp = getchar()) != '\n');
}

void Delete_Table(Tables** head, Tables* found)
{
	Tables* tmphead = *head;
	Tables* tmp = *head;
	Tables* prev = NULL;

	while (tmp)
	{
		if (tmp == found)
		{
			if (prev)
				prev->next = tmp->next;

			else
				tmphead = tmp->next;

			if (tmp->date)
				if (tmp->date->hour)
					Find_And_Delete_by_Phone_Number(tmp->date->hour->pdata->phone_number, tmp, false);

			free(tmp);
			break;
		}
		prev = tmp;
		tmp = tmp->next;
	}
	*head = tmphead;
}

void Select_Table_To_Delete(Tables** head)
{
	char tmp;
	int number;

	for (;;)
	{
		printf("\n Podaj numer stolika do usuniecia: ");
		if (scanf_s("%i", &number) == 0)
		{
			while ((tmp = getchar()) != '\n');
			Alert_Red(" Nieprawidlowa wartosc. \n");
			continue;
		}
		else
			if (!Find_Table(number, *head))
			{
				Alert_Red(" Taki stolik nie istnieje. \n");
				continue;
			}
		break;
	}
	while ((tmp = getchar()) != '\n');

	Tables* found = NULL;
	char ch;

	if (found = Find_Table(number, *head))
	{
		for (;;)
		{
			Alert_Red(" \n UWAGA! Wszystkie rezerwacje z wybranego stolika zostana usuniete.");
			printf(" \n Czy na pewno chcesz usunac:  numer stolika: %-i  max. ilosc osob: %i?", found->table_number, found->seats);
			printf(" \n\n [1] Tak");
			printf(" \n [2] Nie \n");
			printf(" >  ");
			scanf_s("%c", &ch, 1);

			while ((tmp = getchar()) != '\n');

			if (ch == '0')
				break;

			switch (ch)
			{
			case '1':
				Delete_Table(head, found);
				Green_Text(" \n Stolik zostal usuniety. \n");
				break;
			case '2':
				break;
			default:
			{
				Alert_Red(" Nieprawidlowa opcja. \n");
				continue;
			}
			}
			break;
		}
	}
	puts("\n\n Wcisnij dowolny klawisz aby kontynuowac...");
	getch();
}

void Tables_Manager(Tables** head)
{
	for (;;)
	{
		system("cls");
		if (*head == NULL)
		{
			Alert_Red(" \n Uwaga! Brak stolikow w systemie. Dodaj informacje o stolikach w Dodaj stolik. ");
		}
		printf("\n");
		printf("\n\n");
		printf("                                            [1] Wyswietl stoliki \n");
		printf("                                            [2] Dodaj stolik \n");
		printf("                                            [3] Usun stolik \n");
		printf("                                            [0] Wyjdz \n");
		printf("\n");
		char ch, tmp;

		for (;;)
		{
			printf(" >  ");
			scanf_s("%c", &ch, 1);

			while ((tmp = getchar()) != '\n');

			if (ch == '0')
				break;

			switch (ch)
			{
			case '1':
				if (*head == NULL)
				{
					Alert_Red(" \n Brak stolikow w systemie. \n");
					puts("\n\n Wcisnij dowolny klawisz aby kontynuowac...");
					getch();
					break;
				}
				else
					Show_Tables(*head);
				break;
			case '2':
				Add_New_Table(head);
				break;
			case '3':
				if (*head == NULL)
				{
					Alert_Red(" \n Brak stolikow w systemie. \n");
					puts("\n\n Wcisnij dowolny klawisz aby kontynuowac...");
					getch();
					break;
				}
				else
					Select_Table_To_Delete(head);
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



void Show_One_Reservation(Tables* table, Hour* hour)
{
	char buff[30];
	printf("\n  nr stolika: %i   liczba gosci: %i  jak dlugo: %.2fh", table->table_number, hour->customers, hour->how_long);
	strftime(buff, 30, " %d.%m.%y  %H:%M ", hour->hour);
	printf(" %s   ", buff);
	printf("\n  Dane: %-5s %-7s %-12s\n  komentarz: %-s \n", hour->pdata->name, hour->pdata->last_name, hour->pdata->phone_number, hour->pdata->comment);

	if (hour->discount)
		printf("  znizka:    %-10s  %-3d%% \n", hour->discount->discount_name, hour->discount->percent);
	printf("----------------------------------------------------------------------------------------------------------------------");
	printf("\n");
}

bool Find_Reservation_by_Phone_Number(char* num, Tables* head)
{
	Tables* tmpR = head;
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
					if (strcmp(num, tmpH->pdata->phone_number) == 0)
					{
						Show_One_Reservation(head, tmpH);
						tmpH = tmpH->next;
						return true;
					}
					tmpH = tmpH->next;
				}
				tmpD = tmpD->next;
			}
			tmpR = tmpR->next;
		}
	return false;
>>>>>>> Stashed changes

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
<<<<<<< Updated upstream
=======

	if (Find_Day_And_Show(head, &data) == false)
		Alert_Red(" Brak rezerwacji w danym dniu. \n");

	puts("\n\n Wcisnij dowolny klawisz aby kontynuowac...");
	getch();
	while ((tmpo = getchar()) != '\n');
}

void Show_Chosen_Reservation(Tables* head)
{
	printf("\n\n Wpisz numer telefonu osoby, ktorej rezerwacje chcesz znalezc: ");
	char number[MAX_PHONE_NUM];
	char tmp;

	fgets(number, MAX_PHONE_NUM, stdin);
	if (number[(strlen(number) - 1)] != '\n')
		while ((tmp = getchar()) != '\n');
	else
		number[(strlen(number) - 1)] = '\0';


	if (Find_Reservation_by_Phone_Number(number, head) == false)
		Alert_Red(" Nie znaleziono takiego numeru. \n");

	puts("\n\n Wcisnij dowolny klawisz aby kontynuowac...");
	getch();
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
			while (fscanf_s(file_restaurant, "%i %i", &table, &amount) != EOF /*&& fscanf_s(file_restaurant, "%i", &amount) != EOF*/)
=======
bool Save_Restaurant_File(Tables* head)
{
	FILE* file_restaurant;
	errno_t err;
	Tables* pH = NULL;

	err = fopen_s(&file_restaurant, FILENAME_RESTAURANT, "w");
	if (file_restaurant != NULL)
	{
		if (err)
		{
			Alert_Red("\n Blad przy otwieraniu pliku ze stolikami.");
			return false;
		}
		else
		{
			while (head)
>>>>>>> Stashed changes
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