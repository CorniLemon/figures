#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <cstdlib>
#include <cstdio>
#include <Windows.h>
#include <fstream>
#include <math.h>
using namespace std;

const float M_PI = 3.1415926535;

ifstream fin;

size_t count() {
    char* str = new char[1024];
    size_t i = 0;
    ifstream base("fig.txt");
    while (!base.eof())
    {
        base.getline(str, 1024, '\n');
        i++;
    }
    base.close();
    delete[] str;
    cout << "в файле " << i << " фигур(ы)\n\n";
    return i;
}

class Figure
{
protected:
    int x = 0, y = 0;
public:
    virtual void draw(BYTE* matr) = 0;
    virtual void getxy() = 0;
};

class Circle : public Figure
{
    int X0, Y0, R;
    int y1;
    float t;
    int L;
public:
    virtual void draw(BYTE* matr)
    {
        L = 2 * M_PI * R;
        for (t = 0; t <= 2 * M_PI; t += 2 * M_PI / L) {
            x = X0 + R * cos(t);
            y = Y0 + R * sin(t);
            if ((x <= 999) || (x >= 0) || (y <= 999) || (y >= 0))
                matr[1000 * y + x] = 0;
        }
    }

    virtual void getxy() {
        fin >> X0;
        fin >> Y0;
        fin >> R;
    }
};

class Triangle : public Figure
{
    int X[3], Y[3];
    float a;
    float HIa, ZNa;
    float Hx;//шаг по x
    float Xa, Ya;
    void line(int now, int next, BYTE* matr) {
        HIa = Y[next] - Y[now];
        ZNa = X[next] - X[now];
        a = HIa / ZNa;
        if (abs(a) <= 1)
            Hx = 1;
        else
            Hx = 1 / abs(a);
        for (Xa = X[now]; Xa <= X[next]; Xa += Hx) {
            y = Y[now] + a * (Xa - X[now]);
            x = round(Xa);
            matr[1000 * y + x] = 0;
        }
    }
public:
    virtual void draw(BYTE* matr)
    {
        line(0, 1, matr);
        line(1, 2, matr);//не правильно считает(
        line(0, 2, matr);
    }

    virtual void getxy() {
        for (int i = 0; i < 3; i++) {
            fin >> X[i];
            fin >> Y[i];
        }
    }
};

class Square : public Figure
{
    int X0, Y0, A;
    float a;
    int max;
public:
    virtual void draw(BYTE* matr)
    {
        max = 999;

        if (X0 + A <= 999)
            max = X0 + A;
        for (x = X0; x <= max; x++)
            matr[1000 * Y0 + x] = 0;

        y = Y0 + A;
        if (y <= 999)
            for (x = X0; x <= max; x++)
                matr[1000 * y + x] = 0;

        if (Y0 + A <= 999)
            max = Y0 + A;
        else max = 999;
        for (y = Y0; y <= max; y++)
            matr[1000 * y + X0] = 0;

        x = X0 + A;
        if (x <= 999)
            for (y = Y0; y <= max; y++)
                matr[1000 * y + x] = 0;
    }

    virtual void getxy() {
        fin >> X0;
        fin >> Y0;
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
    BYTE* matr = new BYTE[1000 * 1000];//очень не очень. не красивое//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    for (int i = 0; i < 1000 * 1000; ++i) {
        matr[i] = 255;
    }

    FILE* f = NULL;
    FILE* fr = NULL;
    setlocale(LC_ALL, "Russian");

    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    RGBTRIPLE rgb;

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
        f = fopen("new_art.bmp", "wb");//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        fwrite(&bfh, sizeof(bfh), 1, f);
        fwrite(&bih, sizeof(bih), 1, f);

        size_t n = 0;
        n = count();//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        Figure** mas = new Figure * [n];//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        fin.open("fig.txt");
        string name;
        for (int i = 0; i < n; ++i) {
            fin >> name;
            if (name == "TRIANGLE") {
                mas[i] = new Triangle();
            }
            else if (name == "CIRCLE") {
                mas[i] = new Circle();
            }
            else if (name == "SQUARE") {
                mas[i] = new Square();
            }
            else {
                cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\nв исходном файле не верно задано имя фигуры";
                return 0;
            }
            cout << name << endl;
            mas[i]->getxy();
        }

        DrawAll(mas, n, matr);

        for (int i = 0; i < 1000 * 1000; ++i) {
            fwrite(&matr[i], 1, 1, f);
            fwrite(&matr[i], 1, 1, f);
            fwrite(&matr[i], 1, 1, f);

        }
        fclose(f);
    }
    catch (...) {

    }

    return 0;
}