#ifndef FILE_OPERATIONS_C
#define FILE_OPERATIONS_C
//#include "functions.h"
//
//
//
//
//void Delete_Front_Space(char* text)
//{
//	for (int i = 0; i < strlen(text); i++)
//		text[i] = text[i + 1];
//}
//
//Tables* Read_Restaurant_File()
//{
//	FILE* file_restaurant;
//	errno_t err;
//	Tables* pH = NULL;
//
//	err = fopen_s(&file_restaurant, FILENAME_RESTAURANT, "r");
//	if (file_restaurant != NULL)
//	{
//		if (err)
//		{
//			Alert_Red(" Blad przy otwieraniu pliku ze stolikami.\n");
//			return NULL;
//		}
//		else
//		{
//			int amount, table;
//
//			int i = 0;
//			Tables* head = NULL;
//			while (fscanf_s(file_restaurant, "%i %i", &table, &amount) != EOF)
//			{
//				pH = Table_Node(&pH, table, amount);
//			}
//			fclose(file_restaurant);
//		}
//	}
//	return pH;
//}
//
//bool Save_Restaurant_File(Tables* head)
//{
//	FILE* file_restaurant;
//	errno_t err;
//	Tables* pH = NULL;
//
//	err = fopen_s(&file_restaurant, FILENAME_RESTAURANT, "w");
//	if (file_restaurant != NULL)
//	{
//		if (err)
//		{
//			Alert_Red("\n Blad przy otwieraniu pliku ze stolikami.");
//			return false;
//		}
//		else
//		{
//			while (head)
//			{
//				fprintf(file_restaurant, "%i %i \n", head->table_number, head->seats);
//				head = head->next;
//			}
//			fclose(file_restaurant);
//		}
//	}
//	return true;
//}
//
//bool Read_Reservations_File(Tables* Thead, Discounts* headDiscaunts)
//{
//	FILE* file_reservations;
//	errno_t err;
//
//	err = fopen_s(&file_reservations, FILENAME_RESERVATIONS, "r");
//
//	if (file_reservations != NULL)
//	{
//		if (err)
//		{
//			Alert_Red(" Nastapil blad przy otwieraniu pliku z rezerwacjami.\n");
//			return false;
//		}
//		else
//		{
//			Hour* reservation = NULL;
//
//			struct Position* position = malloc(sizeof(struct Position));
//			position->date = NULL;
//			position->hour = NULL;
//			position->table = NULL;
//			position->definitive = true;
//
//			int  amount, table, discount;
//			float how_long;
//			char tmpc;
//			char date[12];
//			char hour[12];
//			char tmp[12];
//
//			Personal_Data pdata;
//			struct tm time;
//			struct tm* dtime = &time;
//
//			while (fscanf_s(file_reservations, "%d", &table) != EOF)
//			{
//				fscanf_s(file_reservations, "%d", &amount);
//				fscanf_s(file_reservations, "%s", date, 12);
//				fscanf_s(file_reservations, "%s", hour, 12);
//				fscanf_s(file_reservations, "%f", &how_long);
//				fscanf_s(file_reservations, "%s", pdata.name, MAX_NAME);
//				fscanf_s(file_reservations, "%s", pdata.last_name, MAX_L_NAME);
//				fscanf_s(file_reservations, "%s", pdata.phone_number, MAX_PHONE_NUM);
//				fscanf_s(file_reservations, "%d", &discount);
//
//				Scrambler(true, pdata.name);
//				Scrambler(true, pdata.last_name);
//				Number_Scrambler(true, pdata.phone_number);
//
//				if (!(position->table = Find_Table(table, Thead)))
//					continue;
//
//				Personal_Data* pdata2 = malloc(sizeof(Personal_Data));
//				struct tm* dtime2 = malloc(sizeof(struct tm));
//
//				Convert_Date(date, &time, tmp);
//				Convert_Hour(hour, dtime, tmp);
//
//				if (Is_Up_To_Date(dtime) == false)
//				{
//					free(dtime2);
//					free(pdata2);
//					fgets(pdata.comment, MAX_COMMENT, file_reservations);
//
//					continue;
//				}
//
//				if (fgets(pdata.comment, MAX_COMMENT, file_reservations))
//				{
//					if (pdata.comment == NULL)
//					{
//						Alert_Red(" \n Blad oczytu pliku reservations.txt - nieprawidlowy format.\n");
//						return false;
//					}
//
//					Delete_Front_Space(pdata.comment);
//
//					if (pdata.comment[(strlen(pdata.comment) - 1)] != '\n')
//						while ((tmpc = getchar()) != '\n');
//					else
//					{
//						pdata.comment[(strlen(pdata.comment) - 1)] = '\0';
//					}
//					Scrambler(true, pdata.comment);
//					*(pdata2) = pdata;
//					*(dtime2) = time;
//
//					reservation = Check_Tables(position, position->table, dtime2, amount, how_long);
//					if (reservation)
//					{
//						reservation->customers = amount;
//						reservation->pdata = pdata2;
//
//						if (discount != 0)
//							Connect_Discount_And_Record(&reservation, &headDiscaunts, discount);
//					}
//					else
//					{
//						Alert_Red(" \n Nieprawidlowe dane w pliku. \n");
//						return false;
//					}
//					position->date = NULL;
//					position->hour = NULL;
//					position->table = NULL;
//				}
//				else
//					break;
//			}
//			free(position);
//			fclose(file_reservations);
//		}
//	}
//	return true;
//
//
//}
//
//bool Save_Reservations_File(Tables* table)
//{
//	FILE* file_reservations;
//	errno_t err;
//
//	err = fopen_s(&file_reservations, FILENAME_RESERVATIONS, "w");
//	if (file_reservations != NULL)
//	{
//		if (err)
//		{
//			Alert_Red("\n Nie udalo sie otworzyc pliku z rezerwacjami.");
//			return false;
//		}
//		else
//		{
//			Tables* tmpR = table;
//			Date* tmpD = NULL;
//			Hour* tmpH = NULL;
//
//			Tables* head = table;
//			if (tmpR)
//				while (tmpR)
//				{
//					tmpD = tmpR->date;
//					while (tmpD)
//					{
//						tmpH = tmpD->hour;
//						while (tmpH)
//						{
//							char buff[30];
//							strftime(buff, 30, "%d.%m.%y %H:%M", tmpH->hour);
//
//							Scrambler(false, tmpH->pdata->name);
//							Scrambler(false, tmpH->pdata->last_name);
//							Number_Scrambler(false, tmpH->pdata->phone_number);
//							Scrambler(false, tmpH->pdata->comment);
//
//							fprintf(file_reservations, "%d %d %s %.2f %s %s %s", tmpR->table_number, tmpH->customers, buff, tmpH->how_long, tmpH->pdata->name, tmpH->pdata->last_name, tmpH->pdata->phone_number);
//							if (tmpH->discount)
//								fprintf(file_reservations, " %d", tmpH->discount->percent);
//							else
//								fprintf(file_reservations, " %d", 0);
//
//							fprintf(file_reservations, " %s\n", tmpH->pdata->comment);
//
//							Scrambler(true, tmpH->pdata->name);
//							Scrambler(true, tmpH->pdata->last_name);
//							Number_Scrambler(true, tmpH->pdata->phone_number);
//							Scrambler(true, tmpH->pdata->comment);
//
//							tmpH = tmpH->next;
//						}
//						tmpD = tmpD->next;
//					}
//					tmpR = tmpR->next;
//				}
//			head = table;
//
//			fclose(file_reservations);
//
//		}
//	}
//
//	return true;
//}
//
//Discounts* Read_Discounts_File()
//{
//	FILE* file_discaunts;
//	errno_t err;
//	Discounts* head = NULL;
//
//	err = fopen_s(&file_discaunts, FILENAME_DISCAUNTS, "r");
//	if (file_discaunts != NULL)
//	{
//		if (err)
//		{
//			Alert_Red(" Blad przy otwieraniu pliku ze zniznami.\n");
//			return NULL;
//		}
//		else
//		{
//			int percent;
//			char tmp[MAX_DISCAUNT_NAME];
//			char tmpc;
//
//			while (fscanf_s(file_discaunts, "%i", &percent) != EOF)
//			{
//				if (fgets(tmp, MAX_DISCAUNT_NAME, file_discaunts))
//				{
//					if (tmp == NULL)
//					{
//						Alert_Red(" \n Blad oczytu pliku discaunts.txt - nieprawidlowy format.\n");
//						return NULL;
//					}
//					Delete_Front_Space(tmp);
//					if (tmp[(strlen(tmp) - 1)] != '\n')
//						while ((tmpc = getchar()) != '\n');
//					else
//					{
//						tmp[(strlen(tmp) - 1)] = '\0';
//					}
//					if (Check_Discounts_Repetitions(head, percent))
//						head = Discount_Node(&head, tmp, percent);
//				}
//				else
//					break;
//			}
//			fclose(file_discaunts);
//		}
//	}
//	return head;
//}
//
//bool Save_Discounts_File(Discounts* head)
//{
//	FILE* file_discaunts;
//	errno_t err;
//	Discounts* tmpHead = head;
//
//	err = fopen_s(&file_discaunts, FILENAME_DISCAUNTS, "w");
//	if (file_discaunts != NULL)
//	{
//		if (err)
//		{
//			Alert_Red("\n Blad przy otwieraniu pliku ze zniznami.");
//			return false;
//		}
//		else
//		{
//			int percent;
//			int i = 0;
//			char tmpc;
//
//			while (tmpHead)
//			{
//				fprintf(file_discaunts, "%i %s\n", tmpHead->percent, tmpHead->discaunt_name);
//				tmpHead = tmpHead->next;
//			}
//			fclose(file_discaunts);
//		}
//	}
//	return true;
//}
//
//void Save_Everything(Tables* head, Discounts* headDisc)
//{
//	if (Save_Restaurant_File(head) && Save_Reservations_File(head) && Save_Discounts_File(headDisc))
//		Green_Text(" \n\n  Zapisano. ");
//	else
//		Alert_Red(" \n\n Nastapil blad przy zapisie.");
//
//	puts("\n\n Wcisnij dowolny klawisz aby kontynuowac...");
//	getch();
//}


#endif