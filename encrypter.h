#ifndef ENCRYPTER_H
#define ENCRYPTER_H

#include <QFileDialog>
#include <QString>
#include <QImage>
#include <QObject>
#include <QtGlobal>
#include <math.h>
#include <matrix.h>
#include "mainwindow.h"
#include "windowoutput.h"


struct LOCAL
{
    int i;
    int j;
    int value;
};

class Encrypter
{
public:
    QImage image; //obraz oryginalny
    Matrix **M; //przedstawienie oryginalnego obrazu jako tablicy
    Matrix **Cnext; //Tworzenie nowego obrazu C
    QList<Matrix **>  C; //Obrazy C
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
    int transition(int i, int j); //wykonanie przekształcenia
    int localTransition(int rulenum, QList<LOCAL> V, Matrix** Ctime); //lokalna funkcja przekształcenia
    int transitionFunction(int rulenum, QList<LOCAL> V, Matrix** Ctime); //funkcja przekształcająca
    void createMBandW(); //utwrzoenie M tablicy dla obrazów czarno białych
    void createMGrey(); //utwozenie M tablicy dla obrazów szarych
    void createMColor(); //utworzenie M tablicy dla obrazów kolorwych
    void initCtresholds(int k, int c); //inicjalizacja k-1 konfiguracyjnych macierzy
    QList<std::pair<int,int>> getLocals(int i, int j); //funkcja zwraca sąsiadów punktu <i,j> wg. definicji Moor'a
    QList<LOCAL> getLocalss(int i, int j);

    void saveCImages();
    Matrix** newMatrix();
};


#endif // ENCRYPTER_H
