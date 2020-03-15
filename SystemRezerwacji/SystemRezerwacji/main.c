#include "functions_header.h"

int main()
{
	//Restaurant* pH = Node(NULL, 3, 4);
	//pH = Node(pH, 6, 1);
	//pH = Node(pH, 8, 2);
	//pH = Node(pH, 9, 9);
	//pH = Node(pH, 1, 4);
	//pH = Node(pH, 2, 3);

	//WYSWIETL(pH);
	/*char a[2];
	for (int i = 0; i < 2; i++)
		a[i] = getchar();
	for (int i = 0; i < 2; i++)
		printf("%c", a[i]);*/
	Read_Reservations_File();
	int a = 2;
	Read_Restaurant_File(a);
	
	
	return 0;

}