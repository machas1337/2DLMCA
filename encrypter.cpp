#include "encrypter.h"
#include <QtDebug>
#include <QTime>
#include <QThread>
#include <QRandomGenerator>


Encrypter::Encrypter()
{

}

Encrypter::Encrypter(int k, int m, int n)
{
    this->k = k;
    this->m = m;
    this->n = n;
    t = 0;
}
Encrypter::~Encrypter()
{
    delete[] rulenum;
    C.clear();
    for(int i=0; i<r; i++)
    {
        delete[] Cnext[i];
        delete[] M[i];
    }
    delete[] Cnext;
    delete[] M;
}

void Encrypter::loadFile(QImage img)
{
    //utworzeni obrazu
    image= img;

    //pobranie rozmiaru
    r= image.size().rwidth();
    s= image.size().rheight();

    //utworzenie matriksa dla obrazu
    M= new Pixel *[r];
    for(int i= 0; i<r; i++)
    {
        M[i]= new Pixel[s];
    }
    t++;
}

void Encrypter::createMGrey()
{
    QColor tmp;
    for(int i= 0; i< r; i++)
    {
        for(int j= 0; j<s; j++)
        {
            tmp.setRgb(image.pixel(i,j));
            M[i][j].red = tmp.red();
        }
    }
}

void Encrypter::createMColor()
{
    QColor tmp;
    for(int i= 0; i< r; i++)
    {
        for(int j= 0; j<s; j++)
        {
           tmp.setRgb(image.pixel(i,j));
           M[i][j].red = tmp.red();
           M[i][j].green = tmp.green();
           M[i][j].blue = tmp.blue();
        }
    }

}

void Encrypter::kindOfImage(IMAGETYPE typee)
{
    b = 8;
    type = typee;
    switch(type)
    {
    case IMAGETYPE::GREY:
    {
        createMGrey();
        break;
    }
    case IMAGETYPE::COLOR:
    {
        createMColor();
        break;
    }
    }

    C.append(M); //pierwszy szyfrogram sekretu C(0)
}

void Encrypter::encrypt(int k, IMAGETYPE typeTMP)
{
    kindOfImage(typeTMP);
    //SETUP PHASE
    //Faza I - utworzenie ciągu losowych liczb całkowitych
    rulenum = new int[k]; //rulenum = &static_cast<int &>(k);
    for(int i= 0; i< k-1; i++)
    {
        rulenum[i]= QRandomGenerator::global()->bounded(1,511);
    }
    rulenum_count= 0;
    
    //Faza II - przetwarzanie funkcji transformacji
    c= int(pow(2.0, b));

    //wygenerowanie k-1 startowych C-obrazów
    initCtresholds(k, c);

    //wygenerowanie szyfrogramów
    for(int secret = C.size(); secret <= n+m-1; secret++)
    {
        Cnext = newMatrix();

        for(int i = 0; i<r ; i++)
        {
            for(int j = 0; j<s ; j++)
            {
                switch(type)
                {
                    case IMAGETYPE::GREY:
                    {
                        Cnext[i][j].red = transitionGrey(i,j);
                        break;
                    }
                    case IMAGETYPE::COLOR:
                    {
                        Cnext[i][j] = transitionColor(i,j);
                        break;
                    }
                }
            }
        }
        C.append(Cnext);
        t++;
    }

    saveCImages();
}

int Encrypter::transitionGrey(int i, int j)
{
    int a = 0;
    QList<LOCAL> V = getLocals(i, j);

    a = C.at(C.size()-k)[i][j].red;

    for(int x = C.size()-k+1; x < C.size(); x++)
    {
        a += localTransition(rulenum[rulenum_count], V, C.at(x));
        if(rulenum_count == k - 2){
            rulenum_count = 0;
        }
        else{
            rulenum_count++;
        }
    }

    a = (c+(a % c))%c;

    return a;
}

Pixel Encrypter::transitionColor(int i, int j)
{
    Pixel a;
    QList<LOCAL> V = getLocals(i, j);

    a = C.at(C.size()-k)[i][j];

    for(int x = C.size()-k+1; x < C.size(); x++)
    {
        a += localTransitionColor(rulenum[rulenum_count], V, C.at(x));
        if(rulenum_count == k - 2){
            rulenum_count = 0;
        }else{
            rulenum_count++;
        }
    }

    a.red = (c+(a.red % c))%c;
    a.green = (c+(a.green % c))%c;
    a.blue = (c+(a.blue % c))%c;

    return a;
}


int Encrypter::localTransition(int rulenum, QList<LOCAL> V, Pixel** Ctime)
{
    int binary_rulenum[9] = {0}; //binarna postać liczby rządzącej
    int binary_size;    //wielkość
    int decimal = 0;    //zmienna pomocnicza
    int _rulenum = rulenum; //zmienna pomocnicza
    QList<LOCAL> output; //zwracana lista pixeli sąsiadujących
    LOCAL temp; //zmienna pomocnicza

    std::reverse(V.begin(), V.end());

    //przekształcenie wartości reguły na postać binarną
    for(binary_size = 0; _rulenum > 0; binary_size++)
    {
        binary_rulenum[binary_size]=_rulenum%2;
        _rulenum = _rulenum/2;
    }

    //pętla sprawdzająca poszczególne wartości kolejnych bitów
    for(binary_size = binary_size - 1; binary_size >= 0; binary_size--)
    {
        if(binary_rulenum[binary_size]==1)
        {
            decimal = int(pow(2.0, binary_size));
            if(decimal <= rulenum)
            {
                temp.i = V[binary_size].i;
                temp.j = V[binary_size].j;
                output.append(temp);
            }
        }
    }

    int sum = 0, _i, _j;

    //pętla zwracająca wartość poszczególnych pixeli
    for(int x = 0; x < output.size(); x++)
    {
        _i = output[x].i;
        _j = output[x].j;
        sum += Ctime[(r+(_i%r))%r][(s+(_j%s))%s].red;
    }

    return sum;
}


Pixel Encrypter::localTransitionColor(int rulenum, QList<LOCAL> V, Pixel** Ctime)
{
    int binary_rulenum[9] = {0}; //binarna postać liczby rządzącej
    int binary_size;    //wielkość
    int decimal = 0;    //zmienna pomocnicza
    int _rulenum = rulenum; //zmienna pomocnicza
    QList<LOCAL> output; //zwracana lista pixeli sąsiadujących
    LOCAL temp; //zmienna pomocnicza

    std::reverse(V.begin(), V.end());

    //przekształcenie wartości reguły na postać binarną
    for(binary_size = 0; _rulenum > 0; binary_size++)
    {
        binary_rulenum[binary_size]=_rulenum%2;
        _rulenum = _rulenum/2;
    }

    //pętla sprawdzająca poszczególne wartości kolejnych bitów
    for(binary_size = binary_size - 1; binary_size >= 0; binary_size--)
    {
        if(binary_rulenum[binary_size]==1)
        {
            decimal = int(pow(2.0, binary_size));
            if(decimal <= rulenum)
            {
                temp.i = V[binary_size].i;
                temp.j = V[binary_size].j;
                output.append(temp);
            }
        }
    }

    Pixel sum;
    int _i, _j;

    //pętla zwracająca wartość poszczególnych pixeli
    for(int x = 0; x < output.size(); x++)
    {
        _i = output[x].i;
        _j = output[x].j;
        sum.red += Ctime[(r+(_i%r))%r][(s+(_j%s))%s].red;
        sum.green += Ctime[(r+(_i%r))%r][(s+(_j%s))%s].green;
        sum.blue += Ctime[(r+(_i%r))%r][(s+(_j%s))%s].blue;
    }

    return sum;
}


void Encrypter::initCtresholds(int k, int c)
{
    k--;
    Pixel **temp;

    for(int x=0; x<k; x++)
    {
        temp = newMatrix();
        for(int i=0; i<r; i++)
        {
            for(int j=0; j<s; j++)
            {
                switch(type)
                {
                case IMAGETYPE::GREY:
                {
                   temp[i][j].red = QRandomGenerator::global()->bounded(c);
                   break;
                }
                case IMAGETYPE::COLOR:
                {
                    temp[i][j].red = QRandomGenerator::global()->bounded(c);
                    temp[i][j].green = QRandomGenerator::global()->bounded(c);
                    temp[i][j].blue = QRandomGenerator::global()->bounded(c);
                    break;
                }
                }

            }
        }
        C.append(temp);
        t++;
    }
}

void Encrypter::saveCImages()
{
    QString output_path = QFileDialog::getExistingDirectory();

    for(int x=0; x<C.size(); x++)
    {
        QImage output(image);
        QRgb value = qRgb(0,0,0);
        Pixel **temp = C[x];
        for(int i=0; i<r; i++)
        {
            for(int j=0; j<s; j++)
            {
                switch(type)
                {
                case IMAGETYPE::COLOR:
                {
                    value = qRgb(temp[i][j].red, temp[i][j].green, temp[i][j].blue);
                    break;
                }
                case IMAGETYPE::GREY:
                {
                    value = qRgb(temp[i][j].red, temp[i][j].red, temp[i][j].red);
                    break;
                }
                }
                output.setPixel(i,j,value);
            }
        }
        if(x==0)      output.save(output_path+QObject::tr("/oryginalny_plik.png"), "PNG");
        else if(x<k)  output.save(output_path+QObject::tr("/C_setup_%1.png").arg(x), "PNG");
        else          output.save(output_path+QObject::tr("/C%1.png").arg(x-k), "PNG");
    }

    WindowOutput &window = *new WindowOutput;
    window.passItemList(QObject::tr("Liczba rządząca: %1").arg(k));
    window.passItemList(QObject::tr("Parametr m: %1").arg(m));
    window.passItemList(QObject::tr("Liczba utworzonych sekretów: %1").arg(n));
    window.passItemList(QObject::tr("Sekrety: "));
    for(int i=k; i<C.size(); i++)
        window.passItemList(QObject::tr("C%1.png").arg(i-k));
    window.passItemList(QObject::tr("Liczby rządzące: "));
    for(int i=0; i<k-1; i++)
        window.passItemList(QObject::tr("%1").arg(rulenum[i]));
    window.show();
}

Pixel** Encrypter::newMatrix()
{
    Pixel **new_Pixel= new Pixel *[r];
    for(int i= 0; i<r; i++)
    {
        new_Pixel[i]= new Pixel[s];
    }
    return new_Pixel;
}

QList<LOCAL> Encrypter::getLocals(int i, int j)
{
    QList<LOCAL> list;
    LOCAL local;

    local.i = i-1;
    local.j = j-1;
    list.append(local);

    local.i = i-1;
    local.j = j;
    list.append(local);

    local.i = i-1;
    local.j = j+1;
    list.append(local);

    local.i = i;
    local.j = j-1;
    list.append(local);

    local.i = i;
    local.j = j;
    list.append(local);

    local.i = i;
    local.j = j+1;
    list.append(local);

    local.i = i+1;
    local.j = j-1;
    list.append(local);

    local.i = i+1;
    local.j = j;
    list.append(local);

    local.i = i+1;
    local.j = j+1;
    list.append(local);

    return list;
}
