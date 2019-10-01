#include "windowoutput.h"
#include "ui_windowoutput.h"

WindowOutput::WindowOutput(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WindowOutput)
{
    ui->setupUi(this);
}

WindowOutput::~WindowOutput()
{
    delete ui;
}


void WindowOutput::passItemList(QString arg)
{
    ui->textBrowser->insertPlainText(arg + tr("\n"));
}
