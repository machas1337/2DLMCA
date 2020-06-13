#ifndef MATRIX_H
#define MATRIX_H

class Matrix
{
public:
    int red;
    int green;
    int blue;
    Matrix();
    //PixelColors value;//wartość piksela
    Matrix& operator=(Matrix equal);
    Matrix& operator+=(Matrix sum);
    Matrix& operator-=(Matrix diff);
};

#endif // MATRIX_H
