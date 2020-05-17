#include "functions_header.h"

//#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#define _CRTDBG_MAP_ALLOC
int main()
{


	

	Manager();

	//int* a = malloc(6*sizeof(int));
	//a[1] = 1;
	//a[2] = 3;
	//
	//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

_CrtDumpMemoryLeaks();
return 0;
}