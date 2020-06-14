#ifndef Pixel_H
#define Pixel_H

class Pixel
{
public:
    int red;
    int green;
    int blue;
    Pixel();
    //PixelColors value;//wartość piksela
    Pixel& operator=(Pixel equal);
    Pixel& operator+=(Pixel sum);
    Pixel& operator-=(Pixel diff);
};

#endif // Pixel_H
