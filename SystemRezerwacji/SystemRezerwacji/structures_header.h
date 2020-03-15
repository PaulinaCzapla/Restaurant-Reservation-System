#pragma once
#ifndef STRUCTURES_HEADER_H
#define STRUCTURES_HEADER_H
#include <stdbool.h>

#define MAX_HOUR 5
#define MAX_NAME 20
#define MAX_L_NAME 20
#define MAX_COMMENT 200
#define MAX_PHONE_NUM 10

typedef struct Restaurant 
{
	int table_number;
	int seats;
	struct Restaurant* prev;
	struct Restaurant* next;

} Restaurant;

typedef struct Date_And_Hour 
{
	int day;
	int month;
	int year;
	char hour[MAX_HOUR];

} Date_And_Hour;

typedef struct Personal_Data 
{
	char name[MAX_NAME];
	char last_name[MAX_L_NAME];
	char phone_number[MAX_PHONE_NUM];

} Personal_Data;

typedef struct Reservations 
{
	int table_number;
	int seats;
	char comment[MAX_COMMENT];
	struct Personal_Data *personal_data;
	struct Date_And_Hour *date;
	struct Reservations* next;

} Reservations;

#endif