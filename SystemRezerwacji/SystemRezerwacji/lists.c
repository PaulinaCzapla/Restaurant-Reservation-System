/** @file */
#ifndef LISTS_C
#define LISTS_C

#include "functions.h"


Tables* Add_Table_Node(Tables* prevpointer, Tables* nextpointer, int number, int amount)
{
	Tables* node;
	node = (Tables*)malloc(sizeof(Tables));

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
Tables* Table_Node(Tables** pHead, int number, int amount)
{
	if ((*pHead) == NULL)
	{
		Tables* first = Add_Table_Node(NULL, NULL, number, amount);
		return first;
	}
	else
	{
		Tables* node = (*pHead);
		Tables* prev = NULL;
		Tables* tmpHead = (*pHead);

		while (*pHead)
		{
			if (amount <= (*pHead)->seats)
			{
				node = Add_Table_Node(prev, (*pHead), number, amount);
				(*pHead)->prev = node;

				if (prev)
					prev->next = node;
				else
					tmpHead = node;
				break;
			}

			prev = (*pHead);
			(*pHead) = (*pHead)->next;

			if ((*pHead) == NULL)
			{
				node = Add_Table_Node(prev, NULL, number, amount);
				prev->next = node;
				break;
			}
		}

		return tmpHead;
	}
}


Date* Add_Date_Node(Date* next, struct tm* time)
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
Date* Date_Node(Tables** ptrTable, struct tm* time)
{
	Date* pHead = (*ptrTable)->date;

	if (pHead == NULL)
	{
		Date* first = Add_Date_Node(NULL, time);
		(*ptrTable)->date = first;
		return first;
	}
	else
	{
		Date* node = pHead;
		Date* prev = NULL;
		Date* tmpHead = pHead;

		if (pHead == NULL) //dodawanie na koniec
		{

			node = Add_Date_Node(NULL, time);
			(*ptrTable)->date = node;
		}

		while (pHead)
		{
			if (time->tm_year > pHead->date->tm_year&& pHead->next == NULL)
			{
				node = Add_Date_Node(NULL, time);
				if (pHead)
					pHead->next = node;

				break;
			}
			if (time->tm_year < pHead->date->tm_year)
			{
				node = Add_Date_Node(pHead, time);

				if (prev)
					prev->next = node;
				else
					(*ptrTable)->date = node;

				break;
			}
			if (time->tm_year == pHead->date->tm_year)
			{

				if (time->tm_mon > pHead->date->tm_mon&& pHead->next == NULL)
				{
					node = Add_Date_Node(NULL, time);
					if (pHead)
						pHead->next = node;
					break;
				}
				if (time->tm_mon < pHead->date->tm_mon)
				{
					node = Add_Date_Node(pHead, time);
					if (prev)
						prev->next = node;
					else
						(*ptrTable)->date = node;
					break;
				}
				if (time->tm_mon == pHead->date->tm_mon)
				{
					if (time->tm_mday > pHead->date->tm_mday&& pHead->next == NULL)
					{
						node = Add_Date_Node(NULL, time);
						if (pHead)
							pHead->next = node;
						break;
					}
					if (time->tm_mday < pHead->date->tm_mday)
					{

						node = Add_Date_Node(pHead, time);
						if (prev)
							prev->next = node;
						else
							(*ptrTable)->date = node;

						break;
					}
				}
			}
			prev = pHead;
			pHead = pHead->next;
		}
		return node;
	}
}


Hour* Add_Hour_Node(Hour* next, struct tm* time, float how_long)
{
	Hour* node;
	node = (Hour*)malloc(sizeof(Hour));

	if (node != NULL)
	{
		node->next = next;
		node->hour = time;
		node->pdata = NULL;
		node->customers = 0;
		node->how_long = how_long;
		node->discount = NULL;
	}
	return node;
}
Hour* Hour_Node(Date** ptrDate, struct tm* time, float how_long)
{
	Hour* pHead = (*ptrDate)->hour;
	if (pHead == NULL)
	{
		Hour* first = Add_Hour_Node(NULL, time, how_long);
		(*ptrDate)->hour = first;
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

			if (time->tm_hour < pHead->hour->tm_hour)
			{
				node = Add_Hour_Node(pHead, time, how_long);
				if (prev)
					prev->next = node;
				else
					(*ptrDate)->hour = node;

				break;
			}
			if (time->tm_hour == pHead->hour->tm_hour)
			{

				if (time->tm_min <= pHead->hour->tm_min)
				{
					node = Add_Hour_Node(pHead, time, how_long);
					if (prev)
						prev->next = node;
					else
						(*ptrDate)->hour = node;
					break;
				}

			}
			if ((time->tm_hour > pHead->hour->tm_hour) && pHead->next == NULL)
			{
				node = Add_Hour_Node(NULL, time, how_long);
				if (prev)
					prev->next = node;
				else
					pHead->next = node;
				break;
			}
			prev = pHead;
			pHead = pHead->next;
		}
		return node;
	}
}

Discounts* Add_Discount_Node(char* discountname, int percent)
{
	Discounts* node;
	node = malloc(sizeof(Discounts));

	if (node != NULL)
	{
		strcpy_s(node->discount_name, MAX_DISCOUNT_NAME, discountname);
		node->percent = percent;
		node->next = NULL;
	}
	return node;
}
Discounts* Discount_Node(Discounts** pHead, char* discountname, int percent)
{

	if ((*pHead) == NULL)
	{
		Discounts* first = Add_Discount_Node(discountname, percent);
		pHead = &first;
		return first;
	}
	else
	{
		Discounts* node = (*pHead);
		Discounts* tmpHead = (*pHead);

		while (*pHead)
		{

			if ((*pHead)->next == NULL)
			{
				node = Add_Discount_Node(discountname, percent);
				(*pHead)->next = node;
				break;
			}
			(*pHead) = (*pHead)->next;
		}
		return tmpHead;
	}
}

void Delete_Reservations_List(Tables* head)
{
	Date* tmpD = NULL;
	Tables* tmpR = NULL;
	Hour* tmpH = NULL;
	Date* D = NULL;
	Tables* R = NULL;
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
void Delete_Discounts_List(Discounts* head)
{
	Discounts* tmp = NULL;
	while (head)
	{
		tmp = head->next;
		free(head);
		head = tmp;
	}
}


#endif