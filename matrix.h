#ifndef MATRIX_H
#define MATRIX_H

#include <QColor>

class Matrix : public QColor
{
public:
    Matrix();
    int status; //wartość piksela
};

#endif // MATRIX_H
