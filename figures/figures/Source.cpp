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

size_t count(ifstream& base) {
    char str[1024];
    size_t i = 0;
    if (!base) {
        throw 1;
    }
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

public:
    virtual void draw(BYTE* matr) = 0;
};

class Circle : public Figure
{
    int X0, Y0, R;
    int y1;
    float Ht;
public:
    Circle() {
        fin >> X0;
        fin >> Y0;
        fin >> R;//если даже не один пиксель круга не попадёт на изображение ничего не сломается
        Ht = 1. / R;
    }

    virtual void draw(BYTE* matr)
    {
        float t;
        int x = 0, y = 0;
        for (t = 0; t <= 2 * M_PI; t += Ht) {
            x = X0 + R * cos(t);
            y = Y0 + R * sin(t);
            if ((x <= 999) && (x >= 0) && (y <= 999) && (y >= 0))
                matr[1000 * y + x] = 0;
        }
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
        int x = 0, y = 0;
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
    Triangle() {
        for (int i = 0; i < 3; i++) {
            fin >> X[i];
            fin >> Y[i];
            if ((X[i] > 999) || (X[i] < 0) || (Y[i] > 999) || (Y[i] < 0))
                throw 2;
        }
    }

    virtual void draw(BYTE* matr)
    {
        line(0, 1, matr);
        line(1, 2, matr);//не правильно считает(
        line(0, 2, matr);
    }
};

class Square : public Figure
{
    int X0, Y0, A;
    float a;
    int max;
public:
    Square() {
        fin >> X0;
        fin >> Y0;
        fin >> A;
        if ((X0 > 999) || (X0 < 0) || (Y0 > 999) || (Y0 < 0))
            throw 3;
    }

    virtual void draw(BYTE* matr)
    {
        int x = 0, y = 0;
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
};

void DrawAll(Figure** figs, size_t n, BYTE* matr)
{
    for (Figure** mas = figs; mas < figs + n; ++mas)
    {
        (*mas)->draw(matr);
    }
}

Figure** mas = NULL;//почему если её переставить в начало, то вылазит ошибка "нужна ; перед *"?

int main()
{
    setlocale(LC_ALL, "Russian");
    FILE* f = NULL;
    FILE* fr = NULL;
    BYTE* matr = NULL;
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    RGBTRIPLE rgb;

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
        matr = new BYTE[1000 * 1000];//очень не очень. не красивое
        for (int i = 0; i < 1000 * 1000; ++i) {//инициализация белым цветом
            matr[i] = 255;
        }
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
                mas[i] = new Triangle();//надо ли закрывать все i элементов?
            }
            else if (name == "CIRCLE") {
                mas[i] = new Circle();
            }
            else if (name == "SQUARE") {
                mas[i] = new Square();
            }
            else {
                throw 4;
            }
            cout << name << endl;
            ++num;
        }
        fin.close();

        DrawAll(mas, n, matr);

        for (int i = 0; i < 1000 * 1000; ++i) {
            fwrite(&matr[i], 1, 1, f);
            fwrite(&matr[i], 1, 1, f);
            fwrite(&matr[i], 1, 1, f);

        }
        fclose(f);
        for (int i = 0; i < n; ++i) {
            delete mas[i];
            mas[i] = NULL;
        }
        delete[] mas;
        delete[] matr;
        matr = NULL;
        mas = NULL;
        return 0;
    }
    catch (int ex) {
        switch (ex) {
        case 1:
            cout << "\nошибка count. Не удалось открыть файл для чтения количества строк!\n";
            break;
        case 2:
            cout << "\nошибка Triangle.getxy. координаты одной из точек лежат за границей изображения!\n";
            break;
        case 3:
            cout << "\nошибка Square.getxy. координаты левого нижнего угла лежат за границей изображения!\n";
            break;
        case 4:
            cout << "\nошибка при чтении. в исходном файле не верно задано имя фигуры\n";
            break;
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
        if (matr)
        {
            delete[] matr;
            matr = NULL;
        }
        if (fin) {
            fin.close();
        }
        return 0;
    }
    catch (...) {
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
        if (matr)
        {
            delete[] matr;
            matr = NULL;
        }
        if (fin) {
            fin.close();
        }
        return 0;
    }
}