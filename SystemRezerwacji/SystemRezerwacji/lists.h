/** @file */
#pragma once

#ifndef LISTS_H
#define LISTS_H


/** Funkcja dodaje nowy wêze³ listy z informacj¹ o stolikach (najbardziej zewnêtrzna lista).
@param prevpointer poprzedni element listy
@param nextpointer nastêpny element listy
@param number numer stolika
@param amount maksymalna liczba goœci przy stoliku
@return wskaŸnik na utworzony wêze³
*/
Tables* Add_Table_Node(Tables* prevpointer, Tables* nextpointer, int number, int amount);

/** Funkcja znajduje odpowiednie miejsce do dodania nowego elementu listy ze stolikami (sortowanie wed³ug maksymalnej liczby goœci przy stoliku, od najmniejszej do najwiêkszej).
@param pHead wskaŸnik na wskaŸnik g³owy listy
@param number numer stolika
@param amount maksymalna liczba goœci przy stoliku
@return wskaŸnik na g³owê listy
*/
Tables* Table_Node(Tables** pHead, int number, int amount);

/** Funkcja dodaje nowy wêze³ listy z datami (œrodkowa lista).
@param next nastêpny element listy
@param time struktura przechowuj¹ca informacje o czasie
@return wskaŸnik na utworzony wêze³
*/
Date* Add_Date_Node(Date* next,struct tm* time);


/** Funkcja znajduje odpowiednie miejsce do dodania nowego elementu listy z Datami (sortowanie od najnowszej do najstarszej daty).
@param ptrTabl wskaŸnik na wskaŸnik elementu listy ze stolikami
@param time struktura przechowuj¹ca informacje o czasie
@return wskaŸnik na utworzony wêze³
*/
Date* Date_Node(Tables** ptrTable,struct tm* time);

/** Funkcja dodaje nowy wêze³ do listy z godzinami (najbardziej wewnêtrzna lista).
@param next wskaŸnik na nastêpny element w liœcie
@param time struktura przechowuj¹ca informacje o czasie
@param how_long informacja o tym jak d³ugo goœcie bêd¹ przebywaæ w restauracji
@return wskaŸnik na utworzony wêze³
*/
Hour* Add_Hour_Node(Hour* next, struct tm* time, float how_long);


/** Funkcja znajduje odpowiednie miejsce do dodania nowego elementu listy z godzinami (sortowanie od najwczeœniejszej godziny do najstarszej).
@param ptrDate wskaŸnik na wskaŸnik na g³owê listy z datami
@param time struktura przechowuj¹ca informacje o czasie
@param how_long informacja o tym jak d³ugo goœcie bêd¹ przebywaæ w restauracji
@return wskaŸnik na utworzony wêze³
*/
Hour* Hour_Node(Date** ptrDate, struct tm* time, float how_long);


/** Funkcja usuwa listê z rezerwacjami. Najpierw usuwane s¹ struktury: Personal_Data oraz tm, póŸniej kolejno lista z godzinami, lista z datami dla ka¿dego stolika.
@param head wskaŸnik na g³owê listy ze stolikami
*/
void Delete_Reservations_List(Tables* head);


/** Funkcja tworzy nowy wêze³ listy ze zni¿kami.
@param discountname nazwa zni¿ki
@param percent procentowa wartoœæ zni¿ki
@return wskaŸnik na utworzony wêze³
*/
Discounts* Add_Discount_Node(char* discountname, int percent);

/** Funkcja znajduje koniec listy ze zni¿kami i umieszcza nowo stworzony wêze³ do listy.
@param pHead wskaŸnik na wskaŸnik na g³owê listy ze zni¿kami
@param discountname nazwa zni¿ki
@param percent procentowa wartoœæ zni¿ki
@return wskaŸnik na utworzony wêze³
*/
Discounts* Discount_Node(Discounts** pHead, char* discountname, int percent);

/** Funkcja usuwa listê ze zni¿kami od pocz¹tku.
@param pHead wskaŸnik na wskaŸnik na g³owê listy ze zni¿kami
*/
void Delete_Discounts_List(Discounts* head);

#endif
