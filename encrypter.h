#ifndef ENCRYPTER_H
#define ENCRYPTER_H

#include <QFileDialog>
#include <QString>
#include <QImage>
#include <QObject>
#include <QtGlobal>
#include <math.h>
#include <pixel.h>
#include "mainwindow.h"
#include "windowoutput.h"


struct LOCAL
{
    int i;
    int j;
};

class Encrypter
{
public:
    QImage image; //obraz oryginalny
    Pixel **M; //przedstawienie oryginalnego obrazu jako tablicy
    Pixel **Cnext; //Tworzenie nowego obrazu C
    QList<Pixel **>  C; //Obrazy C
    int k; //liczba "rządząca"
    int m; //parametr m
    int n; //liczba utworzonych sekretów
    int r; //szerokość obrazu
    int s; //wysokość obrazu
    int c; //ilość kolorów; =2^b
    int b; //=1 (czarno-białe); =8 (szare); =24 (kolorowe)
    int t; //obecny czas
    int *rulenum;//tablica liczb rządzących
    int rulenum_count;
    IMAGETYPE type;

    Encrypter();
    Encrypter(int k, int m, int n);
    ~Encrypter();

    void loadFile(QImage img); //ładuje plik obrazu
    void kindOfImage(IMAGETYPE type); //rozpoznaje rodzja obrazu: czarno-biały, szary, kolorowy
    void encrypt(int k, IMAGETYPE type); //główny kod procesu kodowania

    int transitionGrey(int i, int j); //wykonanie przekształcenia
    Pixel transitionColor(int i, int j); //wykonanie przekształcenia dla obrazów kolorowych

    int localTransition(int rulenum, QList<LOCAL> V, Pixel** Ctime); //lokalna funkcja przekształcenia
    Pixel localTransitionColor(int rulenum, QList<LOCAL> V, Pixel** Ctime);

    void createMGrey(); //utwozenie M tablicy dla obrazów szarych
    void createMColor(); //utworzenie M tablicy dla obrazów kolorwych

    void initCtresholds(int k, int c); //inicjalizacja k-1 konfiguracyjnych macierzy

    QList<LOCAL> getLocals(int i, int j); //funkcja zwraca sąsiadów punktu <i,j> wg. definicji Moor'a

    void saveCImages();
    Pixel** newMatrix();
};


#endif // ENCRYPTER_H
