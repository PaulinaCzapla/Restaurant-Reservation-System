#ifndef LIST_C
#define LIST_C
#include "functions_header.h"
#include "functions.c"

//
//Restaurant* First_Node(Restaurant* prevpointer, Restaurant* nextpointer, int number, int amount)
//{
//	Restaurant* node;
//	node = (Restaurant*)malloc(sizeof(Restaurant));
//
//	if (node != NULL)
//	{
//		node->prev = prevpointer;
//		node->next = nextpointer;
//		node->table_number = number;
//		node->seats = amount;
//		node->date = NULL;
//	}
//	return node;
//}
//Restaurant* Node(Restaurant* pHead, int number, int amount)
//{
//	if (pHead == NULL)
//	{
//		Restaurant* first = First_Node(NULL, NULL, number, amount);
//		return first;
//	}
//	else
//	{
//		Restaurant* node = pHead;
//		Restaurant* prev = NULL;
//		Restaurant* tmpHead = pHead;
//
//		while (pHead)
//		{
//			if (amount <= pHead->seats)
//			{
//				node = First_Node(prev, pHead, number, amount);
//				pHead->prev = node;
//
//				if (prev)
//					prev->next = node;
//				else
//					tmpHead = node;
//
//				/*if (pHead == tmpHead)
//					tmpHead = node;*/
//
//				break;
//			}
//
//			prev = pHead;
//			pHead = pHead->next;
//
//			if (pHead == NULL)
//			{
//				node = First_Node(prev, NULL, number, amount);
//				prev->next = node;
//				break;
//			}
//		}
//
//		return tmpHead;
//	}
//}
//
//
//Date* AddDateNode(Date* next, struct tm* time)
//{
//	Date* node;
//	node = (Date*)malloc(sizeof(Date));
//
//	if (node != NULL)
//	{
//		node->next = next;
//		node->hour = NULL;
//		node->date = time;
//	}
//	return node;
//}
//Date* DateNode(Restaurant* ptrTable, struct tm* time)
//{
//	Date* pHead = ptrTable->date;
//	if (pHead == NULL)
//	{
//		Date* first = AddDateNode(NULL, time);
//		ptrTable->date = first;
//		return first;
//	}
//	else
//	{
//		Date* node = pHead;
//		Date* prev = NULL;
//		Date* tmpHead = pHead;
//		bool is_added = false;
//		while (pHead)
//		{
//			if (time->tm_year < pHead->date->tm_year)
//			{
//				//dodaj
//				node = AddDateNode(pHead, time);
//				if (prev)
//					prev->next = node;
//				is_added = true;
//				break;
//			}
//			if (time->tm_year == pHead->date->tm_year)
//			{
//
//				if (time->tm_mon < pHead->date->tm_mon)
//				{
//					node = AddDateNode(pHead, time);
//					if (prev)
//						prev->next = node;
//					is_added = true;
//					break;
//				}
//				if (time->tm_mon == pHead->date->tm_mon)
//				{
//					if (time->tm_mday <= pHead->date->tm_mday)
//					{
//						node = AddDateNode(pHead, time);
//						if (prev)
//							prev->next = node;
//						is_added = true;
//						break;
//					}
//
//				}
//				prev = pHead;
//				pHead = pHead->next;
//			}
//		}
//
//		if (is_added == false) //dodawanie na koniec
//		{
//			node = AddDateNode(NULL, time);
//			prev->next = node;
//		}
//		ptrTable->date = node;
//		return node;
//	}
//}
//
//
//Hour* AddHourNode(Hour* next, struct tm* time)
//{
//	Hour* node;
//	node = (Hour*)malloc(sizeof(Hour));
//
//	if (node != NULL)
//	{
//		node->next = next;
//		node->hour = time;
//		node->pdata = NULL;
//		node->customers = 0;
//	}
//	return node;
//}
//Hour* HourNode(Date* ptrDate, struct tm* time)
//{
//	Hour* pHead = ptrDate->hour;
//	if (pHead == NULL)
//	{
//		Hour* first = AddHourNode(NULL, time);
//		ptrDate->hour = first;
//		return first;
//	}
//	else
//	{
//		Hour* node = pHead;
//		Hour* prev = NULL;
//		Hour* tmpHead = pHead;
//		bool is_added = false;
//
//		while (pHead)
//		{
//
//			int diff = time->tm_hour - pHead->hour->tm_hour;
//			if (diff < 0)
//				diff = -diff;
//
//			int diffmin = time->tm_min - pHead->hour->tm_min;
//			if (diffmin < 0)
//				diffmin = -diffmin;
//
//
//			if (time->tm_hour < pHead->hour->tm_hour)
//			{
//				//dodaj
//				node = AddHourNode(pHead, time);
//				prev->next = node;
//				is_added = true;
//				break;
//			}
//			if (time->tm_hour == pHead->hour->tm_hour)
//			{
//
//				if (time->tm_min <= pHead->hour->tm_min)
//				{
//					node = AddHourNode(pHead, time);
//					prev->next = node;
//					is_added = true;
//					break;
//				}
//
//				prev = pHead;
//				pHead = pHead->next;
//			}
//		}
//
//		if (is_added == false) //dodawanie na koniec
//		{
//			node = AddHourNode(pHead, time);
//			prev->next = node;
//		}
//		ptrDate->hour = node;
//		return node;
//	}
//}
//
//
//void DeleteList(Restaurant* head)
//{
//	Date* tmpD = NULL;
//	Restaurant* tmpR = NULL;
//	Hour* tmpH = NULL;
//	Date* D = NULL;
//	Restaurant* R = NULL;
//	Hour* H = NULL;
//
//	if (head)
//		tmpR = head;
//
//	while (tmpR)
//	{
//		if (tmpR->date)
//		{
//			tmpD = tmpR->date;
//			while (tmpD)
//			{
//				if (tmpD->hour)
//					tmpH = tmpD->hour;
//
//				while (tmpH)
//				{
//					free(tmpH->pdata);
//					free(tmpH->hour);
//					H = tmpH;
//					tmpH = tmpH->next;
//					free(H);
//				}
//				//free(tmpD->date);
//				D = tmpD;
//				tmpD = tmpD->next;
//				free(D);
//			}
//		}
//		R = tmpR;
//		tmpR = tmpR->next;
//		free(R);
//	}
//}

#endif