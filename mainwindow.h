#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsPixmapItem>


enum IMAGETYPE{
    COLOR,
    GREY
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private:
    Ui::MainWindow *ui;
    //Prywatne zmiene
    QImage img; //obraz pliku
    QGraphicsScene *scene; //scena widoku oryginalnego obrazu
    IMAGETYPE type; //typ obrazu

    IMAGETYPE setImageType(); //pobiera informacje o typie obrazu

private slots:
    void loadFile();    //ładuj plik obrazu
    void encryption();  //rozpoczyna szyfrowanie
    void on_pushDecryptButton_clicked();
};

#endif // MAINWINDOW_H
