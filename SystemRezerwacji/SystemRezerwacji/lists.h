/** @file */
#pragma once

#ifndef LISTS_H
#define LISTS_H


/** Funkcja dodaje nowy w�ze� listy z informacj� o stolikach (najbardziej zewn�trzna lista).
@param prevpointer poprzedni element listy
@param nextpointer nast�pny element listy
@param number numer stolika
@param amount maksymalna liczba go�ci przy stoliku
@return wska�nik na utworzony w�ze�
*/
Tables* Add_Table_Node(Tables* prevpointer, Tables* nextpointer, int number, int amount);

/** Funkcja znajduje odpowiednie miejsce do dodania nowego elementu listy ze stolikami (sortowanie wed�ug maksymalnej liczby go�ci przy stoliku, od najmniejszej do najwi�kszej).
@param pHead wska�nik na wska�nik g�owy listy
@param number numer stolika
@param amount maksymalna liczba go�ci przy stoliku
@return wska�nik na g�ow� listy
*/
Tables* Table_Node(Tables** pHead, int number, int amount);

/** Funkcja dodaje nowy w�ze� listy z datami (�rodkowa lista).
@param next nast�pny element listy
@param time struktura przechowuj�ca informacje o czasie
@return wska�nik na utworzony w�ze�
*/
Date* Add_Date_Node(Date* next,struct tm* time);


/** Funkcja znajduje odpowiednie miejsce do dodania nowego elementu listy z Datami (sortowanie od najnowszej do najstarszej daty).
@param ptrTabl wska�nik na wska�nik elementu listy ze stolikami
@param time struktura przechowuj�ca informacje o czasie
@return wska�nik na utworzony w�ze�
*/
Date* Date_Node(Tables** ptrTable,struct tm* time);

/** Funkcja dodaje nowy w�ze� do listy z godzinami (najbardziej wewn�trzna lista).
@param next wska�nik na nast�pny element w li�cie
@param time struktura przechowuj�ca informacje o czasie
@param how_long informacja o tym jak d�ugo go�cie b�d� przebywa� w restauracji
@return wska�nik na utworzony w�ze�
*/
Hour* Add_Hour_Node(Hour* next, struct tm* time, float how_long);


/** Funkcja znajduje odpowiednie miejsce do dodania nowego elementu listy z godzinami (sortowanie od najwcze�niejszej godziny do najstarszej).
@param ptrDate wska�nik na wska�nik na g�ow� listy z datami
@param time struktura przechowuj�ca informacje o czasie
@param how_long informacja o tym jak d�ugo go�cie b�d� przebywa� w restauracji
@return wska�nik na utworzony w�ze�
*/
Hour* Hour_Node(Date** ptrDate, struct tm* time, float how_long);


/** Funkcja usuwa list� z rezerwacjami. Najpierw usuwane s� struktury: Personal_Data oraz tm, p�niej kolejno lista z godzinami, lista z datami dla ka�dego stolika.
@param head wska�nik na g�ow� listy ze stolikami
*/
void Delete_Reservations_List(Tables* head);


/** Funkcja tworzy nowy w�ze� listy ze zni�kami.
@param discountname nazwa zni�ki
@param percent procentowa warto�� zni�ki
@return wska�nik na utworzony w�ze�
*/
Discounts* Add_Discount_Node(char* discountname, int percent);

/** Funkcja znajduje koniec listy ze zni�kami i umieszcza nowo stworzony w�ze� do listy.
@param pHead wska�nik na wska�nik na g�ow� listy ze zni�kami
@param discountname nazwa zni�ki
@param percent procentowa warto�� zni�ki
@return wska�nik na utworzony w�ze�
*/
Discounts* Discount_Node(Discounts** pHead, char* discountname, int percent);

/** Funkcja usuwa list� ze zni�kami od pocz�tku.
@param pHead wska�nik na wska�nik na g�ow� listy ze zni�kami
*/
void Delete_Discounts_List(Discounts* head);

#endif
