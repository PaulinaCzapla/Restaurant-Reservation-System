
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

}

void Show_All_Reservations(Tables* table)
{
	Tables* tmpR = table;
	Date* tmpD = NULL;
	Hour* tmpH = NULL;
	bool check = false;
	if (tmpR)
		while (tmpR)
		{
			tmpD = tmpR->date;
			while (tmpD)
			{
				tmpH = tmpD->hour;
				while (tmpH)
				{
					Show_One_Reservation(tmpR, tmpH);
					tmpH = tmpH->next;
					check = true;
				}
				tmpD = tmpD->next;
			}
			tmpR = tmpR->next;
		}
	if (!check)
		Alert_Red(" \n Brak rezerwacji do wyswietlenia. ");
	puts("\n\n Wcisnij dowolny klawisz aby kontynuowac...");
	getch();
}

bool Find_Day_And_Show(Tables* head, struct tm* dat)
{
	struct tm date;
	bool check = false;

	if (dat == NULL)
	{
		time_t current_time;
		time(&current_time);

		localtime_s(&date, &current_time);

		while (date.tm_year >= 100)
			date.tm_year -= 100;
	}
	else
		date = *dat;

	while (head)
	{
		if (head->date)
		{
			Date* day = head->date;
			while (day)
			{
				if (day->date->tm_year == date.tm_year && day->date->tm_mon == date.tm_mon && day->date->tm_mday == date.tm_mday)
				{
					Hour* tmpH = day->hour;
					while (tmpH)
					{
						Show_One_Reservation(head, tmpH);
						tmpH = tmpH->next;
						check = true;
					}
				}
				day = day->next;
			}
		}
		head = head->next;
	}
	return check;
}

void Show_Reservations_by_Day(Tables* head)
{
	char tmpo;
	struct tm data;
	int month;

	printf("\n Podaj date: \n");

	for (;;)
	{
		for (;;)
		{
			printf(" Dzien: ");
			if (scanf_s("%i", &(data.tm_mday)) == 0)
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
			data.tm_mon = month - 1;
			break;
		}
		for (;;)
		{
			printf(" Rok: ");
			if (scanf_s("%i", &(data.tm_year)) == 0)
			{
				while ((tmpo = getchar()) != '\n');
				Alert_Red(" Nieprawidlowa wartosc. \n");
				continue;
			}
			if (data.tm_year > 2000)
				data.tm_year = data.tm_year - 2000;
			break;
		}
		break;
	}

	if (Find_Day_And_Show(head, &data) == false)
		Alert_Red(" Brak rezerwacji w danym dniu. \n");

	while ((tmpo = getchar()) != '\n');
	puts("\n\n Wcisnij dowolny klawisz aby kontynuowac...");
	getch();
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
}

void Show_Today_Reservations(Tables* head)
{
	if (Find_Day_And_Show(head, NULL) == false)
		Alert_Red(" Brak rezerwacji w danym dniu. \n");

	puts("\n\n Wcisnij dowolny klawisz aby kontynuowac...");
	getch();
}

void Show_Reservations(Tables* head)
{

	if (head == NULL)
	{
		Alert_Red(" \n Brak rezerwacji w systemie. \n");
		puts("\n\n Wcisnij dowolny klawisz aby kontynuowac...");
		getch();
		return;

	}

	for (;;)
	{
		system("cls");
		printf("\n");
		printf("\n\n");
		printf("                                            [1] Wszystkie Rezerwacje \n");
		printf("                                            [2] Rezerwacje wybranego dnia \n");
		printf("                                            [3] Dzisiejsze rezerwacje \n");
		printf("                                            [4] Znajdz rezerwacje \n");
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
				Show_All_Reservations(head);
				break;
			case '2':
				Show_Reservations_by_Day(head);
				break;
			case '3':
				Show_Today_Reservations(head);

				break;
			case '4':
				Show_Chosen_Reservation(head);
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



int Check_Char(char ch)
{
	if (ch >= 'a' && ch <= 'z')
		return 0;
	if (ch >= 'A' && ch <= 'Z')
		return 1;
	return 2;
}

void Number_Scrambler(bool descrambler, char* tab)
{
	int key = KEY;
	char zero = '0', nine = '9';

	if (descrambler)
		key *= (-1);

	for (int i = 0; i < strlen(tab); i++)
	{
		if (tab[i] == ' ')
			continue;

		if (key >= 0)

			if (tab[i] + key <= nine)
				tab[i] += key;
			else
				tab[i] = tab[i] + key - 10;
		else
			if (tab[i] + key >= zero)
				tab[i] += key;
			else
				tab[i] = tab[i] + key + 10;
	}
}

void Scrambler(bool descrambler, char* tab)
{
	int key = KEY;
	int tmp;
	char a, z;

	if (descrambler)
		key *= (-1);

	for (int i = 0; i < strlen(tab); i++)
	{
		if (tab[i] == ' ')
			continue;
		tmp = Check_Char(tab[i]);
		if (tmp < 2)
		{
			if (tmp == 0)
				a = 'a', z = 'z';
			else
				a = 'A', z = 'Z';

			if (key >= 0)

				if (tab[i] + key <= z)
					tab[i] += key;
				else
					tab[i] = tab[i] + key - 26;
			else
				if (tab[i] + key >= a)
					tab[i] += key;
				else
					tab[i] = tab[i] + key + 26;
		}
	}
}


void Delete_Front_Space(char* text)
{
	for (int i = 0; i < strlen(text); i++)
		text[i] = text[i + 1];
}

Tables* Read_Restaurant_File()
{
	FILE* file_restaurant;
	errno_t err;
	Tables* pH = NULL;

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
			Tables* head = NULL;
			while (fscanf_s(file_restaurant, "%i %i", &table, &amount) != EOF)
			{
				pH = Table_Node(&pH, table, amount);
			}
			fclose(file_restaurant);
		}
	}
	return pH;
}

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
			{
				fprintf(file_restaurant, "%i %i \n", head->table_number, head->seats);
				head = head->next;
			}
			fclose(file_restaurant);
		}
	}
	return true;
}

bool Read_Reservations_File(Tables* Thead, Discounts* headDiscounts)
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

			int  amount, table, discount;
			float how_long;
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
				fscanf_s(file_reservations, "%f", &how_long);
				fscanf_s(file_reservations, "%s", pdata.name, MAX_NAME);
				fscanf_s(file_reservations, "%s", pdata.last_name, MAX_L_NAME);
				fscanf_s(file_reservations, "%s", pdata.phone_number, MAX_PHONE_NUM);
				fscanf_s(file_reservations, "%d", &discount);

				Scrambler(true, pdata.name);
				Scrambler(true, pdata.last_name);
				Number_Scrambler(true, pdata.phone_number);

				if (!(position->table = Find_Table(table, Thead)))
					continue;

				Personal_Data* pdata2 = malloc(sizeof(Personal_Data));
				struct tm* dtime2 = malloc(sizeof(struct tm));

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
						Alert_Red(" \n Blad oczytu pliku reservations.txt - nieprawidlowy format.\n");
						return false;
					}

					Delete_Front_Space(pdata.comment);

					if (pdata.comment[(strlen(pdata.comment) - 1)] != '\n')
						while ((tmpc = getchar()) != '\n');
					else
					{
						pdata.comment[(strlen(pdata.comment) - 1)] = '\0';
					}
					Scrambler(true, pdata.comment);
					*(pdata2) = pdata;
					*(dtime2) = time;

					reservation = Check_Tables(position, position->table, dtime2, amount, how_long);
					if (reservation)
					{
						reservation->customers = amount;
						reservation->pdata = pdata2;

						if (discount != 0)
							Connect_Discount_And_Record(&reservation, &headDiscounts, discount);
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

bool Save_Reservations_File(Tables* table)
{
	FILE* file_reservations;
	errno_t err;

	err = fopen_s(&file_reservations, FILENAME_RESERVATIONS, "w");
	if (file_reservations != NULL)
	{
		if (err)
		{
			Alert_Red("\n Nie udalo sie otworzyc pliku z rezerwacjami.");
			return false;
		}
		else
		{
			Tables* tmpR = table;
			Date* tmpD = NULL;
			Hour* tmpH = NULL;

			Tables* head = table;
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
							strftime(buff, 30, "%d.%m.%y %H:%M", tmpH->hour);

							Scrambler(false, tmpH->pdata->name);
							Scrambler(false, tmpH->pdata->last_name);
							Number_Scrambler(false, tmpH->pdata->phone_number);
							Scrambler(false, tmpH->pdata->comment);

							fprintf(file_reservations, "%d %d %s %.2f %s %s %s", tmpR->table_number, tmpH->customers, buff, tmpH->how_long, tmpH->pdata->name, tmpH->pdata->last_name, tmpH->pdata->phone_number);
							if (tmpH->discount)
								fprintf(file_reservations, " %d", tmpH->discount->percent);
							else
								fprintf(file_reservations, " %d", 0);

							fprintf(file_reservations, " %s\n", tmpH->pdata->comment);

							Scrambler(true, tmpH->pdata->name);
							Scrambler(true, tmpH->pdata->last_name);
							Number_Scrambler(true, tmpH->pdata->phone_number);
							Scrambler(true, tmpH->pdata->comment);

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

	return true;
}

Discounts* Read_Discounts_File()
{
	FILE* file_discaunts;
	errno_t err;
	Discounts* head = NULL;

	err = fopen_s(&file_discaunts, FILENAME_DISCAUNTS, "r");
	if (file_discaunts != NULL)
	{
		if (err)
		{
			Alert_Red(" Blad przy otwieraniu pliku ze zniznami.\n");
			return NULL;
		}
		else
		{
			int percent;
			char tmp[MAX_DISCOUNT_NAME];
			char tmpc;

			while (fscanf_s(file_discaunts, "%i", &percent) != EOF)
			{
				if (fgets(tmp, MAX_DISCOUNT_NAME, file_discaunts))
				{
					if (tmp == NULL)
					{
						Alert_Red(" \n Blad oczytu pliku discaunts.txt - nieprawidlowy format.\n");
						return NULL;
					}
					Delete_Front_Space(tmp);
					if (tmp[(strlen(tmp) - 1)] != '\n')
						while ((tmpc = getchar()) != '\n');
					else
					{
						tmp[(strlen(tmp) - 1)] = '\0';
					}
					if (Check_Discounts_Repetitions(head, percent))
						head = Discount_Node(&head, tmp, percent);
				}
				else
					break;
			}
			fclose(file_discaunts);
		}
	}
	return head;
}

bool Save_Discounts_File(Discounts* head)
{
	FILE* file_discaunts;
	errno_t err;
	Discounts* tmpHead = head;

	err = fopen_s(&file_discaunts, FILENAME_DISCAUNTS, "w");
	if (file_discaunts != NULL)
	{
		if (err)
		{
			Alert_Red("\n Blad przy otwieraniu pliku ze zniznami.");
			return false;
		}
		else
		{
			int i = 0;

			while (tmpHead)
			{
				fprintf(file_discaunts, "%i %s\n", tmpHead->percent, tmpHead->discount_name);
				tmpHead = tmpHead->next;
			}
			fclose(file_discaunts);
		}
	}
	return true;
}


void Save_Everything(Tables* head, Discounts* headDisc)
{
	if (Save_Restaurant_File(head) && Save_Reservations_File(head) && Save_Discounts_File(headDisc))
		Green_Text(" \n\n  Zapisano. ");
	else
		Alert_Red(" \n\n Nastapil blad przy zapisie.");

	puts("\n\n Wcisnij dowolny klawisz aby kontynuowac...");
	getch();
}


void Action_Center(Tables* Thead, Discounts** DiscHead)
{

	for (;;)
	{
		system("cls");
		Tables* head = Thead;
		printf("\n");
		printf("\n                                        Restauracyjny System Rezerwacji\n\n");
		printf("                                            [1] Dodaj Rezerwacje \n");
		printf("                                            [2] Usun Rezerwacje \n");
		printf("                                            [3] Wyswietl Rezerwacje \n");
		printf("                                            [4] Wprowadz zmiany w systemie znizkowym \n");
		printf("                                            [5] Edytuj stoliki \n");
		printf("                                            [6] Zapisz zmiany \n");
		printf("                                            [0] Zamknij program \n");
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
				Add_Reservation(head, *DiscHead);
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
			case '4':
				Discount_Manager(DiscHead, Thead);
				break;
			case '5':
				Tables_Manager(&Thead);
				break;
			case '6':
				Save_Everything(head, *DiscHead);
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

void Manager()
{
	Tables* Thead = Read_Restaurant_File();
	Tables* tmphead = Thead;

	if (Thead == NULL)
	{
		Alert_Red("\n\n\n                 Uwaga!\n                 Stoliki nie zostaly wczytane. \n                 Przed dodaniem rezerwacji, nalezy dodac informacje o stolikach do systemu. \n");
		Sleep(1000);
		printf("                                                  .");
		Sleep(1000);
		printf("  .");
		Sleep(1000);
		printf("  .");
		Sleep(1000);
	}
	Discounts* DiscountsHead = NULL;
	DiscountsHead = Read_Discounts_File();

	if (Read_Reservations_File(Thead, DiscountsHead) == false)
		return;


	Action_Center(Thead, &DiscountsHead);
	Thead = tmphead;
	Save_Restaurant_File(Thead);
	Save_Reservations_File(Thead);
	Save_Discounts_File(DiscountsHead);

	Delete_Reservations_List(Thead);
	Delete_Discounts_List(DiscountsHead);
}


#endif