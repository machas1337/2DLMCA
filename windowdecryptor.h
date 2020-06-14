#ifndef WINDOWDECRYPTOR_H
#define WINDOWDECRYPTOR_H

#include <QDialog>
#include <QSpinBox>
#include "mainwindow.h"
#include "decrypter.h"

namespace Ui {
class WindowDecryptor;
}

class WindowDecryptor : public QDialog
{
    Q_OBJECT

public:
    Decrypter *dec;
    int clicked_decrypting;
    QStringList images;
    QGraphicsScene *scene;
    QImage output_image;
    IMAGETYPE type;
    explicit WindowDecryptor(QWidget *parent = nullptr);
    explicit WindowDecryptor(QStringList images, IMAGETYPE type);
    ~WindowDecryptor();
    QList<QImage> outputImages();

private slots:
    void on_pushButton_clicked();
    void on_pushButtonDecode_clicked();
    void on_pushButtonSave_clicked();

private:
    Ui::WindowDecryptor *ui;
};

#endif // WINDOWDECRYPTOR_H
