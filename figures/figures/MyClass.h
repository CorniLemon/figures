#pragma once
#include <Windows.h>
//#ifndef MY_CLASS_H
//#define MY_CLASS_H

class MyClass
{
	MyClass();
};

//#endif // MY_CLASS_H

extern BYTE matr[1000 * 1000 * 3];

template<class T, typename T2>
T sum(T x, T2 y)
{
	return x + y;
}

template<class T>
T sum(T x, double y)
{
	return x + y - 2;
}

template<typename T, int n>
class StaticArray
{
	T array[n];
};

template<typename T, int n=1>
class StaticArray
{
	T array;
};


template<class T>
T sum2(int a, int b)
{
	return (T)a + (T)b;
}

double sum2(int a, int b)
{
	return (double)a + (double)b + 0.5;
}

inline void fff()
{
	StaticArray<double, 5> sa;
	int a = 0, b = 0;
	int c = sum(a, b);
	double d = sum2<double>(a, b);
	float d2 = sum2<float>(a, b);
}

