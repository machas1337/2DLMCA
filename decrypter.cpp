#include "decrypter.h"
#include <QDebug>

Decrypter::Decrypter()
{

}

Decrypter::Decrypter(QList<QImage> images, int *rulenum, int rulenum_size, IMAGETYPE type)
{
    this->images = images;
    this->rulenum = rulenum;
    this->rulenum_size = rulenum_size;
    this->rulenum_count = 0;
    this->type = type;
    kindOfImage(this->type);
    C.clear();
    r= images.first().size().rwidth();
    s= images.first().size().rheight();
    m = images.size();
    k = images.size();
    for(int i=images.size()-1; i >= 0; i--)
        C.append(convertToPixel(images[i]));
    c= int(pow(2.0, b));
    iteration = 0;
}

Pixel** Decrypter::convertToPixel(QImage image)
{
    Pixel** current = newMatrix();
    QColor color;
    for(int i=0; i<r; i++)
    {
        for(int j=0; j<s; j++)
        {
            color = image.pixelColor(i,j);
            current[i][j].red = color.red();
            current[i][j].green = color.green();
            current[i][j].blue = color.blue();
        }
    }
    return current;
}

QImage Decrypter::convertToQImage(Pixel **image)
{
    QImage output(r,s,QImage::Format_RGB32);
    Pixel **image_render = newMatrix();

    QColor color;
    for(int i=0; i < r; i++)
    {
        for(int j=0; j < s; j++)
        {
            switch(type)
            {
            case IMAGETYPE::GREY:
            {
                image_render[i][j].red = image[i][j].red;
                color.setRgb(image_render[i][j].red, image_render[i][j].red, image_render[i][j].red, 255);
                break;
            }
            case IMAGETYPE::COLOR:
            {
                image_render[i][j] = image[i][j];
                color.setRgb(image_render[i][j].red, image_render[i][j].green, image_render[i][j].blue, 255);
                break;
            }
            }
            output.setPixelColor(i,j, color);

        }
    }
    for(int i = 0; i < r; i++)
        delete[] image_render[i];
    delete image_render;
    return output;
}

QImage Decrypter::decrypt()
{

    QImage output;

    Cnext = newMatrix();

    for(int i = 0; i<r ; i++)
    {
        for(int j = 0; j < s ; j++)
        {
            switch(type)
            {
            case IMAGETYPE::GREY:
            {
                Cnext[i][j].red = inverseTransition(i,j);
                break;
            }
            case IMAGETYPE::COLOR:
            {
                Cnext[i][j] = inverseTransitionColor(i,j);
                break;
            }
            }
        }
    }

    C.append(Cnext);
    iteration++;
    output = convertToQImage(C.last());
    return output;
}

int Decrypter::inverseTransition(int i, int j)
{
    int a = 0;
    int negative_sum = 0;
    rulenum_count = 0;
    QList<LOCAL> V = getLocals(i,j);

    a = C.at(iteration)[i][j].red;

    for(int x = iteration+1; x < k+iteration; x++)
    {
        negative_sum -= localTransition(rulenum[rulenum_count], V, C.at(x));
        if(rulenum_count == rulenum_size-1){
            rulenum_count = 0;
        }
        else{
            rulenum_count++;
        }
    }

    a += negative_sum;
    a = (c+(a % c))%c;
    return a;
}

Pixel Decrypter::inverseTransitionColor(int i, int j)
{
    Pixel a;
    Pixel result;
    Pixel negative_sum;
    rulenum_count = 0;
    QList<LOCAL> V = getLocals(i,j);

    a = C.at(iteration)[i][j];

    for(int x = iteration+1; x < k+iteration; x++)
    {
        negative_sum -= localTransitionColor(rulenum[rulenum_count], V, C.at(x));

        if(rulenum_count == rulenum_size-1){
            rulenum_count = 0;
        }
        else{
            rulenum_count++;
        }
    }

    a += negative_sum;
    a.red = (c+(a.red % c))%c;
    a.green = (c+(a.green % c))%c;
    a.blue = (c+(a.blue % c))%c;

    return a;
}
