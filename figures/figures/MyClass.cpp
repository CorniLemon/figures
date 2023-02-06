#include "MyClass.h"

MyClass::MyClass()
{
#ifdef _DEBUG
	if (1)
		return;
#endif

}


void ff()
{
	int x = 5, y = 6;
	int z = sum(x, y);//x+y
}