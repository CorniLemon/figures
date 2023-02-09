#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <cstdlib>
#include <cstdio>
#include <Windows.h>
#include <fstream>
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;
//#include "MyClass.h"
//
//struct Point
//{
//private:
//    double X, Y;
//public:
//    Point()
//        : X(0), Y(0)
//    {}
//
//    Point(double X, double Y)
//        : X(X), Y(Y)
//    {}
//
//    Point(const Point & p)
//        : X(p.X), Y(p.Y)
//    {}
//
//    Point operator+(const Point& p)
//    {
//        return Point(X + p.X, Y + p.Y);
//    }
//
//    friend Point operator-(const Point& p1, const Point& p2);
//    friend Point operator-(double x, const Point& p);
//
//    Point & operator+=(const Point& p)
//    {
//        X += p.X;
//        Y += p.Y;
//        return *this;
//    }
//
//    Point operator*(const Point& p);
//};
//
//Point Point::operator*(const Point& p)
//{
//    return Point(X * p.X, Y * p.Y);
//}
//
//Point operator-(const Point& p1, const Point& p2)
//{
//    return Point(p1.X - p2.X, p1.Y - p2.Y);
//}
//
//Point operator-(double x, const Point& p)
//{
//    return Point(x - p.X, -p.Y);
//}
//
//void f()
//{
//    Point p1, p2(0, 5);
//    Point p3 = p1 + p2;
//    Point p4 = p1.operator+(p2);
//}
//
//class Array
//{
//private:
//    int* p;
//    int n;
//
//    Array()
//        : p(NULL), n(0)
//    {}
//
//    Array(int n)
//        : p(new int[n])
//    {}
//
//    Array(const Array& a)
//    {
//        if (a.p)
//        {
//            p = new int[a.n];
//            n = a.n;
//            memcpy(p, a.p, n * sizeof(int));
//        }
//        else
//        {
//            p = NULL;
//            n = 0;
//        }
//    }
//};

BYTE matr[1000 * 1000*3];

size_t count(ifstream& base) {
    char str[1024];
    size_t i = 0;
    while (!base.eof())
    {
        base.getline(str, 1024, '\n');
        i++;
    }
    base.seekg(0);
    cout << "в файле " << i << " фигур(ы)\n\n";
    return i;
}

class Figure
{
protected:
    int X0, Y0;
public:
    virtual void draw(BYTE* matr) = 0;

    virtual void init(ifstream& fin)
    {
        fin >> X0;
        fin >> Y0;
    }

    virtual ~Figure() {}
};

class Circle : public Figure
{
    int R;
    int y1;
    float Ht;
public:
    Circle(ifstream& fin)
    {
        init(fin);
    }

    virtual void draw(BYTE* matr)
    {
        float t;
        int x = 0, y = 0;
        for (t = 0; t <= 2 * M_PI; t += Ht) {
            x = X0 + R * cos(t);
            y = Y0 + R * sin(t);
            if ((x <= 999) && (x >= 0) && (y <= 999) && (y >= 0))
                memset(matr + 3 * (1000 * y + x), 0, 3);
        }
    }

    virtual void init(ifstream& fin)
    {
        Figure::init(fin);
        fin >> R;
        Ht = 1. / R;
    }
};

class Triangle : public Figure
{
    int X1, X2, Y1, Y2;

    void line(int xnow, int ynow, int xnext, int ynext, BYTE* matr) {
        float a;
        float HIa, ZNa;
        float Hx;//шаг по x
        float Xa;
        int x = 0, y = 0;
        HIa = ynext - ynow;
        ZNa = xnext - xnow;
        if (ZNa) {
            a = HIa / ZNa;
            if (abs(a) <= 1)
                Hx = 1;
            else
                Hx = 1 / abs(a);
            if (ZNa > 0){
                for (Xa = xnow; Xa <= xnext; Xa += Hx) {
                    y = ynow + a * (Xa - xnow);
                    x = round(Xa);
                    if ((x <= 999) && (x >= 0) && (y <= 999) && (y >= 0))
                        memset(matr + 3 * (1000 * y + x), 0, 3);
                }
            }
            else {
                for (Xa = xnext; Xa <= xnow; Xa += Hx) {
                    y = ynext + a * (Xa - xnext);
                    x = round(Xa);
                    if ((x <= 999) && (x >= 0) && (y <= 999) && (y >= 0))
                        memset(matr + 3 * (1000 * y + x), 0, 3);
                }
            }
        }
        else {
            int Y1 = min(ynow, ynext);
            int Y2 = max(ynow, ynext);
            if ((xnow <= 999) && (xnow >= 0))
                for (y = Y1; y <= Y2; y++)
                    if ((y <= 999) && (y >= 0))
                        memset(matr + 3 * (1000 * y + xnow), 0, 3);
        }
        
    }
public:
    Triangle(ifstream& fin)
    {
        init(fin);
    }

    virtual void draw(BYTE* matr)
    {
        line(X0, Y0, X1, Y1, matr);
        line(X1, Y1, X2, Y2, matr);
        line(X0, Y0, X2, Y2, matr);
    }

    virtual void init(ifstream& fin)
    {
        Figure::init(fin);
        fin >> X1;
        fin >> Y1;
        fin >> X2;
        fin >> Y2;
    }
};

class Square : public Figure
{
    int X0, Y0, A;
public:
    Square(ifstream& fin)
    {
        init(fin);
    }

    virtual void draw(BYTE* matr)
    {
        int x = 0, y = 0;
        int Xmin=0, Xmax=999, Ymin=0, Ymax=999;

        if (X0 > 0)
            Xmin = X0;
        if (X0 + A < 999)
            Xmax = X0 + A;
        if (Y0 > 0)
            Ymin = Y0;
        if (Y0 + A < 999)
            Ymax = Y0 + A;

        if ((Y0 >= 0) && (Y0 <= 999))
            for (x = Xmin; x <= Xmax; x++)
                memset(matr + 3 * (1000 * Y0 + x), 0, 3);

        y = Y0 + A;
        if ((y >= 0) && (y <= 999))
            for (x = Xmin; x <= Xmax; x++)
                memset(matr + 3 * (1000 * y + x), 0, 3);

        if ((X0 >= 0) && (X0 <= 999))
            for (y = Ymin; y <= Ymax; y++)
                memset(matr + 3 * (1000 * y + X0), 0, 3);

        x = X0 + A;
        if ((x >= 0) && (x <= 999))
            for (y = Ymin; y <= Ymax; y++)
                memset(matr + 3 * (1000 * y + x), 0, 3);
    }

    virtual void init(ifstream& fin)
    {
        Figure::init(fin);
        fin >> A;
    }
};

void DrawAll(Figure** figs, size_t n, BYTE* matr)
{
    for (Figure** mas = figs; mas < figs + n; ++mas)
    {
        (*mas)->draw(matr);
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    Figure** mas = NULL;
    FILE* f = NULL;
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    ifstream fin;

    size_t n = 0;
    size_t num = 0;

    bfh.bfType = 19778;
    bfh.bfSize = sizeof(bfh) + sizeof(bih) + 3 * 1000 * 1000;
    bfh.bfReserved1 = 0;
    bfh.bfReserved2 = 0;
    bfh.bfOffBits = sizeof(bfh) + sizeof(bih);

    bih.biSize = sizeof(bih);
    bih.biWidth = 1000;
    bih.biHeight = 1000;
    bih.biPlanes = 1;
    bih.biBitCount = 24;
    bih.biCompression = 0;
    bih.biSizeImage = 1000 * 1000 * 3;
    bih.biXPelsPerMeter = 2835;
    bih.biYPelsPerMeter = 2835;
    bih.biClrUsed = 0;
    bih.biClrImportant = 0;

    try {
        memset(matr, 255, 1000 * 1000 * 3);

        f = fopen("new_art.bmp", "wb");
        if (!f) {
            cout << "не удалось создать .bmp файл\n";
            return 0;
        }
        fwrite(&bfh, sizeof(bfh), 1, f);
        fwrite(&bih, sizeof(bih), 1, f);

        fin.open("fig.txt");
        n = count(fin);
        mas = new Figure * [n];
        
        string name;
        for (int i = 0; i < n; ++i) {
            fin >> name;
            if (name == "TRIANGLE") {
                mas[i] = new Triangle(fin);//надо ли закрывать все i элементов?
            }
            else if (name == "CIRCLE") {
                mas[i] = new Circle(fin);
            }
            else if (name == "SQUARE") {
                mas[i] = new Square(fin);
            }
            else {
                throw std::exception("\nошибка при чтении. в исходном файле не верно задано имя фигуры\n");
            }
            cout << name << endl;
            ++num;
        }
        fin.close();

        DrawAll(mas, n, matr);

        fwrite(&matr, 1, 1000*1000*3, f);
    }
    catch (std::exception & ex) {
        cout << ex.what();
    }

    if (mas)
    {
        for (int i = 0; i < num; ++i) {
            delete mas[i];
            mas[i] = NULL;
        }
        delete[] mas;
        mas = NULL;
    }
    if (f)
    {
        fclose(f);
        f = NULL;
    }
    if (fin) {
        fin.close();
    }
    return 0;
}