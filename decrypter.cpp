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
        C.append(convertToMatrix(images[i]));
    c= int(pow(2.0, b));
    iteration = 0;
}

Matrix** Decrypter::convertToMatrix(QImage image)
{
    Matrix** current = newMatrix();
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

QImage Decrypter::convertToQImage(Matrix **image)
{
    QImage output(r,s,QImage::Format_RGB32);
    Matrix **image_render = newMatrix();

    QColor color;
    for(int i=0; i < r; i++)
    {
        for(int j=0; j < s; j++)
        {
            switch(type)
            {
            case IMAGETYPE::BINARY:
            {
                break;
            }
            case IMAGETYPE::GREY:
            {
                image_render[i][j].red = image[i][j].red;
                color.setRgb(image_render[i][j].red, image_render[i][j].red, image_render[i][j].red, 255);
                break;
            }
            case IMAGETYPE::COLOR:
            {
                image_render[i][j] = image[i][j];
                //image_render[i][j].value.G = image[i][j].value.G;
                //image_render[i][j].value.B = image[i][j].value.B;
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
            case IMAGETYPE::BINARY:
            {
                break;
            }
            case IMAGETYPE::GREY:
            {
                Cnext[i][j].red = inverseTransition(i,j);
                //Cnext[i][j].red = outt;
                break;
            }
            case IMAGETYPE::COLOR:
            {
                Cnext[i][j] = inverseTransitionColor(i,j);
                //Cnext[i][j].value.R = outt.value.R;
                //Cnext[i][j].value.G = outt.value.G;
                //Cnext[i][j].value.B = outt.value.B;
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

/*
int* Decrypter::inverseTransitionColor(int i, int j)
{
    int *a = new int[3];
    std::fill_n(a, 3, 0);

    int negative_sum[3] = {0};
    rulenum_count = 0;
    QList<LOCAL> V = getLocals(i,j);

    a[0] = C.at(iteration)[i][j].value.R;
    a[1] = C.at(iteration)[i][j].value.G;
    a[2] = C.at(iteration)[i][j].value.B;

    for(int x = iteration+1; x < k+iteration; x++)
    {
        int *result = localTransitionColor(rulenum[rulenum_count], V, C.at(x));
        negative_sum[0] -= result[0];
        negative_sum[1] -= result[1];
        negative_sum[2] -= result[2];

        if(rulenum_count == rulenum_size-1){
            rulenum_count = 0;
        }
        else{
            rulenum_count++;
        }
    }

    a[0] += negative_sum[0];
    a[1] += negative_sum[1];
    a[2] += negative_sum[2];
    a[0] = (c+(a[0] % c))%c;
    a[1] = (c+(a[1] % c))%c;
    a[2] = (c+(a[2] % c))%c;

    if(i == 0 && j == 0)
        qDebug()<<"output: "<<a[0]<<a[1]<<a[2];
    return a;
}
*/

Matrix Decrypter::inverseTransitionColor(int i, int j)
{
    Matrix a;
    Matrix result;
    Matrix negative_sum;
    rulenum_count = 0;
    QList<LOCAL> V = getLocals(i,j);

    a = C.at(iteration)[i][j];
    //a.value.G = C.at(iteration)[i][j].value.G;
    //a.value.B = C.at(iteration)[i][j].value.B;

    if(i == 0 && j == 0)
        qDebug()<<"resulted in interation: "<<iteration<<":: "<<a.red<<a.green<<a.blue;

    for(int x = iteration+1; x < k+iteration; x++)
    {
        negative_sum -= localTransitionColor(rulenum[rulenum_count], V, C.at(x));
        //result = localTransitionColor(rulenum[rulenum_count], V, C.at(x));
        //negative_sum.value.R -= result.value.R;
        //negative_sum.value.G -= result.value.G;
        //negative_sum.value.B -= result.value.B;

        if(rulenum_count == rulenum_size-1){
            rulenum_count = 0;
        }
        else{
            rulenum_count++;
        }
    }

    a += negative_sum;
    //a.value.G += negative_sum.value.G;
    //a.value.B += negative_sum.value.B;
    a.red = (c+(a.red % c))%c;
    a.green = (c+(a.green % c))%c;
    a.blue = (c+(a.blue % c))%c;

    if(i == 0 && j == 0)
        qDebug()<<"output: "<<a.red<<a.green<<a.blue;
    return a;
}
