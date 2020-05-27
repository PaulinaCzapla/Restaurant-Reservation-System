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

/** Funkcja do zmieniania napisów na czerwone.
@param text tablica z napisem
*/
void Alert_Red(char* text);

/** Funkcja do zamieniania napisów na zielone.
@param text tablica z napisem
*/
void Green_Text(char* text);

/** Funkcja, która pyta siê u¿ytkownika czy na pewno chce usun¹æ dan¹ rezerwacjê.
@param hour wskaŸnik na element listy z godzinami
@return true jeœli u¿ytkownik siê zgodzi, false jeœli nie
*/
bool Ask_Before_Delete(Hour* hour);

/** Funkcja przeszukuje listê w celu znalezienia rezerwacji z okreœlonym numerem telefonu.
@param num tablica z numerem telefonu
@param head wskaŸnik na g³owê listy ze stolikami
@param ask zmienna pomocnicza, której wartoœæ zale¿y od miejsca w programie, gdzie zosta³a wywo³ana funkcja. Jeœli ask=false, nie zostanie wywo³ana funkcja Ask_Before_Delete, 
jeœli ask=true,funkcja zostanie wywo³ana i od jej wartoœci bêdzie zale¿eæ wykonanie dalejszej czêœci funkcji.
@return 0 jeœli nie znaleziono numeru, 1 jeœli rezerwacja zosta³a usuniêta, 2 jeœli u¿ytkownik zrezygnowa³ z usuwania rezerwacji
*/
short int Find_And_Delete_by_Phone_Number(char* num, Tables* head, bool ask);

/** Funkcja, która pobiera od u¿ytkownika numer telefonu z rezerwacji do usuniêcia i wyœwietla informacje o zakoñczonym sukcesem lub nie usuniêciu elementu
@param head wskaŸnik na g³owê listy ze stolikami
*/
void Delete_Record(Tables* head);

/** Funkcja, która posiada na podstawie otrzymanych parametrów, wywo³uje odpowiednie funkcje dodawania rezerwacji do listy.
Jeœli ptrDate != NULL to oznacza, ¿e w danym stoliku ju¿ znajduj¹ siê jakieœ rezerwacje w podanym dniu. 
Z tego powodu, zostaje dodany nowy element listy Hour do konkretnego elementu listy Date.
Jeœli ptrDate = NULL, oznacza to, ¿e w danym dniu nie ma jeszcze rezerwacji na dan¹ godzinê, wiêc do struktury jest dodawany najpierw element listy Date, a dopiero póŸniej
Hour.
@param position wskaŸnik na strukturê Position z lokalizacj¹ w strukturze aktualnie dodawanego elementu
@param ptrTable wskaŸnik na element listy ze stolikami
@param ptrDate wskaŸnik na element listy z datami
@param time struktura przechowuj¹ca informacje o dacie i godzinie
@param how_long informacja jak d³ugo goœcie zamierzaj¹ zostaæ w restauracji
@return wskaŸnik na nowo powsta³y element listy Hours
*/
Hour* Add_New_Reservation(Position* position, Tables* ptrTable, Date* ptrDate,struct tm* time, float how_long);

/** Funkcja obliczaj¹ca ró¿nicê miêdzy godzinami rezerwacji. W przypadkku, gdy ró¿nica minut 
jest mniejsza od 0, zmniejszona zostaje wartoœæ godziny, a nowa wartoœæ minut to 60 + (poprzednia wartoœæ minut). Po wyliczeniu ró¿nicy godzin, zamieniana
jest ona na liczbê dziesiêtn¹.
@param h1 wczeœniejsza godzina
@param h2 póŸniejsza godzina
@return ró¿nica
*/
float Hours_Difference(struct tm* h1, struct  tm* h2);


/** Funkcja obliczaj¹ca ró¿nicê miêdzy godzinami rezerwacji w przypadku, gdy s¹ one w innych dniach. Funkcja oblicza iloœæ czasu do pó³nocy dla ka¿dej godziny i sumuje te wartoœci. Nastêpnie sprawdza, czy rezerwacje nie zachodz¹ na siebie.
@param prev poprzednia data
@param next nastêpna data
@param currtime czas sprawdzanej rezerwacji
@param how_long iloœæ czasu, któr¹ klienci nowej rezerwacji chc¹ spêdziæ w restauracji
@return false jeœli jest konflikt i true jeœli go nie ma
*/
bool Check_Conflict(Date* prev, Date* next, struct tm* currtime, float how_long);

/** Funkcja bêd¹ca elementem algorytmu zapobiegaj¹cego konfliktom terminów. Przeszukuje ona listê godzin dla danej 
daty i sprawdza, czy ró¿nica pomiêdzy godzin¹ w liœcie, a godzin¹ nowej rezerwacji jest dostateczna. Robi to 
wywo³uj¹c funkcje Hours_Difference z ró¿nymi parametrami i porównuje otrzymane wartoœci z wartoœciami how_long 
dla poszczególnych rezerwacji. Dodanie nowej rezerwacji zale¿y równie¿ od funkcji Check_Conflict. Jeœli o danej godzinie nie ma wolnego terminu, zmieniona zostaje wartoœæ position->definitive na false, co 
umo¿liwia poszukiwanie wolnych miejsc w innych terminach. Jeœli nie ma konfliktu terminów, wywo³ana zostaje funkcja
Add_New_Reservation.
@param position wskaŸnik na strukturê Position z lokalizacj¹ w strukturze aktualnie dodawanego elementu
@param ptrTab wskaŸnik na element listy ze stolikami
@param ptrDate wskaŸnik na element listy z datami
@param time struktura przechowuj¹ca informacje o dacie i godzinie
@param how_long informacja jak d³ugo goœcie zamierzaj¹ zostaæ w restauracji
@param prevDate poprzednia data (dzieñ poprzedni do aktualnie sprawdzanego)
@param nextDate nastêpna data (dzieñ nastêpny do aktualnie sprawdzanego)
@return wskaŸnik na nowo powsta³y element listy Hours
*/
Hour* Check_Hours(Position* position, Tables* ptrTable, Date* ptrDate, struct  tm* time, float how_long, Date* prevDate, Date* nextDate);

/** Funkcja jest elementem algorytmu zapobiegaj¹cego konfliktom terminów. Sprawdza czy w liœcie wystêpuje rezerwacja
w tym samym dniu, albo w dniu poprzednim lub nastêpnym. Jeœli tak, wywo³ana jest funkcja Check_Hours. W przeciwnym przypadku wywo³ana jest funkcja Add_New_Reservation.
@param position wskaŸnik na strukturê Position z lokalizacj¹ w strukturze aktualnie dodawanego elementu
@param ptrTab wskaŸnik na element listy ze stolikami
@param time struktura przechowuj¹ca informacje o dacie i godzinie
@param how_long informacja jak d³ugo goœcie zamierzaj¹ zostaæ w restauracji
@return wskaŸnik na nowo powsta³y element listy Hours
*/
Hour* Check_Dates(Position* position, Tables* ptrTable, struct  tm* time, float how_long);

/** Funkcja jest elementem algorytmu zapobiegaj¹cego konfliktom terminów. Sprawdza czy w strukturze istnieje stolik na 
odpowiedni¹ iloœæ osób. Jeœli taki istnieje i nie ma ¿adnych rezerwacji, wywo³ana jest funkcja Add_New_Reservation.
 W przeciwnym przypadku wywo³ana zostaje funkcja CHeck_Dates.
@param position wskaŸnik na strukturê Position z lokalizacj¹ w strukturze aktualnie dodawanego elementu
@param head wskaŸnik na g³owê listy ze stolikami
@param time struktura przechowuj¹ca informacje o dacie i godzinie
@param amount iloœæ goœci
@param how_long informacja jak d³ugo goœcie zamierzaj¹ zostaæ w restauracji
@return wskaŸnik na nowo powsta³y element listy Hours
*/
Hour* Check_Tables(Position* position, Tables* head, struct  tm* time, int amount, float how_long);

/** Funkcja sprawdzaj¹ca czy podana data jest aktualna. Pobiera aktualny czas za pomoc¹ funkcji time() i rzutuje go na struct tm za pomoc¹ localtime_s.
@param time_to_check czas, który ma zostaæ sprawdzony
@return false jeœli data jest przestarza³a i true jeœli aktualna
*/
bool Is_Up_To_Date(struct tm* time_to_check);

/** Prosta funkcja sprawdzaj¹ca czy w liœcie istnieje ju¿ dana zni¿ka.
@param head wskaŸnik na g³owê listy ze zni¿kiami
@param liczba procentowa zni¿ki do dodania
@return false jeœli isnieje ju¿ taka zni¿ka
*/
bool Check_Discounts_Repetitions(Discounts* head, int percent);

/** Funkcja do wyœwietlania ni¿ek na ekranie.
@param head wskaŸnik na g³owê listy ze zni¿kiami
*/
void Show_Discounts(Discounts* head);

/** Funkcja prosi u¿ytkownika o podanie danych nowej zni¿ki. Sprawdza czy dane s¹ prawid³owe i czy mo¿na dodaæ zni¿kê.
@param head wskaŸnik na g³owê listy ze zni¿kiami
@return wskaŸnik na g³owê listy
*/
Discounts* Add_New_Discount_Type(Discounts* head);

/** Funkcja znajduje dan¹ zni¿kê na podstawie jej wartoœci procentowej.
@param head wskaŸnik na g³owê listy ze zni¿kiami
@param percent wartoœæ procentowa zni¿ki do znalezienia
@return wskaŸnik na znalezion¹ zni¿kê, jeœli nie ma takiej zni¿ki, zwraca NULL
*/
Discounts* Find_Discount(Discounts* head, int percent);

/** Funkcja usuwa zni¿kê z listy
@param head wskaŸnik na wskaŸnik na g³owê listy ze zni¿kami
@param found wskaŸnik na zni¿ke do usuniêcia
*/
void Delete_Discount_Safely(Discounts** head, Discounts* found);

/** Funkcja znajduje wszystkie rezerwacje z okreœlon¹ zni¿k¹ i zamienia je na NULL.
@param found wskaŸnik na zni¿ke do znalezienia
@param reservationsHead wskaŸnik na g³owê listy z rezerwacjami/stolikami
*/
void Delete_Discount_From_Reservation(Discounts* found, Tables* reservationsHead);

/** Funkcja prosi u¿ytkownika o podanie danych zni¿ki do usuniêcia i odwo³uje siê do innych, koniecznych funkcji.
@param head wskaŸnik na wskaŸnik na g³owê listy ze zni¿kami
@param reservationsHead wskaŸnik na g³owê listy z rezerwacjami/stolikami
*/
void Delete_Discount(Discounts** head, Tables* reservationsHead);

/** Menu systemmu zarz¹dzania zni¿kami.
@param head wskaŸnik na wskaŸnik na g³owê listy ze zni¿kami
@param reservationsHead wskaŸnik na g³owê listy z rezerwacjami/stolikami
*/
void Discount_Manager(Discounts** head, Tables* reservationsHead);

/** Funkcja dodaje do konkretnej rezerwacji zni¿kê.
@param reservation wskaŸnik na wskaŸnik na element listy Hours
@param headDiscounts wskaŸnik na wskaŸnik na g³owê listy ze zni¿kami
@param percent wartoœæ procentowa zni¿ki do dodania
@return true jeœli uda³o siê dodaæ zni¿kê
*/
bool Connect_Discount_And_Record(Hour** reservation, Discounts** headDiscounts, int percent);

/** Funkcja pyta siê czy u¿ytkownik chce dodaæ zni¿kê do rezerwacji i prosi o podanie danych.
@param reservation wskaŸnik na element listy Hours z rezerwacj¹
@param head wskaŸnik na g³owê listy ze zni¿kami
*/
void Add_Discount_To_Reservation(Hour* reservation, Discounts* head);

/** G³ówna funkcja dodawania rezerwacji do systemu przez u¿ytkownika. Jeœli w systemie nie ma listy ze stolikami,
 funkcja wyœwietla komunikat i siê koñczy. w przeciwnym przypadku, prosi u¿ytkownika o podanie danych. Jeœli u¿ytkownik
  poda nieprawid³owe dane (wychodz¹ce poza zakres, litery zamiast liczb itd.) funkcja wyœwietla odpowiedni komunikat. 
  Funkcja alokuje równie¿ pamiêæ dla struct tm, struct Personal_Data i struct position. Pierwsze dwie struktury wpisywane s¹ do 
  g³ównej struktury danych, ostatnia jest zwalniana pod koniec dzia³ania funkcji.
@param headT wskaŸnik na g³owê listy ze stolikami
@param headDiscounts wskaŸnik na g³owê listy ze zni¿kami
*/
void Add_Reservation(Tables* headT, Discounts* headDiscounts);

/** Funkcja do znajdowania stolika w strukturze po jego numerze.
@param num numer stolika
@param head wskaŸnik na g³owê listy ze stolikami
@return wskaŸnik na znaleziony stolik
*/
Tables* Find_Table(int num, Tables* head);

/** Funkcja do wyœwietlania stolików na ekranie.
@param pH wskaŸnik na g³owê listy ze stolikami
*/
void Show_Tables(Tables* pH);

/** Funkcja do dodawania nowego stolika. Prosi u¿ytkownika o podanie danych i sprawdza ich poprawnoœæ.
@param head wskaŸnik na wskaŸnik nag³owê listy ze stolikami
*/
void Add_New_Table(Tables** head);

/** Funkcja do usuwania stolika. Wywo³uje funkcjjê Find_And_Delete_by_Phone_Number w celu usuniêcia wszystkich danych, dla danego stolika. Nastêpnie usuwa stolik.
@param head wskaŸnik na wskaŸnik nag³owê listy ze stolikami
@param found wskaŸnik na stolik do usuniêcia
*/
void Delete_Table(Tables** head, Tables* found);

/** Funkcja prosi u¿ytkownika o podanie danych stolika do usuniêcia i upewnia siê, czy u¿ytkownik na pewno chce go usun¹æ.
@param head wskaŸnik na wskaŸnik nag³owê listy ze stolikami
*/
void Select_Table_To_Delete(Tables** head);

/** Menu systemu zarz¹dzania stolikami.
@param head wskaŸnik na wskaŸnik nag³owê listy ze stolikami
*/
void Tables_Manager(Tables** head);

/** Funkcja do wyœwietlania na ekran jednej rezerwacji.
@param table wskaŸnik na element listy ze stolikami
@param hour wskaŸnik na element listy hour
*/
void Show_One_Reservation(Tables* table, Hour* hour);

/** Funkcja do znajdowania rezerwacji na dany numer telefonu w strukturze.
@param num numer telefonu do znalezienia
@param head wskaŸnik na g³owê listy ze stolikami
@return true jeœli znaleziono, false jeœli nie
*/
bool Find_Reservation_by_Phone_Number(char* num, Tables* head);

/** Funkcja do wywo³ywania Show_One_Reservation dla wszystkich rezerwacji.
@param table wskaŸnik na g³owê listy ze stolikami
*/
void Show_All_Reservations(Tables* table);

/** Funkcja znajduje rezerwacje danego dnia i je wyœwietla. Jeœli dat==NULL to pobierana jest aktualna data i wyœwietlane s¹
 aktualne rezerwacje
@param head wskaŸnik na g³owê listy ze stolikami
@param dat wskaŸnik na struct tm z dat¹ do znalezienia
@return true jeœli znaleziono rezerwacje w danym dniu, false jeœli nie znaleziono
*/
bool Find_Day_And_Show(Tables* head, struct  tm* dat);

/** Funkcja pobiera od u¿ytkownika informacje o dniu, w którym rezerwacje maj¹ zostaæ wyœwietlone.
@param head wskaŸnik na g³owê listy ze stolikami
*/
void Show_Reservations_by_Day(Tables* head);

/** Funkcja pobiera od u¿ytkownika numer telefonu osoby, której rezerwacja ma zostaæ wyœwietlona.
@param head wskaŸnik na g³owê listy ze stolikami
*/
void Show_Chosen_Reservation(Tables* head);

/** Funkcja wywo³uje Find_Day_And_Show z okreœlonymi parametrami, aby wyœwietliæ rezerwacje w aktualnym dniu.
@param head wskaŸnik na g³owê listy ze stolikami
*/
void Show_Today_Reservations(Tables* head);

/** Menu wyœwietlania rezerwacji.
@param head wskaŸnik na g³owê listy ze stolikami
*/
void Show_Reservations(Tables* head);

/** Funkcja zmieniaj¹ca datê z char* na struct tm.
@param date data jako ci¹g znaków
@param dtime wskaŸnik na strukturê struct tm, do której ma zostaæ wpisana data
@param tmp tymczasowa tablica
*/
void Convert_Date(char* date, struct tm* dtime, char* tmp);

/** Funkcja zmieniaj¹ca godzinê z char* na struct tm.
@param hour godzina jako ci¹g znaków
@param dtime wskaŸnik na strukturê struct tm, do której ma zostaæ wpisana godzina
@param tmp tymczasowa tablica
*/
void Convert_Hour(char* hour, struct tm* dtime, char* tmp);

/** Funkcja sprawdza z jakiego zakresu jest dany znak.
@param ch znak
@return 0 jeœli znak od 'a' do 'z', 1 jeœli pd 'A' do 'Z', 2 jeœli z innego przedzia³u
*/
int Check_Char(char ch);

/** Funkcja do szyfrowania numeru telefonu szyfrem cezara. Szybforwanie zachodzi w taki sposób, ¿e zaszyfrowany numer sk³ada siê z samych cyfr.
@param descrambler zmienna decyduj¹ca, czy funkcja ma szyfrowaæ czy deszyfrowaæ
@param tab tablica znaków do zaszyfrowania
*/
void Number_Scrambler(bool descrambler, char* tab);

/** Funkcja do szyfrowania tekstu szyfrem cezara. Zaszyfrowane dane skladaj¹ siê z liter, przy uwzglêdnieniu ich wielkoœci.
@param descrambler zmienna decyduj¹ca, czy funkcja ma szyfrowaæ czy deszyfrowaæ
@param tab tablica znaków do zaszyfrowania
*/
void Scrambler(bool descrambler, char* tab);

/** Funkcja usuwania spacji.
@param text tekst
*/
void Delete_Front_Space(char* text);

/** Funkcja odczytuje dane z pliku ze stolikami i wpisuje je do listy Tables.
@return wskaŸnik na g³owê listy ze stolikami
*/
Tables* Read_Restaurant_File();

/** Funkcja zapisuje informacje o stolikach w pliku. Odczytuje je z listy.
@param head wskaŸnik na g³owê listy ze stolikami
@return true, jeœli zakoñczone powodzeniem
*/
bool Save_Restaurant_File(Tables* head);

/** Funkcja odczytuje z pliku informacje o rezerwacjach. Alokuje pamiêæ na struct Personal_Data i struct tm. W przypadku braku 
stolika, rezerwacja nie jest wczytywana do listy.
@param Thead wskaŸnik na g³owê listy ze stolikami
@patam headDiscounts wskaŸnik na g³owê listy ze zni¿kami
@return true, jeœli zakoñczone powodzeniem
*/
bool Read_Reservations_File(Tables* Thead, Discounts* headDiscounts);

/** Funkcja zapisuje do pliku rezerwacje.
@param table wskaŸnik na g³owê listy ze stolikami
@return true, jeœli zakoñczone powodzeniem
*/
bool Save_Reservations_File(Tables* table);

/** Funkcja odczytuje dane z pliku ze zni¿kami i wpisuje je do listy Discounts.
@return wskaŸnik na g³owê listy ze zni¿kami
*/
Discounts* Read_Discounts_File();

/** Funkcja zapisuje do pliku zni¿ki.
@param head wskaŸnik na g³owê listy ze zni¿kami
@return true, jeœli zakoñczone powodzeniem
*/
bool Save_Discounts_File(Discounts* head);

/** Funkcja wywo³uje wszystkie funkcje do zapisu danych w pliku.
@param head wskaŸnik na g³owê listy ze stolikami
@param headDisc wskaŸnik na g³owê listy ze zni¿kami
*/
void Save_Everything(Tables* head, Discounts* headDisc);

/** Menu g³ówne aplikacji.
@param Thead wskaŸnik na g³owê listy ze stolikami
@param Dischead wskaŸnik na wskaŸnik na g³owê listy ze zni¿kami
*/
void Action_Center(Tables* Thead, Discounts** DiscHead);

/** Funkcja wywo³uj¹ca funkcje do oczytu z pliku, Action_Center i do zapisu danych oraz zwolnienia 
pamiêci przed zamkniêciem programu.
*/
void Manager();
