/** @file */

#pragma once

/*!
\mainpage System Rezerwacji w Restauracji
\author Paulina Czapla [SSI] [INF] [SEM2] [GR4]
\date 
*/

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "structures.h"
#include "lists.h"



#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>
#include <assert.h>
#include <string.h>





#endif

/** Funkcja do zmieniania napis�w na czerwone.
@param text tablica z napisem
*/
void Alert_Red(char* text);

/** Funkcja do zamieniania napis�w na zielone.
@param text tablica z napisem
*/
void Green_Text(char* text);

/** Funkcja, kt�ra pyta si� u�ytkownika czy na pewno chce usun�� dan� rezerwacj�.
@param hour wska�nik na element listy z godzinami
@return true je�li u�ytkownik si� zgodzi, false je�li nie
*/
bool Ask_Before_Delete(Hour* hour);

/** Funkcja przeszukuje list� w celu znalezienia rezerwacji z okre�lonym numerem telefonu.
@param num tablica z numerem telefonu
@param head wska�nik na g�ow� listy ze stolikami
@param ask zmienna pomocnicza, kt�rej warto�� zale�y od miejsca w programie, gdzie zosta�a wywo�ana funkcja. Je�li ask=false, nie zostanie wywo�ana funkcja Ask_Before_Delete, 
je�li ask=true,funkcja zostanie wywo�ana i od jej warto�ci b�dzie zale�e� wykonanie dalejszej cz�ci funkcji.
@return 0 je�li nie znaleziono numeru, 1 je�li rezerwacja zosta�a usuni�ta, 2 je�li u�ytkownik zrezygnowa� z usuwania rezerwacji
*/
short int Find_And_Delete_by_Phone_Number(char* num, Tables* head, bool ask);

/** Funkcja, kt�ra pobiera od u�ytkownika numer telefonu z rezerwacji do usuni�cia i wy�wietla informacje o zako�czonym sukcesem lub nie usuni�ciu elementu
@param head wska�nik na g�ow� listy ze stolikami
*/
void Delete_Record(Tables* head);

/** Funkcja, kt�ra posiada na podstawie otrzymanych parametr�w, wywo�uje odpowiednie funkcje dodawania rezerwacji do listy.
Je�li ptrDate != NULL to oznacza, �e w danym stoliku ju� znajduj� si� jakie� rezerwacje w podanym dniu. 
Z tego powodu, zostaje dodany nowy element listy Hour do konkretnego elementu listy Date.
Je�li ptrDate = NULL, oznacza to, �e w danym dniu nie ma jeszcze rezerwacji na dan� godzin�, wi�c do struktury jest dodawany najpierw element listy Date, a dopiero p�niej
Hour.
@param position wska�nik na struktur� Position z lokalizacj� w strukturze aktualnie dodawanego elementu
@param ptrTable wska�nik na element listy ze stolikami
@param ptrDate wska�nik na element listy z datami
@param time struktura przechowuj�ca informacje o dacie i godzinie
@param how_long informacja jak d�ugo go�cie zamierzaj� zosta� w restauracji
@return wska�nik na nowo powsta�y element listy Hours
*/
Hour* Add_New_Reservation(Position* position, Tables* ptrTable, Date* ptrDate,struct tm* time, float how_long);

/** Funkcja obliczaj�ca r�nic� mi�dzy godzinami rezerwacji. W przypadkku, gdy r�nica minut 
jest mniejsza od 0, zmniejszona zostaje warto�� godziny, a nowa warto�� minut to 60 + (poprzednia warto�� minut). Po wyliczeniu r�nicy godzin, zamieniana
jest ona na liczb� dziesi�tn�.
@param h1 wcze�niejsza godzina
@param h2 p�niejsza godzina
@return r�nica
*/
float Hours_Difference(struct tm* h1, struct  tm* h2);


/** Funkcja obliczaj�ca r�nic� mi�dzy godzinami rezerwacji w przypadku, gdy s� one w innych dniach. Funkcja oblicza ilo�� czasu do p�nocy dla ka�dej godziny i sumuje te warto�ci. Nast�pnie sprawdza, czy rezerwacje nie zachodz� na siebie.
@param prev poprzednia data
@param next nast�pna data
@param currtime czas sprawdzanej rezerwacji
@param how_long ilo�� czasu, kt�r� klienci nowej rezerwacji chc� sp�dzi� w restauracji
@return false je�li jest konflikt i true je�li go nie ma
*/
bool Check_Conflict(Date* prev, Date* next, struct tm* currtime, float how_long);

/** Funkcja b�d�ca elementem algorytmu zapobiegaj�cego konfliktom termin�w. Przeszukuje ona list� godzin dla danej 
daty i sprawdza, czy r�nica pomi�dzy godzin� w li�cie, a godzin� nowej rezerwacji jest dostateczna. Robi to 
wywo�uj�c funkcje Hours_Difference z r�nymi parametrami i por�wnuje otrzymane warto�ci z warto�ciami how_long 
dla poszczeg�lnych rezerwacji. Dodanie nowej rezerwacji zale�y r�wnie� od funkcji Check_Conflict. Je�li o danej godzinie nie ma wolnego terminu, zmieniona zostaje warto�� position->definitive na false, co 
umo�liwia poszukiwanie wolnych miejsc w innych terminach. Je�li nie ma konfliktu termin�w, wywo�ana zostaje funkcja
Add_New_Reservation.
@param position wska�nik na struktur� Position z lokalizacj� w strukturze aktualnie dodawanego elementu
@param ptrTab wska�nik na element listy ze stolikami
@param ptrDate wska�nik na element listy z datami
@param time struktura przechowuj�ca informacje o dacie i godzinie
@param how_long informacja jak d�ugo go�cie zamierzaj� zosta� w restauracji
@param prevDate poprzednia data (dzie� poprzedni do aktualnie sprawdzanego)
@param nextDate nast�pna data (dzie� nast�pny do aktualnie sprawdzanego)
@return wska�nik na nowo powsta�y element listy Hours
*/
Hour* Check_Hours(Position* position, Tables* ptrTable, Date* ptrDate, struct  tm* time, float how_long, Date* prevDate, Date* nextDate);

/** Funkcja jest elementem algorytmu zapobiegaj�cego konfliktom termin�w. Sprawdza czy w li�cie wyst�puje rezerwacja
w tym samym dniu, albo w dniu poprzednim lub nast�pnym. Je�li tak, wywo�ana jest funkcja Check_Hours. W przeciwnym przypadku wywo�ana jest funkcja Add_New_Reservation.
@param position wska�nik na struktur� Position z lokalizacj� w strukturze aktualnie dodawanego elementu
@param ptrTab wska�nik na element listy ze stolikami
@param time struktura przechowuj�ca informacje o dacie i godzinie
@param how_long informacja jak d�ugo go�cie zamierzaj� zosta� w restauracji
@return wska�nik na nowo powsta�y element listy Hours
*/
Hour* Check_Dates(Position* position, Tables* ptrTable, struct  tm* time, float how_long);

/** Funkcja jest elementem algorytmu zapobiegaj�cego konfliktom termin�w. Sprawdza czy w strukturze istnieje stolik na 
odpowiedni� ilo�� os�b. Je�li taki istnieje i nie ma �adnych rezerwacji, wywo�ana jest funkcja Add_New_Reservation.
 W przeciwnym przypadku wywo�ana zostaje funkcja CHeck_Dates.
@param position wska�nik na struktur� Position z lokalizacj� w strukturze aktualnie dodawanego elementu
@param head wska�nik na g�ow� listy ze stolikami
@param time struktura przechowuj�ca informacje o dacie i godzinie
@param amount ilo�� go�ci
@param how_long informacja jak d�ugo go�cie zamierzaj� zosta� w restauracji
@return wska�nik na nowo powsta�y element listy Hours
*/
Hour* Check_Tables(Position* position, Tables* head, struct  tm* time, int amount, float how_long);

/** Funkcja sprawdzaj�ca czy podana data jest aktualna. Pobiera aktualny czas za pomoc� funkcji time() i rzutuje go na struct tm za pomoc� localtime_s.
@param time_to_check czas, kt�ry ma zosta� sprawdzony
@return false je�li data jest przestarza�a i true je�li aktualna
*/
bool Is_Up_To_Date(struct tm* time_to_check);

/** Prosta funkcja sprawdzaj�ca czy w li�cie istnieje ju� dana zni�ka.
@param head wska�nik na g�ow� listy ze zni�kiami
@param liczba procentowa zni�ki do dodania
@return false je�li isnieje ju� taka zni�ka
*/
bool Check_Discounts_Repetitions(Discounts* head, int percent);

/** Funkcja do wy�wietlania ni�ek na ekranie.
@param head wska�nik na g�ow� listy ze zni�kiami
*/
void Show_Discounts(Discounts* head);

/** Funkcja prosi u�ytkownika o podanie danych nowej zni�ki. Sprawdza czy dane s� prawid�owe i czy mo�na doda� zni�k�.
@param head wska�nik na g�ow� listy ze zni�kiami
@return wska�nik na g�ow� listy
*/
Discounts* Add_New_Discount_Type(Discounts* head);

/** Funkcja znajduje dan� zni�k� na podstawie jej warto�ci procentowej.
@param head wska�nik na g�ow� listy ze zni�kiami
@param percent warto�� procentowa zni�ki do znalezienia
@return wska�nik na znalezion� zni�k�, je�li nie ma takiej zni�ki, zwraca NULL
*/
Discounts* Find_Discount(Discounts* head, int percent);

/** Funkcja usuwa zni�k� z listy
@param head wska�nik na wska�nik na g�ow� listy ze zni�kami
@param found wska�nik na zni�ke do usuni�cia
*/
void Delete_Discount_Safely(Discounts** head, Discounts* found);

/** Funkcja znajduje wszystkie rezerwacje z okre�lon� zni�k� i zamienia je na NULL.
@param found wska�nik na zni�ke do znalezienia
@param reservationsHead wska�nik na g�ow� listy z rezerwacjami/stolikami
*/
void Delete_Discount_From_Reservation(Discounts* found, Tables* reservationsHead);

/** Funkcja prosi u�ytkownika o podanie danych zni�ki do usuni�cia i odwo�uje si� do innych, koniecznych funkcji.
@param head wska�nik na wska�nik na g�ow� listy ze zni�kami
@param reservationsHead wska�nik na g�ow� listy z rezerwacjami/stolikami
*/
void Delete_Discount(Discounts** head, Tables* reservationsHead);

/** Menu systemmu zarz�dzania zni�kami.
@param head wska�nik na wska�nik na g�ow� listy ze zni�kami
@param reservationsHead wska�nik na g�ow� listy z rezerwacjami/stolikami
*/
void Discount_Manager(Discounts** head, Tables* reservationsHead);

/** Funkcja dodaje do konkretnej rezerwacji zni�k�.
@param reservation wska�nik na wska�nik na element listy Hours
@param headDiscounts wska�nik na wska�nik na g�ow� listy ze zni�kami
@param percent warto�� procentowa zni�ki do dodania
@return true je�li uda�o si� doda� zni�k�
*/
bool Connect_Discount_And_Record(Hour** reservation, Discounts** headDiscounts, int percent);

/** Funkcja pyta si� czy u�ytkownik chce doda� zni�k� do rezerwacji i prosi o podanie danych.
@param reservation wska�nik na element listy Hours z rezerwacj�
@param head wska�nik na g�ow� listy ze zni�kami
*/
void Add_Discount_To_Reservation(Hour* reservation, Discounts* head);

/** G��wna funkcja dodawania rezerwacji do systemu przez u�ytkownika. Je�li w systemie nie ma listy ze stolikami,
 funkcja wy�wietla komunikat i si� ko�czy. w przeciwnym przypadku, prosi u�ytkownika o podanie danych. Je�li u�ytkownik
  poda nieprawid�owe dane (wychodz�ce poza zakres, litery zamiast liczb itd.) funkcja wy�wietla odpowiedni komunikat. 
  Funkcja alokuje r�wnie� pami�� dla struct tm, struct Personal_Data i struct position. Pierwsze dwie struktury wpisywane s� do 
  g��wnej struktury danych, ostatnia jest zwalniana pod koniec dzia�ania funkcji.
@param headT wska�nik na g�ow� listy ze stolikami
@param headDiscounts wska�nik na g�ow� listy ze zni�kami
*/
void Add_Reservation(Tables* headT, Discounts* headDiscounts);

/** Funkcja do znajdowania stolika w strukturze po jego numerze.
@param num numer stolika
@param head wska�nik na g�ow� listy ze stolikami
@return wska�nik na znaleziony stolik
*/
Tables* Find_Table(int num, Tables* head);

/** Funkcja do wy�wietlania stolik�w na ekranie.
@param pH wska�nik na g�ow� listy ze stolikami
*/
void Show_Tables(Tables* pH);

/** Funkcja do dodawania nowego stolika. Prosi u�ytkownika o podanie danych i sprawdza ich poprawno��.
@param head wska�nik na wska�nik nag�ow� listy ze stolikami
*/
void Add_New_Table(Tables** head);

/** Funkcja do usuwania stolika. Wywo�uje funkcjj� Find_And_Delete_by_Phone_Number w celu usuni�cia wszystkich danych, dla danego stolika. Nast�pnie usuwa stolik.
@param head wska�nik na wska�nik nag�ow� listy ze stolikami
@param found wska�nik na stolik do usuni�cia
*/
void Delete_Table(Tables** head, Tables* found);

/** Funkcja prosi u�ytkownika o podanie danych stolika do usuni�cia i upewnia si�, czy u�ytkownik na pewno chce go usun��.
@param head wska�nik na wska�nik nag�ow� listy ze stolikami
*/
void Select_Table_To_Delete(Tables** head);

/** Menu systemu zarz�dzania stolikami.
@param head wska�nik na wska�nik nag�ow� listy ze stolikami
*/
void Tables_Manager(Tables** head);

/** Funkcja do wy�wietlania na ekran jednej rezerwacji.
@param table wska�nik na element listy ze stolikami
@param hour wska�nik na element listy hour
*/
void Show_One_Reservation(Tables* table, Hour* hour);

/** Funkcja do znajdowania rezerwacji na dany numer telefonu w strukturze.
@param num numer telefonu do znalezienia
@param head wska�nik na g�ow� listy ze stolikami
@return true je�li znaleziono, false je�li nie
*/
bool Find_Reservation_by_Phone_Number(char* num, Tables* head);

/** Funkcja do wywo�ywania Show_One_Reservation dla wszystkich rezerwacji.
@param table wska�nik na g�ow� listy ze stolikami
*/
void Show_All_Reservations(Tables* table);

/** Funkcja znajduje rezerwacje danego dnia i je wy�wietla. Je�li dat==NULL to pobierana jest aktualna data i wy�wietlane s�
 aktualne rezerwacje
@param head wska�nik na g�ow� listy ze stolikami
@param dat wska�nik na struct tm z dat� do znalezienia
@return true je�li znaleziono rezerwacje w danym dniu, false je�li nie znaleziono
*/
bool Find_Day_And_Show(Tables* head, struct  tm* dat);

/** Funkcja pobiera od u�ytkownika informacje o dniu, w kt�rym rezerwacje maj� zosta� wy�wietlone.
@param head wska�nik na g�ow� listy ze stolikami
*/
void Show_Reservations_by_Day(Tables* head);

/** Funkcja pobiera od u�ytkownika numer telefonu osoby, kt�rej rezerwacja ma zosta� wy�wietlona.
@param head wska�nik na g�ow� listy ze stolikami
*/
void Show_Chosen_Reservation(Tables* head);

/** Funkcja wywo�uje Find_Day_And_Show z okre�lonymi parametrami, aby wy�wietli� rezerwacje w aktualnym dniu.
@param head wska�nik na g�ow� listy ze stolikami
*/
void Show_Today_Reservations(Tables* head);

/** Menu wy�wietlania rezerwacji.
@param head wska�nik na g�ow� listy ze stolikami
*/
void Show_Reservations(Tables* head);

/** Funkcja zmieniaj�ca dat� z char* na struct tm.
@param date data jako ci�g znak�w
@param dtime wska�nik na struktur� struct tm, do kt�rej ma zosta� wpisana data
@param tmp tymczasowa tablica
*/
void Convert_Date(char* date, struct tm* dtime, char* tmp);

/** Funkcja zmieniaj�ca godzin� z char* na struct tm.
@param hour godzina jako ci�g znak�w
@param dtime wska�nik na struktur� struct tm, do kt�rej ma zosta� wpisana godzina
@param tmp tymczasowa tablica
*/
void Convert_Hour(char* hour, struct tm* dtime, char* tmp);

/** Funkcja sprawdza z jakiego zakresu jest dany znak.
@param ch znak
@return 0 je�li znak od 'a' do 'z', 1 je�li pd 'A' do 'Z', 2 je�li z innego przedzia�u
*/
int Check_Char(char ch);

/** Funkcja do szyfrowania numeru telefonu szyfrem cezara. Szybforwanie zachodzi w taki spos�b, �e zaszyfrowany numer sk�ada si� z samych cyfr.
@param descrambler zmienna decyduj�ca, czy funkcja ma szyfrowa� czy deszyfrowa�
@param tab tablica znak�w do zaszyfrowania
*/
void Number_Scrambler(bool descrambler, char* tab);

/** Funkcja do szyfrowania tekstu szyfrem cezara. Zaszyfrowane dane skladaj� si� z liter, przy uwzgl�dnieniu ich wielko�ci.
@param descrambler zmienna decyduj�ca, czy funkcja ma szyfrowa� czy deszyfrowa�
@param tab tablica znak�w do zaszyfrowania
*/
void Scrambler(bool descrambler, char* tab);

/** Funkcja usuwania spacji.
@param text tekst
*/
void Delete_Front_Space(char* text);

/** Funkcja odczytuje dane z pliku ze stolikami i wpisuje je do listy Tables.
@return wska�nik na g�ow� listy ze stolikami
*/
Tables* Read_Restaurant_File();

/** Funkcja zapisuje informacje o stolikach w pliku. Odczytuje je z listy.
@param head wska�nik na g�ow� listy ze stolikami
@return true, je�li zako�czone powodzeniem
*/
bool Save_Restaurant_File(Tables* head);

/** Funkcja odczytuje z pliku informacje o rezerwacjach. Alokuje pami�� na struct Personal_Data i struct tm. W przypadku braku 
stolika, rezerwacja nie jest wczytywana do listy.
@param Thead wska�nik na g�ow� listy ze stolikami
@patam headDiscounts wska�nik na g�ow� listy ze zni�kami
@return true, je�li zako�czone powodzeniem
*/
bool Read_Reservations_File(Tables* Thead, Discounts* headDiscounts);

/** Funkcja zapisuje do pliku rezerwacje.
@param table wska�nik na g�ow� listy ze stolikami
@return true, je�li zako�czone powodzeniem
*/
bool Save_Reservations_File(Tables* table);

/** Funkcja odczytuje dane z pliku ze zni�kami i wpisuje je do listy Discounts.
@return wska�nik na g�ow� listy ze zni�kami
*/
Discounts* Read_Discounts_File();

/** Funkcja zapisuje do pliku zni�ki.
@param head wska�nik na g�ow� listy ze zni�kami
@return true, je�li zako�czone powodzeniem
*/
bool Save_Discounts_File(Discounts* head);

/** Funkcja wywo�uje wszystkie funkcje do zapisu danych w pliku.
@param head wska�nik na g�ow� listy ze stolikami
@param headDisc wska�nik na g�ow� listy ze zni�kami
*/
void Save_Everything(Tables* head, Discounts* headDisc);

/** Menu g��wne aplikacji.
@param Thead wska�nik na g�ow� listy ze stolikami
@param Dischead wska�nik na wska�nik na g�ow� listy ze zni�kami
*/
void Action_Center(Tables* Thead, Discounts** DiscHead);

/** Funkcja wywo�uj�ca funkcje do oczytu z pliku, Action_Center i do zapisu danych oraz zwolnienia 
pami�ci przed zamkni�ciem programu.
*/
void Manager();
