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
    //M= newMatrix();
    kindOfImage(this->type);
    C.clear();
    r= images.first().size().rwidth();
    s= images.first().size().rheight();
    m = images.size();
    k = images.size();
    for(int i=images.size()-1; i >= 0; i--)
    //for(int i = 0; i < images.size(); i++)
        C.append(convertToMatrix(images[i]));
    /*for(QImage xx : images)
    {
        C.append(convertToMatrix(xx));
    }
*/
    c= int(pow(2.0, b));
    iteration = 0;
    //dasdadasd

    qDebug()<<"!________________!_ C.SIZE: "<<C.size()<<" rulenum_size:"<<this->rulenum_size;

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
            current[i][j].status = color.red();
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
            if(image[i][j].status < 0 ) image_render[i][j].status = 0;
            else image_render[i][j].status = image[i][j].status;
            color.setRgb(image_render[i][j].status, image_render[i][j].status, image_render[i][j].status);
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
    //do{
    this->bad_pixels = 0;
    Cnext = newMatrix();

    for(int i = 0; i<r ; i++)
    {
        for(int j = 0; j < s ; j++)
        {
            int outt = inverseTransition(i,j);
            Cnext[i][j].status = outt;
            if(outt < 0)
                this->bad_pixels++;

            //if (i == 0 && j == 0)
            //{
            //qDebug()<<"[i: "<<i<<"] [j: "<<j<<"] = "<<outt;
            //}
            //Cnext[i][j].status = inverseTransition(i, j);
        }
    }
    /*
    QDebug deb = qDebug();
    for(int  i=0 ; i<r; i++)
    {
        for(int j=0; j<s ; j++)
        {
            deb<<Cnext[i][j].status;
        }
        deb<<"\n";
    }
    */
    C.append(Cnext);
    iteration++;
    //qDebug()<<"!________________!_ C.SIZE: "<<C.size();
    qDebug()<<"--------------";
    qDebug()<<"W tej iteracji było "<<this->bad_pixels<<" bad pixels";

    //QImage output = convertToQImage(Cnext);
    output = convertToQImage(C.last());
    //qDebug()<<"[1][1]: "<<C.last()[1][1].status;
    //qDebug()<<"[1][2]: "<<C.last()[1][2].status;
    //qDebug()<<"[0][0]: "<<C.last()[0][0].status;
    //}while(C.last()[1][1].status != 0 || C.last()[1][2].status != 63 || C.last()[0][0].status != 255);
    //}while(this->bad_pixels != 0 && iteration <= 1000);
    return output;
}

int Decrypter::inverseTransition(int i, int j)
{
    int a = 0;
    int negative_sum = 0;
    rulenum_count = 0;
    //QList<std::pair<int,int>> V = getLocals(i,j);

    QList<LOCAL> V = getLocalss(i,j);

    a = C.at(iteration)[i][j].status;
    //qDebug()<<"first a: "<<a;
    //qDebug()<<"..... ITERACJA: "<<iteration<<" dla i x j: ["<<i<<"x"<<j<<"]";
    //qDebug()<<"..... A VALUE: "<<a;


    //a = transitionFunction(rulenum[rulenum_count], V);
    //rulenum_count--;
// TEST, ITERACJA OSTATNICH 2-3 ELEMENTÓW TABLICY
    for(int x = iteration+1; x < k+iteration; x++)
    {
        //qDebug()<<"rulnumber: "<<rulenum[rulenum_count]<<" rulenum_count: "<<rulenum_count<<"rulenum_size: "<<rulenum_size;
        //negative_sum -= transitionFunction(rulenum[rulenum_count], V, C.at(x));
        negative_sum -= localTransition(rulenum[rulenum_count], V, C.at(x));
        //qDebug()<<"rulenum"<<rulenum[rulenum_count]<<"Cpos"<<x<<"Csize"<<C.size()<<"k"<<k<<"iteration"<<iteration<<"rulenum_size"<<rulenum_size;
        //qDebug()<<"....... RULE NUMBER : "<<rulenum[rulenum_count];
        if(rulenum_count == rulenum_size-1) { rulenum_count = 0; }
        else { rulenum_count++; }

        if(i == 0 && j == 0)
        {
            qDebug()<<"iteracja x: "<<x<<"  negative sum = "<<negative_sum<<rulenum[rulenum_count]<<" current 'a': "<<a<<" w iteracji: "<<iteration<<" ruenum size: "<<rulenum_size;
        }

    }
//*/
/*
    for(int x = 0; x < C.size(); x++)
    {
         negative_sum -= localTransition(rulenum[rulenum_count], V, C.at(x));
         if(rulenum_count == rulenum_size-1) { rulenum_count = 0; }
         else { rulenum_count++; }
    }
*/
    //qDebug()<<"sum a: "<<a;
    a += negative_sum;
    //qDebug()<<"..... NEGATIVE: "<<negative_sum<<"  ++++iteracja x size: "<<k-1;
    //qDebug()<<"..... A SUME: "<<a;
    a = (c+(a % c))%c;
    //qDebug()<<a<<"%"<<c<<"="<<a%c;
    if(i == 0 && j == 0)
        qDebug()<<"a == : "<<a;

    return a;
}

int Decrypter::inverseTransitionFunction(int rulenum, QList<std::pair<int,int>> V)
{
    int sum=0;
    int tmp1, tmp2;
    Matrix **current = C.last();
    for(int i=0; i< V.size(); i++)
    {
        tmp1 = V[i].first;
        tmp2 = V[i].second;
        sum += current[tmp1][tmp2].status;
    }
    return sum^rulenum;
}
