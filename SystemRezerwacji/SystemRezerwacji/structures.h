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


/** W�ze� listy ze stolikami (najbardziej zewn�trzna lista struktury programu). */
typedef struct Tables
{
	int table_number;					///< numer stolika
	int seats;							///< liczba miejsc przy stoliku
	struct Date* date;					///< wska�nik na pierwszy element listy z datami
	struct Tables* prev;
	struct Tables* next;

} Tables;

/** W�ze� listy z datami (�rodkowa lista struktury programu). */
typedef struct Date
{
	struct tm* date;					///< wska�nik na struktur� przechowuj�c� dat� i godzin�
	struct Hour* hour;					///< wska�nik na pierwszy element listy z godzinami
	struct Date* next;

} Date;

/** Struktura przechowuj�ca dane osobowe zamawiaj�cego. */
typedef struct Personal_Data
{
	char name[MAX_NAME];
	char last_name[MAX_L_NAME];
	char phone_number[MAX_PHONE_NUM];
	char comment[MAX_COMMENT];

} Personal_Data;

/** W�ze� listy z godzinami (najbardziej wewn�trzna lista struktury programu). */
typedef struct Hour
{
	int customers;						///< liczba go�ci
	float how_long;						///< informacja o tym, jak d�ugo go�cie b�d� przebywa� w restauracji
	struct tm* hour;					///< wska�nik na struktur� przechowuj�c� dat� i godzin�
	struct Discounts* discount;			///< wska�nik na element listy ze zni�kami
	struct Personal_Data* pdata;		///< wska�nik na struktur� przechowuj�c� dane osobowe
	struct Hour* next;
} Hour;

/** W�ze� listy ze zni�kami. */
typedef struct Discounts
{
	char discount_name[MAX_DISCOUNT_NAME]; ///< nazwa zni�ki
	int percent;							///< procentowa warto�� zni�ki
	struct Discounts* next;

}Discounts;

/** Pomocnicza struktura, u�ywana do przekazywania mi�dzy funkcjami informacji o pozycji danego w�z�a w li�cie. */
typedef struct Position
{
	struct Tables* table;	
	struct Date* date;
	struct Hour* hour;
	bool definitive;		///< zmienna typu bool, kt�ra jest wa�nym elementem systemu przeciw konflikom termin�w

} Position;

#endif