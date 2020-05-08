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

#endif