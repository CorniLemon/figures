#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <cstdlib>
#include <cstdio>
#include <Windows.h>
#include <fstream>
using namespace std;

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
    delete str;
    cout <<"в файле "<< i<<" фигур\n\n";
    return i;
}

class Figure
{
protected:
    int x=0,y=0;
public:
    virtual void draw(BYTE* matr) = 0;
    virtual void getxy() = 0;
};

class Circle : public Figure
{
    int X0, Y0, R;
    int y1;
public:
    virtual void draw(BYTE* matr)
    {
        for ( x = X0 - R; x <= X0 + R; x++) {
            y = sqrt(R * R - pow((x - X0), 2));
            y1 = Y0 - y;
            y += Y0;
            matr[1000 * y + x] = 0;
            matr[1000 * y1 + x] = 0;
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
    void line(int g, int k, BYTE* matr) {
        if (X[g] - X[k]) {
            a = (Y[k] - Y[g]) / (X[k] - X[g]);
            for (x = X[g]; x <= X[k]; x++) {
                y = Y[g] + a * (x-X[g]);
                matr[1000 * y + x] = 0;
            }
        }
    }
public:
    virtual void draw(BYTE* matr)
    {
        line(0, 1, matr);
        line(1, 2, matr);
        line(0, 2,matr);
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
    int min, max;
public:
    virtual void draw(BYTE* matr)
    {
        y = Y0 + A;
        for (x = X0; x <= X0 + A; x++) {
            matr[1000 * Y0 + x] = 0;
            matr[1000 * y +x] = 0;
        }

        x = X0 + A;
        for (y =Y0; y <= Y0 + A; y++) {
            matr[1000 * y + X0] = 0;
            matr[1000 * y + x] = 0;
        }
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
    BYTE* matr = new BYTE[1000 * 1000];//очень не очень. не красивое
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

    f = fopen("new_art.bmp", "wb");
    fwrite(&bfh, sizeof(bfh), 1, f);
    fwrite(&bih, sizeof(bih), 1, f);

    size_t n = 0;
    n = count();

    //fr = fopen("fig.txt", "rb");
    Figure** mas = new Figure * [n];
    fin.open("fig.txt");
    string name;
    for (int i=0; i < n; ++i) {
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
        cout << name<<endl;
        mas[i]->getxy();
    }

    DrawAll(mas, n, matr);

    for (int i = 0; i < 1000 * 1000; ++i) {
        fwrite(&matr[i], 1, 1, f);
        fwrite(&matr[i], 1, 1, f);
        fwrite(&matr[i], 1, 1, f);

    }
    fclose(f);
    
	return 0;
}