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
#include <vector>
#include <memory>
using namespace std;

BYTE matr[1000 * 1000 * 3];

class Figure
{
protected:
    int X0, Y0;
public:
    virtual void draw(BYTE* matr) = 0;

    virtual string Name() = 0;

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
        cout << "нарисовано " << this->Name() << endl;
        float t;
        int x = 0, y = 0;
        for (t = 0; t <= 2 * M_PI; t += Ht) {
            x = X0 + R * cos(t);
            y = Y0 + R * sin(t);
            if ((x <= 999) && (x >= 0) && (y <= 999) && (y >= 0))
                memset(matr + 3 * (1000 * y + x), 0, 3);
        }
    }

    virtual string Name() {
        return "Circle";
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
            if (ZNa > 0) {
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

    virtual void draw(BYTE* matr){
        cout << "нарисовано " << this->Name() << endl;
        line(X0, Y0, X1, Y1, matr);
        line(X1, Y1, X2, Y2, matr);
        line(X0, Y0, X2, Y2, matr);
    }

    virtual string Name() {
        return "Triangle";
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
    int /*X0, Y0, */A;
public:
    Square(ifstream& fin)
    {
        init(fin);
    }

    virtual void draw(BYTE* matr){
        cout << "нарисовано " << this->Name() << endl;
        int x = 0, y = 0;
        int Xmin = 0, Xmax = 999, Ymin = 0, Ymax = 999;

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

    virtual string Name() {
        return "Square";
    }

    virtual void init(ifstream& fin)
    {
        Figure::init(fin);//не инициализирует X0 и Y0
        //fin >> X0;
        //fin >> Y0;
        fin >> A;
    }
};

void DrawAll(vector<unique_ptr<Figure>> mas, BYTE* matr)
{
    for (int i = 0; i < mas.size(); ++i) {
        mas[i]->draw(matr);
    }
}

//void ReadFigs(ifstream& fin, vector<Figure*>& mas) {
//    string name;
//    int n=0;
//    while (!fin.eof()) {
//        fin >> name;
//        ++n;
//        if (name == "TRIANGLE") {
//            mas.push_back(new Triangle(fin));//надо ли закрывать все i элементов?
//        }
//        else if (name == "CIRCLE") {
//            mas.push_back(new Circle(fin));
//        }
//        else if (name == "SQUARE") {
//            mas.push_back(new Square(fin));
//        }
//        else {
//            throw std::exception("\nошибка при чтении. в исходном файле не верно задано имя фигуры\n");
//        }
//        cout << name << endl;
//    }
//    cout << "\nвсего " << n << " фигур\n\n";
//}

vector<unique_ptr<Figure>> ReadFigs(ifstream& fin) {//вектор умных указ остаётся тут же
    string name;
    vector<unique_ptr<Figure>> mas;
    int n = 0;
    while (!fin.eof()) {
        fin >> name;
        ++n;
        if (name == "TRIANGLE") {
            mas.push_back(unique_ptr<Figure>(new Triangle(fin)));//надо ли закрывать все i элементов?
        }
        else if (name == "CIRCLE") {
            mas.push_back(unique_ptr<Figure>(new Circle(fin)));
        }
        else if (name == "SQUARE") {
            mas.push_back(unique_ptr<Figure>(new Square(fin)));
        }
        else {
            throw std::exception("\nошибка при чтении. в исходном файле не верно задано имя фигуры\n");
        }
        cout << name << endl;
    }
    cout << "\nвсего " << n << " фигур\n\n";
    return move(mas);
}


int main() {
    setlocale(LC_ALL, "Russian");
    FILE* f = NULL;
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    ifstream fin;

    //vector <Figure*> mas;
    fin.open("fig.txt");
    vector <unique_ptr<Figure>> mas=ReadFigs(fin);
    fin.close();

    memset(matr, 255, 1000 * 1000 * 3);

    {//оформление шапок
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
    }

    f = fopen("new_art.bmp", "wb");
    if (!f) {
        cout << "не удалось создать .bmp файл\n";
        return 0;
    }
    fwrite(&bfh, sizeof(bfh), 1, f);
    fwrite(&bih, sizeof(bih), 1, f);

    DrawAll(move(mas), matr);


    fwrite(&matr, 1, 1000 * 1000 * 3, f);

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