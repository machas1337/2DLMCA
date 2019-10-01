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
    M= new Matrix *[r];
    for(int i= 0; i<r; i++)
    {
        M[i]= new Matrix[s];
    }
    //Cnext = M;
    t++;
}

void Encrypter::createMBandW()
{
    for(int i= 0; i< r; i++)
    {
        for(int j= 0; j<s; j++)
        {
           if(image.pixel(i,j)== qRgb(0,0,0))
               M[i][j].status= 1;
           else
               M[i][j].status= 0;
        }
    }
}

void Encrypter::createMGrey()
{
    QColor tmp;
    for(int i= 0; i< r; i++)
    {
        for(int j= 0; j<s; j++)
        {
            tmp.setRgb(image.pixel(i,j));
            if(tmp.red() < 256) M[i][j].status= tmp.red();
            else M[i][j].status= 255;
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
           tmp = M[i][j].fromRgb(image.pixel(i,j));
           M[i][j].status= tmp.red()+tmp.blue()+tmp.green();
        }
    }

}

void Encrypter::kindOfImage(IMAGETYPE typee)
{
    if(typee==IMAGETYPE::GREY){
        b= 8;
        createMGrey();
    }
    else if(typee==IMAGETYPE::BINARY){
        b= 1;
        createMBandW();
    }
    else{
        b= 24;
        createMColor();
    }
    type = typee;
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
        // 1 <= rulenum[i] <= 511
        //qsrand(qrand());
        //rulenum[i]= (2 * std::rand()%255) + 1;//(2 * qrand()%255) + 1;
        rulenum[i]= QRandomGenerator::global()->bounded(511);
    }
    rulenum_count= 0;
    
    //Faza II - przetwarzanie funkcji transformacji
    c= int(pow(2.0, b));

    //wygenerowanie k-1 startowych C-obrazów
    initCtresholds(k, c);

    //wygenerowanie szyfrogramów
    for(int secret = 0; secret < n+m-k; secret++)
    {
        Cnext = newMatrix();

        for(int i = 0; i<r; i++)
        {
            for(int j = 0; j<s; j++)
            {
                Cnext[i][j].status = transition(i,j);
            }
        }
        C.append(Cnext);
        t++;
        qDebug()<<"--------------";
    }

    saveCImages();
}

int Encrypter::transition(int i, int j)
{
    int a = 0;
    //QList<std::pair<int,int>> V = getLocals(i,j);
    QList<LOCAL> V = getLocalss(i, j);

    a = C.at(C.size()-k)[i][j].status;
    //qDebug()<<"first a: "<<a;
    for(int x = C.size()-k+1; x < C.size(); x++)
    {
        //a += transitionFunction(rulenum[rulenum_count], V, C.at(x));
        a += localTransition(rulenum[rulenum_count], V, C.at(x));
        //qDebug()<<"rulenum"<<rulenum[rulenum_count]<<"Cpos"<<x<<"Csize"<<C.size()<<"k"<<k;
        if(rulenum_count == k - 2) { rulenum_count = 0; }
        else { rulenum_count++; }

    }
    //qDebug()<<"sum a: "<<a;
    //rulenum_count = 0;
    //if(rulenum_count >= k-1) { rulenum_count = 0; }
    //a = transitionFunction(rulenum[rulenum_count], V);
    //rulenum_count++;
    /*
    for(int cstat=0; cstat < C.size(); cstat++)
    {
        a += C.at(cstat)[i][j].status;
    }
    */
    //qDebug()<<a<<"%"<<c<<"="<<a%c;
    a = a % c;

    return a;
}

int Encrypter::localTransition(int rulenum, QList<LOCAL> V, Matrix** Ctime)
{
    int binary_rulenum[10];
    int binary_size;
    int decimal = 0;
    int _rulenum = rulenum;
    QList<LOCAL> output;
    LOCAL temp;

    std::reverse(V.begin(), V.end());

    //przekształcenie wartości reguły na postać binarną
    for(binary_size = 0; _rulenum > 0; binary_size++)
    {
        binary_rulenum[binary_size]=_rulenum%2;
        _rulenum = _rulenum/2;
    }
    for(binary_size = binary_size - 1; binary_size >= 0; binary_size--)
    //for( ; binary_size >= 0; binary_size--)
    {
        if(binary_rulenum[binary_size]==1)
        {
            decimal = int(pow(2.0, binary_size));
            //qDebug()<<"decimal "<<decimal<<"rulenum "<<rulenum<<"V[binart_size].value"<<V[binary_size].value;
            if(decimal <= rulenum && V[binary_size].value == 1)
            //if(V[binary_size].value == 1)
            {
                temp.i = V[binary_size].i;
                temp.j = V[binary_size].j;
                output.append(temp);
            }
        }
    }

    int sum = 0, _i, _j;
    for(int x = 0; x < output.size(); x++)
    {
        _i = output[x].i;
        _j = output[x].j;
        sum += Ctime[_i][_j].status;
        //qDebug()<<"_i"<<_i<<"_j"<<_j<<"sum"<<sum;
    }
    //qDebug()<<"total sum: "<<sum;
    return sum;
}

int Encrypter::transitionFunction(int rulenum, QList<LOCAL> V, Matrix **Ctime)
{
    int sum = 0, _i, _j;
    QList<LOCAL> output;
    //Matrix **current = C.last();
    //Matrix **current = Ctime;
    //std::reverse(V.begin(), V.end());
    for(int x = 0; x< V.size(); x++)
    {
        if(V[x].value > 0)
        {
            _i = V[x].i;
            _j = V[x].j;
            sum += Ctime[_i][_j].status;
        }
    }
    return sum^rulenum;
    //return rulenum^sum;

}
/*
int Encrypter::transitionFunction(int rulenum, QList<std::pair<int,int>> V, Matrix **Ctime)
{
    int sum=0;
    int tmp1, tmp2;
    //Matrix **current = C.last();
    Matrix **current = Ctime;
    for(int i=0; i< V.size(); i++)
    {
        tmp1 = V[i].first;
        tmp2 = V[i].second;
        sum += current[tmp1][tmp2].status;
    }
    return sum^rulenum;
}
*/


void Encrypter::initCtresholds(int k, int c)
{
    k--;
    Matrix **temp;

    for(int x=0; x<k; x++)
    {
        temp = newMatrix();
        for(int i=0; i<r; i++)
        {
            for(int j=0; j<s; j++)
            {
                //qsrand(qrand());
                //std::srand(time(NULL));
                //temp[i][j].status = std::rand()%c; //qrand()%c;
                temp[i][j].status = QRandomGenerator::global()->bounded(c);
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
        QRgb value;
        Matrix **temp = C[x];
        for(int i=0; i<r; i++)
        {
            for(int j=0; j<s; j++)
            {
                if(type==IMAGETYPE::COLOR)
                {
                    value = qRgb(0,0,0); //temporary value for color images
                }
                else
                {
                    value = qRgb(temp[i][j].status, temp[i][j].status, temp[i][j].status);
                }
                output.setPixel(i,j,value);
            }
        }
        if(x==0)        output.save(output_path+QObject::tr("/oryginalny_plik.png"), "PNG");
        else if(x<k)  output.save(output_path+QObject::tr("/C_setup_%1.png").arg(x), "PNG");
        else            output.save(output_path+QObject::tr("/C%1.png").arg(x-k), "PNG");
    }

    WindowOutput &window = *new WindowOutput;
    window.passItemList(QObject::tr("Liczba rządząca: %1").arg(k));
    window.passItemList(QObject::tr("Parametr m: %1").arg(m));
    window.passItemList(QObject::tr("Liczba utworzonych sekretów: %1").arg(n));
    window.passItemList(QObject::tr("Sekrety: "));
    for(int i=k; i<C.size(); i++) window.passItemList(QObject::tr("C%1.png").arg(i-k));
    window.passItemList(QObject::tr("Liczby rządzące: "));
    for(int i=0; i<k-1; i++) window.passItemList(QObject::tr("%1").arg(rulenum[i]));
    window.show();
}

Matrix** Encrypter::newMatrix()
{
    Matrix **new_matrix= new Matrix *[r];
    for(int i= 0; i<r; i++)
    {
        new_matrix[i]= new Matrix[s];
    }
    return new_matrix;
}

QList<std::pair<int,int>> Encrypter::getLocals(int i, int j)
{
    QList<std::pair<int,int>> V;
    if(i-1 >= 0 && j-1 >= 0) V.append(std::make_pair(i-1,j-1));
    if(i-1 >= 0)             V.append(std::make_pair(i-1,j));
    if(i-1 >= 0 && j+1 < s)  V.append(std::make_pair(i-1,j+1));
    if(j-1 >= 0)             V.append(std::make_pair(i,j-1));
    V.append(std::make_pair(i,j));
    if(j+1 < s)              V.append(std::make_pair(i,j+1));
    if(i+1 < r && j-1 >= 0)  V.append(std::make_pair(i+1,j-1));
    if(i+1 < r)              V.append(std::make_pair(i+1,j));
    if(i+1 < r && j+1 < s)   V.append(std::make_pair(i+1,j+1));

    return V;
}

QList<LOCAL> Encrypter::getLocalss(int i, int j)
{
    QList<LOCAL> list;
    LOCAL local;

    local.i = i-1;
    local.j = j-1;
    if(i-1 >= 0 && j-1 >= 0)
    { local.value = 1;
    } else { local.value = 0; }
    list.append(local);

    local.i = i-1;
    local.j = j;
    if(i-1 >= 0)
    { local.value = 1;
    } else { local.value = 0; }
    list.append(local);

    local.i = i-1;
    local.j = j+1;
    if(i-1 >= 0 && j+1 < s)
    { local.value = 1;
    } else { local.value = 0; }
    list.append(local);

    local.i = i;
    local.j = j-1;
    if(j-1 >= 0)
    { local.value = 1;
    } else { local.value = 0; }
    list.append(local);

    local.i = i;
    local.j = j;
    local.value = 1;
    list.append(local);

    local.i = i;
    local.j = j+1;
    if(j+1 < s)
    { local.value = 1;
    } else { local.value = 0; }
    list.append(local);

    local.i = i+1;
    local.j = j-1;
    if(i+1 < r && j-1 >= 0)
    { local.value = 1;
    } else { local.value = 0; }
    list.append(local);

    local.i = i+1;
    local.j = j;
    if(i+1 < r)
    { local.value = 1;
    } else { local.value = 0; }
    list.append(local);

    local.i = i+1;
    local.j = j+1;
    if(i+1 < r && j+1 < s)
    { local.value = 1;
    } else { local.value = 0; }
    list.append(local);

    return list;
}
