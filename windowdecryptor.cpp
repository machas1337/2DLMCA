#include "windowdecryptor.h"
#include "ui_windowdecryptor.h"
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

WindowDecryptor::WindowDecryptor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WindowDecryptor)
{
    ui->setupUi(this);
}

WindowDecryptor::~WindowDecryptor()
{
    delete ui;
}

WindowDecryptor::WindowDecryptor(QStringList images, IMAGETYPE type) :
    ui(new Ui::WindowDecryptor)
{
    ui->setupUi(this);
    for(int i=0; i<images.size(); i++)
    {
        QString current = images[i];
        QRegularExpression expresion("([^\//]+$)");
        QRegularExpressionMatch match = expresion.match(current);
        if(match.hasMatch() == true)
        {
            current = match.captured(0);
        }

        ui->textBrowser->append(current);
    }
    this->images = images;
    this->type = type;
    clicked_decrypting = 0;
}

void WindowDecryptor::on_pushButton_clicked()
{
    QSpinBox *spinbox = new QSpinBox;
    spinbox->setMaximum(511);
    spinbox->setParent(this);
    spinbox->setObjectName(tr("rulenum%1").arg(ui->gridLayout_2->count()));
    ui->gridLayout_2->addWidget(spinbox);
}

QList<QImage> WindowDecryptor::outputImages()
{
    QList<QImage> image_list;
    for(int i=0; i<images.size(); i++)
    {
        image_list.append(QImage(images[i]));
    }
    return image_list;
}

void WindowDecryptor::on_pushButtonDecode_clicked()
{
    QList<int> rulenum_list;
    int *rulenum;
    QList<QSpinBox *> items = this->findChildren<QSpinBox *>();
    if(clicked_decrypting == 0)
    {
        for(int i = 0; i < items.size(); i++)
        {
            if(items[i]->value() > 0)
                rulenum_list.append(items[i]->value());
        }
        std::reverse(rulenum_list.begin(), rulenum_list.end());
        rulenum = new int[rulenum_list.size()];
        for(int i = 0; i< rulenum_list.size(); i++)
        {
            rulenum[i] = rulenum_list[i];
        }
        dec = new Decrypter(outputImages(), rulenum, rulenum_list.size(), type);
    }
    clicked_decrypting++;
    ui->iterationLabel->setText(tr("Iteracja: %1").arg(clicked_decrypting));
    
    QImage output = dec->decrypt();
    scene= new QGraphicsScene(ui->graphicsView);
    ui->graphicsView->setScene(scene);
    QGraphicsPixmapItem *tmpimg = new QGraphicsPixmapItem(QPixmap::fromImage(output));
    scene->addItem(tmpimg);
    ui->graphicsView->fitInView((scene->sceneRect()), Qt::KeepAspectRatio);
    scene->update();
    ui->graphicsView->show();
}
