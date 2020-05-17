/** @file */

#pragma once

/*!
\mainpage System Rezerwacji w Restauracji
\author Paulina Czapla [SSI] [INF] [SEM2] [GR4]
\date 
*/

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifndef FUNCTIONS_HEADER_H
#define FUNCTIONS_HEADER_H

#include "structures_header.h"


#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>
#include <assert.h>
#include <string.h>

void WYSWIETL(Restaurant* pHe);

/** Funkcja do wy�wietlania czerwonych napis�w.
@param text tekst do wy�wietlenia
*/
void Alert_Red(char text[]);
char* tr(char* str);
/** Funkcja dodaje nowy w�ze� listy z informacj� o stolikach (najbardziej zewn�trzna lista).
@param prevpointer poprzedni element listy
@param nextpointer nast�pny element listy
@param number numer stolika
@param amount maksymalna liczba go�ci przy stoliku
@return wska�nik na utworzony w�ze�
*/
Restaurant* First_Node(Restaurant* prevpointer, Restaurant* nextpointer, int number, int amount);

/** Funkcja znajduje odpowiednie miejsce do dodania nowego elementu (sortowanie wed�ug maksymalnej liczby go�ci przy stoliku, od najmniejszej do najwi�kszej).
@param pHead g�owa listy
@param number numer stolika
@param amount maksymalna liczba go�ci przy stoliku
@return wska�nik na g�ow� listy
*/
Restaurant* Node(Restaurant* pHead, int number, int amount);

/** Funkcja dodaje nowy w�ze� listy z datami (�rodkowa lista).
@param next nast�pny element listy
@param time struktura przechowuj�ca informacje o czasie
@return wska�nik na utworzony w�ze�
*/
Date* AddDateNode(Date* next,struct tm* time);
/** Funkcja znajduje odpowiednie miejsce do dodania nowego elementu (sortowanie wed�ug daty).
@param ptrTable wska�nik do odpowiedniego elementu listy ze stolikami
@param time struktura przechowuj�ca informacje o czasie
@return wska�nik na g�ow� listy
*/
Date* DateNode(Restaurant* ptrTable,struct tm* time);
Hour* AddHourNode(Hour* next,struct tm* time);
Hour* HourNode(Date* ptrDate,struct tm* time);
void DeleteList(Restaurant* head);
Restaurant* Find_Table(int num, Restaurant* head);
Hour* Add_New_Reservation(Position* position, Restaurant* ptrTable, Date* ptrDate, struct tm* time);
int Difference(struct tm* h1, Hour* h2);
Hour* Check_Hours(Position* position, Restaurant* headTab, Date* headDat, struct tm* time);
Hour* Check_Dates(Position* position, Restaurant* headTab, struct tm* time);
Hour* Check_Tables(Position* position, Restaurant* head, struct tm* time, int amount);
bool Is_Up_To_Date(struct tm* time_to_check);
void Add_Reservation(Restaurant* headT);
void Show_Reservations(Restaurant* table);
void Convert_Date(char* date, struct tm* dtime, char* tmp);
void Convert_Hour(char* hour, struct tm* dtime, char* tmp);
Restaurant* Read_Restaurant_File();
void Scrambler(char* text);
void Descrambler(char* text);
bool Read_Reservations_File(Restaurant* Thead);
void Save_File(Restaurant* table);
bool Find_And_Delete_Phone_Number(char* num, Restaurant* head);
void Delete_Record(Restaurant* head);
void Action_Center(Restaurant* Thead);
void Manager();


#endif