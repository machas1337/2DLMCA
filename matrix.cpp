#include "matrix.h"

Matrix::Matrix()
{
    red = 0;
    green = 0;
    blue = 0;
}

Matrix& Matrix::operator=(Matrix tmp)
{
    this->red = tmp.red;
    this->green = tmp.green;
    this->blue = tmp.blue;
    return *this;
}

Matrix& Matrix::operator+=(Matrix sum)
{
    this->red += sum.red;
    this->green += sum.green;
    this->blue += sum.blue;
    return *this;
}

Matrix& Matrix::operator-=(Matrix sum)
{
    this->red -= sum.red;
    this->green -= sum.green;
    this->blue -= sum.blue;
    return *this;
}
