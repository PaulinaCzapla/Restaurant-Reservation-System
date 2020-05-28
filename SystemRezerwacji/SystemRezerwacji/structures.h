/** @file */
#pragma once

#ifndef STRUCTURES_H
#define STRUCTURES_H
#include <stdbool.h>

#define MAX_HOUR 5
#define MAX_NAME 20
#define MAX_L_NAME 20
#define MAX_COMMENT 200
#define MAX_PHONE_NUM 10
#define MAX_DISCOUNT_NAME 50
#define KEY -7
#define FILENAME_RESTAURANT "restaurant.txt"
#define FILENAME_RESERVATIONS "reservations.txt"
#define FILENAME_DISCAUNTS "discounts.txt"


/** Wêze³ listy ze stolikami (najbardziej zewnêtrzna lista struktury programu). */
typedef struct Tables
{
	int table_number;					///< numer stolika
	int seats;							///< liczba miejsc przy stoliku
	struct Date* date;					///< wskaŸnik na pierwszy element listy z datami
	struct Tables* prev;
	struct Tables* next;

} Tables;

/** Wêze³ listy z datami (œrodkowa lista struktury programu). */
typedef struct Date
{
	struct tm* date;					///< wskaŸnik na strukturê przechowuj¹c¹ datê i godzinê
	struct Hour* hour;					///< wskaŸnik na pierwszy element listy z godzinami
	struct Date* next;

} Date;

/** Struktura przechowuj¹ca dane osobowe zamawiaj¹cego. */
typedef struct Personal_Data
{
	char name[MAX_NAME];
	char last_name[MAX_L_NAME];
	char phone_number[MAX_PHONE_NUM];
	char comment[MAX_COMMENT];

} Personal_Data;

/** Wêze³ listy z godzinami (najbardziej wewnêtrzna lista struktury programu). */
typedef struct Hour
{
	int customers;						///< liczba goœci
	float how_long;						///< informacja o tym, jak d³ugo goœcie bêd¹ przebywaæ w restauracji
	struct tm* hour;					///< wskaŸnik na strukturê przechowuj¹c¹ datê i godzinê
	struct Discounts* discount;			///< wskaŸnik na element listy ze zni¿kami
	struct Personal_Data* pdata;		///< wskaŸnik na strukturê przechowuj¹c¹ dane osobowe
	struct Hour* next;
} Hour;

/** Wêze³ listy ze zni¿kami. */
typedef struct Discounts
{
	char discount_name[MAX_DISCOUNT_NAME]; ///< nazwa zni¿ki
	int percent;							///< procentowa wartoœæ zni¿ki
	struct Discounts* next;

}Discounts;

/** Pomocnicza struktura, u¿ywana do przekazywania miêdzy funkcjami informacji o pozycji danego wêz³a w liœcie. */
typedef struct Position
{
	struct Tables* table;	
	struct Date* date;
	struct Hour* hour;
	bool definitive;		///< zmienna typu bool, która jest wa¿nym elementem systemu przeciw konflikom terminów

} Position;

#endif