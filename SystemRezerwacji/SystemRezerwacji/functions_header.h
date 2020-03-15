#pragma once

#ifndef FUNCTIONS_HEADER_H
#define FUNCTIONS_HEADER_H
#include "structures_header.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>

void WYSWIETL(Restaurant* pHe);
void WYSWIETLRESER(Reservations* pH);
Restaurant* First_Node(Restaurant* prevpointer, Restaurant* nextpointer, int number, int amount);
Restaurant* Node(Restaurant* pHead, int number, int amount);
Reservations* First_Node_Reservations(Reservations* nextpointer, Personal_Data pdata, Date_And_Hour dateandhour, unsigned int table_number, unsigned int amount, char* comment);
Reservations* Node_Reservations(Reservations* pHead, Personal_Data pdata, Date_And_Hour dateandhour, unsigned int table_number, unsigned int amount, char* comment);

void Read_Restaurant_File(int limit);
void Read_Reservations_File();
//extern Restaurant* First_Node(Restaurant* , Restaurant* , int , int );
//extern Restaurant* Node(Restaurant*, int , int);

#endif