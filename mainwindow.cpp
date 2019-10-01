#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "encrypter.h"
#include "decrypter.h"
#include "windowdecryptor.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->spinBox->setMinimum(2);
    ui->spinBox->setMaximum(9);
    ui->spinBoxM->setMinimum(2);
    ui->spinBoxM->setMaximum(9);
    ui->spinBoxN->setMinimum(2);
    ui->spinBoxN->setMaximum(9);
    ui->radioImageColor->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

IMAGETYPE MainWindow::setImageType()
{
    if(ui->radioImageColor->isChecked()==true)
        return IMAGETYPE::COLOR;
    else if(ui->radioImageGrey->isChecked()==true)
        return IMAGETYPE::GREY;
    else if(ui->radioImageBinary->isChecked()==true)
        return IMAGETYPE::BINARY;
    else
        throw "BŁĄD TYPU OBRAZKA";
}

void MainWindow::loadFile()
{
    QString link;
    //otwieranie i ładowanie pliku z okna dialogowego przez użytkownika
    link = QFileDialog::getOpenFileName(this, tr("Otwórz obraz"), "", tr("Pliki obrazów (*.png *.jpg *.jpeg *.bmp)"));
    img.load(link);

    //utworzenie sceny i ustawienie nowo-wybranego obrazu
    scene= new QGraphicsScene(ui->originalImage);
    ui->originalImage->setScene(scene);
    QGraphicsPixmapItem *tmpimg = new QGraphicsPixmapItem(QPixmap::fromImage(img));
    scene->addItem(tmpimg);
    ui->originalImage->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    scene->update();
    ui->originalImage->show();
}

void MainWindow::encryption()
{
    //pobranie informacji o wartości przetwarzanego obrazu
    this->type = this->setImageType();
    //weryfikacja, czy plik obrazu został wybrany
    if(img.isNull()==true)
    {
        QMessageBox error;
        error.setText("Nie wybrano pliku obrazu!");
        error.exec();
    }
    //jeżeli został, następuje szyfrowanie obrazu
    else
    {
        Encrypter *enc = new Encrypter(ui->spinBox->value(), ui->spinBoxM->value(), ui->spinBoxN->value());
        enc->loadFile(img);
        enc->encrypt(ui->spinBox->value(), this->setImageType());
    }
}
void MainWindow::on_pushDecryptButton_clicked()
{
    QStringList link;
    link = QFileDialog::getOpenFileNames(this, tr("Wybierz obrazy"), "", tr("Pliki obrazów (*.png)"));
    WindowDecryptor *decryptor_window = new WindowDecryptor(link, this->setImageType());
    decryptor_window->show();
}
