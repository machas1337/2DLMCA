#include "pixel.h"

Pixel::Pixel()
{
    red = 0;
    green = 0;
    blue = 0;
}

Pixel& Pixel::operator=(Pixel tmp)
{
    this->red = tmp.red;
    this->green = tmp.green;
    this->blue = tmp.blue;
    return *this;
}

Pixel& Pixel::operator+=(Pixel sum)
{
    this->red += sum.red;
    this->green += sum.green;
    this->blue += sum.blue;
    return *this;
}

Pixel& Pixel::operator-=(Pixel sum)
{
    this->red -= sum.red;
    this->green -= sum.green;
    this->blue -= sum.blue;
    return *this;
}
