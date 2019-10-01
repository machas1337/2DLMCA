#ifndef DECRYPTER_H
#define DECRYPTER_H

#include <QFileDialog>
#include <QString>
#include <QImage>
#include <QObject>
#include <QtGlobal>
#include <math.h>
#include <matrix.h>
#include "mainwindow.h"
#include "windowoutput.h"
#include "encrypter.h"

class Decrypter : public Encrypter
{
public:
    QList<QImage> images; //lista szyfrogramów
    int rulenum_size; //liczba podanych liczb rządzących
    int iteration;

    Decrypter();
    Decrypter(QList<QImage> images, int *rulenum, int rulenum_size, IMAGETYPE type);
    QImage decrypt();//główna funkcja deszyfrująca
    int inverseTransition(int i, int j); //funkcja obecnego stanu inwersji
    int inverseTransitionFunction(int rulenum, QList<std::pair<int,int>> V); //funkcja przekształcająca
    Matrix** convertToMatrix(QImage image);
    QImage convertToQImage(Matrix **image);
};

#endif // DECRYPTER_H
