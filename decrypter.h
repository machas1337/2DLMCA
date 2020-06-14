#ifndef DECRYPTER_H
#define DECRYPTER_H

#include <QFileDialog>
#include <QString>
#include <QImage>
#include <QObject>
#include <QtGlobal>
#include <math.h>
#include <pixel.h>
#include "mainwindow.h"
#include "windowoutput.h"
#include "encrypter.h"

class Decrypter : public Encrypter
{
public:
    QList<QImage> images; //lista szyfrogramów
    int rulenum_size; //liczba podanych liczb rządzących
    int iteration;  //licznik iteracji

    Decrypter();
    Decrypter(QList<QImage> images, int *rulenum, int rulenum_size, IMAGETYPE type);

    QImage decrypt();//główna funkcja deszyfrująca
    int inverseTransition(int i, int j); //funkcja obecnego stanu inwersji
    Pixel inverseTransitionColor(int i, int j); //funkcja obecnego stanu inwersji

    Pixel** convertToPixel(QImage image);
    QImage convertToQImage(Pixel **image);
};

#endif // DECRYPTER_H
