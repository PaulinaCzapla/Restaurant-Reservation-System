#pragma once
#ifndef STRUCTURES_HEADER_H
#define STRUCTURES_HEADER_H
#include <stdbool.h>

#define MAX_HOUR 5
#define MAX_NAME 20
#define MAX_L_NAME 20
#define MAX_COMMENT 200
#define MAX_PHONE_NUM 10
#define DIFFERENCE 2


typedef struct Restaurant
{
	int table_number;
	int seats;
	struct Restaurant* prev;
	struct Restaurant* next;
	struct Date* date;

} Restaurant;

typedef struct Date
{
	struct tm* date;
	struct Hour* hour;
	struct Date* next;

} Date;

typedef struct Personal_Data
{
	char name[MAX_NAME];
	char last_name[MAX_L_NAME];
	char phone_number[MAX_PHONE_NUM];
	char comment[MAX_COMMENT];

} Personal_Data;

typedef struct Hour
{
	struct tm* hour;
	int customers;
	struct Hour* next;
	struct Personal_Data* pdata;

} Hour;

typedef struct Position
{
	struct Restaurant* table;
	struct Date* date;
	struct Hour* hour;
	bool definitive;
} Position;
//typedef struct Position
//{
//
//};


//typedef struct Reservations
//{
//	int table_number;
//	int seats;
//
//	struct Date* date;
//	struct Reservations* next;
//
//} Reservations;



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
//				node = AddDateNode(pHead, time);
//				if (prev)
//					prev->next = node;
//				is_added = true;
//				break;
//			}
//			if (time->tm_year > pHead->date->tm_year)
//			{
//				if (pHead->next == NULL)
//				{
//					node = AddDateNode(NULL, time);
//					pHead->next = node;
//					is_added = true;
//					break;
//				}
//				else
//				{
//					prev = pHead;
//					pHead = pHead->next;
//					continue;
//				}
//			}
//			if (time->tm_year == pHead->date->tm_year)
//			{
//
//				if (time->tm_mon > pHead->date->tm_mon&& pHead->next == NULL)
//				{
//					node = AddDateNode(NULL, time);
//					pHead->next = node;
//					is_added = true;
//					break;
//				}
//				else
//				{
//					prev = pHead;
//					pHead = pHead->next;
//					continue;
//				}
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
//					if (time->tm_mday > pHead->date->tm_mday&& pHead->next == NULL)
//					{
//						node = AddDateNode(NULL, time);
//						pHead->next = node;
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
#endif